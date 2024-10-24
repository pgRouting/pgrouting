/*PGR-GNU*****************************************************************
File: get_check_data.hpp

Copyright (c) 2023 pgRouting developers
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

/** @brief  Function will check whether the colNumber represent any specific column or NULL (SPI_ERROR_NOATTRIBUTE).  */
bool column_found(int);

/** @brief Function tells expected type of each column and then check the correspondence type of each column.  */
void fetch_column_info(const TupleDesc&, std::vector<Column_info_t>&);

/** @brief Function return the value of specified column in char type. */
char getChar(const HeapTuple, const TupleDesc&, const Column_info_t&, bool, char);

/** @brief get postgres array into c++ set container */
std::set<int64_t> get_pgset(ArrayType*);

/** @brief get postgres array into c++ vector container */
std::vector<int64_t> get_pgarray(ArrayType*, bool);

/** @brief Enforces the input array to be @b NOT empty */
int64_t* get_array(ArrayType*, size_t*, bool);

/** @brief Function returns the values of specified columns in array. */
int64_t* getBigIntArr(const HeapTuple, const TupleDesc&, const Column_info_t&, size_t*);

/** @brief Function returns the value of specified column in integer type.  */
int64_t getBigInt(const HeapTuple, const TupleDesc&, const Column_info_t&);

/** @brief Function returns the value of specified column in double type.  */
double  getFloat8(const HeapTuple, const TupleDesc&, const Column_info_t&);

/** @brief Function returns the string representation of the value of specified column.  */
char* getText(const HeapTuple, const TupleDesc&, const Column_info_t&);

}  // namespace pgrouting


#endif  // INCLUDE_CPP_COMMON_GET_CHECK_DATA_HPP_
