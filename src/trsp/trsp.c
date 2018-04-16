/*PGR-GNU*****************************************************************

File: trsp.c

Generated with Template by:
Copyright (c) 2013 pgRouting developers
Mail: project@pgrouting.org

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

********************************************************************PGR-GNU*/


#include "c_common/postgres_connection.h"
#include "catalog/pg_type.h"
#include "c_common/debug_macro.h"


#if 0
#include "utils/array.h"
#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"
#endif

#include "c_types/trsp/trsp.h"

#if 0
PGDLLEXPORT Datum turn_restrict_shortest_path_vertex(PG_FUNCTION_ARGS);
#endif
PGDLLEXPORT Datum turn_restrict_shortest_path_edge(PG_FUNCTION_ARGS);

#if 0
#undef DEBUG
// #define DEBUG 1

#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, ...) do { ; } while (0)
#endif
#endif

#if 0
// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000

// #ifdef PG_MODULE_MAGIC
// PG_MODULE_MAGIC;
// #endif
#endif

typedef struct edge_columns {
  int id;
  int source;
  int target;
  int cost;
  int reverse_cost;
} edge_columns_t;

typedef struct restrict_columns {
  int target_id;
  int via_path;
  int to_cost;
} restrict_columns_t;



#if 0
static char *
text2char(text *in) {
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}
#endif

static int
finish(int code, int ret) {
  PGR_DBG("In finish, trying to disconnect from spi %d", ret);
  code = SPI_finish();
  if (code != SPI_OK_FINISH) {
    elog(ERROR, "couldn't disconnect from SPI");
    return -1;
  }
  return ret;
}

/*
 * This function fetches the resturction columns from an SPITupleTable..
 *
*/
static int
fetch_restrict_columns(SPITupleTable *tuptable,
                       restrict_columns_t *restrict_columns) {
  restrict_columns->target_id = SPI_fnumber(tuptable->tupdesc, "target_id");
  restrict_columns->via_path = SPI_fnumber(tuptable->tupdesc, "via_path");
  restrict_columns->to_cost =  SPI_fnumber(tuptable->tupdesc, "to_cost");
  if (restrict_columns->target_id == SPI_ERROR_NOATTRIBUTE ||
      restrict_columns->via_path == SPI_ERROR_NOATTRIBUTE ||
      restrict_columns->to_cost == SPI_ERROR_NOATTRIBUTE) {
    elog(ERROR, "Error, restriction query must return columns "
        "'target_id', 'via_path' and 'to_cost'");
    return -1;
  }

  if (SPI_gettypeid(tuptable->tupdesc,
       restrict_columns->target_id) != INT4OID ||
     SPI_gettypeid(tuptable->tupdesc, restrict_columns->via_path) != TEXTOID ||
     SPI_gettypeid(tuptable->tupdesc, restrict_columns->to_cost) != FLOAT8OID) {
    elog(ERROR, "Error, restriction columns 'target_id' must be of type int4,"
     "'via_path' must be of type text, 'to_cost' must be of type float8");
    return -1;
  }

  return 0;
}

/*
 * This function fetches the edge columns from the SPITupleTable.
 *
*/
static int
fetch_edge_columns(SPITupleTable *tuptable, edge_columns_t *edge_columns,
                   bool has_reverse_cost) {
  edge_columns->id = SPI_fnumber(tuptable->tupdesc, "id");
  edge_columns->source = SPI_fnumber(tuptable->tupdesc, "source");
  edge_columns->target = SPI_fnumber(tuptable->tupdesc, "target");
  edge_columns->cost = SPI_fnumber(tuptable->tupdesc, "cost");
  if (edge_columns->id == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->source == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->target == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->cost == SPI_ERROR_NOATTRIBUTE) {
      elog(ERROR, "Error, query must return columns "
           "'id', 'source', 'target' and 'cost'");
      return -1;
    }

  if (SPI_gettypeid(tuptable->tupdesc, edge_columns->source) != INT4OID ||
      SPI_gettypeid(tuptable->tupdesc, edge_columns->target) != INT4OID ||
      SPI_gettypeid(tuptable->tupdesc, edge_columns->cost) != FLOAT8OID) {
      elog(ERROR, "Error, columns 'source', 'target' must be of type int4, "
      "'cost' must be of type float8");
      return -1;
    }

  PGR_DBG("columns: id %i source %i target %i cost %i",
      edge_columns->id, edge_columns->source,
      edge_columns->target, edge_columns->cost);

  if (has_reverse_cost) {
      edge_columns->reverse_cost = SPI_fnumber(tuptable->tupdesc,
                                               "reverse_cost");

      if (edge_columns->reverse_cost == SPI_ERROR_NOATTRIBUTE) {
          elog(ERROR, "Error, reverse_cost is used, but query did't return "
               "'reverse_cost' column");
          return -1;
        }

      if (SPI_gettypeid(tuptable->tupdesc, edge_columns->reverse_cost)
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
           edge_columns_t *edge_columns, edge_t *target_edge) {
  Datum binval;
  bool isnull;

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->id, &isnull);
  if (isnull)
    elog(ERROR, "id contains a null value");
  target_edge->id = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->source, &isnull);
  if (isnull)
    elog(ERROR, "source contains a null value");
  target_edge->source = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->target, &isnull);
  if (isnull)
    elog(ERROR, "target contains a null value");
  target_edge->target = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->cost, &isnull);
  if (isnull)
    elog(ERROR, "cost contains a null value");
  target_edge->cost = DatumGetFloat8(binval);

  if (edge_columns->reverse_cost != -1) {
      binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->reverse_cost,
                             &isnull);
      if (isnull)
        elog(ERROR, "reverse_cost contains a null value");
      target_edge->reverse_cost =  DatumGetFloat8(binval);
    }

  /*
  PGR_DBG("edge: %i, %i, %i, %f, %f", target_edge->id, target_edge->source,
    target_edge->target, target_edge->cost, target_edge->reverse_cost);
  */
}


/*
 * To fetch a edge from Tuple.
 *
 */

static void
fetch_restrict(HeapTuple *tuple, TupleDesc *tupdesc,
           restrict_columns_t *restrict_columns, restrict_t *rest) {
  Datum binval;
  bool isnull;
  int t;

  for (t = 0; t < MAX_RULE_LENGTH; ++t)
    rest->via[t] = -1;

  binval = SPI_getbinval(*tuple, *tupdesc, restrict_columns->target_id,
       &isnull);
  if (isnull)
    elog(ERROR, "target_id contains a null value");
  rest->target_id = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, restrict_columns->to_cost, &isnull);
  if (isnull)
    elog(ERROR, "to_cost contains a null value");
  rest->to_cost = DatumGetFloat8(binval);
  char *str = DatumGetCString(SPI_getvalue(*tuple, *tupdesc,
       restrict_columns->via_path));

  // PGR_DBG("restriction: %f, %i, %s", rest->to_cost, rest->target_id, str);

  if (str != NULL) {
    char* pch = NULL;
    int ci = 0;

    pch = (char *)strtok(str, " ,");

    while (pch != NULL && ci < MAX_RULE_LENGTH) {
      rest->via[ci] = atoi(pch);
      // PGR_DBG("    rest->via[%i]=%i", ci, rest->via[ci]);
      ci++;
      pch = (char *)strtok(NULL, " ,");
    }
  }
}



static int compute_trsp(
    char* sql,
    int dovertex,
    int64_t start_id,
    double start_pos,
    int64_t end_id,
    double end_pos,
    bool directed,
    bool has_reverse_cost,
    char* restrict_sql,
    path_element_tt **path,
    size_t *path_count) {

  int SPIcode;
  SPIPlanPtr SPIplan;
  Portal SPIportal;
  bool moredata = true;
  uint32_t TUPLIMIT = 1000;
  uint32_t ntuples;

  edge_t *edges = NULL;
  uint32_t total_tuples = 0;
#ifndef _MSC_VER
  edge_columns_t edge_columns = {.id = -1, .source = -1, .target = -1,
                                 .cost = -1, .reverse_cost = -1};
#else  // _MSC_VER
  edge_columns_t edge_columns = {-1, -1, -1, -1, -1};
#endif  // _MSC_VER
  restrict_t *restricts = NULL;
  uint32_t total_restrict_tuples = 0;
  restrict_columns_t restrict_columns = {.target_id = -1, .via_path = -1,
                                 .to_cost = -1};
  int64_t v_max_id = 0;
  int64_t v_min_id = INT_MAX;

  /* track if start and end are both in edge tuples */
  int s_count = 0;
  int t_count = 0;

  char *err_msg;
  int ret = -1;
  uint32_t z;

  PGR_DBG("start turn_restrict_shortest_path\n");

  SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT) {
      elog(ERROR, "turn_restrict_shortest_path: "
      "couldn't open a connection to SPI");
      return -1;
  }

  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL) {
      elog(ERROR, "turn_restrict_shortest_path: "
      "couldn't create query plan via SPI");
      return -1;
  }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
      elog(ERROR, "turn_restrict_shortest_path: "
      "SPI_cursor_open('%s') returns NULL", sql);
      return -1;
  }

  while (moredata == true) {
      // PGR_DBG("calling SPI_cursor_fetch");
      SPI_cursor_fetch(SPIportal, true, TUPLIMIT);

      if (SPI_tuptable == NULL) {
          elog(ERROR, "SPI_tuptable is NULL");
          return finish(SPIcode, -1);
      }

      if (edge_columns.id == -1) {
          if (fetch_edge_columns(SPI_tuptable, &edge_columns,
                                 has_reverse_cost) == -1)
                return finish(SPIcode, ret);
      }

      ntuples = SPI_processed;

      // PGR_DBG("Reading edges: %i - %i", total_tuples, total_tuples+ntuples);

      total_tuples += ntuples;

      if (ntuples > 0) {
          if (!edges)
            edges = palloc(total_tuples * sizeof(edge_t));
          else
            edges = repalloc(edges, total_tuples * sizeof(edge_t));

          if (edges == NULL) {
              elog(ERROR, "Out of memory");
              return finish(SPIcode, ret);
          }

          uint32_t t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          for (t = 0; t < ntuples; t++) {
              // if (t%100 == 0) { PGR_DBG("    t: %i", t); }
              HeapTuple tuple = tuptable->vals[t];
              fetch_edge(&tuple, &tupdesc, &edge_columns,
                         &edges[total_tuples - ntuples + t]);
          }
          // PGR_DBG("calling SPI_freetuptable");
          SPI_freetuptable(tuptable);
          // PGR_DBG("back from SPI_freetuptable");
      } else {
          moredata = false;
      }
  }
  SPI_cursor_close(SPIportal);

  // defining min and max vertex id

  // DBG("Total %i edge tuples", total_tuples);

  for (z = 0; z < total_tuples; z++) {
    if (edges[z].source < v_min_id)
      v_min_id = edges[z].source;

    if (edges[z].source > v_max_id)
      v_max_id = edges[z].source;

    if (edges[z].target < v_min_id)
      v_min_id = edges[z].target;

    if (edges[z].target > v_max_id)
      v_max_id = edges[z].target;

    // DBG("%i <-> %i", v_min_id, v_max_id);
  }

  // ::::::::::::::::::::::::::::::::::::
  // :: reducing vertex id (renumbering)
  // ::::::::::::::::::::::::::::::::::::
  for (z = 0; z < total_tuples; z++) {
    // check if edges[] contains source and target
    if (dovertex) {
        if (edges[z].source == start_id || edges[z].target == start_id)
          ++s_count;
        if (edges[z].source == end_id || edges[z].target == end_id)
          ++t_count;
    } else {
        if (edges[z].id == start_id)
          ++s_count;
        if (edges[z].id == end_id)
          ++t_count;
    }

    edges[z].source -= v_min_id;
    edges[z].target -= v_min_id;
    edges[z].cost = edges[z].cost;
    // PGR_DBG("edgeID: %i SRc:%i - %i, cost: %f",
    // edges[z].id,edges[z].source, edges[z].target,edges[z].cost);
  }

  PGR_DBG("Min vertex id: %ld , Max vid: %ld", v_min_id, v_max_id);
  PGR_DBG("Total %i edge tuples", total_tuples);

  if (s_count == 0) {
    elog(ERROR, "Start id was not found.");
    return -1;
  }

  if (t_count == 0) {
    elog(ERROR, "Target id was not found.");
    return -1;
  }

  if (dovertex) {
      start_id -= v_min_id;
      end_id   -= v_min_id;
  }

  PGR_DBG("Fetching restriction tuples\n");

  if (restrict_sql == NULL) {
      PGR_DBG("Sql for restrictions is null.");
  } else {
      SPIplan = SPI_prepare(restrict_sql, 0, NULL);
      if (SPIplan  == NULL) {
          elog(ERROR, "turn_restrict_shortest_path: "
          "couldn't create query plan via SPI");
          return -1;
      }

      if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) \
       == NULL) {
          elog(ERROR, "turn_restrict_shortest_path:"
          " SPI_cursor_open('%s') returns NULL", restrict_sql);
          return -1;
      }

      moredata = true;
      while (moredata == true) {
          SPI_cursor_fetch(SPIportal, true, TUPLIMIT);

          if (restrict_columns.target_id == -1) {
              if (fetch_restrict_columns(SPI_tuptable, &restrict_columns) \
              == -1) {
                PGR_DBG("fetch_restrict_columns failed!");
                return finish(SPIcode, ret);
              }
          }

          ntuples = SPI_processed;
          total_restrict_tuples += ntuples;

          // DBG("Reading Restrictions: %i", total_restrict_tuples);

          if (ntuples > 0) {
              if (!restricts)
                restricts = palloc(total_restrict_tuples * sizeof(restrict_t));
              else
                restricts = repalloc(restricts,
                    total_restrict_tuples * sizeof(restrict_t));

              if (restricts == NULL) {
                  elog(ERROR, "Out of memory");
                  return finish(SPIcode, ret);
              }

              uint32_t t;
              SPITupleTable *tuptable = SPI_tuptable;
              TupleDesc tupdesc = SPI_tuptable->tupdesc;

              for (t = 0; t < ntuples; t++) {
                  HeapTuple tuple = tuptable->vals[t];
                  fetch_restrict(&tuple, &tupdesc, &restrict_columns,
                             &restricts[total_restrict_tuples - ntuples + t]);
              }
              SPI_freetuptable(tuptable);
          } else {
              moredata = false;
          }
      }
      SPI_cursor_close(SPIportal);
  }

#ifdef DEBUG_OFF
    int t;
    for (t=0; t < total_restrict_tuples; t++) {
        PGR_DBG("restricts: %.2f, %i, %i, %i, %i, %i, %i",
        restricts[t].to_cost, restricts[t].target_id, restricts[t].via[0],
        restricts[t].via[1], restricts[t].via[2], restricts[t].via[3],
        restricts[t].via[4]);
    }
#endif

  PGR_DBG("Total %i restriction tuples", total_restrict_tuples);

  if (dovertex) {
      PGR_DBG("Calling trsp_node_wrapper\n");
      /** hack always returns 0 -1 when
      installed on EDB VC++ 64-bit without this **/
      #if defined(__MINGW64__)
        // elog(NOTICE,"Calling trsp_node_wrapper\n");
      #endif
      ret = trsp_node_wrapper(edges, total_tuples,
                        restricts, total_restrict_tuples,
                        start_id, end_id,
                        directed, has_reverse_cost,
                        path, path_count, &err_msg);
  } else {
      PGR_DBG("Calling trsp_edge_wrapper\n");
      ret = trsp_edge_wrapper(edges, total_tuples,
                        restricts, total_restrict_tuples,
                        start_id, start_pos, end_id, end_pos,
                        directed, has_reverse_cost,
                        path, path_count, &err_msg);
  }

  PGR_DBG("Message received from inside:");
  PGR_DBG("%s", err_msg);

  // DBG("SIZE %i\n",*path_count);

  // ::::::::::::::::::::::::::::::::
  // :: restoring original vertex id
  // ::::::::::::::::::::::::::::::::
  for (z = 0; z < *path_count; z++) {
    // PGR_DBG("vetex %i\n",(*path)[z].vertex_id);
    if (z || (*path)[z].vertex_id != -1)
        (*path)[z].vertex_id += v_min_id;
  }

  PGR_DBG("ret = %i\n", ret);

  PGR_DBG("*path_count = %ld\n", *path_count);

  if (ret < 0) {
      // elog(ERROR, "Error computing path: %s", err_msg);
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
        errmsg("Error computing path: %s", err_msg)));
    }

  return finish(SPIcode, ret);
}


#if 0
PG_FUNCTION_INFO_V1(turn_restrict_shortest_path_vertex);
PGDLLEXPORT Datum
turn_restrict_shortest_path_vertex(PG_FUNCTION_ARGS) {
  FuncCallContext     *funcctx;
#if 0
  uint32_t             call_cntr;
  int                  max_calls;
#endif
  TupleDesc            tuple_desc;
  path_element_tt      *path;
  char *               sql;


  // stuff done only on the first call of the function
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      size_t path_count = 0;

      int ret = -1;
      if (ret == -1) {}  // to avoid warning set but not used

      int i;

      // create a function context for cross-call persistence
      funcctx = SRF_FIRSTCALL_INIT();

      // switch to memory context appropriate for multiple function calls
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      // verify that the first 5 args are not NULL
      for (i = 0; i < 5; i++)
        if (PG_ARGISNULL(i)) {
            elog(ERROR, "turn_restrict_shortest_path(): "
            "Argument %i may not be NULL", i+1);
        }

      if (PG_ARGISNULL(5)) {
        sql = NULL;
      } else {
        sql = text_to_cstring(PG_GETARG_TEXT_P(5));
        if (strlen(sql) == 0)
            sql = NULL;
      }

      PGR_DBG("Calling compute_trsp");


      ret = compute_trsp(text_to_cstring(PG_GETARG_TEXT_P(0)),
                                   1,  // do vertex
                                   PG_GETARG_INT32(1),
                                   0.5,
                                   PG_GETARG_INT32(2),
                                   0.5,
                                   PG_GETARG_BOOL(3),
                                   PG_GETARG_BOOL(4),
                                   sql,
                                   &path, &path_count);
#ifdef DEBUG
      double total_cost = 0;
      PGR_DBG("Ret is %i", ret);
      if (ret >= 0) {
          int i;
          for (i = 0; i < path_count; i++) {
         // PGR_DBG("Step %i vertex_id  %i ", i, path[i].vertex_id);
           // PGR_DBG("        edge_id    %i ", path[i].edge_id);
             // PGR_DBG("        cost       %f ", path[i].cost);
              total_cost += path[i].cost;
            }
        }
        PGR_DBG("Total cost is: %f", total_cost);
#endif

      // total number of tuples to be returned
#if 1
#if PGSQL_VERSION > 95
        funcctx->max_calls = path_count;
#else
        funcctx->max_calls = (uint32_t)path_count;
#endif
#else
      funcctx->max_calls = path_count;
#endif
      funcctx->user_fctx = path;

      funcctx->tuple_desc =
        BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

      MemoryContextSwitchTo(oldcontext);
    }

  // stuff done on every call of the function
  funcctx = SRF_PERCALL_SETUP();

#if 0
  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
#endif
  tuple_desc = funcctx->tuple_desc;
  path = (path_element_tt*) funcctx->user_fctx;

  if (funcctx->call_cntr < funcctx->max_calls) {
      // do when there is more left to send
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      bool* nulls;

      values = palloc(4 * sizeof(Datum));
      nulls = palloc(4 * sizeof(char));

      values[0] = Int32GetDatum(funcctx->call_cntr);
      nulls[0] = false;
      values[1] = Int32GetDatum(path[funcctx->call_cntr].vertex_id);
      nulls[1] = false;
      values[2] = Int32GetDatum(path[funcctx->call_cntr].edge_id);
      nulls[2] = false;
      values[3] = Float8GetDatum(path[funcctx->call_cntr].cost);
      nulls[3] = false;

      tuple = heap_form_tuple(tuple_desc, values, nulls);

      // make the tuple into a datum
      result = HeapTupleGetDatum(tuple);

      // clean up (this is not really necessary)
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
  } else {  // do when there is no more left
      PGR_DBG("Going to free path");
      if (path) free(path);
      SRF_RETURN_DONE(funcctx);
    }
}
#endif

PG_FUNCTION_INFO_V1(turn_restrict_shortest_path_edge);
PGDLLEXPORT Datum
turn_restrict_shortest_path_edge(PG_FUNCTION_ARGS) {
  FuncCallContext     *funcctx;
#if 0
  uint32_t             call_cntr;
  uint32_t             max_calls;
#endif
  TupleDesc            tuple_desc;
  path_element_tt      *path;
  char *               sql;

  // stuff done only on the first call of the function
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      size_t path_count = 0;
#ifdef DEBUG
      int ret = -1;
#endif
      int i;
      double s_pos;
      double e_pos;

      // create a function context for cross-call persistence
      funcctx = SRF_FIRSTCALL_INIT();

      // switch to memory context appropriate for multiple function calls
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      // verify that the first 5 args are not NULL
      for (i = 0; i < 7; i++) {
        if (i == 2 || i == 4) continue;
        if (PG_ARGISNULL(i)) {
            elog(ERROR, "turn_restrict_shortest_path(): "
            "Argument %i may not be NULL", i+1);
        }
      }

      if (PG_ARGISNULL(2)) {
        s_pos = 0.5;
      } else {
        s_pos = PG_GETARG_FLOAT8(2);
        if (s_pos < 0.0) s_pos = 0.5;
        if (s_pos > 1.0) s_pos = 0.5;
      }

      if (PG_ARGISNULL(4)) {
        e_pos = 0.5;
      } else {
        e_pos = PG_GETARG_FLOAT8(4);
        if (e_pos < 0.0) e_pos = 0.5;
        if (e_pos > 1.0) e_pos = 0.5;
      }

      if (PG_ARGISNULL(7)) {
        sql = NULL;
      } else {
        sql = text_to_cstring(PG_GETARG_TEXT_P(7));
        if (strlen(sql) == 0)
            sql = NULL;
      }

      PGR_DBG("Calling compute_trsp");

#ifdef DEBUG
      ret =
#endif
         compute_trsp(text_to_cstring(PG_GETARG_TEXT_P(0)),
                                   0,  // sdo edge
                                   PG_GETARG_INT32(1),
                                   s_pos,
                                   PG_GETARG_INT32(3),
                                   e_pos,
                                   PG_GETARG_BOOL(5),
                                   PG_GETARG_BOOL(6),
                                   sql,
                                   &path, &path_count);
#ifdef DEBUG
      double total_cost = 0;
      PGR_DBG("Ret is %i", ret);
      if (ret >= 0) {
          int i;
          for (i = 0; i < path_count; i++) {
         //     PGR_DBG("Step %i vertex_id  %i ", i, path[i].vertex_id);
           //   PGR_DBG("        edge_id    %i ", path[i].edge_id);
             // PGR_DBG("        cost       %f ", path[i].cost);
              total_cost+=path[i].cost;
            }
        }
        PGR_DBG("Total cost is: %f", total_cost);
#endif

      // total number of tuples to be returned
#if 1
#if PGSQL_VERSION > 95
        funcctx->max_calls = path_count;
#else
        funcctx->max_calls = (uint32_t)path_count;
#endif
#else
      funcctx->max_calls = path_count;
#endif
      funcctx->user_fctx = path;

      funcctx->tuple_desc =
        BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

      MemoryContextSwitchTo(oldcontext);
    }

  // stuff done on every call of the function
  funcctx = SRF_PERCALL_SETUP();

#if 0
  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
#endif
  tuple_desc = funcctx->tuple_desc;
  path = (path_element_tt*) funcctx->user_fctx;

  if (funcctx->call_cntr < funcctx->max_calls) {
      // do when there is more left to send
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      bool* nulls;

      values = palloc(4 * sizeof(Datum));
      nulls = palloc(4 * sizeof(char));

      values[0] = Int32GetDatum(funcctx->call_cntr);
      nulls[0] = false;
      values[1] = Int32GetDatum(path[funcctx->call_cntr].vertex_id);
      nulls[1] = false;
      values[2] = Int32GetDatum(path[funcctx->call_cntr].edge_id);
      nulls[2] = false;
      values[3] = Float8GetDatum(path[funcctx->call_cntr].cost);
      nulls[3] = false;

      tuple = heap_form_tuple(tuple_desc, values, nulls);

      // make the tuple into a datum
      result = HeapTupleGetDatum(tuple);

      // clean up (this is not really necessary)
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
    } else {  // do when there is no more left
      PGR_DBG("Going to free path");
      if (path) free(path);
      SRF_RETURN_DONE(funcctx);
    }
}
