/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
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
#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"
#include "trsp.h"

PGDLLEXPORT Datum turn_restrict_shortest_path_vertex(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum turn_restrict_shortest_path_edge(PG_FUNCTION_ARGS);

// #define DEBUG 
#include "../../common/src/debug_macro.h"
#include "../../common/src/pgr_types.h"
#include "../../common/src/postgres_connection.h"
#include "../../common/src/edges_input.h"
#include "../../common/src/restrictions_input.h"


static int compute_trsp(
        char* edges_sql,
        int dovertex,
        int start_id,
        double start_pos,
        int end_id,
        double end_pos,
        bool directed, 
        bool has_reverse_cost,
        char *restrict_sql,
        path_element_t **path,
        size_t *path_count) {
    pgr_SPI_connect();

    PGR_DBG("Load edges");
    pgr_edge_t *edges = NULL;
    size_t total_tuples = 0;
    pgr_get_edges(edges_sql, &edges, &total_tuples);
    PGR_DBG("Total %ld edges", total_tuples);

    PGR_DBG("Load restrictions");
    restrict_t *restricts = NULL;
    size_t total_restrict_tuples = 0;
    if (restrict_sql == NULL) {
        PGR_DBG("Sql for restrictions is null.");
    } else {
        pgr_get_restriction_data(restrict_sql, &restricts, &total_restrict_tuples);
    }
#ifdef DEBUG
    int t1;
    for (t1=0; t1<total_restrict_tuples; t1++) {
        PGR_DBG("restricts: %.2f, %ld, %ld, %ld, %ld, %ld, %ld", restricts[t1].to_cost,
                restricts[t1].target_id, restricts[t1].via[0], restricts[t1].via[1], restricts[t1].via[2], restricts[t1].via[3], restricts[t1].via[4]);
    }
#endif
    PGR_DBG("Total %ld restriction", total_restrict_tuples);


    int v_max_id=0;
    int v_min_id=INT_MAX;

    /* track if start and end are both in edge tuples */
    int s_count = 0;
    int t_count = 0;

    char *err_msg;
    int ret = -1;
    register int z;

    PGR_DBG("start turn_restrict_shortest_path\n");

    if (start_id == end_id) {
        PGR_DBG("Starting vertex and Ending Vertex are equal");
        *path = NULL;
        return 0;
    }


    //defining min and max vertex id

    PGR_DBG("Total %i edge tuples", total_tuples);

    for(z=0; z<total_tuples; z++) {
        if(edges[z].source<v_min_id)
            v_min_id=(int)edges[z].source;

        if(edges[z].source>v_max_id)
            v_max_id=(int)edges[z].source;

        if(edges[z].target<v_min_id)
            v_min_id=(int)edges[z].target;

        if(edges[z].target>v_max_id)
            v_max_id=(int)edges[z].target;      
        //PGR_DBG("%i <-> %i", v_min_id, v_max_id);

    }

//<<<<<<< HEAD
    //::::::::::::::::::::::::::::::::::::  
    //:: reducing vertex id (renumbering)
    //::::::::::::::::::::::::::::::::::::
    for(z=0; z<total_tuples; z++) {
        //check if edges[] contains source and target
        if (dovertex) {
            if(edges[z].source == start_id || edges[z].target == start_id)
                ++s_count;
            if(edges[z].source == end_id || edges[z].target == end_id)
                ++t_count;
        }
        else {
            if(edges[z].id == start_id)
                ++s_count;
            if(edges[z].id == end_id)
                ++t_count;
        }

        edges[z].source-=v_min_id;
        edges[z].target-=v_min_id;
        edges[z].cost = edges[z].cost;
        //PGR_DBG("edgeID: %i SRc:%i - %i, cost: %f", edges[z].id,edges[z].source, edges[z].target,edges[z].cost);      

    }

    PGR_DBG("Min vertex id: %i , Max vid: %i",v_min_id,v_max_id);
    PGR_DBG("Total %ld edge tuples", total_tuples);

    if(s_count == 0) {
        elog(ERROR, "Start id was not found.");
        return -1;
    }

    if(t_count == 0) {
        elog(ERROR, "Target id was not found.");
        return -1;
    }

    if (dovertex) {
        start_id -= v_min_id;
        end_id   -= v_min_id;
    }

    if (dovertex) {
        PGR_DBG("Calling trsp_node_wrapper\n");
        ret = trsp_node_wrapper(edges, total_tuples, 
                restricts, total_restrict_tuples,
                start_id, end_id,
                directed, has_reverse_cost,
                path, path_count, &err_msg);
    }
    else {
        PGR_DBG("Calling trsp_edge_wrapper\n");
        ret = trsp_edge_wrapper(edges, total_tuples, 
                restricts, total_restrict_tuples,
                start_id, start_pos, end_id, end_pos,
                directed, has_reverse_cost,
                path, path_count, &err_msg);
    }

    PGR_DBG("Message received from inside:");
    PGR_DBG("%s",err_msg);

    //PGR_DBG("SIZE %i\n",*path_count);

    //::::::::::::::::::::::::::::::::
    //:: restoring original vertex id
    //::::::::::::::::::::::::::::::::
    for(z=0;z<*path_count;z++) {
        //PGR_DBG("vetex %i\n",(*path)[z].vertex_id);
        if (z || (*path)[z].vertex_id != -1)
            (*path)[z].vertex_id+=v_min_id;
    }

    PGR_DBG("ret = %i\n", ret);

    PGR_DBG("*path_count = %i\n", *path_count);

    if (ret < 0)
    {
        //elog(ERROR, "Error computing path: %s", err_msg);
        ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), 
                    errmsg("Error computing path: %s", err_msg)));
    } 

    pgr_SPI_finish();
    return 0;
}



PG_FUNCTION_INFO_V1(turn_restrict_shortest_path_vertex);
PGDLLEXPORT Datum
turn_restrict_shortest_path_vertex(PG_FUNCTION_ARGS)
{

    FuncCallContext     *funcctx;
    uint32_t                  call_cntr;
    uint32_t                  max_calls;
    TupleDesc            tuple_desc;
    path_element_t      *path;
    char *               sql;


    // stuff done only on the first call of the function 
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        size_t path_count = 0;

        int ret = -1;
        if (ret == -1) {}; // to avoid warning set but not used

        int i;

        // create a function context for cross-call persistence
        funcctx = SRF_FIRSTCALL_INIT();

        // switch to memory context appropriate for multiple function calls
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        // verify that the first 5 args are not NULL
        for (i=0; i<5; i++)
            if(PG_ARGISNULL(i)) {
                elog(ERROR, "turn_restrict_shortest_path(): Argument %i may not be NULL", i+1);
            }

        if (PG_ARGISNULL(5))
            sql = NULL;
        else {
            sql = pgr_text2char(PG_GETARG_TEXT_P(5));
            if (strlen(sql) == 0)
                sql = NULL;
        }

        PGR_DBG("Calling compute_trsp");


        ret =

            compute_trsp(pgr_text2char(PG_GETARG_TEXT_P(0)),
                    1, // do vertex
                    PG_GETARG_INT32(1),
                    0.5,
                    PG_GETARG_INT32(2),
                    0.5,
                    PG_GETARG_BOOL(3),
                    PG_GETARG_BOOL(4), 
                    sql,
                    &path, &path_count);
#ifdef DEBUG
        double total_cost = 0;
        PGR_DBG("Ret is %i", ret);
        if (ret >= 0) 
        {
            int i;
            for (i = 0; i < path_count; i++) 
            {
                //     PGR_DBG("Step %i vertex_id  %i ", i, path[i].vertex_id);
                //   PGR_DBG("        edge_id    %i ", path[i].edge_id);
                // PGR_DBG("        cost       %f ", path[i].cost);
                total_cost+=path[i].cost;
            }
        }
        PGR_DBG("Total cost is: %f",total_cost);
#endif

        // total number of tuples to be returned 
        funcctx->max_calls = (uint32_t)path_count;
        funcctx->user_fctx = path;

        funcctx->tuple_desc = 
            BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

        MemoryContextSwitchTo(oldcontext);
    }

    // stuff done on every call of the function 
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    path = (path_element_t*) funcctx->user_fctx;

    if (call_cntr < max_calls)    // do when there is more left to send 
    {
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        bool* nulls;

        values = palloc(4 * sizeof(Datum));
        nulls = palloc(4 * sizeof(bool));

        values[0] = Int32GetDatum(call_cntr);
        nulls[0] = false;
        values[1] = Int32GetDatum(path[call_cntr].vertex_id);
        nulls[1] = false;
        values[2] = Int32GetDatum(path[call_cntr].edge_id);
        nulls[2] = false;
        values[3] = Float8GetDatum(path[call_cntr].cost);
        nulls[3] = false;

        tuple = heap_form_tuple(tuple_desc, values, nulls);

        // make the tuple into a datum 
        result = HeapTupleGetDatum(tuple);

        // clean up (this is not really necessary) 
        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    }
    else    // do when there is no more left 
    {
        PGR_DBG("Going to free path");
        if (path) free(path);
        SRF_RETURN_DONE(funcctx);
    }
}

PG_FUNCTION_INFO_V1(turn_restrict_shortest_path_edge);
PGDLLEXPORT Datum
turn_restrict_shortest_path_edge(PG_FUNCTION_ARGS)
{

    FuncCallContext     *funcctx;
    uint32_t                  call_cntr;
    uint32_t                  max_calls;
    TupleDesc            tuple_desc;
    path_element_t      *path;
    char *               sql;

    // stuff done only on the first call of the function 
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        size_t path_count = 0;
#ifdef DEBUG
        int ret = -1;
#endif
        int i;
        double s_pos;
        double e_pos;

        // create a function context for cross-call persistence
        funcctx = SRF_FIRSTCALL_INIT();

        // switch to memory context appropriate for multiple function calls
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        // verify that the first 5 args are not NULL
        for (i=0; i<7; i++) {
            if(i==2 || i==4) continue;
            if(PG_ARGISNULL(i)) {
                elog(ERROR, "turn_restrict_shortest_path(): Argument %i may not be NULL", i+1);
            }
        }

        if (PG_ARGISNULL(2))
            s_pos = 0.5;
        else {
            s_pos = PG_GETARG_FLOAT8(2);
            if (s_pos < 0.0) s_pos = 0.5;
            if (s_pos > 1.0) s_pos = 0.5;
        }

        if (PG_ARGISNULL(4))
            e_pos = 0.5;
        else {
            e_pos = PG_GETARG_FLOAT8(4);
            if (e_pos < 0.0) e_pos = 0.5;
            if (e_pos > 1.0) e_pos = 0.5;
        }

        if (PG_ARGISNULL(7))
            sql = NULL;
        else {
            sql = pgr_text2char(PG_GETARG_TEXT_P(7));
            if (strlen(sql) == 0)
                sql = NULL;
        }

        PGR_DBG("Calling compute_trsp");

#ifdef DEBUG
        ret =
#endif
            compute_trsp(pgr_text2char(PG_GETARG_TEXT_P(0)),
                    0,  //sdo edge
                    PG_GETARG_INT32(1),
                    s_pos,
                    PG_GETARG_INT32(3),
                    e_pos,
                    PG_GETARG_BOOL(5),
                    PG_GETARG_BOOL(6), 
                    sql,
                    &path, &path_count);
#ifdef DEBUG
        double total_cost = 0;
        PGR_DBG("Ret is %i", ret);
        if (ret >= 0) 
        {
            int i;
            for (i = 0; i < path_count; i++) 
            {
                //     PGR_DBG("Step %i vertex_id  %i ", i, path[i].vertex_id);
                //   PGR_DBG("        edge_id    %i ", path[i].edge_id);
                // PGR_DBG("        cost       %f ", path[i].cost);
                total_cost+=path[i].cost;
            }
        }
        PGR_DBG("Total cost is: %f",total_cost);
#endif

        // total number of tuples to be returned 
        funcctx->max_calls = (uint32_t)path_count;
        funcctx->user_fctx = path;

        funcctx->tuple_desc = 
            BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

        MemoryContextSwitchTo(oldcontext);
    }

    // stuff done on every call of the function 
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    path = (path_element_t*) funcctx->user_fctx;

    if (call_cntr < max_calls)    // do when there is more left to send 
    {
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        bool* nulls;

        values = palloc(4 * sizeof(Datum));
        nulls = palloc(4 * sizeof(bool));

        values[0] = Int32GetDatum(call_cntr);
        nulls[0] = false;
        values[1] = Int32GetDatum(path[call_cntr].vertex_id);
        nulls[1] = false;
        values[2] = Int32GetDatum(path[call_cntr].edge_id);
        nulls[2] = false;
        values[3] = Float8GetDatum(path[call_cntr].cost);
        nulls[3] = false;

        tuple = heap_form_tuple(tuple_desc, values, nulls);

        // make the tuple into a datum 
        result = HeapTupleGetDatum(tuple);

        // clean up (this is not really necessary) 
        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    }
    else    // do when there is no more left 
    {
        PGR_DBG("Going to free path");
        if (path) free(path);
        SRF_RETURN_DONE(funcctx);
    }
}
