/*
 * Traveling Sales Problem solver for pgRouting and PostgreSQL
 *
 * Copyright 2013, Stephen Woodbridge, iMaptools.com
 * This program is released under an MIT-X license.
 *
*/

#include "tsp.h"
#include <math.h>

#include "postgres.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#include "utils/array.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#undef DEBUG
//#define DEBUG 1

#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif

#ifndef INFINITY
#define INFINITY (1.0/0.0)
#endif

// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000

static DTYPE *get_pgarray(int *num, ArrayType *input)
{
    int         ndims, *dims, *lbs;
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
            elog(ERROR, "Invalid input data type");
            break;
    }

    /* get various pieces of data from the input array */
    ndims = ARR_NDIM(input);
    dims = ARR_DIMS(input);
    lbs = ARR_LBOUND(input);

    if (ndims != 2 || dims[0] != dims[1]) {
        elog(ERROR, "Error: matrix[num][num] in its definition.");
    }

    /* get src data */
    deconstruct_array(input, i_eltype, i_typlen, i_typbyval, i_typalign,
&i_data, &nulls, &n);

    DBG("get_pgarray: ndims=%d, n=%d", ndims, n);

#ifdef DEBUG
    for (i=0; i<ndims; i++) {
        DBG("   dims[%d]=%d, lbs[%d]=%d", i, dims[i], i, lbs[i]);
    }
#endif

    /* construct a C array */
    data = (DTYPE *) palloc(n * sizeof(DTYPE));
    if (!data) {
        elog(ERROR, "Error: Out of memory!");
    }

    for (i=0; i<n; i++) {
        if (nulls[i]) {
            data[i] = INFINITY;
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
            /* we assume negative values are INFINTY */
            /********************************************************
               TODO: based on trying to add an endpt it is likely
               that this will not work and you will get and error in
               findEulerianPath
            **********************************************************/
            if (data[i] < 0)
                data[i] = INFINITY;
        }
        DBG("    data[%d]=%.4f", i, data[i]);
    }

    pfree(nulls);
    pfree(i_data);

    *num = dims[0];

    return data;
}


// macro to store distance values as matrix[num][num]
#define D(i,j) matrix[(i)*num + j]

static int solve_tsp(DTYPE *matrix, int num, int start, int end, int **results)
{
    int ret;
    int i;
    int *ids;
    DTYPE fit;
    char *err_msg;

    DBG("In solve_tsp: num: %d, start: %d, end: %d", num, start, end);

    if (num < 4)
        elog(ERROR, "Error TSP requires four or more locations to optimize. Only %d were supplied.", num);

    if (start < 0 || start >= num)
        elog(ERROR, "Error start must be in the range of 0 <= start(%d) < num(%d).", start, num);

    if (end >= num)
        elog(ERROR, "Error end must be in the range of 0 <= end(%d) < num(%d).", end, num);

    /* if start and end are the same this is the same as setting end = -1 */
    if (start == end)
        end = -1;

    /*
       fix up matrix id we have an end point
       basically set D(start,end)=INFINITY and D(end,start)=0.0
    */
    if (end >= 0) {
        DBG("Updating start end costs");
        D(start,end)=0.0;
        D(end,start)=0.0;
    }

    DBG("Alloc ids");

    ids = (int *) palloc(num * sizeof(int));
    if (!ids) {
        elog(ERROR, "Error: Out of memory (solve_tsp)");
    }

    for(i=0; i<num; i++) {
        ids[i] = i;
    }

    DBG("Calling find_tsp_solution");

// int find_tsp_solution(int num, DTYPE *dist, int *p_ids, int source, DTYPE *fit, char* err_msg);
    ret = find_tsp_solution(num, matrix, ids, start, end, &fit, err_msg);
    if (ret < 0) {
        elog(ERROR, "Error solving TSP, %s", err_msg);
    }

    DBG("TSP solved, Score: %f", fit);

    *results = ids;
    return ret;
}

/*
 * select seq, id from pgr_tsp(matrix float8[][], start int,
 *                             OUT seq int, OUT id int);
*/

PG_FUNCTION_INFO_V1(tsp_matrix);
Datum
tsp_matrix(PG_FUNCTION_ARGS)
{
    FuncCallContext     *funcctx;
    int                  call_cntr;
    int                  max_calls;
    TupleDesc            tuple_desc;
    AttInMetadata       *attinmeta;

    DTYPE               *matrix;
    int                 *tsp_res;
    int                  num;

    /* stuff done only on the first call of the function */
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        //int path_count;
        int ret=-1;

        /* create a function context for cross-call persistence */
        funcctx = SRF_FIRSTCALL_INIT();

        /* switch to memory context appropriate for multiple function calls */
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        matrix = get_pgarray(&num, PG_GETARG_ARRAYTYPE_P(0));

        ret = solve_tsp(matrix, num,
                        PG_GETARG_INT32(1), // start index
                        PG_GETARG_INT32(2), // end  index 
                        &tsp_res);

        pfree(matrix);
        if (ret < 0) {
            elog(ERROR, "Error, failed to solve TSP.");
        }

        funcctx->max_calls = num;
        funcctx->user_fctx = tsp_res;

        /* Build a tuple descriptor for our result type */
        if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                            "that cannot accept type record")));

        funcctx->tuple_desc = BlessTupleDesc(tuple_desc);

        /*
         * generate attribute metadata needed later to produce tuples from raw
         * C strings
         */
        //attinmeta = TupleDescGetAttInMetadata(tuple_desc);
        //funcctx->attinmeta = attinmeta;

        MemoryContextSwitchTo(oldcontext);
    }

    /* stuff done on every call of the function */
    funcctx = SRF_PERCALL_SETUP();

    call_cntr  = funcctx->call_cntr;
    max_calls  = funcctx->max_calls;
    tuple_desc = funcctx->tuple_desc;
    tsp_res    = funcctx->user_fctx;

    DBG("Trying to allocate some memory");
    DBG("call_cntr = %i, max_calls = %i", call_cntr, max_calls);

    if (call_cntr < max_calls) {   /* do when there is more left to send */
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        char* nulls;

        values = palloc(2 * sizeof(Datum));
        nulls = palloc(2 * sizeof(char));

        values[0] = Int32GetDatum(call_cntr);
        nulls[0] = ' ';
        values[1] = Int32GetDatum(tsp_res[call_cntr]);
        nulls[1] = ' ';

        DBG("RESULT: %d, %d", call_cntr, tsp_res[call_cntr]);

        DBG("Heap making");

        tuple = heap_formtuple(tuple_desc, values, nulls);

        DBG("Datum making");

        /* make the tuple into a datum */
        result = HeapTupleGetDatum(tuple);

        DBG("RESULT: seq:%d, id:%d", call_cntr, tsp_res[call_cntr]);
        DBG("Trying to free some memory");

        /* clean up (this is not really necessary) */
        pfree(values);
        pfree(nulls);


        SRF_RETURN_NEXT(funcctx, result);
    }
    else {   /* do when there is no more left */
        DBG("Ending function");

        SRF_RETURN_DONE(funcctx);
    }
}

