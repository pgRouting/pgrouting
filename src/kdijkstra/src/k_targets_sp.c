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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "utils/array.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"

#include "k_targets_sp.h"

Datum manytomany_dijkstra_dmatrix(PG_FUNCTION_ARGS);

#undef DEBUG
//#define DEBUG 1

#include "../../common/src/debug_macro.h"

// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000


typedef struct edge_columns
{
  int id;
  int source;
  int target;
  int cost;
  int reverse_cost;
} edge_columns_t;




static char * text2char(text *in)
{
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

#define DTYPE int

static DTYPE *get_pgarray(int *num, ArrayType *input)
{
    int         ndims, *dims; // , *lbs;
    bool       *nulls;
    Oid         i_eltype;
    int16       i_typlen;
    bool        i_typbyval;
    char        i_typalign;
    Datum      *i_data;
    int         i, n;
    DTYPE      *data;

    /* get input array element type */
    i_eltype = ARR_ELEMTYPE(input);
    get_typlenbyvalalign(i_eltype, &i_typlen, &i_typbyval, &i_typalign);


    /* validate input data type */
    switch(i_eltype){
    case INT2OID:
    case INT4OID:
    case FLOAT4OID:
    case FLOAT8OID:
            break;
    default:
            elog(ERROR, "target must be integer[]");
            break;
    }

    /* get various pieces of data from the input array */
    ndims = ARR_NDIM(input);
    dims = ARR_DIMS(input);
    // lbs = ARR_LBOUND(input);

    if (ndims != 1) {
        elog(ERROR, "target must be integer[]");
    }

    /* get src data */
    deconstruct_array(input, i_eltype, i_typlen, i_typbyval, i_typalign,
&i_data, &nulls, &n);

    PGR_DBG("get_pgarray: ndims=%d, n=%d", ndims, n);

#ifdef DEBUG
    for (i=0; i<ndims; i++) {
        PGR_DBG("   dims[%d]=%d, lbs[%d]=%d", i, dims[i], i, lbs[i]);
    }
#endif

    /* construct a C array */
    data = (DTYPE *) palloc(n * sizeof(DTYPE));
    if (!data) {
        elog(ERROR, "Error: Out of memory!");
    }

    for (i=0; i<n; i++) {
        if (nulls[i]) {
            data[i] = -1;
        }
        else {
            switch(i_eltype){
                case INT2OID:
                    data[i] = (DTYPE) DatumGetInt16(i_data[i]);
                    break;
                case INT4OID:
                    data[i] = (DTYPE) DatumGetInt32(i_data[i]);
                    break;
                case FLOAT4OID:
                    data[i] = (DTYPE) DatumGetFloat4(i_data[i]);
                    break;
                case FLOAT8OID:
                    data[i] = (DTYPE) DatumGetFloat8(i_data[i]);
                    break;
            }
        }
        PGR_DBG("    data[%d]=%.4f", i, data[i]);
    }

    pfree(nulls);
    pfree(i_data);

    *num = dims[0];

    return data;
}


static int
finish(int code, int ret)
{  
  PGR_DBG("In finish, trying to disconnect from spi %d",ret);

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

    PGR_DBG("columns: id %i source %i target %i cost %i", 
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

        PGR_DBG("columns: reverse_cost cost %i", edge_columns->reverse_cost);
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





static int many2many_dijkstra_dm(char *sql, int *vids, int num, bool directed,
        bool has_reverse_cost, bool symmetric, float8 *dm)
{
    int SPIcode;
    void *SPIplan;
    Portal SPIportal;
    bool moredata = TRUE;
    int ntuples;
    edge_t *edges = NULL;
    int total_tuples = 0;
    edge_columns_t edge_columns = {.id= -1, .source= -1, .target= -1,
                                   .cost= -1, .reverse_cost= -1};
    int v_max_id = 0;
    int v_min_id = INT_MAX;

    // int sumFoundVids = 0;

    int i, j;
    int zcnt = 0;

    int vvids[num];
    int v_count[num];
    for (i=0; i<num; i++)
        v_count[i] = 0;

    char *err_msg;
    int ret = -1;

    pgr_cost_t *dists;

    PGR_DBG("start many2many_dijkstra_dm");

    SPIcode = SPI_connect();
    if (SPIcode != SPI_OK_CONNECT) {
        elog(ERROR, "many2many_dijkstra_dm: couldn't open an SPI connection");
        return -1;
    }

    PGR_DBG("Calling SPI_prepare");
    SPIplan = SPI_prepare(sql, 0, NULL);
    if (SPIplan == NULL) {
        elog(ERROR, "many2many_dijkstra_dm: SPI_prepare failed for (%s)", sql);
        return -1;
    }

    PGR_DBG("Calling SPI_cursor_open");
    SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true);
    if (SPIportal == NULL) {
        elog(ERROR, "many2many_dijkstra_dm: SPI_cursor_open(%s) failed!", sql);
        return -1;
    }

    PGR_DBG("Starting while loop to collect edges ...");

    while (moredata == TRUE) {
        PGR_DBG("Calling SPI_cursor_fetch");
        SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

        if (fetch_edge_columns(SPI_tuptable, &edge_columns,
                has_reverse_cost) == -1) {
            return finish(SPIcode, ret);
        }

        ntuples = SPI_processed;
        PGR_DBG("ntuples=%d", ntuples);
        if (ntuples > 0) {
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
            int t;

            total_tuples += ntuples;

            if (!edges)
                edges = palloc(total_tuples * sizeof(edge_t));
            else
                edges = repalloc(edges, total_tuples * sizeof(edge_t));

            if (edges == NULL) {
                elog(ERROR, "Out of memory!");
                return finish(SPIcode, ret);
            }

            for (t=0; t < ntuples; t++) {
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
    PGR_DBG("Total %d edges!", total_tuples);

    // find min and max vertex ids

    for (i=0; i<total_tuples; i++) {
        if (edges[i].source < v_min_id) v_min_id = edges[i].source;
        if (edges[i].source > v_max_id) v_max_id = edges[i].source;
        if (edges[i].target < v_min_id) v_min_id = edges[i].target;
        if (edges[i].target > v_max_id) v_max_id = edges[i].target;
    }
    PGR_DBG("v_min_id: %d, v_max_id: %d", v_min_id, v_max_id);

    // renumber vertices

    for (i=0; i<total_tuples; i++) {
        // check if edge contains vids[]
        for (j=0; j<num; j++) {
            if (edges[i].source == vids[j] || edges[i].target == vids[j])
                v_count[j]++;
        }
        edges[i].source -= v_min_id;
        edges[i].target -= v_min_id;
    }

    for (j=0; j< num; j++) {
        if (v_count[j] == 0) zcnt++;
        PGR_DBG("vids[%d]: %d, cnt: %d", j, vids[j], v_count[j]);
        vvids[j] = vids[j] - v_min_id;
    }

    if (zcnt > 0) {
        elog(ERROR, "%d vid(s) were not found in the edges!", zcnt);
        return -1;
    }

    PGR_DBG("Starting loop to build dmatrix!");

    for (j=0; j<num; j++) {
        PGR_DBG("Calling onetomany_dijkstra_boostdist j=%d", j);

        ret = onetomany_dijkstra_boostdist(edges, total_tuples, vvids[j],
                vvids, num, directed, has_reverse_cost, &dists, &err_msg);

        if (ret < 0) {
            elog(ERROR, "onetomany_dijkstra_boostdist failed on j=%d", j);
        }

        // ASSUMING: results are in order of vvids array
        for (i=0; i<num; i++) {
            dm[j*num + i] = dists[i].cost;
        }

        free(dists);
        dists = NULL;
    }
    
    PGR_DBG("Making the matrix symmertic if requested!");

    // if symmetric requsted, then average cells to make it symmetric

    if (symmetric) {
        for (i=0; i<num; i++) {
            dm[i * num + i] = 0.0;
            for (j=i+1; j<num; j++) {
                if (dm[j*num + i] < 0.0 && dm[i*num + j] > 0.0)
                    dm[j*num + i] = dm[i*num + j];
                else if (dm[i*num + j] < 0.0 && dm[j*num + i] > 0.0)
                    dm[i*num + j] = dm[j*num + i];
                else if (dm[j*num + i] < 0.0 && dm[i*num + j] < 0.0)
                    dm[i*num + j] = dm[j*num + i] = -1.0;
                else
                    dm[j*num + i] = dm[i*num + j] = (dm[j*num + i] + dm[i*num + j]) / 2.0;
            }
        }
    }

    PGR_DBG("Leaving many2many_dijkstra_dm");

    return finish(SPIcode, ret);
}

/*
    create or replace function pgr_vidsToDMatrix(sql text,
        vids integer[], dir bool, has_rcost bool, symmetric bool)
    return real8[]
    as '', 'manytomany_dijkstra_dmatrix' language C stable strict;

*/

PG_FUNCTION_INFO_V1(manytomany_dijkstra_dmatrix);

Datum manytomany_dijkstra_dmatrix(PG_FUNCTION_ARGS)
{
    ArrayType   *result;
    Datum  *result_data;
    // Datum  *values;
    bool   *nulls;
    int     i;
    int     num;
    int     ret;
    float8 *dm;

    Oid     o_eltype   = FLOAT8OID;
    int16   o_typlen;
    bool    o_typbyval;
    char    o_typalign;

    int     ndims = 2;
    int     dims[2];
    int     lbs[2] = {1, 1};;

    char   *sql = text2char(PG_GETARG_TEXT_P(0));
    int    *vids = get_pgarray(&num, PG_GETARG_ARRAYTYPE_P(1));

    dm = (float8 *)palloc(num * num * sizeof(float8));

    ret = many2many_dijkstra_dm(sql, vids, num,
        PG_GETARG_BOOL(2), PG_GETARG_BOOL(3), PG_GETARG_BOOL(4),
        dm);
    
    if (ret) {
        elog(ERROR, "Error computing distance matrix!");
    }

    result_data = (Datum *)palloc(num * num * sizeof(Datum));
    nulls       = (bool *)palloc(num * num * sizeof(bool));

    for (i=0; i<num*num; i++) {
        if (dm[i] < 0.0) {
            nulls[i] = true;
            result_data[i] = PointerGetDatum(NULL);
        }
        else {
            nulls[i] = false;
            result_data[i] = Float8GetDatum((float8)dm[i]);
        }
    }

    pfree(dm);

    dims[0] = dims[1] = num;
    get_typlenbyvalalign(o_eltype, &o_typlen, &o_typbyval, &o_typalign);

    result = construct_md_array((void *)result_data, nulls, ndims, dims,
        lbs, o_eltype, o_typlen, o_typbyval, o_typalign);

    pfree(result_data);
    pfree(nulls);

    PG_RETURN_ARRAYTYPE_P(result);
}

