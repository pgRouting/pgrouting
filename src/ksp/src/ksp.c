/*PGR-GNU*****************************************************************
File: ksp.c

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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
#include "funcapi.h"
#include "catalog/pg_type.h"
#include "fmgr.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/time_msg.h"
#include "./../../common/src/edges_input.h"
#include "./ksp_driver.h"

PGDLLEXPORT Datum kshortest_path(PG_FUNCTION_ARGS);


static
void compute(char* sql, int64_t start_vertex,
         int64_t end_vertex, int k,
         bool directed,
         bool heap_paths,
         General_path_element_t **ksp_path, size_t *path_count) {
  pgr_SPI_connect();
  pgr_edge_t *edges = NULL;
  size_t total_tuples = 0;

  char *err_msg = NULL;

  if (start_vertex == end_vertex) {
    (*path_count) = 0;
    *ksp_path = NULL;
    pgr_SPI_finish();
    return;
  }

  PGR_DBG("Load data");
  pgr_get_edges(sql, &edges, &total_tuples);

  PGR_DBG("Total %ld tuples in query:", total_tuples);
  PGR_DBG("Calling do_pgr_ksp\n");
  PGR_DBG("heap_paths = %i\n", heap_paths);

  clock_t start_t = clock();
  int errcode = do_pgr_ksp(edges, total_tuples,
            start_vertex, end_vertex,
            k, directed, heap_paths,
            ksp_path, path_count, &err_msg);
  time_msg(" processing KSP", start_t, clock());

  PGR_DBG("total tuples found %ld\n", *path_count);
  PGR_DBG("Exit Status = %i\n", errcode);
  PGR_DBG("Returned message = %s\n", err_msg);



  if (err_msg) free(err_msg);
  pfree(edges);
  pgr_SPI_finish();

  if (errcode) {
      pgr_send_error(errcode);
  }
}


PG_FUNCTION_INFO_V1(kshortest_path);
PGDLLEXPORT Datum
kshortest_path(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    uint32_t               call_cntr;
    uint32_t               max_calls;
    TupleDesc            tuple_desc;
    General_path_element_t      *path;
    //  void * toDel;

    /* stuff done only on the first call of the function */
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        size_t path_count = 0;
        path = NULL;

        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* switch to memory context appropriate for multiple function calls */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        // CREATE OR REPLACE FUNCTION _pgr_ksp(sql text, start_vid bigint, end_vid bigint, k integer, directed boolean, heap_paths boolean...
        compute(
                pgr_text2char(PG_GETARG_TEXT_P(0)), /* SQL  */
                PG_GETARG_INT64(1),   /* start_vid */
                PG_GETARG_INT64(2),   /* end_vid */
                PG_GETARG_INT32(3),   /* k */
                PG_GETARG_BOOL(4),    /* directed */
                PG_GETARG_BOOL(5),    /* heap_paths */
                &path,
                &path_count);
        //      toDel = path;

        PGR_DBG("Total number of tuples to be returned %ld \n", path_count);

        /* total number of tuples to be returned */
        funcctx->max_calls = (uint32_t)path_count;
        funcctx->user_fctx = path;

        /* Build a tuple descriptor for our result type */
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record\n")));

        // funcctx->tuple_desc = BlessTupleDesc(RelationNameGetTupleDesc("__pgr_2i3b2f"));
        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }


    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = (uint32_t)funcctx->call_cntr;
    max_calls = (uint32_t)funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    path = (General_path_element_t*) funcctx->user_fctx;

    if (call_cntr < max_calls) {   /* do when there is more left to send */
        PGR_DBG("returning %u \n", call_cntr);
        HeapTuple    tuple;
        Datum        result;
        /* //Datum values[4];
        //bool nulls[4]; */

        Datum *values;
        bool* nulls;

        values = (Datum *)palloc(7 * sizeof(Datum));
        nulls = (bool *) palloc(7 * sizeof(bool));

        nulls[0] = false;
        nulls[1] = false;
        nulls[2] = false;
        nulls[3] = false;
        nulls[4] = false;
        nulls[5] = false;
        nulls[6] = false;

        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int32GetDatum(path[call_cntr].start_id + 1);
        values[2] = Int32GetDatum(path[call_cntr].seq);
        values[3] = Int64GetDatum(path[call_cntr].node);
        values[4] = Int64GetDatum(path[call_cntr].edge);
        values[5] = Float8GetDatum(path[call_cntr].cost);
        values[6] = Float8GetDatum(path[call_cntr].agg_cost);

        tuple = heap_form_tuple(tuple_desc, values, nulls);

        /* make the tuple into a datum */
        result = HeapTupleGetDatum(tuple);

        /* clean up (this is not really necessary) */
        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {   /* do when there is no more left */
        if (path == (General_path_element_t *) NULL) free(path);
        SRF_RETURN_DONE(funcctx);
    }
}


