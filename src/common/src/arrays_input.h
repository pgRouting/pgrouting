/*PGR-GNU*****************************************************************
File: arrays_input.h

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

#ifndef SRC_COMMON_SRC_ARRAYS_INPUT_H_
#define SRC_COMMON_SRC_ARRAYS_INPUT_H_

#include "postgres.h"
#include "executor/spi.h"
#include "utils/array.h"


#include "./pgr_types.h"


int64_t* pgr_get_bigIntArray(size_t *arrlen, ArrayType *input);

#if 0
void pgr_SPI_finish(void);
void pgr_SPI_connect(void);
SPIPlanPtr pgr_SPI_prepare(char* sql);
Portal pgr_SPI_cursor_open(SPIPlanPtr SPIplan);

void 
pgr_fetch_column_info(
        int *colNumber,
        int *coltype,
        char *colName);

void pgr_check_any_integer_type(char* colName, int type);
void pgr_check_any_numerical_type(char* colName, int type);
void pgr_check_text_type(char* colName, int type);
void pgr_check_char_type(char* colName, int type);

char
pgr_SPI_getChar(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType, bool strict, char default_value);
int64_t pgr_SPI_getBigInt(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType);
float8 pgr_SPI_getFloat8(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType);
char* pgr_SPI_getText(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType);

// char* pgr_text2char(text *in);
char* pgr_stradd(const char *a, const char *b);

#endif


#endif  // SRC_COMMON_SRC_ARRAYS_INPUT_H_
