/*PGR-GNU*****************************************************************
File: maxWeightedMatching.c

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2026 Mayur Galhate
Mail: galhatemayur at gmail.com

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
#include "process/ordering_process.h"

PGDLLEXPORT Datum _pgr_maxweightedmatch(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_maxweightedmatch);

PGDLLEXPORT Datum _pgr_maxweightedmatch(PG_FUNCTION_ARGS) {
    FuncCallContext *funcctx;

    int64_t  *result_tuples = NULL;
    size_t    result_count  = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        pgr_process_ordering(
            text_to_cstring(PG_GETARG_TEXT_P(0)),
            false,
            MAXWEIGHTMATCH,
            &result_tuples,
            &result_count);

        funcctx->max_calls = result_count;
        funcctx->user_fctx = result_tuples;

        MemoryContextSwitchTo(oldcontext);
    }

    funcctx       = SRF_PERCALL_SETUP();
    result_tuples = funcctx->user_fctx;
    uint64_t call_cntr = funcctx->call_cntr;

    if (call_cntr < funcctx->max_calls) {
        Datum      result;

        result = Int64GetDatum(result_tuples[call_cntr]);

        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
