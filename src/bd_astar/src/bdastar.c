/*
 * Bi Directional A* Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <search.h>

#include "bdastar.h"

//-------------------------------------------------------------------------

/*
 * Define this to have profiling enabled
 */
//#define PROFILE

#ifdef PROFILE
#include <sys/time.h>

struct timeval prof_astar, prof_store, prof_extract, prof_total;
long proftime[5];
long profipts1, profipts2, profopts;

#define profstart(x) do { gettimeofday(&x, NULL); } while (0);
#define profstop(n, x) do { struct timeval _profstop;   \
        long _proftime;                         \
        gettimeofday(&_profstop, NULL);                         \
        _proftime = ( _profstop.tv_sec*1000000+_profstop.tv_usec) -     \
                ( x.tv_sec*1000000+x.tv_usec); \
        elog(NOTICE, \
                "PRF(%s) %lu (%f ms)", \
                (n), \
             _proftime, _proftime / 1000.0);    \
        } while (0);

#else

#define profstart(x) do { } while (0);
#define profstop(n, x) do { } while (0);

#endif // PROFILE


//-------------------------------------------------------------------------

Datum bidir_astar_shortest_path(PG_FUNCTION_ARGS);

#undef DEBUG
//#define DEBUG 1
#include "../../common/src/debug_macro.h"


// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000

#ifndef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

static char *
text2char(text *in)
{
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

static int
finish(int code, int ret)
{
  code = SPI_finish();
  if (code  != SPI_OK_FINISH ) {
      elog(ERROR,"couldn't disconnect from SPI");
      return -1 ;
  }

  return ret;
}

typedef struct edge_astar_columns
{
  int id;
  int source;
  int target;
  int cost;
  int reverse_cost;
  int s_x;
  int s_y;
  int t_x;
  int t_y;
} edge_astar_columns_t;


static int
fetch_edge_astar_columns(SPITupleTable *tuptable,
             edge_astar_columns_t *edge_columns,
             bool has_reverse_cost)
{
  edge_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
  edge_columns->source = SPI_fnumber(SPI_tuptable->tupdesc, "source");
  edge_columns->target = SPI_fnumber(SPI_tuptable->tupdesc, "target");
  edge_columns->cost = SPI_fnumber(SPI_tuptable->tupdesc, "cost");

  if (edge_columns->id == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->source == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->target == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->cost == SPI_ERROR_NOATTRIBUTE) {

      elog(ERROR, "Error, query must return columns "
            "'id', 'source', 'target' and 'cost'");
      return -1;
  }

  if (SPI_gettypeid(SPI_tuptable->tupdesc,
            edge_columns->source) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc,
            edge_columns->target) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->cost) != FLOAT8OID) {

      elog(ERROR, "Error, columns 'source', 'target' must be of type int4, "
            "'cost' must be of type float8");
      return -1;
    }

  PGR_DBG("columns: id %i source %i target %i cost %i",
      edge_columns->id, edge_columns->source,
      edge_columns->target, edge_columns->cost);

  if (has_reverse_cost) {
      edge_columns->reverse_cost = SPI_fnumber(SPI_tuptable->tupdesc,
                           "reverse_cost");

      if (edge_columns->reverse_cost == SPI_ERROR_NOATTRIBUTE) {
          elog(ERROR, "Error, reverse_cost is used, but query did't return "
               "'reverse_cost' column");
          return -1;
      }

      if (SPI_gettypeid(SPI_tuptable->tupdesc,
            edge_columns->reverse_cost) != FLOAT8OID) {
          elog(ERROR, "Error, columns 'reverse_cost' must be of type float8");
          return -1;
      }

      PGR_DBG("columns: reverse_cost cost %i", edge_columns->reverse_cost);
  }

  edge_columns->s_x = SPI_fnumber(SPI_tuptable->tupdesc, "x1");
  edge_columns->s_y = SPI_fnumber(SPI_tuptable->tupdesc, "y1");
  edge_columns->t_x = SPI_fnumber(SPI_tuptable->tupdesc, "x2");
  edge_columns->t_y = SPI_fnumber(SPI_tuptable->tupdesc, "y2");

  if (edge_columns->s_x == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->s_y == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->t_x == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->t_y == SPI_ERROR_NOATTRIBUTE) {

      elog(ERROR, "Error, query must return columns "
       "'x1', 'x2', 'y1' and 'y2'");
      return -1;
  }

  PGR_DBG("columns: x1 %i y1 %i x2 %i y2 %i",
      edge_columns->s_x, edge_columns->s_y,
      edge_columns->t_x,edge_columns->t_y);

  return 0;
}

static void
fetch_edge_astar(HeapTuple *tuple, TupleDesc *tupdesc,
         edge_astar_columns_t *edge_columns,
         edge_astar_t *target_edge)
{
  Datum binval;
  bool isnull;

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->id, &isnull);
  if (isnull) elog(ERROR, "id contains a null value");
  target_edge->id = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->source, &isnull);
  if (isnull) elog(ERROR, "source contains a null value");
  target_edge->source = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->target, &isnull);
  if (isnull) elog(ERROR, "target contains a null value");
  target_edge->target = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->cost, &isnull);
  if (isnull) elog(ERROR, "cost contains a null value");
  target_edge->cost = DatumGetFloat8(binval);

  if (edge_columns->reverse_cost != -1) {
      binval = SPI_getbinval(*tuple, *tupdesc,
                 edge_columns->reverse_cost, &isnull);
      if (isnull)
        elog(ERROR, "reverse_cost contains a null value");
      target_edge->reverse_cost =  DatumGetFloat8(binval);
  }

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->s_x, &isnull);
  if (isnull) elog(ERROR, "source x contains a null value");
  target_edge->s_x = DatumGetFloat8(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->s_y, &isnull);
  if (isnull) elog(ERROR, "source y contains a null value");
  target_edge->s_y = DatumGetFloat8(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->t_x, &isnull);
  if (isnull) elog(ERROR, "target x contains a null value");
  target_edge->t_x = DatumGetFloat8(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->t_y, &isnull);
  if (isnull) elog(ERROR, "target y contains a null value");
  target_edge->t_y = DatumGetFloat8(binval);
}


static int compute_shortest_path_astar(char* sql, int source_vertex_id,
                       int target_vertex_id, bool directed,
                       bool has_reverse_cost,
                       path_element_t **path, int *path_count)
{
  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  int ntuples;
  edge_astar_t *edges = NULL;
  int total_tuples = 0;

  int v_max_id=0;
  int v_min_id=INT_MAX;

  edge_astar_columns_t edge_columns = {.id= -1, .source= -1, .target= -1,
                       .cost= -1, .reverse_cost= -1,
                       .s_x= -1, .s_y= -1, .t_x= -1, .t_y= -1};
  char *err_msg;
  int ret = -1;
  register int z;

  int s_count=0;
  int t_count=0;

  struct vItem {
    int id;
    int key;
  };

  PGR_DBG("start shortest_path_astar\n");

  SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT) {
      elog(ERROR, "shortest_path_astar: couldn't open a connection to SPI");
      return -1;
  }

  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL) {
      elog(ERROR, "shortest_path_astar: couldn't create query plan via SPI");
      return -1;
  }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
      elog(ERROR, "shortest_path_astar: SPI_cursor_open('%s') returns NULL",
            sql);
      return -1;
  }

  while (moredata == TRUE) {
      SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

      if (edge_columns.id == -1) {
          if (fetch_edge_astar_columns(SPI_tuptable, &edge_columns,
                           has_reverse_cost) == -1)
            return finish(SPIcode, ret);
      }

      ntuples = SPI_processed;
      total_tuples += ntuples;
      if (!edges)
        edges = palloc(total_tuples * sizeof(edge_astar_t));
      else
        edges = repalloc(edges, total_tuples * sizeof(edge_astar_t));

      if (edges == NULL) {
          elog(ERROR, "Out of memory");
          return finish(SPIcode, ret);
      }

      if (ntuples > 0) {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          for (t = 0; t < ntuples; t++) {
              HeapTuple tuple = tuptable->vals[t];
              fetch_edge_astar(&tuple, &tupdesc, &edge_columns,
                       &edges[total_tuples - ntuples + t]);
          }
          SPI_freetuptable(tuptable);
      }
      else {
          moredata = FALSE;
      }
  }

  //defining min and max vertex id

  PGR_DBG("Total %i tuples", total_tuples);

  for(z=0; z<total_tuples; z++)
  {
    if(edges[z].source<v_min_id) v_min_id=edges[z].source;
    if(edges[z].source>v_max_id) v_max_id=edges[z].source;
    if(edges[z].target<v_min_id) v_min_id=edges[z].target;
    if(edges[z].target>v_max_id) v_max_id=edges[z].target;
    PGR_DBG("%i <-> %i", v_min_id, v_max_id);
  }

  //::::::::::::::::::::::::::::::::::::
  //:: reducing vertex id (renumbering)
  //::::::::::::::::::::::::::::::::::::
  for(z=0; z<total_tuples; z++) {
    //check if edges[] contains source and target
    if(edges[z].source == source_vertex_id ||
       edges[z].target == source_vertex_id)
      ++s_count;
    if(edges[z].source == target_vertex_id ||
       edges[z].target == target_vertex_id)
      ++t_count;

    edges[z].source-=v_min_id;
    edges[z].target-=v_min_id;
    PGR_DBG("%i - %i", edges[z].source, edges[z].target);
  }

  PGR_DBG("Total %i tuples", total_tuples);

  if(s_count == 0) {
    elog(ERROR, "Start vertex was not found.");
    return -1;
  }

  if(t_count == 0) {
    elog(ERROR, "Target vertex was not found.");
    return -1;
  }

  PGR_DBG("Total %i tuples", total_tuples);

  profstop("extract", prof_extract);
  profstart(prof_astar);

  PGR_DBG("Calling bidir_astar <%i>\n", total_tuples);

  // calling C++ A* function
  ret = bdastar_wrapper(edges, total_tuples, v_max_id + 1, source_vertex_id-v_min_id,
            target_vertex_id-v_min_id,
            directed, has_reverse_cost,
            path, path_count, &err_msg);

  PGR_DBG("SIZE %i\n",*path_count);

  PGR_DBG("ret =  %i\n",ret);

  //::::::::::::::::::::::::::::::::
  //:: restoring original vertex id
  //::::::::::::::::::::::::::::::::
  for(z=0; z<*path_count; z++) {
    //PGR_DBG("vetex %i\n",(*path)[z].vertex_id);
    (*path)[z].vertex_id+=v_min_id;
  }

  profstop("astar", prof_astar);
  profstart(prof_store);

  if (ret < 0) {
      elog(ERROR, "Error computing path: %s", err_msg);
  }
  return finish(SPIcode, ret);
}


PG_FUNCTION_INFO_V1(bidir_astar_shortest_path);
Datum
bidir_astar_shortest_path(PG_FUNCTION_ARGS)
{
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  path_element_t      *path;

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      int path_count = 0;
#ifdef DEBUG
      int ret;
#endif

      // XXX profiling messages are not thread safe
      profstart(prof_total);
      profstart(prof_extract);

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


#ifdef DEBUG
      ret =
#endif
         compute_shortest_path_astar(text2char(PG_GETARG_TEXT_P(0)),
                    PG_GETARG_INT32(1),
                    PG_GETARG_INT32(2),
                    PG_GETARG_BOOL(3),
                    PG_GETARG_BOOL(4),
                    &path, &path_count);

#ifdef DEBUG
      PGR_DBG("Ret is %i", ret);
      if (ret >= 0) {
          int i;
          for (i = 0; i < path_count; i++) {
              PGR_DBG("Step # %i vertex_id  %i ", i, path[i].vertex_id);
              PGR_DBG("        edge_id    %i ", path[i].edge_id);
              PGR_DBG("        cost       %f ", path[i].cost);
          }
      }
#endif

      /* total number of tuples to be returned */
      PGR_DBG("Conting tuples number\n");
      funcctx->max_calls = path_count;
      funcctx->user_fctx = path;

      PGR_DBG("Path count %i", path_count);

      funcctx->tuple_desc =
            BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

      MemoryContextSwitchTo(oldcontext);
  }

  /* stuff done on every call of the function */
  PGR_DBG("Strange stuff doing\n");

  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  path = (path_element_t*) funcctx->user_fctx;

  PGR_DBG("Trying to allocate some memory\n");

  if (call_cntr < max_calls) {   /* do when there is more left to send */
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;

      values = palloc(4 * sizeof(Datum));
      nulls = palloc(4 * sizeof(char));

      values[0] = Int32GetDatum(call_cntr);
      nulls[0] = ' ';
      values[1] = Int32GetDatum(path[call_cntr].vertex_id);
      nulls[1] = ' ';
      values[2] = Int32GetDatum(path[call_cntr].edge_id);
      nulls[2] = ' ';
      values[3] = Float8GetDatum(path[call_cntr].cost);
      nulls[3] = ' ';

      PGR_DBG("Heap making\n");

      tuple = heap_formtuple(tuple_desc, values, nulls);

      PGR_DBG("Datum making\n");

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);


      PGR_DBG("Trying to free some memory\n");

      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
  }
  else {   /* do when there is no more left */
      PGR_DBG("Freeing path");
      if (path) free(path);

      profstop("store", prof_store);
      profstop("total", prof_total);
#ifdef PROFILE
      elog(NOTICE, "_________");
#endif
      SRF_RETURN_DONE(funcctx);
  }
}
