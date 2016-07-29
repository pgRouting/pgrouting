/*PGR-GNU*****************************************************************
File: arrays_input.c

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#include "./pgr_types.h"
#include "postgres.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#include "utils/array.h"


// #define DEBUG
#include "./time_msg.h"
#include "./debug_macro.h"
#include "./arrays_input.h"


int64_t* pgr_get_bigIntArray(size_t *arrlen, ArrayType *input) {
    int         ndims;
    bool       *nulls;
    Oid         i_eltype;
    int16       i_typlen;
    bool        i_typbyval;
    char        i_typalign;
    Datum      *i_data;
    int         i;
    int         n;
    int64_t      *data;
    clock_t start_t = clock();

    PGR_DBG("Geting integer array");
    /* get input array element type */
    i_eltype = ARR_ELEMTYPE(input);
    get_typlenbyvalalign(i_eltype, &i_typlen, &i_typbyval, &i_typalign);

    /* validate input data type */
    switch (i_eltype) {
        case INT2OID:
        case INT4OID:
        case INT8OID:
            break;
        default:
            elog(ERROR, "Expected array of ANY-INTEGER");
            return (int64_t*) NULL;
            break;
    }

    /* get various pieces of data from the input array */
    ndims = ARR_NDIM(input);
    n = (*ARR_DIMS(input));
    (*arrlen) = (size_t)(n);

    if (ndims != 1) {
        elog(ERROR, "One dimension expected");
    }

    /* get src data */
    deconstruct_array(input, i_eltype, i_typlen, i_typbyval, i_typalign,
            &i_data, &nulls, &n);

    /* construct a C array */
    data = (int64_t *) malloc((*arrlen) * sizeof(int64_t));

    if (!data) {
        elog(ERROR, "Out of memory!");
    }

    PGR_DBG("array size %ld", (*arrlen));

    for (i = 0; i < (*arrlen); i++) {
        if (nulls[i]) {
            free(data);
            elog(ERROR, "NULL value found in Array!");
        } else {
            switch (i_eltype) {
                case INT2OID:
                    data[i] = (int64_t) DatumGetInt16(i_data[i]);
                    break;
                case INT4OID:
                    data[i] = (int64_t) DatumGetInt32(i_data[i]);
                    break;
                case INT8OID:
                    data[i] = DatumGetInt64(i_data[i]);
                    break;
            }
        }
    }

    pfree(nulls);
    pfree(i_data);

    PGR_DBG("Finished processing array");
    time_msg(" reading Array", start_t, clock());
    return (int64_t*)data;
}



int64_t* pgr_get_bigIntArray_allowEmpty(size_t *arrlen, ArrayType *input) {
    int         ndims;
    bool       *nulls;
    Oid         i_eltype;
    int16       i_typlen;
    bool        i_typbyval;
    char        i_typalign;
    Datum      *i_data;
    int         i;
    int         n;
    int64_t      *data;
    clock_t start_t = clock();

    PGR_DBG("Geting integer array");
    /* get input array element type */
    i_eltype = ARR_ELEMTYPE(input);
    get_typlenbyvalalign(i_eltype, &i_typlen, &i_typbyval, &i_typalign);

    /* validate input data type */
    switch (i_eltype) {
        case INT2OID:
        case INT4OID:
        case INT8OID:
            break;
        default:
            elog(ERROR, "Expected array of ANY-INTEGER");
            return (int64_t*) NULL;
            break;
    }

    /* get various pieces of data from the input array */
    ndims = ARR_NDIM(input);
    n = (*ARR_DIMS(input));
    (*arrlen) = (size_t)(n);
    // PGR_DBG("dimensions %d", ndims);
    // PGR_DBG("array size %ld", (*arrlen));

    if (ndims == 0) {
        (*arrlen) = 0;
        PGR_DBG("array size %ld", (*arrlen));
        return (int64_t*) NULL;
    }

    if (ndims > 1) {
        elog(ERROR, "Expected less than two dimension");
    }

    /* get src data */
    deconstruct_array(input, i_eltype, i_typlen, i_typbyval, i_typalign,
            &i_data, &nulls, &n);

    /* construct a C array */
    data = (int64_t *) malloc((*arrlen) * sizeof(int64_t));

    if (!data) {
        elog(ERROR, "Out of memory!");
    }

    PGR_DBG("array size %ld", (*arrlen));

    for (i = 0; i < (*arrlen); i++) {
        if (nulls[i]) {
            free(data);
            elog(ERROR, "NULL value found in Array!");
        } else {
            switch (i_eltype) {
                case INT2OID:
                    data[i] = (int64_t) DatumGetInt16(i_data[i]);
                    break;
                case INT4OID:
                    data[i] = (int64_t) DatumGetInt32(i_data[i]);
                    break;
                case INT8OID:
                    data[i] = DatumGetInt64(i_data[i]);
                    break;
            }
        }
    }

    pfree(nulls);
    pfree(i_data);

    PGR_DBG("Finished processing array");
    time_msg(" reading Array", start_t, clock());
    return (int64_t*)data;
}
