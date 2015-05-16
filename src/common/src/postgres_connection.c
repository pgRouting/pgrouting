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


#include "postgres.h"
#include "catalog/pg_type.h"
#include "executor/spi.h"

// #define DEBUG

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
  }

    

/********************
Functions for pgr_foo with sql:
 id, source, target, cost, reverse_cost(optional) 
************/
static int pgr_fetch_edge_columns(
     int (*edge_columns)[5], 
     int (*edge_types)[5], 
     bool has_rcost) {

  int i, error;
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
  int64_t value;
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
  float8 value;
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

  
  if (!sourceFound) {
      elog(ERROR, "Starting Vertex does not exist in the data");
          return pgr_finish(SPIcode, ret);	  
      return -1;
  }
  if (!targetFound) {
      elog(ERROR, "Ending Vertex does not exist in the data");
          return pgr_finish(SPIcode, ret);	  
      return -1;
  }

  (*totalTuples) = total_tuples;
  return 0;
}

pgr_path_element3_t* pgr_get_memory3(int size, pgr_path_element3_t *path){
	if(path ==0  ){
		path=malloc(size * sizeof(pgr_path_element3_t));
	} else {
		path=realloc(path,size * sizeof(pgr_path_element3_t));
	}
	return path;
}


pgr_path_element3_t * noPathFound3(int64_t start_id) {
        pgr_path_element3_t *no_path;
        no_path = pgr_get_memory3(1, no_path);
        no_path[0].route_id  = 0;
        no_path[0].vertex_id = start_id;
        no_path[0].cost = 0;
        no_path[0].edge_id = -1;
        return no_path;
}


