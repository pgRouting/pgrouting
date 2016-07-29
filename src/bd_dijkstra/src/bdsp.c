/*PGR-MIT*****************************************************************

* $Id$
*
* Project: pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
Copyright (c) 2015 pgRouting developers

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-MIT*/


#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"



#include "bdsp_driver.h"


#undef DEBUG
#include "../../common/src/debug_macro.h"
#include "../../common/src/pgr_types.h"
#include "../../common/src/postgres_connection.h"

// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000

PGDLLEXPORT Datum bidir_dijkstra_shortest_path(PG_FUNCTION_ARGS);

typedef struct edge_columns
{
  int id;
  int source;
  int target;
  int cost;
  int reverse_cost;
} edge_columns_t;



/*
 * This function fetches the edge columns from the SPITupleTable.
 *
*/
static int
fetch_edge_columns(SPITupleTable *tuptable, edge_columns_t *edge_columns,
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

  if (SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->source) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->target) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->cost) != FLOAT8OID) {

      elog(ERROR, "Error, columns 'source', 'target' must be of type int4, 'cost' must be of type float8");
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

      if (SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->reverse_cost)
            != FLOAT8OID) {
          elog(ERROR, "Error, columns 'reverse_cost' must be of type float8");
          return -1;
      }

      PGR_DBG("columns: reverse_cost cost %i", edge_columns->reverse_cost);
  }

  return 0;
}

/*
 * To fetch a edge from Tuple.
 *
 */

static void
fetch_edge(HeapTuple *tuple, TupleDesc *tupdesc,
           edge_columns_t *edge_columns, edge_t *target_edge)
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
      binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->reverse_cost,
                             &isnull);
      if (isnull) elog(ERROR, "reverse_cost contains a null value");
      target_edge->reverse_cost = DatumGetFloat8(binval);
  }
}

static int compute_bidirsp(char* sql, int64_t start_vertex,
                                 int64_t end_vertex, bool directed,
                                 bool has_reverse_cost,
                                 path_element_t **path, int *path_count)
{
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  uint32_t ntuples;
  edge_t *edges = NULL;
  uint32_t total_tuples = 0;
#ifndef _MSC_VER
  edge_columns_t edge_columns = {.id = -1, .source = -1, .target = -1,
                                 .cost = -1, .reverse_cost = -1};
#else  //  _MSC_VER
  edge_columns_t edge_columns = {-1, -1, -1, -1, -1};
#endif  //  _MSC_VER
  int64_t v_max_id = 0;
  int64_t v_min_id = INT_MAX;

  int s_count = 0;
  int t_count = 0;

  char *err_msg;
  int ret = -1;
  int64_t z;

  PGR_DBG("start shortest_path\n");

  pgr_SPI_connect();
  SPIplan = pgr_SPI_prepare(sql);
  SPIportal = pgr_SPI_cursor_open(SPIplan);

  while (moredata == TRUE) {
      SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

      if (edge_columns.id == -1) {
          if (fetch_edge_columns(SPI_tuptable, &edge_columns,
                                 has_reverse_cost) == -1) {
            pgr_SPI_finish();
            return -1;
          }
      }

      ntuples = SPI_processed;
      total_tuples += ntuples;
      if (!edges)
        edges = palloc(total_tuples * sizeof(edge_t));
      else
        edges = repalloc(edges, total_tuples * sizeof(edge_t));

      if (edges == NULL) {
          elog(ERROR, "Out of memory");
            pgr_SPI_finish();
            return -1;
      }

      if (ntuples > 0) {
          uint64_t t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          for (t = 0; t < ntuples; t++) {
              HeapTuple tuple = tuptable->vals[t];
              fetch_edge(&tuple, &tupdesc, &edge_columns,
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

  for (z = 0; z<total_tuples; z++) {
    if (edges[z].source<v_min_id) v_min_id = edges[z].source;
    if (edges[z].source>v_max_id) v_max_id = edges[z].source;
    if (edges[z].target<v_min_id) v_min_id = edges[z].target;
    if (edges[z].target>v_max_id) v_max_id = edges[z].target;
    //PGR_DBG("%i <-> %i", v_min_id, v_max_id);
  }

  //::::::::::::::::::::::::::::::::::::
  //:: reducing vertex id (renumbering)
  //::::::::::::::::::::::::::::::::::::
  for (z = 0; z<total_tuples; z++) {
    //check if edges[] contains source and target
    if (edges[z].source == start_vertex ||  edges[z].target == start_vertex)
      ++s_count;
    if (edges[z].source == end_vertex ||  edges[z].target == end_vertex)
      ++t_count;

    edges[z].source -= v_min_id;
    edges[z].target -= v_min_id;
    //PGR_DBG("%i - %i", edges[z].source, edges[z].target);
  }

  PGR_DBG("Total %i tuples", total_tuples);

  if (s_count == 0) {
    elog(ERROR, "Start vertex was not found.");
    return -1;
  }

  if (t_count == 0) {
    elog(ERROR, "Target vertex was not found.");
    return -1;
  }

  start_vertex -= v_min_id;
  end_vertex   -= v_min_id;

  //v_max_id -= v_min_id;

  PGR_DBG("Calling bidirsp_wrapper(edges, %u, %ld, %ld, %ld, %d, %d, ...)\n",
        total_tuples, v_max_id + 2, start_vertex, end_vertex,
        directed, has_reverse_cost);

  ret = bidirsp_wrapper(edges, total_tuples, (int)v_max_id + 2, (int)start_vertex, (int)end_vertex,
                       directed, has_reverse_cost,
                       path, path_count, &err_msg);

  PGR_DBG("Back from bidirsp_wrapper() ret: %d", ret);
  if (ret < 0) {
      elog(ERROR, "Error computing path: %s", err_msg);
  }

  PGR_DBG("*path_count = %i\n", *path_count);

  //::::::::::::::::::::::::::::::::
  //:: restoring original vertex id
  //::::::::::::::::::::::::::::::::
  for (z = 0; z<*path_count; z++) {
    //PGR_DBG("vetex %i\n", (*path)[z].vertex_id);
    (*path)[z].vertex_id+= v_min_id;
  }

  PGR_DBG("ret = %i\n", ret);

  pgr_SPI_finish();
  return 0;
}


PG_FUNCTION_INFO_V1(bidir_dijkstra_shortest_path);
PGDLLEXPORT Datum
bidir_dijkstra_shortest_path(PG_FUNCTION_ARGS)
{

  FuncCallContext     *funcctx;
  uint32_t                  call_cntr;
  uint32_t                  max_calls;
  TupleDesc            tuple_desc;
  path_element_t      *path;
  // char *               sql;


  // stuff done only on the first call of the function
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      int path_count = 0;
#ifdef DEBUG
      int ret = -1;
#endif
      int i;

      // create a function context for cross-call persistence
      funcctx = SRF_FIRSTCALL_INIT();

      // switch to memory context appropriate for multiple function calls
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      // verify that the first 5 args are not NULL
      for (i = 0; i<5; i++)
        if (PG_ARGISNULL(i)) {
            elog(ERROR, "bidir_dijkstra_shortest_path(): Argument %i may not be NULL", i+1);
        }

      PGR_DBG("Calling compute_bidirsp");

#ifdef DEBUG
      ret =
#endif
        compute_bidirsp(pgr_text2char(PG_GETARG_TEXT_P(0)),
                                   (int64_t)PG_GETARG_INT32(1),
                                   (int64_t)PG_GETARG_INT32(2),
                                   PG_GETARG_BOOL(3),
                                   PG_GETARG_BOOL(4),
                                   &path, &path_count);
#ifdef DEBUG
    double total_cost = 0;
      PGR_DBG("Ret is %i", ret);
      if (ret >= 0) {
          int i;
          for (i = 0; i < path_count; i++) {
             // PGR_DBG("Step %i vertex_id %i ", i, path[i].vertex_id);
             // PGR_DBG("        edge_id    %i ", path[i].edge_id);
             // PGR_DBG("        cost       %f ", path[i].cost);
              total_cost+= path[i].cost;
            }
        }
        PGR_DBG("Total cost is: %f", total_cost);
#endif

      // total number of tuples to be returned
      funcctx->max_calls = (uint32_t)path_count;
      funcctx->user_fctx = path;

      funcctx->tuple_desc =
          BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

      MemoryContextSwitchTo(oldcontext);
  }

  // stuff done on every call of the function
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  path = (path_element_t*) funcctx->user_fctx;

  if (call_cntr < max_calls) {   // do when there is more left to send
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      bool* nulls;

      values = palloc(4 * sizeof(Datum));
      nulls = palloc(4 * sizeof(bool));

      values[0] = Int32GetDatum(call_cntr);
      nulls[0] = false;
      values[1] = Int32GetDatum(path[call_cntr].vertex_id);
      nulls[1] = false;
      values[2] = Int32GetDatum(path[call_cntr].edge_id);
      nulls[3] = false;
      values[3] = Float8GetDatum(path[call_cntr].cost);
      nulls[3] = false;

      tuple = heap_form_tuple(tuple_desc, values, nulls);

      // make the tuple into a datum
      result = HeapTupleGetDatum(tuple);

      // clean up (this is not really necessary)
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
  }
  else {   // do when there is no more left
      PGR_DBG("Going to free path");
      if (path) free(path);
      SRF_RETURN_DONE(funcctx);
  }
}
