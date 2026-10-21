/*PGR-GNU*****************************************************************
File: hawickCircuits.c
Generated with Template by:

Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Nitish Chauhan
Mail: nitishchauhan0022 at gmail.com
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
#include "c_types/path_rt.h"
#include "process/circuits_process.h"

PGDLLEXPORT Datum _pgr_hawickcircuits(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_hawickcircuits);


PGDLLEXPORT Datum _pgr_hawickcircuits(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc           tuple_desc;

    Path_rt *result_tuples = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

#ifdef SHOWMSG
        ereport(NOTICE, (
                    errcode(ERRCODE_WARNING_DEPRECATED_FEATURE),
                    errmsg("A stored procedure is using deprecated C internal function '%s'", __func__),
                    errdetail("Library function '%s' was deprecated in pgRouting %s", __func__, "4.0.0"),
                    errhint("Consider upgrade pgRouting")));
#endif

        pgr_process_circuits(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                NULL,

                1, 9223372036854775807, true, true,

                OLDHAWICKCIRCUITS,
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
    result_tuples = (Path_rt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool         *nulls;
        size_t       call_cntr = funcctx->call_cntr;


        size_t numb_out = 9;
        values = palloc(numb_out * sizeof(Datum));
        nulls = palloc(numb_out * sizeof(bool));
        size_t i;
        for (i = 0; i< numb_out; ++i) {
            nulls[i] = false;
        }

        /* edge < 0 marks the depth-0 row that starts each circuit */
        int64_t depth;
        int64_t circuit_id;
        if (call_cntr == 0) {
            depth = 0;
            circuit_id = 1;
        } else if (result_tuples[call_cntr].edge < 0) {
            depth = 0;
            circuit_id = result_tuples[call_cntr - 1].end_id + 1;
        } else {
            depth = result_tuples[call_cntr - 1].start_id + 1;
            circuit_id = result_tuples[call_cntr - 1].end_id;
        }

        values[0] = Int32GetDatum((int32_t)call_cntr + 1);
        values[1] = Int32GetDatum((int32_t)circuit_id);
        values[2] = Int32GetDatum((int32_t)depth);
        values[3] = Int64GetDatum(result_tuples[call_cntr].start_id);
        values[4] = Int64GetDatum(result_tuples[call_cntr].end_id);
        values[5] = Int64GetDatum(result_tuples[call_cntr].node);
        values[6] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[7] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[8] = Float8GetDatum(result_tuples[call_cntr].agg_cost);

        /* scratch: this row's depth / circuit_id for the next row to read */
        result_tuples[call_cntr].start_id = depth;
        result_tuples[call_cntr].end_id   = circuit_id;

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
