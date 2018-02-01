/*PGR-GNU*****************************************************************
File: get_check_data.h

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

#ifndef INCLUDE_C_COMMON_GET_CHECK_DATA_H_
#define INCLUDE_C_COMMON_GET_CHECK_DATA_H_
#pragma once

#include "c_common/postgres_connection.h"
#include "c_types/column_info_t.h"

/*!
@brief  Function will check whether the colNumber represent any specific column or NULL (SPI_ERROR_NOATTRIBUTE).

@param[in] colNumber Column number (count starts at 1).

@return @b TRUE when colNumber exist. 
        @b FALSE when colNumber was not found.
 
 */
bool column_found(int colNumber);

/*!
@brief Function tells expected type of each column and then check the correspondence type of each column.

@param[in] info[]     contain one or more column information.
@param[in] info_size  number of columns.

@throw ERROR Unknown type of column.
  
@return NULL is always returned.
 
 */
void pgr_fetch_column_info(
        Column_info_t info[],
        int info_size);

/*!
@brief The function check whether column type is ANY-INTEGER or not.
       Where ANY-INTEGER is SQL type:
            SMALLINT, INTEGER, BIGINT

@param[in] info contain column information.

@throw ERROR Unexpected Column type. Expected column type is ANY-INTEGER.

 */
void pgr_check_any_integer_type(Column_info_t info);

/*!
@brief The function check whether column type is ANY-NUMERICAL.
       Where ANY-NUMERICAL is SQL type:
            SMALLINT, INTEGER, BIGINT, REAL, FLOAT

@param[in] info contain column information.

@throw ERROR Unexpected Column type. Expected column type is ANY-NUMERICAL.

 */
void pgr_check_any_numerical_type(Column_info_t info);

/*!
@brief The function check whether column type is CHAR or not.
       Where CHAR is SQL type:
            CHARACTER

@param[in] info contain column information.

@throw ERROR Unexpected Column type. Expected column type is CHAR.

 */
void pgr_check_char_type(Column_info_t info);

/*!
@brief The function check whether column type is TEXT or not.
       Where TEXT is SQL type:
            TEXT 

@param[in] info contain column information.

@throw ERROR Unexpected Column type. Expected column type is TEXT.

 */
void pgr_check_text_type(Column_info_t info);
void pgr_check_boolean_type(Column_info_t info);

/*!
@brief The function check whether column type is ANY-INTEGER-ARRAY or not.
       Where ANY-INTEGER-ARRAY is SQL type:
            SMALLINT[], INTEGER[], BIGINT[]

@param[in] info contain column information.

@throw ERROR Unexpected Column type. Expected column type is ANY-INTEGER-ARRAY.
  
 */

void pgr_check_any_integerarray_type(Column_info_t info);

/*!
@brief Function return the value of specified column in char type.

@param[in]  tuple         input row to be examined.
@param[in]  tupdesc       input row description.
@param[in]  info          contain column information.
@param[in]  strict        boolean value of strict.
@param[in]  default_value returned when column contain NULL value.
  
@throw ERROR Unexpected Column type. Expected column type is CHAR.
@throw ERROR When value of column is NULL.

@return Char type of column value is returned.

 */
char pgr_SPI_getChar(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info,
        bool strict,
        char default_value);

/*!
@brief Function returns the values of specified columns in array.

@param[in]  tuple    input row to be examined.
@param[in]  tupdesc  input row description.
@param[in]  info     contain column information.
@param[out] the_size number of element in array.

@throw ERROR No elements found in ARRAY.
@throw ERROR Unexpected Column type. Expected column type is ANY-INTEGER-ARRAY.
@throw ERROR NULL value found in Array.
  
@return Array of columns value is returned.

 */

int64_t*
pgr_SPI_getBigIntArr(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info,
        uint64_t *the_size);

/*!
@brief Function returns the value of specified column in integer type.

@param[in]  tuple   input row to be examined.
@param[in]  tupdesc input row description.
@param[in]  info    contain column information.

@throw ERROR Unexpected Column type. Expected column type is ANY-INTEGER.
@throw ERROR When value of column is NULL.
  
@return Integer type of column value is returned.

 */

int64_t pgr_SPI_getBigInt(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info);

/*!
@brief Function returns the value of specified column in double type.

@param[in] tuple   input row to be examined.
@param[in] tupdesc input row description.
@param[in] info    contain column information.

@throw ERROR Unexpected Column type. Expected column type is ANY-NUMERICAL.
@throw ERROR When value of column is NULL.

@return Double type of column value is returned.

 */

double  pgr_SPI_getFloat8(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info);
/*!
@brief Function returns the string representation of the value of specified column.
 
@param[in]  tuple   input row to be examined.
@param[in]  tupdesc input row description.
@param[in]  info    contain column information.
  
@return Pointer of string is returned.

 */

char* pgr_SPI_getText(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info);


#endif  // INCLUDE_C_COMMON_GET_CHECK_DATA_H_
