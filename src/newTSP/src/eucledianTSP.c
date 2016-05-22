/*PGR-GNU*****************************************************************
File: xyz_tsp.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#define DEBUG

#include "fmgr.h"
#include "./../../common/src/debug_macro.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/coordinates_input.h"
#include "./eucledianTSP_driver.h"



PG_FUNCTION_INFO_V1(eucledianTSP);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
eucledianTSP(PG_FUNCTION_ARGS);

/******************************************************************************/
/*                          MODIFY AS NEEDED                                  */
static
void
process(
        char* coordinates_sql,
        bool randomize,
        int64_t start_vid,
        int64_t end_vid,
        double initial_temperature,
        double final_temperature,
        double cooling_factor,
        int64_t tries_per_temperature,
        int64_t change_per_temperature,
        General_path_element_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    /*
     * errors in parameters
     */
    if (initial_temperature < final_temperature) {
        elog(ERROR, "Illegal: initial_temperature < final_temperature");
    }
    if (final_temperature < 0) {
        elog(ERROR, "Illegal: final_temperature < 0");
    }
    if (cooling_factor <=0 || cooling_factor >=1) {
        elog(ERROR, "Illegal: cooling_factor <=0 || cooling_factor >=1");
    }
    if (tries_per_temperature  < 1) {
        elog(ERROR, "Illegal: tries_per_temperature  < 1");
    }
    if (change_per_temperature  < 1) {
        elog(ERROR, "Illegal: change_per_temperature  < 1");
    }


    PGR_DBG("Load data");
    Coordinate_t *coordinates = NULL;
    size_t total_coordinates = 0;
    pgr_get_coordinates(coordinates_sql, &coordinates, &total_coordinates);

    if (total_coordinates == 0) {
        PGR_DBG("No coordinates found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }
    PGR_DBG("Total %ld coordinates:", total_coordinates);

    PGR_DBG("Starting processing");
    char *err_msg = NULL;
    char *log_msg = NULL;
    do_pgr_eucledianTSP(
            coordinates,
            total_coordinates,
            start_vid,
            end_vid,
            initial_temperature,
            final_temperature,
            cooling_factor,
            tries_per_temperature,
            change_per_temperature,
            randomize,
            result_tuples,
            result_count,
            &log_msg,
            &err_msg);
    PGR_DBG("Returning %ld tuples\n", *result_count);
    PGR_DBG("LOG = %s\n", log_msg);
    free(log_msg);
    if (err_msg) {
        if (*result_tuples) free(*result_tuples);
        elog(ERROR, "%s", err_msg);
        free(err_msg);
    }

    pfree(coordinates);
    pgr_SPI_finish();
}
/*                                                                            */
/******************************************************************************/

#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
eucledianTSP(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    uint32_t              call_cntr;
    uint32_t               max_calls;
    TupleDesc            tuple_desc;

    /**************************************************************************/
    /*                          MODIFY AS NEEDED                              */
    /*                                                                        */
    General_path_element_t  *result_tuples = 0;
    size_t result_count = 0;
    /*                                                                        */
    /**************************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        /* 
           CREATE OR REPLACE FUNCTION pgr_xydtsp(
           coordinate_sql TEXT, -- has id, x, y
           start_id BIGINT DEFAULT -1,
           end_id BIGINT DEFAULT -1,
           initial_temperature FLOAT DEFAULT 100,
           final_temperature FLOAT DEFAULT 100,
           cooling_factor FLOAT DEFAULT 0.9,
           tries_per_temperature FLOAT DEFAULT 500,
           change_per_temperature FLOAT DEFAULT 60,
           fix_random BOOLEAN DEFAULT 0
           */

        PGR_DBG("Calling process");
        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                PG_GETARG_BOOL(1),
                PG_GETARG_INT64(2),
                PG_GETARG_INT64(3),
                PG_GETARG_FLOAT8(4),
                PG_GETARG_FLOAT8(5),
                PG_GETARG_FLOAT8(6),
                PG_GETARG_INT32(7),
                PG_GETARG_INT32(8),
                &result_tuples,
                &result_count);
        /*                                                                    */
        /**********************************************************************/

        funcctx->max_calls = (uint32_t) result_count;
        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc)
                != TYPEFUNC_COMPOSITE) {
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));
        }

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
        bool*        nulls;

        /**********************************************************************/
        /*                          MODIFY AS NEEDED                          */
        // OUT seq INTEGER,
        // OUT node BIGINT,
        // OUT cost FLOAT,
        // OUT agg_cost FLOAT

        values = palloc(4 * sizeof(Datum));
        nulls = palloc(4 * sizeof(bool));


        size_t i;
        for (i = 0; i < 4; ++i) {
            nulls[i] = false;
        }

        // postgres starts counting from 1
        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[call_cntr].node);
        values[2] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[3] = Float8GetDatum(result_tuples[call_cntr].agg_cost);
        /**********************************************************************/

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        // cleanup
        if (result_tuples) free(result_tuples);

        SRF_RETURN_DONE(funcctx);
    }
}

