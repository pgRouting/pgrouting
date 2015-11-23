/*PGR
File: one_to_many_dijkstra.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo


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

*/

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
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./many_to_many_dijkstra_driver.h"

/*******************************************************************************/
/*                          MODIFY AS NEEDED                                   */
static
void
process( char* edges_sql,
        char * start_vids_sql,
        char * end_vids_sql,
        bool directed,
        General_path_element_t **result_tuples,
        size_t *result_count) {
  pgr_SPI_connect();

  PGR_DBG("Load data");
  pgr_edge_t *edges = NULL;
  int64_t total_tuples = 0;

  int64_t *start_vidsArr = NULL;
  size_t size_start_vidsArr;

  int64_t *end_vidsArr = NULL;
  size_t size_end_vidsArr;

  pgr_get_data_5_columns(edges_sql, &edges, &total_tuples);
  pgr_select_bigint(start_vids_sql, &start_vidsArr, &size_start_vidsArr);
  pgr_select_bigint(end_vids_sql, &end_vidsArr, &size_end_vidsArr);

  if (total_tuples == 0 || size_start_vidsArr == 0 ||  size_end_vidsArr == 0 ) {
    PGR_DBG("No edges found or no starting locations found");
    (*result_count) = 0;
    (*result_tuples) = NULL;
    pgr_SPI_finish();
    return;
  }

  PGR_DBG("Total %ld tuples in query:", total_tuples);

  PGR_DBG("Starting processing");
  char *err_msg = (char *)"";
  do_pgr_many_to_many_dijkstra(
        edges,
        total_tuples,
        start_vidsArr,
        size_start_vidsArr,
        end_vidsArr,
        size_end_vidsArr,
        directed,
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

PG_FUNCTION_INFO_V1(many_to_many_dijkstra_sql);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
many_to_many_dijkstra_sql(PG_FUNCTION_ARGS) {
  FuncCallContext     *funcctx;
  size_t              call_cntr;
  size_t               max_calls;
  TupleDesc            tuple_desc;

  /*******************************************************************************/
  /*                          MODIFY AS NEEDED                                   */
  /*                                                                             */
  General_path_element_t  *result_tuples = 0;
  size_t result_count = 0;
  /*                                                                             */
  /*******************************************************************************/

  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      funcctx = SRF_FIRSTCALL_INIT();
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


  /*******************************************************************************/
  /*                          MODIFY AS NEEDED                                   */
      // CREATE OR REPLACE FUNCTION pgr_dijkstra(sql text, start_vids_sql TEXT, end_vids_sql TEXT, directed boolean default true,


      PGR_DBG("Calling process");
      process(
         pgr_text2char(PG_GETARG_TEXT_P(0)),
         pgr_text2char(PG_GETARG_TEXT_P(1)),
         pgr_text2char(PG_GETARG_TEXT_P(2)),
         PG_GETARG_BOOL(3),
         &result_tuples,
         &result_count);

      PGR_DBG("Cleaning arrays");
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
  result_tuples = (General_path_element_t*) funcctx->user_fctx;

  if (call_cntr < max_calls) {
      HeapTuple    tuple;
      Datum        result;
      Datum        *values;
      char*        nulls;

  /*******************************************************************************/
  /*                          MODIFY AS NEEDED                                   */
      // OUT seq integer, OUT path_seq INTEGER, OUT end_vid BIGINT, OUT node bigint, OUT edge bigint, OUT cost float, OUT agg_cost float)

      values = palloc(8 * sizeof(Datum));
      nulls = palloc(8 * sizeof(char));
      size_t i;
      for(i = 0; i < 8; ++i) {
          nulls[i] = ' ';
      }


      values[0] = Int32GetDatum(call_cntr + 1);
      values[1] = Int32GetDatum(result_tuples[call_cntr].seq);
      values[2] = Int64GetDatum(result_tuples[call_cntr].from);
      values[3] = Int64GetDatum(result_tuples[call_cntr].to);
      values[4] = Int64GetDatum(result_tuples[call_cntr].vertex);
      values[5] = Int64GetDatum(result_tuples[call_cntr].edge);
      values[6] = Float8GetDatum(result_tuples[call_cntr].cost);
      values[7] = Float8GetDatum(result_tuples[call_cntr].tot_cost);

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

