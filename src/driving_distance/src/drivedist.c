/*
 * Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2005 Sylvain Pasche
 *               2015 Celia Virginia Vergara Castillo
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

#include "fmgr.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./boost_interface_drivedist.h"


static int compute_driving_distance(char* sql, int64_t start_vertex,
                                 float8 distance, bool directed,
                                 bool has_rcost,
                                 pgr_path_element3_t **path, int *path_count) {
  int SPIcode;
  pgr_edge_t *edges = NULL;
  int64_t total_tuples = 0;


  char *err_msg = (char *)"";
  int ret = -1;

  PGR_DBG("Load data");

  SPIcode = pgr_get_data(sql, &edges, &total_tuples, has_rcost,
               start_vertex, start_vertex);

  if (SPIcode == -1) {
    PGR_DBG("Error getting data\n");
    return SPIcode;
  }

  PGR_DBG("Total %ld tuples in query:", total_tuples);

  ret = do_pgr_driving_distance(edges, total_tuples,
                        start_vertex, distance,
                        directed,
                        path, path_count, &err_msg);

  if (ret < 0) {
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
        errmsg("Error computing path: %s", err_msg)));
  }

  PGR_DBG("total tuples found %i\n", *path_count);
  PGR_DBG("Exist Status = %i\n", ret);
  PGR_DBG("Returned message = %s\n", err_msg);



  if (ret < 0) {
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
      errmsg("Error computing path: %s", err_msg)));
    }

  pfree(edges);
  return pgr_finish(SPIcode, ret);
}

#ifndef _MSC_VER
Datum driving_distance(PG_FUNCTION_ARGS);
#else  // _MSC_VER
PGDLLEXPORT Datum driving_distance(PG_FUNCTION_ARGS);
#endif  // _MSC_VER

PG_FUNCTION_INFO_V1(driving_distance);

#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
driving_distance(PG_FUNCTION_ARGS) {
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


      compute_driving_distance(pgr_text2char(
                                  PG_GETARG_TEXT_P(0)),       // sql
                                  PG_GETARG_INT64(1),         // source_id
                                  PG_GETARG_FLOAT8(2),        // distance
                                  PG_GETARG_BOOL(3),          // directed
                                  PG_GETARG_BOOL(4),          // has_rcost
                                  &ret_path, &path_count);

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

      values = palloc(5 * sizeof(Datum));
      nulls = palloc(5 * sizeof(char));

      values[0] = Int32GetDatum(ret_path[call_cntr].seq);
      nulls[0] = ' ';
      values[1] = Int64GetDatum(ret_path[call_cntr].vertex);
      nulls[1] = ' ';
      values[2] = Int64GetDatum(ret_path[call_cntr].edge);
      nulls[2] = ' ';
      values[3] = Float8GetDatum(ret_path[call_cntr].cost);
      nulls[3] = ' ';
      values[4] = Float8GetDatum(ret_path[call_cntr].tot_cost);
      nulls[4] = ' ';

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

