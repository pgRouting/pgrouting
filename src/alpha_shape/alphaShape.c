/*PGR-GNU*****************************************************************
FILE: alphaShape.c

Copyright (c) 2018 Vicky Vergara
Mail: project@pgrouting.org

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
#include <float.h>

#include "c_common/postgres_connection.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"


#include "c_common/edges_input.h"
#include "c_types/geom_text_rt.h"

#include "drivers/alpha_shape/alphaShape_driver.h"

PGDLLEXPORT Datum _pgr_alphashape(PG_FUNCTION_ARGS);

static void process(
        char* edges_sql,
        double alpha,

        GeomText_t **res,
        size_t *result_count) {
    pgr_SPI_connect();

    Pgr_edge_xy_t *edgesArr = NULL;
    size_t edgesSize = 0;

    pgr_get_edges_xy(edges_sql, &edgesArr, &edgesSize);

    PGR_DBG("total edges %ld", edgesSize);
    PGR_DBG("alpha %f", alpha);
#if 0
    for (size_t i = 0; i < edgesSize; ++i) {
        PGR_DBG("x1=%f y1=%f", edgesArr[i].x1, edgesArr[i].y1);
        PGR_DBG("x2=%f y2=%f", edgesArr[i].x2, edgesArr[i].y2);
    }
#endif

    if (edgesSize < 3) {
        if (edgesArr) pfree(edgesArr);
        elog(ERROR, "Less than 3 vertices."
                " pgr_alphaShape needs at least 3 vertices.");
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Calling alpha-shape driver\n");

    char *err_msg = NULL;
    char* log_msg = NULL;
    char* notice_msg = NULL;

    do_alphaShape(
            edgesArr, edgesSize,
            alpha,

            res,
            result_count,
            &log_msg,
            &notice_msg,
            &err_msg);

    if (err_msg && (*res)) {
        pfree(*res);
        (*res) = NULL;
        (*result_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);
    if (edgesArr) pfree(edgesArr);
    pgr_SPI_finish();
}

PG_FUNCTION_INFO_V1(_pgr_alphashape);

PGDLLEXPORT
Datum _pgr_alphashape(PG_FUNCTION_ARGS) {
    FuncCallContext      *funcctx;
    TupleDesc            tuple_desc;

    /**********************************************************************/
    GeomText_t *result_tuples = NULL;
    size_t      result_count = 0;
    /**********************************************************************/

    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        funcctx = SRF_FIRSTCALL_INIT();
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        /******************************************************************/

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_FLOAT8(1),
                &result_tuples,
                &result_count);

        /******************************************************************/

#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif

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
    tuple_desc = funcctx->tuple_desc;
    result_tuples = (GeomText_t*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool         *nulls;
        size_t       call_cntr = funcctx->call_cntr;

        size_t numb = 4;

        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        values[0] = Int64GetDatum(call_cntr + 1);
        values[1] = CStringGetTextDatum(result_tuples[call_cntr].geom);

        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else  {
        SRF_RETURN_DONE(funcctx);
    }
}
