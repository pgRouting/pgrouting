/*PGR-GNU*****************************************************************
File: many_to_dist_driving_distance.c

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_Vergara@hotmail.com

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

// #define DEBUG
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
#include "./../../common/src/arrays_input.h"
#include "./boost_interface_drivedist.h"



static 
void driving_many_to_dist_driver(
        char* sql,
        int64_t *start_vertex, size_t num,
        float8 distance,
        bool directed,
        bool equicost, 
        General_path_element_t **path, size_t *path_count) {
    pgr_SPI_connect();
    pgr_edge_t *edges = NULL;
    size_t total_tuples = 0;


    char *err_msg = (char *)"";


    pgr_get_edges(sql, &edges, &total_tuples);

    if (total_tuples == 0) {
        PGR_DBG("No edges found");
        (*path_count) = 0;
        *path = NULL;
        return;
    }

    clock_t start_t = clock();
    do_pgr_driving_many_to_dist(
            edges, total_tuples,
            start_vertex, num,
            distance,
            directed,
            equicost,
            path, path_count, &err_msg);
    time_msg(" processing DrivingDistance many starts", start_t, clock());

    pfree(edges);
    pgr_SPI_finish(); 
}


#ifndef _MSC_VER
Datum driving_many_to_dist(PG_FUNCTION_ARGS);
#else  // _MSC_VER
PGDLLEXPORT Datum driving_many_to_dist(PG_FUNCTION_ARGS);
#endif  // _MSC_VER


PG_FUNCTION_INFO_V1(driving_many_to_dist);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
driving_many_to_dist(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    uint32_t                  call_cntr;
    uint32_t                  max_calls;
    TupleDesc            tuple_desc;
    General_path_element_t  *ret_path = 0;

    /* stuff done only on the first call of the function */
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        size_t path_count = 0;

        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* switch to memory context appropriate for multiple function calls */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        int64_t* sourcesArr;
        size_t num;

        sourcesArr = (int64_t*) pgr_get_bigIntArray(&num, PG_GETARG_ARRAYTYPE_P(1));
        PGR_DBG("sourcesArr size %d ", num);

        PGR_DBG("Calling driving_many_to_dist_driver");
        driving_many_to_dist_driver(
                pgr_text2char(PG_GETARG_TEXT_P(0)),  // sql
                sourcesArr, num,                     // array of sources
                PG_GETARG_FLOAT8(2),                 // distance
                PG_GETARG_BOOL(3),                   // directed
                PG_GETARG_BOOL(4),                   // equicost
                &ret_path, &path_count);

        free(sourcesArr);

        /* total number of tuples to be returned */
        funcctx->max_calls = (uint32_t) path_count;
        funcctx->user_fctx = ret_path;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));

        funcctx->tuple_desc = tuple_desc;

        MemoryContextSwitchTo(oldcontext);
    }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    ret_path = (General_path_element_t*) funcctx->user_fctx;

    /* do when there is more left to send */
    if (call_cntr < max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        char* nulls;

        values = palloc(6 * sizeof(Datum));
        nulls = palloc(6 * sizeof(char));
        // id, start_v, node, edge, cost, tot_cost
        nulls[0] = ' ';
        nulls[1] = ' ';
        nulls[2] = ' ';
        nulls[3] = ' ';
        nulls[4] = ' ';
        nulls[5] = ' ';
        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int64GetDatum(ret_path[call_cntr].start_id);
        values[2] = Int64GetDatum(ret_path[call_cntr].node);
        values[3] = Int64GetDatum(ret_path[call_cntr].edge);
        values[4] = Float8GetDatum(ret_path[call_cntr].cost);
        values[5] = Float8GetDatum(ret_path[call_cntr].agg_cost);

        tuple = heap_formtuple(tuple_desc, values, nulls);

        /* make the tuple into a datum */
        result = HeapTupleGetDatum(tuple);

        /* clean up (this is not really necessary) */
        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {
        /* do when there is no more left */
        if (ret_path) free(ret_path);
        SRF_RETURN_DONE(funcctx);
    }
}

