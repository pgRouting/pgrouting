/*PGR-GNU*****************************************************************
File: astarOneToOne.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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
#include "fmgr.h"

#include "./../../common/src/debug_macro.h"
#include "./../../common/src/time_msg.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./../../common/src/edges_input.h"

#include "./astarOneToOne_driver.h"

PGDLLEXPORT Datum astarOneToOne(PG_FUNCTION_ARGS);


static
void
process(char* edges_sql,
        int64_t start_vid,
        int64_t end_vid,
        bool directed,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,
        General_path_element_t **result_tuples,
        size_t *result_count) {
    if (heuristic > 5 || heuristic < 0) {
        ereport(ERROR,
                (errmsg("Unknown heuristic"),
                 errhint("Valid values: 0~5")));
    }
    if (factor <= 0) {
        ereport(ERROR,
                (errmsg("Factor value out of range"),
                 errhint("Valid values: positive non zero")));
    }
    if (epsilon < 1) {
        ereport(ERROR,
                (errmsg("Epsilon value out of range"),
                 errhint("Valid values: 1 or greater than 1")));
    }



    pgr_SPI_connect();

    PGR_DBG("Load data");
    Pgr_edge_xy_t *edges = NULL;
    size_t total_edges = 0;

    pgr_get_edges_xy(edges_sql, &edges, &total_edges);
    PGR_DBG("Total %ld edges in query:", total_edges);

    if (total_edges == 0) {
        PGR_DBG("No edges found");
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting processing");
    char *err_msg = NULL;
    char *log_msg = NULL;
    clock_t start_t = clock();
    do_pgr_astarOneToOne(
            edges,
            total_edges,
            start_vid,
            end_vid,
            directed,
            heuristic,
            factor,
            epsilon,
            only_cost,
            result_tuples,
            result_count,
            &log_msg,
            &err_msg);
    time_msg(" processing pgr_astar(one to one)", start_t, clock());

    PGR_DBG("Returning %ld tuples\n", *result_count);
    PGR_DBG("LOG: %s\n", log_msg);
    if (log_msg) free(log_msg);

    if (err_msg) {
        if (*result_tuples) free(*result_tuples);
        elog(ERROR, "%s", err_msg);
        free(err_msg);
    }

    pfree(edges);
    pgr_SPI_finish();
}

PG_FUNCTION_INFO_V1(astarOneToOne);
PGDLLEXPORT Datum
astarOneToOne(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    uint32_t            call_cntr;
    uint32_t            max_calls;
    TupleDesc           tuple_desc;

    General_path_element_t  *result_tuples = 0;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        /**********************************************************************
           edges_sql TEXT,
           start_vid BIGINT,
           end_vid BIGINT,
           directed BOOLEAN DEFAULT true,
           heuristic INTEGER DEFAULT 0,
           factor FLOAT DEFAULT 1.0,
           epsilon FLOAT DEFAULT 1.0,

         **********************************************************************/

        PGR_DBG("Calling process");
        process(
                pgr_text2char(PG_GETARG_TEXT_P(0)),
                PG_GETARG_INT64(1),
                PG_GETARG_INT64(2),
                PG_GETARG_BOOL(3),
                PG_GETARG_INT32(4),
                PG_GETARG_FLOAT8(5),
                PG_GETARG_FLOAT8(6),
                PG_GETARG_BOOL(7),
                &result_tuples,
                &result_count);


        funcctx->max_calls = (uint32_t) result_count;
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
        bool*        nulls;

        /**********************************************************************
           OUT seq INTEGER,
           OUT path_seq INTEGER,
           OUT node BIGINT,
           OUT edge BIGINT,
           OUT cost FLOAT,
           OUT agg_cost FLOAT
         *********************************************************************/


        values = palloc(6 * sizeof(Datum));
        nulls = palloc(6 * sizeof(bool));

        size_t i;
        for (i = 0; i < 6; ++i) {
            nulls[i] = false;
        }


        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].seq);
        values[2] = Int64GetDatum(result_tuples[call_cntr].node);
        values[3] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[4] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[5] = Float8GetDatum(result_tuples[call_cntr].agg_cost);


        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        // cleanup
        if (result_tuples) free(result_tuples);

        SRF_RETURN_DONE(funcctx);
    }
}

