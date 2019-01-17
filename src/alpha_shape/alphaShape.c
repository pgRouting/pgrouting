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
#include "c_common/postgres_connection.h"
#include <float.h>

#include "c_common/debug_macro.h"
#include "c_common/e_report.h"
#include "c_common/time_msg.h"


#include "c_types/pgr_point_t.h"
#include "c_common/pgr_point_input.h"
#include "c_common/delauny_input.h"

#include "drivers/alpha_shape/alphaShape_driver.h"

PGDLLEXPORT Datum alphaShape(PG_FUNCTION_ARGS);

static void process(
        char* points_sql,
        double alpha,

        Pgr_point_t **res,
        size_t *result_count) {
    pgr_SPI_connect();

    Pgr_point_t *pointsArr = NULL;
    size_t pointsTotal = 0;
    pgr_point_input(points_sql, &pointsArr, &pointsTotal);

    for (size_t i = 0; i < pointsTotal; ++i) {
        PGR_DBG("x %.10f y %.10f", pointsArr[i].x, pointsArr[i].y);
    }
    PGR_DBG("totalpoints1: %ld", pointsTotal);

    PGR_DBG("%ld", pointsTotal);
    pointsTotal = points_size(pointsArr, pointsTotal);
    PGR_DBG("%ld", pointsTotal);

    for (size_t i = 0; i < pointsTotal; ++i) {
        PGR_DBG("x %.10f y %.10f", pointsArr[i].x, pointsArr[i].y);
    }

    PGR_DBG("totalpoints2: %ld", pointsTotal);

    if (pointsTotal < 3) {
        if (pointsArr) pfree(pointsArr);
        elog(ERROR, "Less than 3 vertices."
                " pgr_alphaShape needs at least 3 vertices.");
        pgr_SPI_finish();
        return;
    }

    /*
     * Calculating Delauny triangles using postGIS
     */
    char * delauny_sql = delauny_query(points_sql);
    PGR_DBG("%s", delauny_sql);

    Delauny_t *delaunyArr = NULL;
    size_t delaunyTotal = 0;
    PGR_DBG("HERE1");
    pgr_get_delauny(delauny_sql, &delaunyArr, &delaunyTotal);
    PGR_DBG("HERE2");

    PGR_DBG("%ld", delaunyTotal);

    for (size_t i = 0; i < delaunyTotal; ++i) {
        PGR_DBG("tid %ld pid %ld x %.10f y %.10f", delaunyArr[i].tid, delaunyArr[i].pid, delaunyArr[i].x, delaunyArr[i].y);
    }

    PGR_DBG("Calling CGAL alpha-shape\n");

    char *err_msg = NULL;
    char* log_msg = NULL;
    char* notice_msg = NULL;

    do_alphaShape(
            pointsArr, pointsTotal,
            delaunyArr, delaunyTotal,
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
    if (pointsArr) pfree(pointsArr);
    if (delaunyArr) pfree(delaunyArr);
    pgr_SPI_finish();
}

PG_FUNCTION_INFO_V1(alphaShape);

PGDLLEXPORT
Datum alphaShape(PG_FUNCTION_ARGS) {
    FuncCallContext      *funcctx;
    TupleDesc            tuple_desc;

    /**********************************************************************/
    Pgr_point_t *result_tuples = NULL;
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
    result_tuples = (Pgr_point_t*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum        *values;
        bool         *nulls;
        size_t       call_cntr = funcctx->call_cntr;

        /******************************************************************/
        values = palloc(2 * sizeof(Datum));
        nulls = palloc(2 * sizeof(bool));

        if (result_tuples[call_cntr].x == DBL_MAX && result_tuples[call_cntr].y == DBL_MAX) {
            values[0] = 0;
            values[1] = 0;
            nulls[0] = true;
            nulls[1] = true;
        } else {
            values[0] = Float8GetDatum(result_tuples[call_cntr].x);
            values[1] = Float8GetDatum(result_tuples[call_cntr].y);
            nulls[0] = false;
            nulls[1] = false;
        }
        /******************************************************************/


        tuple = heap_form_tuple(tuple_desc, values, nulls);
        result = HeapTupleGetDatum(tuple);
        SRF_RETURN_NEXT(funcctx, result);
    } else  {
        SRF_RETURN_DONE(funcctx);
    }
}
