/*PGR-GNU*****************************************************************
File: maximum_cardinality_matching.c

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Refactoring
Copyright (c) 2022 Celia Vriginia Vergara Castillo
Mail: vicky_vergara at hotmail.com

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#include <stdbool.h>

#include "c_common/postgres_connection.h"

#include "c_common/e_report.h"
#include "c_common/time_msg.h"
#include "c_common/edges_input.h"

#include "c_types/edge_bool_t.h"

#include "drivers/max_flow/maximum_cardinality_matching_driver.h"


PGDLLEXPORT Datum
_pgr_maxcardinalitymatch(PG_FUNCTION_ARGS);

static
void
process(
        char *edges_sql,
        int64_t **result_tuples,
        size_t *result_count) {
    pgr_SPI_connect();

    Edges *edges = NULL;
    size_t total_edges = 0;
    pgr_get_basic_edges(edges_sql, &edges, &total_edges);

    if (total_edges == 0) {
        pgr_SPI_finish();
        return;
    }

    clock_t start_t = clock();
    char* log_msg = NULL;
    char* notice_msg = NULL;
    char *err_msg = NULL;

    do_maxCardinalityMatch(
            edges, total_edges,
            result_tuples,
            result_count,

            &log_msg,
            &notice_msg,
            &err_msg);

    time_msg("pgr_maxCardinalityMatch()", start_t, clock());

    if (edges) pfree(edges);

    if (err_msg && (*result_tuples)) {
        pfree(*result_tuples);
        (*result_tuples) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);


    pgr_SPI_finish();
}

PG_FUNCTION_INFO_V1(_pgr_maxcardinalitymatch);
PGDLLEXPORT Datum
_pgr_maxcardinalitymatch(PG_FUNCTION_ARGS) {
    FuncCallContext *funcctx;

    int64_t *result_tuples = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                &result_tuples,
                &result_count);

        funcctx->max_calls = result_count;
        funcctx->user_fctx = result_tuples;
        MemoryContextSwitchTo(oldcontext);
    }

    funcctx = SRF_PERCALL_SETUP();
    result_tuples = (int64_t *) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        Datum result;

        result = Int64GetDatum(result_tuples[funcctx->call_cntr]);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
