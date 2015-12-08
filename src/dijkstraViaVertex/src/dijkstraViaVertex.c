/*PGR-GNU*****************************************************************
File: dijkstraViaVertex.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo

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

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "utils/array.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

/*
  Uncomment when needed
*/
//#define DEBUG

#include "fmgr.h"
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./dijkstraViaVertex_driver.h"

PG_FUNCTION_INFO_V1(dijkstraViaVertex);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
dijkstraViaVertex(PG_FUNCTION_ARGS);

/*******************************************************************************/
/*                          MODIFY AS NEEDED                                   */
static
void
process( char* edges_sql,
        int64_t *via_vidsArr,
        size_t size_via_vidsArr,
        bool directed,
        bool strict,
        bool with_U_turns,
        Routes_t **result_tuples,
        size_t *result_count) {
  pgr_SPI_connect();

  PGR_DBG("Load data");
  pgr_edge_t *edges;
  edges = NULL;
  int64_t total_tuples;
  total_tuples = 0;
  pgr_get_data_5_columns(edges_sql, &edges, &total_tuples);

  if (total_tuples == 0) {
    PGR_DBG("No edges found");
    (*result_count) = 0;
    (*result_tuples) = NULL;
    pgr_SPI_finish();
    return;
  }
  PGR_DBG("Total %ld tuples in query:", total_tuples);

  PGR_DBG("Starting processing");
  char *err_msg = (char *)"";
  do_pgr_dijkstraViaVertex(
        edges,
        total_tuples,
        via_vidsArr,
        size_via_vidsArr,
        directed,
        strict,
        with_U_turns,
        result_tuples,
        result_count,
        &err_msg);
  PGR_DBG("Returning %ld tuples\n", *result_count);
  PGR_DBG("Returned message = %s\n", err_msg);

  free(err_msg);
  pfree(edges);
  pgr_SPI_finish();
}
/*                                                                             */
/*******************************************************************************/

#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
dijkstraViaVertex(PG_FUNCTION_ARGS) {
  FuncCallContext     *funcctx;
  size_t               call_cntr;
  size_t               max_calls;
  TupleDesc            tuple_desc;

  /*******************************************************************************/
  /*                          MODIFY AS NEEDED                                   */
  /*                                                                             */
  Routes_t  *result_tuples = 0;
  size_t result_count = 0;
  /*                                                                             */
  /*******************************************************************************/

  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      funcctx = SRF_FIRSTCALL_INIT();
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


  /*******************************************************************************/
  /*                          MODIFY AS NEEDED                                   */
      // CREATE OR REPLACE FUNCTION pgr_dijkstraViaVertices(sql text, vertices anyarray, directed boolean default true, strict boolean default false, with_U_turns boolean default false

      PGR_DBG("Initializing arrays");
      int64_t* via_vidsArr;
      size_t size_via_vidsArr;
      via_vidsArr = (int64_t*) pgr_get_bigIntArray(&size_via_vidsArr, PG_GETARG_ARRAYTYPE_P(1));
      PGR_DBG("Via VertexArr size %ld ", size_via_vidsArr);

      PGR_DBG("Calling process");
      process(
         pgr_text2char(PG_GETARG_TEXT_P(0)),
         via_vidsArr, size_via_vidsArr,
         PG_GETARG_BOOL(2),
         PG_GETARG_BOOL(3),
         PG_GETARG_BOOL(4),
         &result_tuples,
         &result_count);

      PGR_DBG("Cleaning arrays");
      free(via_vidsArr);
  /*                                                                             */
  /*******************************************************************************/

      funcctx->max_calls = result_count;
      funcctx->user_fctx = result_tuples;
      if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                            "that cannot accept type record")));

      funcctx->tuple_desc = tuple_desc;
      MemoryContextSwitchTo(oldcontext);
  }

  funcctx = SRF_PERCALL_SETUP();
  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  result_tuples = (Routes_t*) funcctx->user_fctx;

  if (call_cntr < max_calls) {
      HeapTuple    tuple;
      Datum        result;
      Datum        *values;
      char*        nulls;

  /*******************************************************************************/
  /*                          MODIFY AS NEEDED                                   */
      //    OUT seq BIGINT, OUT path_id BIGINT, OUT path_seq BIGINT, OUT start_vid BIGINT, OUT end_vid BIGINT, OUT node BIGINT, OUT edge BIGINT, OUT cost FLOAT, OUT agg_cost FLOAT, OUT route_agg_cost FLOAT)

      size_t numb_out = 10;
      values = palloc(numb_out * sizeof(Datum));
      nulls = palloc(numb_out * sizeof(char));
      size_t i;
      for(i = 0; i< numb_out; ++i) {
          nulls[i] = ' ';
      }

      // postgres starts counting from 1
      values[0] = Int64GetDatum(call_cntr + 1);
      values[1] = Int64GetDatum(result_tuples[call_cntr].path_id);
      values[2] = Int64GetDatum(result_tuples[call_cntr].path_seq + 1);
      values[3] = Int64GetDatum(result_tuples[call_cntr].start_vid);
      values[4] = Int64GetDatum(result_tuples[call_cntr].end_vid);
      values[5] = Int64GetDatum(result_tuples[call_cntr].node);
      values[6] = Int64GetDatum(result_tuples[call_cntr].edge);
      values[7] = Float8GetDatum(result_tuples[call_cntr].cost);
      values[8] = Float8GetDatum(result_tuples[call_cntr].agg_cost);
      values[9] = Float8GetDatum(result_tuples[call_cntr].route_agg_cost);

  /*******************************************************************************/

      tuple = heap_formtuple(tuple_desc, values, nulls);
      result = HeapTupleGetDatum(tuple);
      SRF_RETURN_NEXT(funcctx, result);
  } else {
      // cleanup
      if (result_tuples) free(result_tuples);

      SRF_RETURN_DONE(funcctx);
  }
}

