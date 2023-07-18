/*PGR-GNU*****************************************************************
File: arrays_input.cpp

Copyright (c) 2023 Celia Virginia Vergara Castillo
Copyright (c) 2015 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev

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

extern "C" {
#include <utils/lsyscache.h>
#include <catalog/pg_type.h>
}

#include "cpp_common/pgr_alloc.hpp"

namespace {

/** @brief get the array contents from postgres
 *
 * @details This function generates the array inputs according to their type
 * received through @a ArrayType *v parameter and store them in @a c_array. It
 * can be empty also if received @a allow_empty true. The cases of failure are:-
 * 1. When @a ndim is not equal to one dimension.
 * 2. When no element is found i.e. nitems is zero or negative.
 * 3. If the element type doesn't lie in switch cases, give the error of expected array of any integer type
 * 4. When size of @a c_array is out of range or memory.
 * 5. When null value is found in the array.
 *
 * All these failures are represented as error through @a elog.
 * @param[in] v Pointer to the postgres C array
 * @param[out] arrlen size of the C array
 * @param[in] allow_empty flag to allow empty arrays
 *
 * @pre the array has to be one dimension
 * @pre Must have elements (when allow_empty is false)
 *
 * @returns The resultant array
 */
int64_t*
get_bigIntArr(ArrayType *v, size_t *arrlen, bool allow_empty) {
    int64_t *c_array = nullptr;

    auto    element_type = ARR_ELEMTYPE(v);
    auto    dim = ARR_DIMS(v);
    auto    ndim = ARR_NDIM(v);
    auto    nitems = ArrayGetNItems(ndim, dim);
    Datum  *elements = nullptr;
    bool   *nulls = nullptr;
    int16   typlen;
    bool    typbyval;
    char    typalign;


    if (allow_empty && (ndim == 0 || nitems <= 0)) {
        return nullptr;
    }

    if (ndim != 1) {
        elog(ERROR, "One dimension expected");
        return nullptr;
    }

    if (nitems <= 0) {
        elog(ERROR, "No elements found");
        return nullptr;
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
            return nullptr;
            break;
    }

    deconstruct_array(v, element_type, typlen, typbyval,
            typalign, &elements, &nulls,
            &nitems);

    c_array = pgrouting::pgr_alloc(static_cast<size_t>(nitems), (c_array));
    if (!c_array) {
        elog(ERROR, "Out of memory!");
    }


    for (int i = 0; i < nitems; i++) {
        if (nulls[i]) {
            pfree(c_array);
            elog(ERROR, "NULL value found in Array!");
        } else {
            switch (element_type) {
                case INT2OID:
                    c_array[i] = static_cast<int64_t>(DatumGetInt16(elements[i]));
                    break;
                case INT4OID:
                    c_array[i] = static_cast<int64_t>(DatumGetInt32(elements[i]));
                    break;
                case INT8OID:
                    c_array[i] = DatumGetInt64(elements[i]);
                    break;
            }
        }
    }
    (*arrlen) = static_cast<size_t>(nitems);

    pfree(elements);
    pfree(nulls);
    return c_array;
}

}  // namespace

/**
 * @param[out] arrlen Length of the array
 * @param[in] input the postgres array
 * @param[in] allow_empty when true, empty arrays are accepted.
 * @returns Returns a C array of integers
 */

int64_t* pgr_get_bigIntArray(size_t *arrlen, ArrayType *input, bool allow_empty) {
    return get_bigIntArr(input, arrlen, allow_empty);
}
