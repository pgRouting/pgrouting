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

#ifndef SRC_COMMON_SRC_GET_CHECK_DATA_H_
#define SRC_COMMON_SRC_GET_CHECK_DATA_H_

#pragma once

bool column_found(int colNumber);

void pgr_fetch_column_info(
        Column_info_t info[],
        int info_size);

void pgr_check_any_integer_type(Column_info_t info);
void pgr_check_any_numerical_type(Column_info_t info);
void pgr_check_text_type(Column_info_t info);
void pgr_check_char_type(Column_info_t info);
void pgr_check_boolean_type(Column_info_t info);


char pgr_SPI_getChar(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info,
        bool strict,
        char default_value);

int64_t pgr_SPI_getBigInt(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info);

double  pgr_SPI_getFloat8(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info);

char* pgr_SPI_getText(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info);

char* pgr_stradd(const char *a, const char *b);

#endif  // SRC_COMMON_SRC_GET_CHECK_DATA_H_
