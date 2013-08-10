/*
 * Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2005 Sylvain Pasche
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"

#include "k_targets_sp.h"

PGDLLEXPORT Datum onetomany_dijkstra_dist(PG_FUNCTION_ARGS);
PGDLLEXPORT Datum onetomany_dijkstra_path(PG_FUNCTION_ARGS);

#undef DEBUG
//#define DEBUG 1

#ifndef _MSC_VER
#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif
#else // _MSC_VER
#ifdef DEBUG
#define DBG(format, ...) \
  pgr_dbg(format, ##__VA_ARGS__)
#else
#define DBG(format, ...) do { ; } while (0)
#endif
#endif // _MSC_VER

// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000

/*
// this is already defined dijkstra.c
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif
*/

typedef struct edge_columns
{
  int id;
  int source;
  int target;
  int cost;
  int reverse_cost;
} edge_columns_t;



static text * charl2text(char *in, int len)
{
      text     *out = (text *) palloc(len + VARHDRSZ);
      memcpy(VARDATA(out), in, len);
      SET_VARSIZE(out, VARHDRSZ + len);
      return out;
}


static text * char2text(char *in)
{
      return charl2text(in, strlen(in));
}


static char * text2char(text *in)
{
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}


static int
finish(int code, int ret)
{  
  DBG("In finish, trying to disconnect from spi %d",ret);

  code = SPI_finish();
  if (code  != SPI_OK_FINISH )
  {
    elog(ERROR,"couldn't disconnect from SPI");
    return -1 ;
  }            
  return ret;
}

/*
 * This function fetches the edge columns from the SPITupleTable.
 * 
*/ 
static int fetch_edge_columns(SPITupleTable *tuptable, edge_columns_t *edge_columns, bool has_reverse_cost)
{
    edge_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
    edge_columns->source = SPI_fnumber(SPI_tuptable->tupdesc, "source");
    edge_columns->target = SPI_fnumber(SPI_tuptable->tupdesc, "target");
    edge_columns->cost = SPI_fnumber(SPI_tuptable->tupdesc, "cost");
    if (edge_columns->id == SPI_ERROR_NOATTRIBUTE ||
        edge_columns->source == SPI_ERROR_NOATTRIBUTE ||
        edge_columns->target == SPI_ERROR_NOATTRIBUTE ||
        edge_columns->cost == SPI_ERROR_NOATTRIBUTE) 
    {
        elog(ERROR, "Error, query must return columns "
            "'id', 'source', 'target' and 'cost'");
        return -1;
    }

    if (SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->source) != INT4OID ||
        SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->target) != INT4OID ||
        SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->cost) != FLOAT8OID) 
    {
        elog(ERROR, "Error, columns 'source', 'target' must be of type int4, 'cost' must be of type float8");
        return -1;
    }

    DBG("columns: id %i source %i target %i cost %i", 
        edge_columns->id, edge_columns->source, 
        edge_columns->target, edge_columns->cost);

    if (has_reverse_cost)
    {
        edge_columns->reverse_cost = SPI_fnumber(SPI_tuptable->tupdesc, 
                                               "reverse_cost");

        if (edge_columns->reverse_cost == SPI_ERROR_NOATTRIBUTE) 
        {
            elog(ERROR, "Error, reverse_cost is used, but query did't return "
                "'reverse_cost' column");
            return -1;
        }

        if (SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->reverse_cost) 
          != FLOAT8OID) 
        {
            elog(ERROR, "Error, columns 'reverse_cost' must be of type float8");
            return -1;
        }

        DBG("columns: reverse_cost cost %i", edge_columns->reverse_cost);
    }

    return 0;
}

static void fetch_edge(HeapTuple *tuple, TupleDesc *tupdesc, edge_columns_t *edge_columns, edge_t *target_edge)
{
    Datum binval;
    bool isnull;

    binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->id, &isnull);
    if (isnull)
        elog(ERROR, "id contains a null value");
    target_edge->id = DatumGetInt32(binval);

    binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->source, &isnull);
    if (isnull)
        elog(ERROR, "source contains a null value");
    target_edge->source = DatumGetInt32(binval);

    binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->target, &isnull);
    if (isnull)
        elog(ERROR, "target contains a null value");
    target_edge->target = DatumGetInt32(binval);

    binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->cost, &isnull);
    if (isnull)
        elog(ERROR, "cost contains a null value");
    target_edge->cost = DatumGetFloat8(binval);

    if (edge_columns->reverse_cost != -1) 
    {
        binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->reverse_cost, 
                             &isnull);
        if (isnull)
            elog(ERROR, "reverse_cost contains a null value");
        target_edge->reverse_cost =  DatumGetFloat8(binval);
    }
}


static int tomanysp_dijkstra_dist(char* sql, int start_vertex, 
         int *end_vertices, int nb_targets, int **t_count, bool directed, 
         bool has_reverse_cost, 
#ifdef PGR_MERGE
         pgr_cost_t **dists,
#else
         dist_fromto_t **dists,
#endif
         int *path_count) 
{

    int SPIcode;
    void *SPIplan;
    Portal SPIportal;
    bool moredata = TRUE;
    int ntuples;
    edge_t *edges = NULL;
    int total_tuples = 0;
#ifndef _MSC_VER
    edge_columns_t edge_columns = {.id= -1, .source= -1, .target= -1, 
                                   .cost= -1, .reverse_cost= -1};
#else // _MSC_VER
    edge_columns_t edge_columns = {-1, -1, -1, -1, -1};
#endif // _MSC_VER
    int v_max_id=0;
    int v_min_id=INT_MAX;

    int sumFoundTargets = 0, numTarget;

    int s_count = 0;
    char *err_msg;
    int ret = -1;
    register int z;

    for (numTarget = 0; numTarget < nb_targets; numTarget++)
        (*t_count)[numTarget] = 0;


    DBG("start shortest_path\n");

    SPIcode = SPI_connect();
    if (SPIcode  != SPI_OK_CONNECT) {
        elog(ERROR, "shortest_path: couldn't open a connection to SPI");
        return -1;
    }

    SPIplan = SPI_prepare(sql, 0, NULL);
    if (SPIplan  == NULL) {
        elog(ERROR, "shortest_path: couldn't create query plan via SPI");
        return -1;
    }

    if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
        elog(ERROR, "shortest_path: SPI_cursor_open('%s') returns NULL", sql);
        return -1;
    }

    DBG("Maybe there is moredata...\n");

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

        if (edge_columns.id == -1) {
            if (fetch_edge_columns(SPI_tuptable, &edge_columns, 
                                 has_reverse_cost) == -1)
            return finish(SPIcode, ret);
        }

        ntuples = SPI_processed;
        total_tuples += ntuples;
        if (!edges)
            edges = palloc(total_tuples * sizeof(edge_t));
        else
            edges = repalloc(edges, total_tuples * sizeof(edge_t));

        if (edges == NULL) {
            elog(ERROR, "Out of memory");
            return finish(SPIcode, ret);      
        }

        if (ntuples > 0) {
            int t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            for (t = 0; t < ntuples; t++) 
            {
                HeapTuple tuple = tuptable->vals[t];
                fetch_edge(&tuple, &tupdesc, &edge_columns, 
                         &edges[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } 
        else {
            moredata = FALSE;
        }
    }

    //defining min and max vertex id

    DBG("Total %i tuples", total_tuples);

    for(z=0; z<total_tuples; z++) {
        if(edges[z].source<v_min_id)
            v_min_id=edges[z].source;

        if(edges[z].source>v_max_id)
            v_max_id=edges[z].source;

        if(edges[z].target<v_min_id)
            v_min_id=edges[z].target;

        if(edges[z].target>v_max_id)
            v_max_id=edges[z].target;      

        // DBG("%i <-> %i", v_min_id, v_max_id);

    }

    //::::::::::::::::::::::::::::::::::::  
    //:: reducing vertex id (renumbering)
    //::::::::::::::::::::::::::::::::::::


    for (numTarget = 0; numTarget < nb_targets; numTarget ++) {
        (*t_count)[numTarget] = 0;
    }

    for(z=0; z<total_tuples; z++) {
        //check if edges[] contains source and target
        if(edges[z].source == start_vertex || edges[z].target == start_vertex)
            ++s_count;
        for (numTarget = 0; numTarget < nb_targets; numTarget++) {
            if (edges[z].source == end_vertices[numTarget] ||
                    edges[z].target == end_vertices[numTarget]) {
                ++((*t_count)[numTarget]);
                ++sumFoundTargets;    
            }
        }
        edges[z].source-=v_min_id;
        edges[z].target-=v_min_id;
        //  DBG("%i - %i", edges[z].source, edges[z].target);      
    }

    DBG("Total %i tuples", total_tuples);

    if(s_count == 0) {
        elog(ERROR, "Start vertex was not found.");
        return -1;
    }

    for (numTarget = 0; numTarget < nb_targets; numTarget++) {
        if((*t_count)[numTarget] == 0) {
            elog(ERROR, "One of the target vertices was not found or several targets are the same.");
        }
    }

    if(sumFoundTargets == 0 ) {
        elog(ERROR, "None of the target vertices has been found; aborting!");
        return -1;
    }

    DBG("Calling boost_dijkstra\n");

    start_vertex -= v_min_id;
    for (numTarget = 0; numTarget < nb_targets; numTarget++) {
        end_vertices[numTarget] -= v_min_id;
    }

    ret = onetomany_dijkstra_boostdist(edges, total_tuples, start_vertex,
                end_vertices, nb_targets, directed, has_reverse_cost,
                dists, &err_msg);

    DBG("ret = %i\n", ret);

    if (ret < 0) {
        //elog(ERROR, "Error computing path: %s", err_msg);
        ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), 
            errmsg("Error computing path: %s", err_msg)));
    } 

    //::::::::::::::::::::::::::::::::
    //:: restoring original vertex id
    //::::::::::::::::::::::::::::::::
    for(z=0; z<nb_targets; z++) {
#ifdef PGR_MERGE
        (*dists)[z].id1 += v_min_id;
        (*dists)[z].id2 += v_min_id;
#else
        (*dists)[z].vertex_id_source += v_min_id;
        (*dists)[z].vertex_id_target += v_min_id;
#endif
    }

    *path_count = nb_targets;

    return finish(SPIcode, ret);
}




PG_FUNCTION_INFO_V1(onetomany_dijkstra_dist);

PGDLLEXPORT Datum onetomany_dijkstra_dist(PG_FUNCTION_ARGS)
{

    FuncCallContext     *funcctx;
    int                  call_cntr;
    int                  max_calls;
    TupleDesc            tuple_desc;
#ifdef PGR_MERGE
    pgr_cost_t          *dist;
#else
    dist_fromto_t       *dist;
#endif

    char * sql = text2char(PG_GETARG_TEXT_P(0));
    int source_ID = PG_GETARG_INT32(1);
    int *myTargets = (int *)PG_GETARG_POINTER(2);
    int i;

    HeapTuple    tuple;
    Datum        result;
    Datum *values;
    char* nulls;    

    /* stuff done only on the first call of the function */
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        int path_count = 0;
        int ret;
        int nb_targets;
        int *t_count;

        DBG("source_ID = %d \n", source_ID);   
        DBG("There may be %d targets : \n", myTargets[4]);
        for (i = 0; i < myTargets[4]; i++)
            DBG("%d => %d\t", i+1, myTargets[6+i]);

        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* switch to memory context appropriate for multiple function calls */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        nb_targets = myTargets[4];
        t_count = (int *)malloc(nb_targets * sizeof(int));
        ret = tomanysp_dijkstra_dist(sql, source_ID, &(myTargets[6]),
                nb_targets, &t_count, PG_GETARG_BOOL(3), PG_GETARG_BOOL(4),
                &dist, &path_count);

        if (ret) {
            elog(ERROR, "Error computing paths!");
        }

        DBG("max_calls: %d", path_count);
        funcctx->max_calls = path_count;
        funcctx->user_fctx = dist;

        DBG("tuple_desc");
#ifdef PGR_MERGE
        funcctx->tuple_desc = BlessTupleDesc(RelationNameGetTupleDesc("pgr_costresult"));
#else
        funcctx->tuple_desc = BlessTupleDesc(RelationNameGetTupleDesc("dist_result"));
#endif

        free(t_count);
        MemoryContextSwitchTo(oldcontext);
    }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
#ifdef PGR_MERGE
    dist = (pgr_cost_t *) funcctx->user_fctx;
#else
    dist = (dist_fromto_t *) funcctx->user_fctx;
#endif

    /* do when there is more left to send */
    if (call_cntr < max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum       *values;
        char        *nulls;

        DBG("INIT values && nulls");
#ifdef PGR_MERGE
        values = palloc(4 * sizeof(Datum));
        nulls = palloc(4 * sizeof(char));

        values[0] = Int32GetDatum(dist[call_cntr].seq);
        nulls[0] = ' ';
        values[1] = Int32GetDatum(dist[call_cntr].id1);
        nulls[1] = ' ';
        values[2] = Int32GetDatum(dist[call_cntr].id2);
        nulls[2] = ' ';
        values[3] = Float8GetDatum(dist[call_cntr].cost);
        nulls[3] = ' ';
#else
        values = palloc(5 * sizeof(Datum));
        nulls = palloc(5 * sizeof(char));

        values[0] = Int32GetDatum(dist[call_cntr].vertex_id_source);
        nulls[0] = ' ';
        values[1] = Int32GetDatum(dist[call_cntr].edge_id_source);
        nulls[1] = ' ';
        values[2] = Int32GetDatum(dist[call_cntr].vertex_id_target);
        nulls[2] = ' ';
        values[3] = Int32GetDatum(dist[call_cntr].edge_id_target);
        nulls[4] = ' ';
        values[4] = Float8GetDatum(dist[call_cntr].cost);
        nulls[4] = ' ';
#endif

        DBG("Create the tuple");
        tuple = heap_formtuple(tuple_desc, values, nulls);

        /* make the tuple into a datum */

        DBG("HeapTupleGetDatum");
        result = HeapTupleGetDatum(tuple);

        /* clean up (this is not really necessary) */

        DBG("pfree");
        pfree(values);
        pfree(nulls);

        DBG("return next");
        SRF_RETURN_NEXT(funcctx, result);
    }

    DBG("freeing dist");
    free(dist);

    SRF_RETURN_DONE(funcctx);
}

static int tomanysp_dijkstra_ways(char* sql, int start_vertex, 
                int *end_vertices, int nb_targets, int **t_count, bool directed, 
                bool has_reverse_cost, 
#ifdef PGR_MERGE
                pgr_cost3_t **distpaths,
#else
                path_fromto_t **distpaths,
#endif
                int *path_count) 
{
    int SPIcode;
    void *SPIplan;
    Portal SPIportal;
    bool moredata = TRUE;
    int ntuples;
    edge_t *edges = NULL;
    int total_tuples = 0;
#ifndef _MSC_VER
    edge_columns_t edge_columns = {.id= -1, .source= -1, .target= -1, 
                                   .cost= -1, .reverse_cost= -1};
#else // _MSC_VER
    edge_columns_t edge_columns = {-1, -1, -1, -1, -1};
#endif // _MSC_VER
    int v_max_id=0;
    int v_min_id=INT_MAX;

    int sumFoundTargets = 0, numTarget;

    int s_count = 0;

    int pcount = 0;

    char *err_msg;
    int ret = -1;
    register int z;

    for (numTarget = 0; numTarget < nb_targets; numTarget++)
        (*t_count)[numTarget] = 0;

    SPIcode = SPI_connect();
    if (SPIcode  != SPI_OK_CONNECT) {
        elog(ERROR, "shortest_path: couldn't open a connection to SPI");
        return -1;
    }

    SPIplan = SPI_prepare(sql, 0, NULL);
    if (SPIplan  == NULL) {
        elog(ERROR, "shortest_path: couldn't create query plan via SPI");
        return -1;
    }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
      elog(ERROR, "shortest_path: SPI_cursor_open('%s') returns NULL", sql);
      return -1;
    }

    DBG("Maybe there is moredata...\n");

    while (moredata == TRUE) {
        SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

        if (edge_columns.id == -1) {
            if (fetch_edge_columns(SPI_tuptable, &edge_columns, 
                                 has_reverse_cost) == -1)
            return finish(SPIcode, ret);
        }

        ntuples = SPI_processed;
        total_tuples += ntuples;
        if (!edges)
            edges = palloc(total_tuples * sizeof(edge_t));
        else
            edges = repalloc(edges, total_tuples * sizeof(edge_t));

        if (edges == NULL) {
            elog(ERROR, "Out of memory");
            return finish(SPIcode, ret);      
        }

        if (ntuples > 0) {
            int t;
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;

            for (t = 0; t < ntuples; t++) {
                HeapTuple tuple = tuptable->vals[t];
                fetch_edge(&tuple, &tupdesc, &edge_columns, 
                         &edges[total_tuples - ntuples + t]);
            }
            SPI_freetuptable(tuptable);
        } 
        else {
            moredata = FALSE;
        }
    }

    //defining min and max vertex id

    DBG("Total %i tuples", total_tuples);

    for(z=0; z<total_tuples; z++) {
        if(edges[z].source<v_min_id)
            v_min_id=edges[z].source;

        if(edges[z].source>v_max_id)
            v_max_id=edges[z].source;

        if(edges[z].target<v_min_id)
            v_min_id=edges[z].target;

        if(edges[z].target>v_max_id)
            v_max_id=edges[z].target;      

        //DBG("%i <-> %i", v_min_id, v_max_id);                    
    }

    //::::::::::::::::::::::::::::::::::::  
    //:: reducing vertex id (renumbering)
    //::::::::::::::::::::::::::::::::::::


    for (numTarget = 0; numTarget < nb_targets; numTarget ++){
        (*t_count)[numTarget] = 0;
    }

    for(z=0; z<total_tuples; z++) {
        //check if edges[] contains source and target
        if(edges[z].source == start_vertex || edges[z].target == start_vertex)
            ++s_count;
        for (numTarget = 0; numTarget < nb_targets; numTarget++) {
            if(edges[z].source == end_vertices[numTarget] || edges[z].target == end_vertices[numTarget]) {
                ++((*t_count)[numTarget]);
                ++sumFoundTargets;    
            }
        }
        edges[z].source-=v_min_id;
        edges[z].target-=v_min_id;
        //DBG("%i - %i", edges[z].source, edges[z].target);      
    }

    DBG("Total %i tuples", total_tuples);

    if(s_count == 0) {
        elog(ERROR, "Start vertex was not found.");
        return -1;
    }

    for (numTarget = 0; numTarget < nb_targets; numTarget++)
        if((*t_count)[numTarget] == 0) {
            elog(ERROR, "One of the target vertices was not found or several targets are the same.");
        }

    if(sumFoundTargets == 0 ) {
        elog(ERROR, "None of the target vertices has been found; aborting!");
        return -1;
    }

    DBG("modifying vertices' idz..\n");

    start_vertex -= v_min_id;
    for (numTarget = 0; numTarget < nb_targets; numTarget++) {
        end_vertices[numTarget] -= v_min_id;
    }

    DBG("Calling boost_dijkstra\n");


    ret = onetomany_dijkstra_boostpath (edges, total_tuples, start_vertex,
            end_vertices, nb_targets, 
            directed, has_reverse_cost,
            distpaths,
#ifdef PGR_MERGE
            &pcount,
#endif
            &err_msg);

    DBG("ret = %i\n", ret);

    if (ret < 0) {
        ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), 
            errmsg("Error computing path: %s", err_msg)));
    }

    //::::::::::::::::::::::::::::::::
    //:: restoring original vertex id
    //::::::::::::::::::::::::::::::::

#ifdef PGR_MERGE
    for(z=0; z<pcount; z++) {
        (*distpaths)[z].id1 += v_min_id;
        (*distpaths)[z].id2 += v_min_id;
    }

    *path_count = pcount;
#else
    for(z=0; z<nb_targets; z++) {
        (*distpaths)[z].vertex_id_source += v_min_id;
        (*distpaths)[z].vertex_id_target += v_min_id;

        DBG("(*distpaths)[z].cost = %f)", (*distpaths)[z].cost);
    }
#endif

    DBG("*path_count = %i\n", *path_count);

    return finish(SPIcode, ret);
}

/**/

PG_FUNCTION_INFO_V1(onetomany_dijkstra_path);

PGDLLEXPORT Datum
onetomany_dijkstra_path(PG_FUNCTION_ARGS)
{

    FuncCallContext     *funcctx;
    int                  call_cntr;
    int                  max_calls;
    TupleDesc            tuple_desc;

#ifdef PGR_MERGE
    pgr_cost3_t          *path_res;
#else
    path_fromto_t       *path_res;
#endif

    char * sql = text2char(PG_GETARG_TEXT_P(0));
    int source_ID = PG_GETARG_INT32(1);
    int *myTargets = (int *)PG_GETARG_POINTER(2);
    int i;

    HeapTuple    tuple;
    Datum        result;
    Datum *values;
    char* nulls;    

    /* stuff done only on the first call of the function */
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        int path_count = 0;
        int ret;
        int nb_targets;
        int *t_count;


        DBG("source_ID = %d \n", source_ID);   
        DBG("There may be %d targets : \n", myTargets[4]);
        for (i = 0; i < myTargets[4]; i++)
            DBG("%d => %d\t", i+1, myTargets[6+i]);

        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* switch to memory context appropriate for multiple function calls */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        nb_targets = myTargets[4];
        t_count = (int *)malloc(nb_targets * sizeof(int));
        ret = tomanysp_dijkstra_ways(sql, source_ID, &(myTargets[6]),
                nb_targets, &t_count, PG_GETARG_BOOL(3),
                PG_GETARG_BOOL(4), &path_res, &path_count);

        if (ret) {
            elog(ERROR, "Error computing paths!");
        }

        /*  Building the whole linestrings in the_way */
        DBG("max_calls: %d", path_count);
        funcctx->max_calls = path_count;
        funcctx->user_fctx = path_res;

        DBG("tuple_desc");
#ifdef PGR_MERGE
        funcctx->tuple_desc = BlessTupleDesc(RelationNameGetTupleDesc("pgr_costresult3"));
#else
        funcctx->tuple_desc = BlessTupleDesc(RelationNameGetTupleDesc("concatpath_result"));
#endif

        free(t_count);
        MemoryContextSwitchTo(oldcontext);

    }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr = funcctx->call_cntr;
    max_calls = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
#ifdef PGR_MERGE
    path_res = (pgr_cost3_t *) funcctx->user_fctx;
#else
    path_res = (path_fromto_t *) funcctx->user_fctx;
#endif

    /* do when there is more left to send */
    if (call_cntr < max_calls) {
        HeapTuple    tuple;
        Datum        result;
        Datum       *values;
        char        *nulls;

        DBG("INIT values && nulls");
#ifdef PGR_MERGE
        values = palloc(5 * sizeof(Datum));
        nulls = palloc(5 * sizeof(char));

        values[0] = Int32GetDatum(path_res[call_cntr].seq);
        nulls[0] = ' ';
        values[1] = Int32GetDatum(path_res[call_cntr].id1);
        nulls[1] = ' ';
        values[2] = Int32GetDatum(path_res[call_cntr].id2);
        nulls[2] = ' ';
        values[3] = Int32GetDatum(path_res[call_cntr].id3);
        nulls[3] = ' ';
        values[4] = Float8GetDatum(path_res[call_cntr].cost);
        nulls[4] = ' ';
#else
        values = palloc(6 * sizeof(Datum));
        nulls = palloc(6 * sizeof(char));

        values[0] = Int32GetDatum(path_res[call_cntr].vertex_id_source);
        nulls[0] = ' ';
        values[1] = Int32GetDatum(path_res[call_cntr].edge_id_source);
        nulls[1] = ' ';
        values[2] = Int32GetDatum(path_res[call_cntr].vertex_id_target);
        nulls[2] = ' ';
        values[3] = Int32GetDatum(path_res[call_cntr].edge_id_target);
        nulls[3] = ' ';
        values[4] = Float8GetDatum(path_res[call_cntr].cost);
        nulls[4] = ' ';
        values[5] = PointerGetDatum(char2text(path_res[call_cntr].the_way));
        nulls[5] = ' ';
#endif

        DBG("Create the tuple");
        tuple = heap_formtuple(tuple_desc, values, nulls);

        /* make the tuple into a datum */
        DBG("HeapTupleGetDatum");
        result = HeapTupleGetDatum(tuple);

        /* clean up (this is not really necessary) */
        DBG("pfree");
        pfree(values);
        pfree(nulls);

        DBG("return next");
        SRF_RETURN_NEXT(funcctx, result);
    }

    DBG("freeing path_res");
    free(path_res);

    DBG("return done");
    SRF_RETURN_DONE(funcctx);    
}

