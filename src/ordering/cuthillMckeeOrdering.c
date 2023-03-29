/*PGR-GNU*****************************************************************
File: cuthillMckeeOrdering.c

Generated with Template by:
Copyright (c) 2022 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Shobhit Chaurasia
Mail: 000shobhitchaurasia@gmail.com
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

/** @file cuthillMckeeOrdering.c
 * @brief Connecting code with postgres.
 *
 */

#include <stdbool.h>
#include "c_common/postgres_connection.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"

#include "c_common/pgdata_getters.h"
#include "c_types/ii_t_rt.h"


#include "drivers/ordering/cuthillMckeeOrdering_driver.h"

PGDLLEXPORT Datum _pgr_cuthillmckeeordering(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_cuthillmckeeordering);

/** @brief Static function, loads the data from postgres to C types for further processing.
 *
 * It first connects the C function to the SPI manager. Then converts
 * the postgres array to C array and loads the edges belonging to the graph
 * in C types. Then it calls the function `do_cuthillMckeeOrdering` defined
 * in the `cuthillMckeeOrdering_driver.h` file for further processing.
 * Finally, it frees the memory and disconnects the C function to the SPI manager.
 *
 * @param edges_sql      the edges of the SQL query
 * @param result_tuples  the rows in the result
 * @param result_count   the count of rows in the result
 *
 * @returns void
 */

static
void
process(
    char* edges_sql,

    II_t_rt **result_tuples,
    size_t *result_count) {
    pgr_SPI_connect();

    (*result_tuples) = NULL;
    (*result_count) = 0;

    Edge_t *edges = NULL;
    size_t total_edges = 0;

    pgr_get_edges(edges_sql, &edges, &total_edges, true, false);
    if (total_edges == 0) {
        ereport(WARNING,
                (errmsg("Insufficient data edges SQL."),
                 errhint("%s", edges_sql)));
        (*result_count) = 0;
        (*result_tuples) = NULL;
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Starting timer");
    clock_t start_t = clock();
    char *log_msg = NULL;
    char *notice_msg = NULL;
    char *err_msg = NULL;

    do_cuthillMckeeOrdering(
            edges, total_edges,
            result_tuples,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg("processing cuthillmckeeordering", start_t, clock());

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);
    if (edges) pfree(edges);

    pgr_SPI_finish();
}

/** @brief Helps in converting postgres variables to C variables, and returns the result.
 *
 */

PGDLLEXPORT Datum
_pgr_cuthillmckeeordering(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    II_t_rt *result_tuples = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

    /***********************************************************************
         *
         *   pgr_cuthillMckeeOrdering(edges_sql TEXT);
         *
    **********************************************************************/

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                &result_tuples,
                &result_count);

    funcctx->max_calls = result_count;

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
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (II_t_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool*        nulls;

    /***********************************************************************
         *
         *   OUT seq BIGINT,
         *   OUT node BIGINT,
         *
    **********************************************************************/

        size_t num  = 3;
        values = palloc(num * sizeof(Datum));
        nulls = palloc(num * sizeof(bool));


        size_t i;
        for (i = 0; i < num; ++i) {
            nulls[i] = false;
        }

        values[0] = Int64GetDatum(funcctx->call_cntr + 1);
        values[1] = Int64GetDatum(result_tuples[funcctx->call_cntr].d2.value);

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
