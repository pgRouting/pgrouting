#include "postgres.h"
#include "executor/spi.h"
#include "catalog/pg_type.h"
#include "utils/builtins.h"
#include "structs.h"
#include "connection.h"
#define TUPLIMIT 1000

#include "./../../common/src/debug_macro.h"
#include "./../../common/src/postgres_connection.h"
#if 0
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

#endif
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

static char* SPI_getString(HeapTuple *tuple, TupleDesc *tupdesc, int colNumber, int colType) {
  //Datum binval;
  //bool isnull;
  char *value =NULL;
  //binval = SPI_getbinval(*tuple, *tupdesc, colNumber, &isnull);
  //if (isnull) elog(ERROR, "Null value found");
  switch (colType) {
    case TEXTOID:
    //elog(INFO,"TEXTOID");
    value = SPI_getvalue(*tuple,*tupdesc,colNumber);
    //elog(INFO,"value is %s",value);
    break;
    case CSTRINGOID:
    //elog(INFO,"CSTRINGOID");
    value = SPI_getvalue(*tuple,*tupdesc,colNumber);
    break;
    default:
    elog(ERROR, "text/varchar expected");
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
int fetch_contracted_graph_columns(int (*edge_columns)[5],int (*edge_types)[5]) 
{

  int error;
  error = fetch_column_info(&(*edge_columns)[0], &(*edge_types)[0], "contracted_graph_name");
  if (error == -1) return error;
  //elog(INFO,"fetched name info" );
  error = fetch_column_info(&(*edge_columns)[1], &(*edge_types)[1], "contracted_graph_blob");
  if (error == -1) return error;
  //elog(INFO,"fetched graph info" );
  error = fetch_column_info(&(*edge_columns)[2], &(*edge_types)[2], "removedvertices");
  if (error == -1) return error;
  //elog(INFO,"fetched rv info" );
  error = fetch_column_info(&(*edge_columns)[3], &(*edge_types)[3], "removededges");
  if (error == -1) return error;
  //elog(INFO,"fetched re info" );
  error = fetch_column_info(&(*edge_columns)[4], &(*edge_types)[4], "psuedoedges");
  if (error == -1) return error;
  //elog(INFO,"fetched pe info" );
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
void fetch_contracted_info(
 HeapTuple *tuple,
 TupleDesc *tupdesc, 
 int (*graph_columns)[5],
 int (*graph_types)[5],
 pgr_contracted_blob *graphInfo) {

  graphInfo->contracted_graph_name = SPI_getString(tuple, tupdesc, (*graph_columns)[0], (*graph_types)[0]);
  graphInfo->contracted_graph_blob = SPI_getString(tuple, tupdesc, (*graph_columns)[1], (*graph_types)[1]);
  graphInfo->removedVertices = SPI_getString(tuple, tupdesc, (*graph_columns)[2], (*graph_types)[2]);
  graphInfo->removedEdges = SPI_getString(tuple, tupdesc, (*graph_columns)[3], (*graph_types)[3]);
  graphInfo->psuedoEdges = SPI_getString(tuple, tupdesc, (*graph_columns)[4], (*graph_types)[4]);
  
}
int get_contracted_graph(char *sql,pgr_contracted_blob **graphInfo)
{
  int ntuples;
  int64_t total_tuples=0;
  int graph_columns[5];
  int graph_types[5];
  int i;
  for (i = 0; i < 5; ++i) graph_columns[i] = -1;
    for (i = 0; i < 5; ++i) graph_types[i] = -1;
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
      if (graph_columns[0] == -1)
      {
            // Fetching column numbers
        if (fetch_contracted_graph_columns(&graph_columns, &graph_types) == -1)
         //return finish(SPIcode, ret);
          pgr_SPI_finish();
          return -1;
            // Finished fetching column numbers
     }

     ntuples = SPI_processed;
     total_tuples += ntuples;   
        // Getting Memory
     if ((*graphInfo) == NULL)
      (*graphInfo) = (pgr_contracted_blob *)palloc(total_tuples * sizeof(pgr_contracted_blob));
    else
      (*graphInfo) = (pgr_contracted_blob *)repalloc((*graphInfo), total_tuples * sizeof(pgr_contracted_blob));
        // Got Memory 

    if ((*graphInfo) == NULL) {
      elog(ERROR, "Out of memory");
      //return finish(SPIcode, ret);
      pgr_SPI_finish();
          return -1;    
    }
    if (ntuples > 0)
    {

      int t;
      //elog(INFO, "processing tuples");
      SPITupleTable *tuptable = SPI_tuptable;
      TupleDesc tupdesc = SPI_tuptable->tupdesc;
      //PGR_DBG("processing %d", ntuples);
      for (t = 0; t < ntuples; t++) 
      {
        HeapTuple tuple = tuptable->vals[t];
        fetch_contracted_info(&tuple, &tupdesc, &graph_columns, &graph_types,
         &(*graphInfo)[total_tuples - ntuples + t]);
      }
      SPI_freetuptable(tuptable);
    }

    else {
      moredata = FALSE;
    }
  }
  return 0;
}








int
fetch_data(char *sql, Edge **edges,int *edge_count,bool rcost)
{
    //bool sourceFound = false;
    //bool targetFound = false;
  bool has_rcost=false;
  if (rcost)
  {
    has_rcost=rcost;
  }
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
               //return finish(SPIcode, ret);
               pgr_SPI_finish();
          return -1;  
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
            pgr_SPI_finish();
          return -1;  
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
  *edge_count=total_tuples;
  return 0;
}

int
fetch_astar_data(char *sql, Edge **edges,int *count,bool rcost)
{
    //bool sourceFound = false;
    //bool targetFound = false;
  bool has_rcost=false;
  if (rcost)
  {
    has_rcost=rcost;
  }
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
               pgr_SPI_finish();
          return -1;
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
            pgr_SPI_finish();
          return -1;   
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
 pgr_SPI_finish();
         // return -1;
}

//function used for testing purpose
int add_one(int a)
{
  return a+1;
}


//function used to insert
int execq(char *sql,int cnt)
{
  int ret;
  int proc = 0;
  
  SPI_connect();
  elog(INFO, "Executing %s .....",sql);
  ret = SPI_exec(sql, cnt);
  
  proc = SPI_processed;
  /*
   * If this is SELECT and some tuple(s) fetched -
   * returns tuples to the caller via elog (NOTICE).
   */
   if ( ret == SPI_OK_SELECT && SPI_processed > 0 )
   {
    TupleDesc tupdesc = SPI_tuptable->tupdesc;
    SPITupleTable *tuptable = SPI_tuptable;
    char buf[8192];
    int i;
    
    for (ret = 0; ret < proc; ret++)
    {
      HeapTuple tuple = tuptable->vals[ret];
      
      for (i = 1, buf[0] = 0; i <= tupdesc->natts; i++)
        sprintf(buf + strlen (buf), " %s%s",
          SPI_getvalue(tuple, tupdesc, i),
          (i == tupdesc->natts) ? " " : " |");
      elog (NOTICE, "EXECQ: %s", buf);
    }
  }

  pgr_SPI_finish();
          //return -1;
  return (proc);
  

}