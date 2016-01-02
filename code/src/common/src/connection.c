
#include "postgres.h"
#include "executor/spi.h"
#include "catalog/pg_type.h"
#include "utils/builtins.h"
#include "structs.h"
#include "connection.h"
#define TUPLIMIT 1000


int finish(int code, int ret)
{
  code = SPI_finish();
  if (code  != SPI_OK_FINISH )
  {
    elog(ERROR,"couldn't disconnect from SPI");
    return -1 ;
  }         
  return ret;
}

char *
text2char(text *in)
{
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

static int64_t SPI_getBigInt(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType) {
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

static float8 SPI_getFloat8(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType) {
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
static int fetch_column_info(
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

/********************
Functions for pgr_foo with sql:
 id, source, target, cost, revcost(optional) 
************/
 int fetch_edge_columns(int (*edge_columns)[5],int (*edge_types)[5], bool has_rcost) 
 {

  int error;
  error = fetch_column_info(&(*edge_columns)[0], &(*edge_types)[0], "id");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[1], &(*edge_types)[1], "source");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[2], &(*edge_types)[2], "target");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[3], &(*edge_types)[3], "cost");
  if (error == -1) return error;
  if (has_rcost) {
    error = fetch_column_info(&(*edge_columns)[4], &(*edge_types)[4], "revcost");
    if (error == -1) return error;
  }

  return 0;

}
int fetch_astar_edge_columns(int (*edge_columns)[9],int (*edge_types)[9], bool has_rcost) 
 {

  int error;
  error = fetch_column_info(&(*edge_columns)[0], &(*edge_types)[0], "id");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[1], &(*edge_types)[1], "source");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[2], &(*edge_types)[2], "target");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[3], &(*edge_types)[3], "x1");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[4], &(*edge_types)[4], "y1");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[5], &(*edge_types)[5], "x2");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[6], &(*edge_types)[6], "y2");
  if (error == -1) return error;
  error = fetch_column_info(&(*edge_columns)[7], &(*edge_types)[7], "cost");
  if (error == -1) return error;
  if (has_rcost) {
    error = fetch_column_info(&(*edge_columns)[8], &(*edge_types)[8], "revcost");
    if (error == -1) return error;
  }

  return 0;

}
void fetch_edge(
 HeapTuple *tuple,
 TupleDesc *tupdesc, 
 int (*edge_columns)[5],
 int (*edge_types)[5],
 Edge *target_edge,
 bool has_rcost) {

  target_edge->id = SPI_getBigInt(tuple, tupdesc, (*edge_columns)[0], (*edge_types)[0]);
  target_edge->source = SPI_getBigInt(tuple, tupdesc, (*edge_columns)[1], (*edge_types)[1]);
  target_edge->target = SPI_getBigInt(tuple, tupdesc, (*edge_columns)[2], (*edge_types)[2]);
  target_edge->cost = SPI_getFloat8(tuple, tupdesc, (*edge_columns)[3], (*edge_types)[3]);

  if (has_rcost) {
    target_edge->revcost = SPI_getFloat8(tuple, tupdesc, (*edge_columns)[4], (*edge_types)[4]);
  } else {
    target_edge->revcost = -1.0;
  }
  //PGR_DBG("id: %li\t source: %li\ttarget: %li\tcost: %f\t,reverse: %f\n",
    //      target_edge->id,  target_edge->source,  target_edge->target,  target_edge->cost,  target_edge->revcost);
}
void fetch_astar_edge(
 HeapTuple *tuple,
 TupleDesc *tupdesc, 
 int (*edge_columns)[9],
 int (*edge_types)[9],
 Edge *target_edge,
 bool has_rcost) {

  target_edge->id = SPI_getBigInt(tuple, tupdesc, (*edge_columns)[0], (*edge_types)[0]);
  target_edge->source = SPI_getBigInt(tuple, tupdesc, (*edge_columns)[1], (*edge_types)[1]);
  target_edge->target = SPI_getBigInt(tuple, tupdesc, (*edge_columns)[2], (*edge_types)[2]);
  target_edge->s_x = SPI_getFloat8(tuple, tupdesc, (*edge_columns)[3], (*edge_types)[3]);
  target_edge->s_y = SPI_getFloat8(tuple, tupdesc, (*edge_columns)[4], (*edge_types)[4]);
  target_edge->t_x = SPI_getFloat8(tuple, tupdesc, (*edge_columns)[5], (*edge_types)[5]);
  target_edge->t_y = SPI_getFloat8(tuple, tupdesc, (*edge_columns)[6], (*edge_types)[6]);
  target_edge->cost = SPI_getFloat8(tuple, tupdesc, (*edge_columns)[7], (*edge_types)[7]);

  if (has_rcost) {
    target_edge->revcost = SPI_getFloat8(tuple, tupdesc, (*edge_columns)[8], (*edge_types)[8]);
  } else {
    target_edge->revcost = -1.0;
  }
  //PGR_DBG("id: %li\t source: %li\ttarget: %li\tcost: %f\t,reverse: %f\n",
    //      target_edge->id,  target_edge->source,  target_edge->target,  target_edge->cost,  target_edge->revcost);
}
/*int fetchVertexCount(char *sql,int *count)
{
  int edge_columns[1];
        int edge_types[1];
        int i,t;
        for (i = 0; i < 1; ++i) edge_columns[i] = -1;
          for (i = 0; i < 1; ++i) edge_types[i] = -1;
// Connecting to SPI;
  int SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT) 
  {
    elog(ERROR, "Couldn't open a connection to SPI");
    return -1;
  }

      // Preparing Plan
  void *SPIplan;
  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL) {
    elog(ERROR, "Couldn't create query plan via SPI");
    return -1;
  }

      // Opening Portal
  Portal SPIportal;
  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
    elog(ERROR, "SPI_cursor_open('%s') returns NULL", sql);
    return -1;
  }

      // Starting Cycle;
  bool moredata = TRUE;
      //(*totalTuples) = total_tuples = 0;
  while (moredata == TRUE) 
  {
    SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

            // on the first tuple get the column numbers 
    if (edge_columns[0] == -1)
    {
            // Fetching column numbers
      if (fetch_edge_columns(&edge_columns, &edge_types,has_rcost) == -1)
       return finish(SPIcode, ret);
            // Finished fetching column numbers
   }

   ntuples = SPI_processed;
   if (ntuples==1)
   {
     SPITupleTable *tuptable = SPI_tuptable;
     TupleDesc tupdesc = SPI_tuptable->tupdesc;
      //PGR_DBG("processing %d", ntuples);
     for (t = 0; t < ntuples; t++) {
      HeapTuple tuple = tuptable->vals[t];
    }
    SPI_freetuptable(tuptable);
  }
  else {
    moredata = FALSE;
  }
}

}*/
int
fetch_data(char *sql, Edge **edges,int *count)
{
    //bool sourceFound = false;
    //bool targetFound = false;
  bool has_rcost=false;
    /*if (start_vertex == -1 && end_vertex == -1) {
        sourceFound = targetFound = true;
    }*/
        int ntuples;
        int64_t total_tuples=0;
        char vertex_query[300]=""; 


        strcpy(vertex_query,"select count(*) from test_ways_vertices");
        //fetchVertexCount(vertex_query,count);
        int edge_columns[5];
        int edge_types[5];
        int i;
        for (i = 0; i < 5; ++i) edge_columns[i] = -1;
          for (i = 0; i < 5; ++i) edge_types[i] = -1;
            int ret = -1;


    // Connecting to SPI;
          int SPIcode = SPI_connect();
          if (SPIcode  != SPI_OK_CONNECT) 
          {
            elog(ERROR, "Couldn't open a connection to SPI");
            return -1;
          }

      // Preparing Plan
          void *SPIplan;
          SPIplan = SPI_prepare(sql, 0, NULL);
          if (SPIplan  == NULL) {
            elog(ERROR, "Couldn't create query plan via SPI");
            return -1;
          }

      // Opening Portal
          Portal SPIportal;
          if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
            elog(ERROR, "SPI_cursor_open('%s') returns NULL", sql);
            return -1;
          }


      // Starting Cycle;
          bool moredata = TRUE;
      //(*totalTuples) = total_tuples = 0;
          while (moredata == TRUE) 
          {
            SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

            /*  on the first tuple get the column numbers */
            if (edge_columns[0] == -1)
            {
            // Fetching column numbers
              if (fetch_edge_columns(&edge_columns, &edge_types,has_rcost) == -1)
               return finish(SPIcode, ret);
            // Finished fetching column numbers
           }

           ntuples = SPI_processed;
           total_tuples += ntuples;

        // Getting Memory
           if ((*edges) == NULL)
            (*edges) = (Edge *)palloc(total_tuples * sizeof(Edge));
          else
            (*edges) = (Edge *)repalloc((*edges), total_tuples * sizeof(Edge));
        // Got Memory 

          if ((*edges) == NULL) {
            elog(ERROR, "Out of memory");
            return finish(SPIcode, ret);    
          }

          if (ntuples > 0) {
            int t;
      //elog(INFO, "processing tuples");
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
      //PGR_DBG("processing %d", ntuples);
            for (t = 0; t < ntuples; t++) {
              HeapTuple tuple = tuptable->vals[t];
              fetch_edge(&tuple, &tupdesc, &edge_columns, &edge_types,
               &(*edges)[total_tuples - ntuples + t], has_rcost);

          /*if (!sourceFound
             && (((*edges)[total_tuples - ntuples + t].source == start_vertex)
                 || ((*edges)[total_tuples - ntuples + t].target == start_vertex))) {
            sourceFound = true;
    }
    if (!targetFound && (((*edges)[total_tuples - ntuples + t].source == end_vertex)
         || ((*edges)[total_tuples - ntuples + t].target == end_vertex))) {
        targetFound = true;
}*/
      }
      SPI_freetuptable(tuptable);
    }
    else {
      moredata = FALSE;
    }
  }


/*#if 0
  if (!sourceFound) {
      // elog(NOTICE, "Starting Vertex does not exist in the data");
      return 0;
  }
  if (!targetFound) {
      // elog(NOTICE, "Ending Vertex does not exist in the data");
      return 0;
  }
#endif*/
  if (total_tuples == 1) {
    // for some reason it needs at least a second edge for boost.graph to work
    // makeing a simple test and asking boost people
    ++total_tuples;
    (*edges) = (Edge *)repalloc((*edges), total_tuples * sizeof(Edge));
    (*edges)[1].source = -1;
    (*edges)[1].target = -1;
    (*edges)[1].cost = 10000;
    (*edges)[1].id = (*edges)[0].id + 1;
    (*edges)[1].revcost = -1;
  }

//(*totalTuples) = total_tuples;
  return total_tuples;
}

int
fetch_astar_data(char *sql, Edge **edges,int *count)
{
    //bool sourceFound = false;
    //bool targetFound = false;
  bool has_rcost=false;
    /*if (start_vertex == -1 && end_vertex == -1) {
        sourceFound = targetFound = true;
    }*/
        int ntuples;
        int64_t total_tuples=0;


        //strcpy(vertex_query,"select count(*) from test_ways_vertices");
        //fetchVertexCount(vertex_query,count);
        int edge_columns[9];
        int edge_types[9];
        int i;
        for (i = 0; i < 9; ++i) edge_columns[i] = -1;
          for (i = 0; i < 9; ++i) edge_types[i] = -1;
            int ret = -1;


    // Connecting to SPI;
          int SPIcode = SPI_connect();
          if (SPIcode  != SPI_OK_CONNECT) 
          {
            elog(ERROR, "Couldn't open a connection to SPI");
            return -1;
          }

      // Preparing Plan
          void *SPIplan;
          SPIplan = SPI_prepare(sql, 0, NULL);
          if (SPIplan  == NULL) {
            elog(ERROR, "Couldn't create query plan via SPI");
            return -1;
          }

      // Opening Portal
          Portal SPIportal;
          if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
            elog(ERROR, "SPI_cursor_open('%s') returns NULL", sql);
            return -1;
          }


      // Starting Cycle;
          bool moredata = TRUE;
      //(*totalTuples) = total_tuples = 0;
          while (moredata == TRUE) 
          {
            SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

            /*  on the first tuple get the column numbers */
            if (edge_columns[0] == -1)
            {
            // Fetching column numbers
              if (fetch_astar_edge_columns(&edge_columns, &edge_types,has_rcost) == -1)
               return finish(SPIcode, ret);
            // Finished fetching column numbers
           }

           ntuples = SPI_processed;
           total_tuples += ntuples;

        // Getting Memory
           if ((*edges) == NULL)
            (*edges) = (Edge *)palloc(total_tuples * sizeof(Edge));
          else
            (*edges) = (Edge *)repalloc((*edges), total_tuples * sizeof(Edge));
        // Got Memory 

          if ((*edges) == NULL) {
            elog(ERROR, "Out of memory");
            return finish(SPIcode, ret);    
          }

          if (ntuples > 0) {
            int t;
      //elog(INFO, "processing tuples");
            SPITupleTable *tuptable = SPI_tuptable;
            TupleDesc tupdesc = SPI_tuptable->tupdesc;
      //PGR_DBG("processing %d", ntuples);
            for (t = 0; t < ntuples; t++) {
              HeapTuple tuple = tuptable->vals[t];
              fetch_astar_edge(&tuple, &tupdesc, &edge_columns, &edge_types,
               &(*edges)[total_tuples - ntuples + t], has_rcost);

          /*if (!sourceFound
             && (((*edges)[total_tuples - ntuples + t].source == start_vertex)
                 || ((*edges)[total_tuples - ntuples + t].target == start_vertex))) {
            sourceFound = true;
    }
    if (!targetFound && (((*edges)[total_tuples - ntuples + t].source == end_vertex)
         || ((*edges)[total_tuples - ntuples + t].target == end_vertex))) {
        targetFound = true;
}*/
      }
      SPI_freetuptable(tuptable);
    }
    else {
      moredata = FALSE;
    }
  }
  if (total_tuples == 1) {
    // for some reason it needs at least a second edge for boost.graph to work
    // makeing a simple test and asking boost people
    ++total_tuples;
    (*edges) = (Edge *)repalloc((*edges), total_tuples * sizeof(Edge));
    (*edges)[1].source = -1;
    (*edges)[1].target = -1;
    (*edges)[1].cost = 10000;
    (*edges)[1].id = (*edges)[0].id + 1;
    (*edges)[1].revcost = -1;
  }

//(*totalTuples) = total_tuples;
  return total_tuples;
}
//printing data in the buf
void print_data(char buf[8192])
{
   // ret = SPI_connect();
 SPI_connect();
 elog(INFO, "OUTPUT: %s", buf);
 SPI_finish();
}

//function used for testing purpose
int add_one(int a)
{
  return a+1;
}