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

#include "postgres.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#include "utils/array.h"
#include "executor/spi.h"


// #define DEBUG
#include "./debug_macro.h"
#include "./pgr_types.h"
#include "./postgres_connection.h"
#include "./get_check_data.h"
#include "./arrays_input.h"

#if 0
char *
pgr_text2char(text *in)
{
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}
char* pgr_stradd(const char* a, const char* b){
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) ,b);
}
#if 0
// http://www.postgresql.org/docs/9.4/static/spi-spi-finish.html
void
pgr_SPI_finish(void) {
    PGR_DBG("Disconnecting SPI");
    int code = SPI_OK_FINISH;
    code = SPI_finish();
    if (code != SPI_OK_FINISH ) {  // SPI_ERROR_UNCONNECTED
        elog(ERROR,"There was no connection to SPI");
    }			
}

void
pgr_SPI_connect(void) {
    PGR_DBG("Connecting to SPI");
    int SPIcode;
    SPIcode = SPI_connect();
    if (SPIcode  != SPI_OK_CONNECT) {
        elog(ERROR, "Couldn't open a connection to SPI");
    }
}

SPIPlanPtr
pgr_SPI_prepare(char* sql) {
    PGR_DBG("Preparing Plan");
    SPIPlanPtr SPIplan;
    SPIplan = SPI_prepare(sql, 0, NULL);
    if (SPIplan  == NULL) {
        elog(ERROR, "Couldn't create query plan via SPI");
    }
    return SPIplan;
}

Portal
pgr_SPI_cursor_open(SPIPlanPtr SPIplan) {
    PGR_DBG("Opening Portal");
    Portal SPIportal;
    SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true);
    if (SPIportal == NULL) {
        elog(ERROR, "SPI_cursor_open returns NULL");
    }
    return SPIportal;
}
#endif

void
pgr_fetch_column_info(
        int *colNumber,
        int *coltype,
        char *colName) {
    PGR_DBG("Fetching column info");
    (*colNumber) =  SPI_fnumber(SPI_tuptable->tupdesc, colName);
    if ((*colNumber) == SPI_ERROR_NOATTRIBUTE) {
        elog(ERROR, "Column '%s' not Found", colName);
    }
    (*coltype) = SPI_gettypeid(SPI_tuptable->tupdesc, (*colNumber));
    if (SPI_result == SPI_ERROR_NOATTRIBUTE) {
        elog(ERROR, "Type of column '%s' not Found", colName);
    }
}

#endif


int64_t* pgr_get_bigIntArray(size_t *arrlen, ArrayType *input) {
    int         ndims;
    // int *lbs;
    bool       *nulls;
    Oid         i_eltype;
    int16       i_typlen;
    bool        i_typbyval;
    char        i_typalign;
    Datum      *i_data;
    int         i, n;
    int64_t      *data;

    PGR_DBG("Geting integer array");
    /* get input array element type */
    i_eltype = ARR_ELEMTYPE(input);
    get_typlenbyvalalign(i_eltype, &i_typlen, &i_typbyval, &i_typalign);


    /* validate input data type */
    switch(i_eltype){
        case INT2OID:
        case INT4OID:
        case INT8OID:
            break;
        default:
            elog(ERROR, "Expected array of any-integer");
            return (int64_t*) NULL;  
            break;
    }

    /* get various pieces of data from the input array */
    ndims = ARR_NDIM(input);
    n = (*ARR_DIMS(input));
    (*arrlen) = n;
    // lbs = ARR_LBOUND(input);

    if ( (ndims) != 1) {
        elog(ERROR, "One dimenton expected");
    }

    /* get src data */
    deconstruct_array(input, i_eltype, i_typlen, i_typbyval, i_typalign,
            &i_data, &nulls, &n);

    /* construct a C array */
    data = (int64_t *) malloc((*arrlen) * sizeof(int64_t));
    if (!data) {
        elog(ERROR, "Error: Out of memory!");
    }
    PGR_DBG("array size %ld", (*arrlen));

    for (i=0; i<(*arrlen); i++) {
        if (nulls[i]) {
            data[i] = -1;
        }
        else {
            switch(i_eltype){
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
        PGR_DBG("    data[%d]=%li", i, data[i]);
    }

    pfree(nulls);
    pfree(i_data);

    return (int64_t*)data;
}

#if 0
void
pgr_check_char_type(char* colName, int type) {
    if (!(type == BPCHAROID)) {
        elog(ERROR, "Unexpected Column '%s' type. Expected CHAR; type: %d found", colName, type);
    }
}

void
pgr_check_text_type(char* colName, int type) {
    if (!(type == TEXTOID)) {
        elog(ERROR, "Unexpected Column '%s' type. Expected TEXT", colName);
    }
}

void
pgr_check_any_integer_type(char* colName, int type) {
    if (!(type == INT2OID 
                || type == INT4OID
                || type == INT8OID)){
        elog(ERROR, "Unexpected Column '%s' type. Expected ANY-INTEGER", colName);
    }
}

void pgr_check_any_numerical_type(char* colName, int type) {
    if (!(type == INT2OID
                || type == INT4OID
                || type == INT8OID
                || type == FLOAT4OID
                || type == FLOAT8OID)){
        elog(ERROR, "Unexpected Column '%s' type. Expected ANY-NUMERICAL", colName);
    }
}


char
pgr_SPI_getChar(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType, bool strict, char default_value) {
    Datum binval;
    bool isNull;
    char value = default_value;

    binval = SPI_getbinval(*tuple, *tupdesc, colNumber, &isNull);
    if (!(colType == BPCHAROID)) {
        /*
         * http://doxygen.postgresql.org/include_2catalog_2pg__type_8h.html;
         */
        elog(ERROR, "pgr_SPI_getChar: type CHAR expected; Type %d found.", colType);
    }
    if (!isNull) {
        value =  ((char*)binval)[1]; // DatumGetChar(binval);
    } else {
        if (strict) {
            elog(ERROR, "pgr_SPI_getChar: Unexpected Null value");
        }
        value = default_value;
    }
    return value;
}



int64_t
pgr_SPI_getBigInt(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType) {
    Datum binval;
    bool isnull;
    int64_t value = 0;
    binval = SPI_getbinval(*tuple, *tupdesc, colNumber, &isnull);
    if (isnull) elog(ERROR, "Null value found");
    switch (colType) {
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
            elog(ERROR, "Unexpected Column type. Expected ANY-INTEGER");
    }
    return value;
}

float8
pgr_SPI_getFloat8(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType) {
    Datum binval;
    bool isnull;
    float8 value = 0.0;
    binval = SPI_getbinval(*tuple, *tupdesc, colNumber, &isnull);
    if (isnull) elog(ERROR, "Null value found");
    switch (colType) {
        case INT2OID:
            value = (float8) DatumGetInt16(binval);
            break;
        case INT4OID:
            value = (float8) DatumGetInt32(binval);
            break;
        case INT8OID:
            value = (float8) DatumGetInt64(binval);
            break;
        case FLOAT4OID:
            value = (float8) DatumGetFloat4(binval);
            break;
        case FLOAT8OID:
            value = DatumGetFloat8(binval);
            break;
        default:
            elog(ERROR, "BigInt, int, SmallInt, real  expected");
    }
    return value;
}

char*
pgr_SPI_getText(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType) {
    char* value = NULL;
    char* val = NULL;
    val = SPI_getvalue(*tuple, *tupdesc, colNumber);
    value = DatumGetCString(&val);
    pfree(val);
    return value;
}
#endif
