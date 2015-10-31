/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

// #define DEBUG
#include "postgres.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#include "utils/array.h"
#include "executor/spi.h"


#include "pgr_types.h"
#include "postgres_connection.h"

#define TUPLIMIT 1000

char *
pgr_text2char(text *in)
{
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

int pgr_finish(int code, int ret)
{
  code = SPI_finish();
  if (code  != SPI_OK_FINISH )
  {
    elog(ERROR,"couldn't disconnect from SPI");
    return -1 ;
  }			
  return ret;
}

static int pgr_fetch_column_info(
  int *colNumber,
  int *coltype,
  char *colName) {
    (*colNumber) =  SPI_fnumber(SPI_tuptable->tupdesc, colName);
    if ((*colNumber) == SPI_ERROR_NOATTRIBUTE) {
      elog(ERROR, "Fetching column number");
      return -1;
    }
    (*coltype) = SPI_gettypeid(SPI_tuptable->tupdesc, (*colNumber));
    if (SPI_result == SPI_ERROR_NOATTRIBUTE) {
      elog(ERROR, "Fetching column type");
      return -1;
    }
    return 0;
  }

    
int64_t* pgr_get_bigIntArray(int *arrlen, ArrayType *input) {
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
    PGR_DBG("array size %d", (*arrlen));

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


/********************
Functions for pgr_foo with sql:
 id, source, target, cost, reverse_cost(optional) 
************/
static int pgr_fetch_edge_columns(
     int (*edge_columns)[5], 
     int (*edge_types)[5], 
     bool has_rcost) {

  int error;
  error = pgr_fetch_column_info(&(*edge_columns)[0], &(*edge_types)[0], "id");
  if (error == -1) return error;
  error = pgr_fetch_column_info(&(*edge_columns)[1], &(*edge_types)[1], "source");
  if (error == -1) return error;
  error = pgr_fetch_column_info(&(*edge_columns)[2], &(*edge_types)[2], "target");
  if (error == -1) return error;
  error = pgr_fetch_column_info(&(*edge_columns)[3], &(*edge_types)[3], "cost");
  if (error == -1) return error;
  if (has_rcost) {
    error = pgr_fetch_column_info(&(*edge_columns)[4], &(*edge_types)[4], "reverse_cost");
    if (error == -1) return error;
  }

 return 0;

}

static int64_t pgr_SPI_getBigInt(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType) {
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
      elog(ERROR, "BigInt, int or SmallInt expected");
  }
  return value;
}

static float8 pgr_SPI_getFloat8(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType) {
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

void pgr_fetch_edge(
   HeapTuple *tuple,
   TupleDesc *tupdesc, 
   int (*edge_columns)[5],
   int (*edge_types)[5],
   pgr_edge_t *target_edge,
   bool has_rcost) {

  target_edge->id = pgr_SPI_getBigInt(tuple, tupdesc, (*edge_columns)[0], (*edge_types)[0]);
  target_edge->source = pgr_SPI_getBigInt(tuple, tupdesc, (*edge_columns)[1], (*edge_types)[1]);
  target_edge->target = pgr_SPI_getBigInt(tuple, tupdesc, (*edge_columns)[2], (*edge_types)[2]);
  target_edge->cost = pgr_SPI_getFloat8(tuple, tupdesc, (*edge_columns)[3], (*edge_types)[3]);

  if (has_rcost) {
    target_edge->reverse_cost = pgr_SPI_getFloat8(tuple, tupdesc, (*edge_columns)[4], (*edge_types)[4]);
  } else {
    target_edge->reverse_cost = -1.0;
  }
  PGR_DBG("id: %li\t source: %li\ttarget: %li\tcost: %f\t,reverse: %f\n",
          target_edge->id,  target_edge->source,  target_edge->target,  target_edge->cost,  target_edge->reverse_cost);
}



int pgr_get_data(
    char *sql,
    pgr_edge_t **edges,
    int64_t *totalTuples,
    bool has_rcost,
    int64_t start_vertex,
    int64_t end_vertex) {
  PGR_DBG("Entering pgr_get_data");

  bool sourceFound = false;
  bool targetFound = false;
  if (start_vertex == -1 && end_vertex == -1) {
    sourceFound = targetFound = true;
  }
  int ntuples;
  int64_t total_tuples;

  int edge_columns[5];
  int edge_types[5];
  int i;
  for (i = 0; i < 5; ++i) edge_columns[i] = -1;
  for (i = 0; i < 5; ++i) edge_types[i] = -1;
  int ret = -1;

        
  PGR_DBG("Connecting to SPI");
  int SPIcode;
  SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT) {
      elog(ERROR, "Couldn't open a connection to SPI");
      return -1;
  }

  PGR_DBG("Preparing Plan");
  void *SPIplan;
  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL) {
      elog(ERROR, "Couldn't create query plan via SPI");
      return -1;
  }

  PGR_DBG("Opening Portal");
  Portal SPIportal;
  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
      elog(ERROR, "SPI_cursor_open('%s') returns NULL", sql);
      return -1;
  }


  PGR_DBG("Starting Cycle");
  bool moredata = TRUE;
  (*totalTuples) = total_tuples = 0;
  while (moredata == TRUE) {
      SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

      /*  on the first tuple get the column numbers */
      if (edge_columns[0] == -1) {
        PGR_DBG("Fetching column numbers");
        if (pgr_fetch_edge_columns(&edge_columns, &edge_types,
                                 has_rcost) == -1)
           return pgr_finish(SPIcode, ret);
        PGR_DBG("Finished fetching column numbers");
      }

      ntuples = SPI_processed;
      total_tuples += ntuples;

      PGR_DBG("Getting Memory");
      if ((*edges) == NULL)
        (*edges) = (pgr_edge_t *)palloc(total_tuples * sizeof(pgr_edge_t));
      else
        (*edges) = (pgr_edge_t *)repalloc((*edges), total_tuples * sizeof(pgr_edge_t));
      PGR_DBG("Got Memory");

      if ((*edges) == NULL) {
          elog(ERROR, "Out of memory");
          return pgr_finish(SPIcode, ret);	  
      }

      if (ntuples > 0) {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;
          PGR_DBG("processing %d", ntuples);
          for (t = 0; t < ntuples; t++) {
              HeapTuple tuple = tuptable->vals[t];
              pgr_fetch_edge(&tuple, &tupdesc, &edge_columns, &edge_types,
                         &(*edges)[total_tuples - ntuples + t], has_rcost);

              if (!sourceFound
                 && (((*edges)[total_tuples - ntuples + t].source == start_vertex)
                 || ((*edges)[total_tuples - ntuples + t].target == start_vertex))) {
                    sourceFound = true;
              }
              if (!targetFound
                 && (((*edges)[total_tuples - ntuples + t].source == end_vertex)
                 || ((*edges)[total_tuples - ntuples + t].target == end_vertex))) {
                    targetFound = true;
             }
           }
           SPI_freetuptable(tuptable);
      } else {
          moredata = FALSE;
      }
  }

  
#if 0
  if (!sourceFound) {
      // elog(NOTICE, "Starting Vertex does not exist in the data");
      return 0;
  }
  if (!targetFound) {
      // elog(NOTICE, "Ending Vertex does not exist in the data");
      return 0;
  }
#endif
  if (total_tuples == 1) {
    // for some reason it needs at least a second edge for boost.graph to work
    // makeing a simple test and asking boost people
    ++total_tuples;
    (*edges) = (pgr_edge_t *)repalloc((*edges), total_tuples * sizeof(pgr_edge_t));
    (*edges)[1].source = -1;
    (*edges)[1].target = -1;
    (*edges)[1].cost = 10000;
    (*edges)[1].id = (*edges)[0].id + 1;
    (*edges)[1].reverse_cost = -1;
  }

  (*totalTuples) = total_tuples;
  return 0;
}


General_path_element_t* get_memory(int size, General_path_element_t *path){
	if(path ==0  ){
		path=malloc(size * sizeof(General_path_element_t));
	} else {
		path=realloc(path,size * sizeof(General_path_element_t));
	}
	return path;
}


General_path_element_t* noPathFound(size_t *count, General_path_element_t *no_path) {
   count = 0;
   return NULL;
}




