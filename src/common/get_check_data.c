/*PGR-GNU*****************************************************************
File: get_check_data.c

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

#include "c_common/get_check_data.h"
#include "c_common/arrays_input.h"

#include "catalog/pg_type.h"

#include "c_common/debug_macro.h"

/*!
@brief  Function will check whether the colNumber represents any specified column or NULL (SPI_ERROR_NOATTRIBUTE).

@param colNumber Column number (count starts at 1).

@return @b true when colNumber exists 
        @b false when colNumber was not found
 
####References####
[SPI_fnumber](https://www.postgresql.org/docs/8.2/static/spi-spi-fnumber.html)
[SPI_ERROR_NOATTRIBUTE](https://doxygen.postgresql.org/spi_8h.html#ac1512d8aaa23c2d57bb0d1eb8f453ee2)

 */

bool
column_found(int colNumber) {
    return !(colNumber == SPI_ERROR_NOATTRIBUTE);
}

/*!
@brief Function will check whether column exists or not.

@param info contain column information.

@todo Fetching the column info.
 
Determine the column number for the specified 
column name and stored in info.

~~~~~~~~~~~~~~~{.c}
info->colNumber =  SPI_fnumber(SPI_tuptable->tupdesc, info->name);
~~~~~~~~~~~~~~~

Returns the OID of the data type of the specified column 
and stored in info.

~~~~~~~~~~~~~~~{.c}
info->type = SPI_gettypeid(SPI_tuptable->tupdesc, (info->colNumber));
~~~~~~~~~~~~~~~

@todo Found the particular column.

@return @b TRUE when column exist 
        @b FALSE when column was not found

####References####
[SPI_fnumber](https://www.postgresql.org/docs/8.2/static/spi-spi-fnumber.html)
[OID](https://www.postgresql.org/docs/9.4/static/datatype-oid.html)
[SPI_gettypeid](https://www.postgresql.org/docs/9.1/static/spi-spi-gettypeid.html)

 */

static
bool
fetch_column_info(
        Column_info_t *info) {
#if 0
    //todo Fetching the column info.  
#endif
    info->colNumber =  SPI_fnumber(SPI_tuptable->tupdesc, info->name);
    if (info->strict && !column_found(info->colNumber)) {
        elog(ERROR, "Column '%s' not Found", info->name);
    }

    if (column_found(info->colNumber)) {
        (info->type) = SPI_gettypeid(SPI_tuptable->tupdesc, (info->colNumber));
        if (SPI_result == SPI_ERROR_NOATTRIBUTE) {
            elog(ERROR, "Type of column '%s' not Found", info->name);
        }
#if 0
     //todo Column was found.    
#endif
        return true;
    }
    PGR_DBG("Column %s not found", info->name);
    return false;
}

/*!
@brief Function tells expected type of each column and then check the correspondence type of column.

@param info[] contain one or more column information.
@param info_size number of columns.

Column expected type should be:
 - ANY_INTEGER
 - ANY_NUMERICAL
 - TEXT
 - CHAR1
 - ANY_INTEGER_ARRAY 

  
@return NULL is always returned.
 
 */

void pgr_fetch_column_info(
        Column_info_t info[],
        int info_size) {
    int i;
    for (i = 0; i < info_size; ++i) {
        if (fetch_column_info(&info[i])) {
            switch (info[i].eType) {
                case ANY_INTEGER:
                    pgr_check_any_integer_type(info[i]);
                    break;
                case ANY_NUMERICAL:
                    pgr_check_any_numerical_type(info[i]);
                    break;
                case TEXT:
                    pgr_check_text_type(info[i]);
                    break;
                case CHAR1:
                    pgr_check_char_type(info[i]);
                    break;
                case ANY_INTEGER_ARRAY:
                    pgr_check_any_integerarray_type(info[i]);
                    break;
                default:
                    elog(ERROR, "Unknown type of column %s", info[i].name);
            }
        }
    }
}

/*!
@brief The function check whether column type is CHAR or not.

For CHAR the value of column type must be 1042(BPCHAROID).

@return NULL is always returned.
 
####References####
[BPCHAROID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#afa7749dbe36d31874205189d9d6b21d7)

 */

void
pgr_check_char_type(Column_info_t info) {
    if (!(info.type == BPCHAROID)) {
        elog(ERROR, "Unexpected Column '%s' type. Expected CHAR", info.name);
    }
}

/*!
@brief The function check whether column type is TEXT or not.

For TEXT column the value of column type must be 25(TEXTOID).

@return NULL is always returned.

####References####
[TEXTOID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a3c945702a9e2e4694e5673db84c2a77d)
 
 */
void
pgr_check_text_type(Column_info_t info) {
    if (!(info.type == TEXTOID)) {
        elog(ERROR, "Unexpected Column '%s' type. Expected TEXT", info.name);
    }
}

/*!
@brief The function check whether column type is ANY-INTEGER or not.

For ANY-INTEGER the value of column type must be 21(INT2OID) or 23(INT4OID) or 20(INT8OID).

@return NULL is always returned.
 
####References####
[INT2OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a31e54a35a82c0ec11b9952a46f4d7af3)
[INT4OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#af473c8bca64740230ff280c0de2c5721)
[INT8OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a59b14b0f52e041ced4e021121ed0b31b)

 */

void
pgr_check_any_integer_type(Column_info_t info) {
    if (!(info.type == INT2OID
                || info.type == INT4OID
                || info.type == INT8OID)) {
        elog(ERROR,
                "Unexpected Column '%s' type. Expected ANY-INTEGER",
                info.name);
    }
}
/*!
@brief The function check whether column type is ANY-INTEGER-ARRAY or not.

For ANY-INTEGER-ARRAY the value of column type must be 1005(INT2ARRAYOID) or 1007(INT4ARRAYOID) or 1016.

@return NULL is always returned.
 
####References####
[INT2ARRAYOID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#ac265fe7b0bb75fead13b16bf072722e9)
[INT4ARRAYOID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a4bb08893df232440487adc7926bebfca)
  
 */

void
pgr_check_any_integerarray_type(Column_info_t info) {
    if (!(info.type == INT2ARRAYOID
                || info.type == INT4ARRAYOID
                || info.type == 1016)) {
        elog(ERROR,
                "Unexpected Column '%s' type. Expected ANY-INTEGER-ARRAY",
                info.name);
    }
}

/*!
@brief The function check whether column type is ANY-NUMERICAL.

For ANY-NUMERICAL the value of column type must be 21(INT2OID) or 23(INT4OID) or 20(INT8OID) or 700(FLOAT4OID) or 701(FLOAT8OID).

@return NULL is always returned.
 
####References####
[INT2OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a31e54a35a82c0ec11b9952a46f4d7af3)
[INT4OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#af473c8bca64740230ff280c0de2c5721)
[INT8OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a59b14b0f52e041ced4e021121ed0b31b)
[FLOAT4OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a355afc3e7ec3b4bb0860f61371d942b6)
[FLOAT8OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a8fa1c5f811247e1b88bc65475218d41c)

 */

void pgr_check_any_numerical_type(Column_info_t info) {
    if (!(info.type == INT2OID
                || info.type == INT4OID
                || info.type == INT8OID
                || info.type == FLOAT4OID
                || info.type == FLOAT8OID)) {
        elog(ERROR,
                "Unexpected Column '%s' type. Expected ANY-NUMERICAL",
                info.name);
    }
}


/*
 * http://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html;
 */
/*!
@brief Function returns the value of the specified column in char type.

@param tuple input row to be examined.
@param tupdesc input row description.
@param info contain column information.
@param strict boolean value of strict.
@param default_value returned when column contain NULL value.

Return the binary value of the specified column
and stored in binval.

~~~~~~~~~~~~~~~{.c}
binval = SPI_getbinval(*tuple, *tupdesc, info.colNumber, &isnull);
~~~~~~~~~~~~~~~
  
@return Char type of column value is returned.

####References####
[SPI_getbinval](https://www.postgresql.org/docs/8.1/static/spi-spi-getbinval.html)
[BPCHAROID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#afa7749dbe36d31874205189d9d6b21d7)
[Datum](https://doxygen.postgresql.org/datum_8h.html)

 */
char
pgr_SPI_getChar(
        HeapTuple *tuple, TupleDesc *tupdesc, Column_info_t info,
        bool strict, char default_value) {
    Datum binval;
    bool isNull;
    char value = default_value;

    binval = SPI_getbinval(*tuple, *tupdesc, info.colNumber, &isNull);
    if (!(info.type == BPCHAROID)) {
        elog(ERROR, "Unexpected Column type of %s. Expected CHAR", info.name);
    }
    if (!isNull) {
        value =  ((char*)binval)[1];
    } else {
        if (strict) {
            elog(ERROR, "Unexpected Null value in column %s", info.name);
        }
        value = default_value;
    }
    return value;
}

/*!
@brief Function returns the values of the specified columns in array.

@param tuple input row to be examined.
@param tupdesc input row description.
@param info contain column information.
@param the_size number of element in array.

Return the binary value of the specified column
and stored in binval.

~~~~~~~~~~~~~~~{.c}
binval = SPI_getbinval(*tuple, *tupdesc, info.colNumber, &isnull);
~~~~~~~~~~~~~~~
  
@return Pointer of columns value is returned.

####References####
[SPI_getbinval](https://www.postgresql.org/docs/8.1/static/spi-spi-getbinval.html)
[DatumGetArrayTypeP](https://doxygen.postgresql.org/array_8h.html#aa1b8e77c103863862e06a7b7c07ec532)
[pgr_get_bigIntArray](http://docs.pgrouting.org/doxy/2.2/arrays__input_8c_source.html)

 */


int64_t*
pgr_SPI_getBigIntArr(
        HeapTuple *tuple,
        TupleDesc *tupdesc,
        Column_info_t info,
        uint64_t *the_size) {
    bool is_null = false;

    Datum raw_array = SPI_getbinval(*tuple, *tupdesc, info.colNumber, &is_null);
    ArrayType *pg_array = DatumGetArrayTypeP(raw_array);

    return (int64_t*) pgr_get_bigIntArray(the_size, pg_array);
}

/*!
@brief Function returns the value of the specified column in integer type.

@param tuple input row to be examined.
@param tupdesc input row description.
@param info contain column information.

Return the binary value of the specified column
and stored in binval.

~~~~~~~~~~~~~~~{.c}
binval = SPI_getbinval(*tuple, *tupdesc, info.colNumber, &isnull);
~~~~~~~~~~~~~~~

@todo Found the specified value of column.
  
@return Integer type in 64 bits of column value is returned.

####References####
[SPI_getbinval](https://www.postgresql.org/docs/8.1/static/spi-spi-getbinval.html)
[INT2OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a31e54a35a82c0ec11b9952a46f4d7af3)
[INT4OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#af473c8bca64740230ff280c0de2c5721)
[INT8OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a59b14b0f52e041ced4e021121ed0b31b)
[DatumGetInt16](https://doxygen.postgresql.org/postgres_8h.html#aec991e04209850f29a8a63df0c78ba2d)
[DatumGetInt32](https://doxygen.postgresql.org/postgres_8h.html#aacbc8a3ac6d52d85feaf0b7ac1b1160c)
[DatumGetInt62](https://doxygen.postgresql.org/postgres_8h.html#aedf6286d5147eaf3c6f7e998f2662eab) 

 */

int64_t
pgr_SPI_getBigInt(HeapTuple *tuple, TupleDesc *tupdesc, Column_info_t info) {
    Datum binval;
    bool isnull;
    int64_t value = 0;
    binval = SPI_getbinval(*tuple, *tupdesc, info.colNumber, &isnull);
    if (isnull)
        elog(ERROR, "Unexpected Null value in column %s", info.name);
    switch (info.type) {
        case INT2OID:
            value = (int64_t) DatumGetInt16(binval);
            break;
        case INT4OID:
            value = (int64_t) DatumGetInt32(binval);
            break;
        case INT8OID:
            value = DatumGetInt64(binval);
            break;
        default:
            elog(ERROR,
                    "Unexpected Column type of %s. Expected ANY-INTEGER",
                    info.name);
    }
#if 0
    // todo Value of specified column was found. 
#endif
    return value;
}

/*!
@brief Function returns the value of the specified column in double type.

@param tuple input row to be examined.
@param tupdesc input row description.
@param info contain column information.

Return the binary value of the specified column
and stored in binval.

~~~~~~~~~~~~~~~{.c}
binval = SPI_getbinval(*tuple, *tupdesc, info.colNumber, &isnull);
~~~~~~~~~~~~~~~
  
@todo Found the specified value of column.

@return Double type of column value is returned.

####References####
[SPI_getbinval](https://www.postgresql.org/docs/8.1/static/spi-spi-getbinval.html)
[INT2OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a31e54a35a82c0ec11b9952a46f4d7af3)
[INT4OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#af473c8bca64740230ff280c0de2c5721)
[INT8OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a59b14b0f52e041ced4e021121ed0b31b)
[FLOAT4OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a355afc3e7ec3b4bb0860f61371d942b6)
[FLOAT8OID](https://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html#a8fa1c5f811247e1b88bc65475218d41c)
[DatumGetInt16](https://doxygen.postgresql.org/postgres_8h.html#aec991e04209850f29a8a63df0c78ba2d)
[DatumGetInt32](https://doxygen.postgresql.org/postgres_8h.html#aacbc8a3ac6d52d85feaf0b7ac1b1160c)
[DatumGetInt62](https://doxygen.postgresql.org/postgres_8h.html#aedf6286d5147eaf3c6f7e998f2662eab) 
[DatumGetFloat4](https://doxygen.postgresql.org/postgres_8h.html#af1b5fdd0a7d9dfe714f728f9bf6dbf4b)
[DatumGetFloat8](https://doxygen.postgresql.org/postgres_8h.html#aa8579111ead7079875ae647287cc00c1)  

 */

double
pgr_SPI_getFloat8(HeapTuple *tuple, TupleDesc *tupdesc, Column_info_t info) {
    Datum binval;
    bool isnull = false;
    double value = 0.0;
    binval = SPI_getbinval(*tuple, *tupdesc, info.colNumber, &isnull);
    if (isnull)
        elog(ERROR, "Unexpected Null value in column %s", info.name);

    switch (info.type) {
        case INT2OID:
            value = (double) DatumGetInt16(binval);
            break;
        case INT4OID:
            value = (double) DatumGetInt32(binval);
            break;
        case INT8OID:
            value = (double) DatumGetInt64(binval);
            break;
        case FLOAT4OID:
            value = (double) DatumGetFloat4(binval);
            break;
        case FLOAT8OID:
            value = DatumGetFloat8(binval);
            break;
        default:
            elog(ERROR,
                    "Unexpected Column type of %s. Expected ANY-NUMERICAL",
                    info.name);
    }
#if 0
    // todo Value of specified column was found.  
#endif
    return value;
}

/*!
###under development###
@brief Function returns the string representation of the value of the specified column.
 
@param tuple input row to be examined.
@param tupdesc input row description.
@param info contain column information.

Return the string value of the specified column.

~~~~~~~~~~~~~~~{.c}
SPI_getvalue(*tuple, *tupdesc, info.colNumber);
~~~~~~~~~~~~~~~
  
@return Pointer of string is returned.

####References####
[SPI_getvalue](https://www.postgresql.org/docs/8.2/static/spi-spi-getvalue.html)
[DatumGetCString](https://doxygen.postgresql.org/postgres_8h.html#ae401c8476d1a12b420e3061823a206a7)

 */
char*
pgr_SPI_getText(HeapTuple *tuple, TupleDesc *tupdesc,  Column_info_t info) {
    return DatumGetCString(SPI_getvalue(*tuple, *tupdesc, info.colNumber));
}
