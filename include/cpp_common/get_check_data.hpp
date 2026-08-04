/*PGR-GNU*****************************************************************
File: get_check_data.hpp

Copyright (c) 2023-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Celia Virginia Vergara Castillo
vicky at erosion.dev

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

#ifndef INCLUDE_CPP_COMMON_GET_CHECK_DATA_HPP_
#define INCLUDE_CPP_COMMON_GET_CHECK_DATA_HPP_
#pragma once


extern "C" {
#include <postgres.h>
#include <utils/array.h>
#include <access/htup_details.h>
#include <catalog/pg_type.h>
}

#include <cstdint>
#include <set>
#include <string>
#include <vector>

#include "cpp_common/undefPostgresDefine.hpp"


namespace pgrouting {
using Column_info_t = struct Column_info_t;

/**
 * @brief  Function will check whether the colNumber represent any specific column or NULL (SPI_ERROR_NOATTRIBUTE).
 * @param[in] colNumber Column number (count starts at 1).
 * @return @b TRUE when colNumber exist.
 *         @b FALSE when colNumber was not found.
 */
bool column_found(int);

/**
 * @brief Function tells expected type of each column and then check the correspondence type of each column.
 * @param[in] tupdesc  tuple descriptor
 * @param[in] info     contain one or more column information.
 * @throw ERROR Unknown type of column.
 */
void fetch_column_info(const TupleDesc&, std::vector<Column_info_t>&);

/**
 * @brief Function return the value of specified column in char type.
 * @param[in] tuple         input row to be examined.
 * @param[in] tupdesc       tuple descriptor
 * @param[in] info          contain column information.
 * @param[in] strict        boolean value of strict.
 * @param[in] default_value returned when column contain NULL value.
 * @throw ERROR Unexpected Column type. Expected column type is CHAR.
 * @throw ERROR When value of column is NULL.
 * @return Char type of column value is returned.
 */
char getChar(const HeapTuple, const TupleDesc&, const Column_info_t&, bool, char);

/**
 * @brief get postgres array into c++ set container
 * @param[in] v Pointer to the postgres C array
 * @pre the array has to be one dimension
 * @pre Must have elements
 * @throw ERROR One dimension expected
 * @throw ERROR Expected array of ANY-INTEGER
 * @throw ERROR NULL value found in Array
 * @return set of elements on the PostgreSQL array
 */
std::set<int64_t> get_pgset(ArrayType*);

/**
 * @brief get postgres array into c++ vector container
 * @param[in] v Pointer to the postgres C array
 * @param[in] allow_empty flag to allow empty arrays
 * @pre the array has to be one dimension
 * @pre Must have elements (when allow_empty is false)
 * @throw ERROR One dimension expected
 * @throw ERROR No elements found
 * @throw ERROR Expected array of ANY-INTEGER
 * @return Vector of elements of the PostgreSQL array
 */
std::vector<int64_t> get_pgarray(ArrayType*, bool);

/**
 * @brief Enforces the input array to be @b NOT empty
 * @param[in] v Pointer to the postgres C array
 * @param[out] arrlen size of the C array
 * @param[in] allow_empty flag to allow empty arrays
 * @pre the array has to be one dimension
 * @pre Must have elements (when allow_empty is false)
 * @throw ERROR One dimension expected
 * @throw ERROR No elements found
 * @throw ERROR Out of memory
 * @return The resultant array
 */
int64_t* get_array(ArrayType*, size_t*, bool);

/**
 * @brief Function returns the values of specified columns in array.
 * @param[in]  tuple    input row to be examined.
 * @param[in]  tupdesc  input row description.
 * @param[in]  info     contain column information.
 * @param[out] the_size number of element in array.
 * @throw ERROR No elements found in ARRAY.
 * @throw ERROR Unexpected Column type. Expected column type is ANY-INTEGER-ARRAY.
 * @throw ERROR NULL value found in Array.
 * @return Array of columns value is returned.
 */
int64_t* getBigIntArr(const HeapTuple, const TupleDesc&, const Column_info_t&, size_t*);

/**
 * @brief Function returns the value of specified column in integer type.
 * @param[in] tuple   input row to be examined.
 * @param[in] tupdesc  tuple descriptor
 * @param[in] info    contain column information.
 * @throw ERROR Unexpected Column type. Expected column type is ANY-INTEGER.
 * @throw ERROR When value of column is NULL.
 * @return Integer type of column value is returned.
 */
int64_t getBigInt(const HeapTuple, const TupleDesc&, const Column_info_t&);

/**
 * @brief Function returns the value of specified column in double type.
 * @param[in] tuple   input row to be examined.
 * @param[in] tupdesc  tuple descriptor
 * @param[in] info    contain column information.
 * @throw ERROR Unexpected Column type. Expected column type is ANY-NUMERICAL.
 * @throw ERROR When value of column is NULL.
 * @return Double type of column value is returned.
 */
double  getFloat8(const HeapTuple, const TupleDesc&, const Column_info_t&);

/**
 * @brief Function returns the string representation of the value of specified column.
 * @note under development - not used, not tested
 * @param[in] tuple   input row to be examined.
 * @param[in] tupdesc tuple descriptor
 * @param[in] info    contain column information.
 * @return Pointer of string is returned.
 */
char* getText(const HeapTuple, const TupleDesc&, const Column_info_t&);

}  // namespace pgrouting


#endif  // INCLUDE_CPP_COMMON_GET_CHECK_DATA_HPP_
