/*PGR-GNU*****************************************************************
FILE: alpha.c

Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
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
# include <stdint.h>
#include "c_common/postgres_connection.h"

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"


#include "c_types/pgr_point_t.h"
#include "c_common/pgr_point_input.h"

#include "drivers/alpha_shape/alpha_driver.h"

PGDLLEXPORT Datum alphashape(PG_FUNCTION_ARGS);

static void process(
        char* points_sql,
        float8 alpha,

        Pgr_point_t **res,
        size_t *res_count) {
    pgr_SPI_connect();

    Pgr_point_t *pointsArr = NULL;
    size_t pointsTotal = 0;
    pgr_point_input(points_sql, &pointsArr, &pointsTotal);

    if (pointsTotal < 3) {
        if (pointsArr) pfree(pointsArr);
        elog(ERROR, "Less than 3 vertices."
                " Alpha shape calculation needs at least 3 vertices.");
        pgr_SPI_finish();
        return;
    }

    PGR_DBG("Calling CGAL alpha-shape\n");

    char *err_msg = NULL;
    char* log_msg = NULL;
    char* notice_msg = NULL;

    alpha_shape(pointsArr, pointsTotal, alpha, res, res_count, &err_msg);

    if (err_msg && (*res)) {
        pfree(*res);
        (*res) = NULL;
        (*res_count) = 0;
    }

    pgr_global_report(log_msg, notice_msg, err_msg);

    if (log_msg) pfree(log_msg);
    if (notice_msg) pfree(notice_msg);
    if (err_msg) pfree(err_msg);
    if (pointsArr) pfree(pointsArr);
    pgr_SPI_finish();
}

PG_FUNCTION_INFO_V1(alphashape);

PGDLLEXPORT
Datum alphashape(PG_FUNCTION_ARGS) {
    FuncCallContext      *funcctx;
    TupleDesc            tuple_desc;
    Pgr_point_t     *res = NULL;

    /* stuff done only on the first call of the function */
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        size_t res_count;


        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* switch to memory context appropriate for multiple function calls */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        process(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_FLOAT8(1),
                &res
                , &res_count);

        /* total number of tuples to be returned */
        PGR_DBG("Conting tuples number\n");
#if PGSQL_VERSION > 95
        funcctx->max_calls = res_count;
#else
        funcctx->max_calls = (uint32_t)res_count;
#endif
        funcctx->user_fctx = res;

        PGR_DBG("Total count %lu", res_count);

        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));

        funcctx->tuple_desc = BlessTupleDesc(tuple_desc);

        MemoryContextSwitchTo(oldcontext);
    }

    funcctx = SRF_PERCALL_SETUP();
    tuple_desc = funcctx->tuple_desc;
    res = (Pgr_point_t*)funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        /* do when there is more left to send */
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        bool* nulls;
        double x;
        double y;

        values = palloc(2 * sizeof(Datum));
        nulls = palloc(2 * sizeof(bool));

        x = res[funcctx->call_cntr].x;
        y = res[funcctx->call_cntr].y;
        if (x == DBL_MAX && y == DBL_MAX) {
            values[0] = 0;
            values[1] = 0;
            nulls[0] = true;
            nulls[1] = true;
        } else {
            values[0] = Float8GetDatum(x);
            values[1] = Float8GetDatum(y);
            nulls[0] = false;
            nulls[1] = false;
        }


        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else  {
        SRF_RETURN_DONE(funcctx);
    }
}
