/*PGR-GNU*****************************************************************
File: pickDeliver.c

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara at erosion.dev

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

#include "c_types/schedule_rt.h"
#include "process/vrp_process.h"

PGDLLEXPORT Datum _pgr_pickdeliver(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(_pgr_pickdeliver);


PGDLLEXPORT Datum
_pgr_pickdeliver(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;

    Schedule_rt *result_tuples = NULL;
    size_t result_count = 0;

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


        pgr_process_vrp(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                text_to_cstring(PG_GETARG_TEXT_P(2)),
                PG_GETARG_FLOAT8(3),
                PG_GETARG_INT32(4),
                PG_GETARG_INT32(5),
                PICKDELIVER,
                &result_tuples,
                &result_count);

        funcctx->max_calls = result_count;
        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE) {
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));
        }

        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }

    funcctx            = SRF_PERCALL_SETUP();
    tuple_desc         = funcctx->tuple_desc;
    result_tuples      = (Schedule_rt*) funcctx->user_fctx;
    uint64_t call_cntr = funcctx->call_cntr;

    if (call_cntr < funcctx->max_calls) {
        HeapTuple   tuple;
        Datum       result;
        Datum       *values;
        bool        *nulls;

        size_t num = 13;
        values = palloc(num * sizeof(Datum));
        nulls = palloc(num * sizeof(bool));
        size_t i;
        for (i = 0; i < num; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum((int32_t)call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].vehicle_seq);
        values[2] = Int64GetDatum(result_tuples[call_cntr].vehicle_id);
        values[3] = Int32GetDatum(result_tuples[call_cntr].stop_seq);
        values[4] = Int32GetDatum(result_tuples[call_cntr].stop_type + 1);
        values[5] = Int64GetDatum(result_tuples[call_cntr].stop_id);
        values[6] = Int64GetDatum(result_tuples[call_cntr].order_id);
        values[7] = Float8GetDatum(result_tuples[call_cntr].cargo);
        values[8] = Float8GetDatum(result_tuples[call_cntr].travelTime);
        values[9] = Float8GetDatum(result_tuples[call_cntr].arrivalTime);
        values[10] = Float8GetDatum(result_tuples[call_cntr].waitTime);
        values[11] = Float8GetDatum(result_tuples[call_cntr].serviceTime);
        values[12] = Float8GetDatum(result_tuples[call_cntr].departureTime);

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else {
        SRF_RETURN_DONE(funcctx);
    }
}
