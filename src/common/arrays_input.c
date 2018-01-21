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

#include "c_common/arrays_input.h"

#include <assert.h>
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"


#include "c_common/time_msg.h"
#include "c_common/debug_macro.h"

/*!
@brief Function returns the values of input array by checking its overflow conditions.

@param v array object to examine.
@param arrlen Number of elements in an array.
@param allow_empty Tell whether array can empty or not.


### Functions Used ###

@b get_typlenbyvalalign() : Given the type of OID return the value of typlen, typbyval, typalign.

@b deconstruct_array() : Simple method for extracting data from an array.

###Description###

Function will check following conditions:

- Return @b NULL if array is empty.
  ~~~~~~~~~~~~~~~~{.c}
  if (allow_empty && (ndim == 0 || nitems <= 0)) {
        PGR_DBG("ndim %i nitems % i", ndim, nitems);
        return (int64_t*) NULL;
  ~~~~~~~~~~~~~~~~

- Return @b NULL if array is multi-dimension or no dimension. 
  ~~~~~~~~~~~~~~~~{.c}
  if (ndim != 1) {
        elog(ERROR, "One dimension expected");
        return (int64_t*)NULL;
  ~~~~~~~~~~~~~~~~

- Return @b NULL if no element found in an array. 
  ~~~~~~~~~~~~~~~~{.c}
  if (nitems <= 0) {
        elog(ERROR, "No elements found");
        return (int64_t*)NULL;
  ~~~~~~~~~~~~~~~~

- Validate input data type is @b ANY-INTEGER or not .

Store the value of an array in c_array and return.

@return Return the pointer of an array.

#### References  ####
[Postgres arrays](https://doxygen.postgresql.org/array_8h_source.html)
[ArrayGetNItems](https://doxygen.postgresql.org/arrayutils_8c.html#a62c174f90143d9a1f45390acbd2b3d4a)
[deconstruct_array()](https://doxygen.postgresql.org/arrayfuncs_8c.html#a96051c6250f4b1eb534b65f6e0dd48bc)
[get_typlenbyvalalign()](https://doxygen.postgresql.org/lsyscache_8c.html#a4fb2f1568d3f42aa631ba12678b13518)
[typlen](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h_source.html#l00044)
[typbyval](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h_source.html#l00053)
[typalign](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h_source.html#l00130)
[fetch_att](https://doxygen.postgresql.org/tupmacs_8h.html#a9f681b643a0cfd41c5e3c6aa9b5f8dee)
[INT2OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a31e54a35a82c0ec11b9952a46f4d7af3)
[INT4OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#af473c8bca64740230ff280c0de2c5721)
[INT8OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a59b14b0f52e041ced4e021121ed0b31b)
[DatumGetInt16](https://doxygen.postgresql.org/postgres_8h.html#aec991e04209850f29a8a63df0c78ba2d)
[DatumGetInt32](https://doxygen.postgresql.org/postgres_8h.html#aacbc8a3ac6d52d85feaf0b7ac1b1160c)
[DatumGetInt62](https://doxygen.postgresql.org/postgres_8h.html#aedf6286d5147eaf3c6f7e998f2662eab) 

*/

static
int64_t*
pgr_get_bigIntArr(ArrayType *v, size_t *arrlen, bool allow_empty) {
    clock_t start_t = clock();
    int64_t *c_array = NULL;;

    Oid     element_type = ARR_ELEMTYPE(v);
    int    *dim = ARR_DIMS(v);
    int     ndim = ARR_NDIM(v);
    int     nitems = ArrayGetNItems(ndim, dim);
    Datum  *elements;
    bool   *nulls;
    int16   typlen;
    bool    typbyval;
    char    typalign;

    assert((*arrlen) == 0);


    if (allow_empty && (ndim == 0 || nitems <= 0)) {
        PGR_DBG("ndim %i nitems % i", ndim, nitems);
        return (int64_t*) NULL;
    }
    /* the array is not empty*/

    if (ndim != 1) {
        elog(ERROR, "One dimension expected");
        return (int64_t*)NULL;
    }

    if (nitems <= 0) {
        elog(ERROR, "No elements found");
        return (int64_t*)NULL;
    }

    get_typlenbyvalalign(element_type,
            &typlen, &typbyval, &typalign);

    /* validate input data type */
    switch (element_type) {
        case INT2OID:
        case INT4OID:
        case INT8OID:
            break;
        default:
            elog(ERROR, "Expected array of ANY-INTEGER");
            return (int64_t*)NULL;
            break;
    }

    deconstruct_array(v, element_type, typlen, typbyval,
            typalign, &elements, &nulls,
            &nitems);

    c_array = (int64_t *) palloc(sizeof(int64_t) * (size_t)nitems);
    if (!c_array) {
        elog(ERROR, "Out of memory!");
    }


    int i;
    for (i = 0; i < nitems; i++) {
        if (nulls[i]) {
            pfree(c_array);
            elog(ERROR, "NULL value found in Array!");
        } else {
            switch (element_type) {
                case INT2OID:
                    c_array[i] = (int64_t) DatumGetInt16(elements[i]);
                    break;
                case INT4OID:
                    c_array[i] = (int64_t) DatumGetInt32(elements[i]);
                    break;
                case INT8OID:
                    c_array[i] = DatumGetInt64(elements[i]);
                    break;
            }
        }
    }
    (*arrlen) = (size_t)nitems;

    pfree(elements);
    pfree(nulls);
    PGR_DBG("Array size %ld", (*arrlen));
    time_msg("reading Array", start_t, clock());
    return c_array;
}
/*!
@brief Function returns the values of input array by checking its overflow conditions but array can't empty.

@param input array object to examine.
@param arrlen Number of elements in an array.

@return Return the pointer of an array.

*/

int64_t* pgr_get_bigIntArray(size_t *arrlen, ArrayType *input) {
    return pgr_get_bigIntArr(input, arrlen, false);
}

/*!
@brief Function returns the values of input array by checking its overflow conditions but array can empty.

@param input array object to examine.
@param arrlen Number of elements in an array.

@return Return the pointer of an array.

*/

int64_t* pgr_get_bigIntArray_allowEmpty(size_t *arrlen, ArrayType *input) {
    return pgr_get_bigIntArr(input, arrlen, true);
}
