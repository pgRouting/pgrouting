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

#include "./pgr_types.h"
#include "postgres.h"

#include "catalog/pg_type.h"
#include "executor/spi.h"


// #define DEBUG
#include "./debug_macro.h"
#include "./get_check_data.h"

char*
pgr_stradd(const char* a, const char* b) {
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a), b);
}

bool
column_found(int colNumber) {
    return !(colNumber == SPI_ERROR_NOATTRIBUTE);
}

static
bool
fetch_column_info(
        Column_info_t *info) {
    PGR_DBG("Fetching column info of %s", info->name);
    info->colNumber =  SPI_fnumber(SPI_tuptable->tupdesc, info->name);
    if (info->strict && !column_found(info->colNumber)) {
        elog(ERROR, "Column '%s' not Found", info->name);
    }
    if (column_found(info->colNumber)) {
        (info->type) = SPI_gettypeid(SPI_tuptable->tupdesc, (info->colNumber));
        if (SPI_result == SPI_ERROR_NOATTRIBUTE) {
            elog(ERROR, "Type of column '%s' not Found", info->name);
        }
        PGR_DBG("Column %s found: %llu", info->name, info->type);
        return true;
    }
    PGR_DBG("Column %s not found", info->name);
    return false;
}


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
                default:
                    elog(ERROR, "Unknown type of column %s", info[i].name);
            }
        }
    }
}


void
pgr_check_char_type(Column_info_t info) {
    if (!(info.type == BPCHAROID)) {
        elog(ERROR, "Unexpected Column '%s' type. Expected CHAR", info.name);
    }
}

void
pgr_check_text_type(Column_info_t info) {
    if (!(info.type == TEXTOID)) {
        elog(ERROR, "Unexpected Column '%s' type. Expected TEXT", info.name);
    }
}

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
    PGR_DBG("Variable: %s Value: %lld", info.name, value);
    return value;
}

double
pgr_SPI_getFloat8(HeapTuple *tuple, TupleDesc *tupdesc, Column_info_t info) {
    Datum binval;
    bool isnull;
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
    PGR_DBG("Variable: %s Value: %lf", info.name, value);
    return value;
}

char*
pgr_SPI_getText(HeapTuple *tuple, TupleDesc *tupdesc,  Column_info_t info) {
    char* value = NULL;
    char* val = NULL;
    val = SPI_getvalue(*tuple, *tupdesc, info.colNumber);
    value = DatumGetCString(&val);
    pfree(val);
    return value;
}
