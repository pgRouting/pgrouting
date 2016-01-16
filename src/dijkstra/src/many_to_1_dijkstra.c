/*pgRouting
 *
 * File: many_to_1_dijkstra.c
 * Copyright (c) 2015 Celia Virginia Vergara Castillo
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
// #define DEBUG
#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "utils/array.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./dijkstra_driver.h"

#ifndef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


static int dijkstra_many_to_1_driver(
          char* sql, int64_t *start_vertex, int num,
          int64_t end_vertex,
          bool directed, bool has_rcost,
          pgr_path_element3_t **path, int *path_count) {
  int SPIcode;
  pgr_edge_t *edges = NULL;
  int64_t total_tuples = 0;


  char *err_msg = (char *)"";
  int ret = -1;


  SPIcode = pgr_get_data(sql, &edges, &total_tuples, has_rcost,
               end_vertex, end_vertex);

  if (SPIcode == -1) {
    return SPIcode;
  }

  ret = do_pgr_dijkstra_many_to_1(edges, total_tuples,
                        start_vertex, num, end_vertex,
                        has_rcost, directed,
                        path, path_count, &err_msg);

  if (ret < 0) {
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
        errmsg("Error computing path: %s", err_msg)));
  }

  pfree(edges);
  return pgr_finish(SPIcode, ret);
}


#ifndef _MSC_VER
Datum dijkstra_many_to_1(PG_FUNCTION_ARGS);
#else  // _MSC_VER
PGDLLEXPORT Datum dijkstra_many_to_1(PG_FUNCTION_ARGS);
#endif  // _MSC_VER


PG_FUNCTION_INFO_V1(dijkstra_many_to_1);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
dijkstra_many_to_1(PG_FUNCTION_ARGS) {
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  pgr_path_element3_t  *ret_path = 0;

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      int path_count = 0;

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      int64_t* sourcesArr;
      int num;

      sourcesArr = (int64_t*) pgr_get_bigIntArray(&num, PG_GETARG_ARRAYTYPE_P(1));
      PGR_DBG("sourcesArr size %d ", num);

      PGR_DBG("Calling dijkstra_many_to_1_driver");
      dijkstra_many_to_1_driver(
               pgr_text2char(PG_GETARG_TEXT_P(0)),
               sourcesArr, num,
               PG_GETARG_INT64(2),
               PG_GETARG_BOOL(3),
               PG_GETARG_BOOL(4), &ret_path, &path_count);

      free(sourcesArr);

      /* total number of tuples to be returned */
      funcctx->max_calls = path_count;
      funcctx->user_fctx = ret_path;

      if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                            "that cannot accept type record")));

      funcctx->tuple_desc = tuple_desc;

      MemoryContextSwitchTo(oldcontext);
  }

  /* stuff done on every call of the function */
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  ret_path = (pgr_path_element3_t*) funcctx->user_fctx;

  /* do when there is more left to send */
  if (call_cntr < max_calls) {
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;

      values = palloc(7 * sizeof(Datum));
      nulls = palloc(7 * sizeof(char));
      // id, start_v, node, edge, cost, tot_cost
      values[0] = Int32GetDatum(call_cntr + 1);
      nulls[0] = ' ';
      values[1] = Int32GetDatum(ret_path[call_cntr].seq);
      nulls[1] = ' ';
      values[2] = Int64GetDatum(ret_path[call_cntr].from);
      nulls[2] = ' ';
      values[3] = Int64GetDatum(ret_path[call_cntr].vertex);
      nulls[3] = ' ';
      values[4] = Int64GetDatum(ret_path[call_cntr].edge);
      nulls[4] = ' ';
      values[5] = Float8GetDatum(ret_path[call_cntr].cost);
      nulls[5] = ' ';
      values[6] = Float8GetDatum(ret_path[call_cntr].tot_cost);
      nulls[6] = ' ';

      tuple = heap_formtuple(tuple_desc, values, nulls);

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
  } else {
      /* do when there is no more left */
      if (ret_path) free(ret_path);
      SRF_RETURN_DONE(funcctx);
  }
}

