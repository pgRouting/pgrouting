/******************************************************************************
* $Id$
*
* Project:  pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
*

******************************************************************************
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies of this Software or works derived from this Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.

*****************************************************************************/

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"

#include "fmgr.h"
#include "bdsp.h"

Datum bidir_dijkstra_shortest_path(PG_FUNCTION_ARGS);


#undef DEBUG
//#define DEBUG 1

#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif

// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000

//#ifdef PG_MODULE_MAGIC
//PG_MODULE_MAGIC;
//#endif

typedef struct edge_columns
{
  int id;
  int source;
  int target;
  int cost;
  int reverse_cost;
} edge_columns_t;


static char *
text2char(text *in)
{
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

static int
finish(int code, int ret)
{
  DBG("In finish, trying to disconnect from spi %d",ret);
  code = SPI_finish();
  if (code  != SPI_OK_FINISH )
  {
    elog(ERROR,"couldn't disconnect from SPI");
    return -1 ;
  }			
  return ret;
}

/*
 * This function fetches the edge columns from the SPITupleTable.
 * 
*/ 
static int
fetch_edge_columns(SPITupleTable *tuptable, edge_columns_t *edge_columns, 
                   bool has_reverse_cost)
{
  edge_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
  edge_columns->source = SPI_fnumber(SPI_tuptable->tupdesc, "source");
  edge_columns->target = SPI_fnumber(SPI_tuptable->tupdesc, "target");
  edge_columns->cost = SPI_fnumber(SPI_tuptable->tupdesc, "cost");
  if (edge_columns->id == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->source == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->target == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->cost == SPI_ERROR_NOATTRIBUTE) 
    {
      elog(ERROR, "Error, query must return columns "
           "'id', 'source', 'target' and 'cost'");
      return -1;
    }

  if (SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->source) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->target) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->cost) != FLOAT8OID) 
    {
      elog(ERROR, "Error, columns 'source', 'target' must be of type int4, 'cost' must be of type float8");
      return -1;
    }

  DBG("columns: id %i source %i target %i cost %i", 
      edge_columns->id, edge_columns->source, 
      edge_columns->target, edge_columns->cost);

  if (has_reverse_cost)
    {
      edge_columns->reverse_cost = SPI_fnumber(SPI_tuptable->tupdesc, 
                                               "reverse_cost");

      if (edge_columns->reverse_cost == SPI_ERROR_NOATTRIBUTE) 
        {
          elog(ERROR, "Error, reverse_cost is used, but query did't return "
               "'reverse_cost' column");
          return -1;
        }

      if (SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->reverse_cost) 
          != FLOAT8OID) 
        {
          elog(ERROR, "Error, columns 'reverse_cost' must be of type float8");
          return -1;
        }

      DBG("columns: reverse_cost cost %i", edge_columns->reverse_cost);
    }
    
  return 0;
}

/*
 * To fetch a edge from Tuple.
 * 
 */
 
static void
fetch_edge(HeapTuple *tuple, TupleDesc *tupdesc, 
           edge_columns_t *edge_columns, edge_t *target_edge)
{
  Datum binval;
  bool isnull;

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->id, &isnull);
  if (isnull)
    elog(ERROR, "id contains a null value");
  target_edge->id = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->source, &isnull);
  if (isnull)
    elog(ERROR, "source contains a null value");
  target_edge->source = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->target, &isnull);
  if (isnull)
    elog(ERROR, "target contains a null value");
  target_edge->target = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->cost, &isnull);
  if (isnull)
    elog(ERROR, "cost contains a null value");
  target_edge->cost = DatumGetFloat8(binval);

  if (edge_columns->reverse_cost != -1) 
    {
      binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->reverse_cost, 
                             &isnull);
      if (isnull)
        elog(ERROR, "reverse_cost contains a null value");
      target_edge->reverse_cost =  DatumGetFloat8(binval);
    }

  /*
  DBG("edge: %i, %i, %i, %f, %f", target_edge->id, target_edge->source,
    target_edge->target, target_edge->cost, target_edge->reverse_cost);
  */
}

static int compute_bidirsp(char* sql, int start_vertex, 
                                 int end_vertex, bool directed, 
                                 bool has_reverse_cost, 
                                 path_element_t **path, int *path_count) 
{
  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  int ntuples;
  edge_t *edges = NULL;
  int total_tuples = 0;
  edge_columns_t edge_columns = {id: -1, source: -1, target: -1, 
                                 cost: -1, reverse_cost: -1};
  int v_max_id=0;
  int v_min_id=INT_MAX;

  int s_count = 0;
  int t_count = 0;

  char *err_msg;
  int ret = -1;
  register int z;

  DBG("start shortest_path\n");
        
  SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT)
    {
      elog(ERROR, "shortest_path: couldn't open a connection to SPI");
      return -1;
    }

  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL)
    {
      elog(ERROR, "shortest_path: couldn't create query plan via SPI");
      return -1;
    }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) 
    {
      elog(ERROR, "shortest_path: SPI_cursor_open('%s') returns NULL", sql);
      return -1;
    }

  while (moredata == TRUE)
    {
      SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

      if (edge_columns.id == -1) 
        {
          if (fetch_edge_columns(SPI_tuptable, &edge_columns, 
                                 has_reverse_cost) == -1)
	    return finish(SPIcode, ret);
        }

      ntuples = SPI_processed;
      total_tuples += ntuples;
      if (!edges)
        edges = palloc(total_tuples * sizeof(edge_t));
      else
        edges = repalloc(edges, total_tuples * sizeof(edge_t));

      if (edges == NULL) 
        {
          elog(ERROR, "Out of memory");
	    return finish(SPIcode, ret);	  
        }

      if (ntuples > 0) 
        {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;
                
          for (t = 0; t < ntuples; t++) 
            {
              HeapTuple tuple = tuptable->vals[t];
              fetch_edge(&tuple, &tupdesc, &edge_columns, 
                         &edges[total_tuples - ntuples + t]);
            }
          SPI_freetuptable(tuptable);
        } 
      else 
        {
          moredata = FALSE;
        }
    }

  //defining min and max vertex id
      
  DBG("Total %i tuples", total_tuples);
    
  for(z=0; z<total_tuples; z++)
  {
    if(edges[z].source<v_min_id)
    v_min_id=edges[z].source;
  
    if(edges[z].source>v_max_id)
      v_max_id=edges[z].source;
		            
    if(edges[z].target<v_min_id)
      v_min_id=edges[z].target;

    if(edges[z].target>v_max_id)
      v_max_id=edges[z].target;      
								        
    DBG("%i <-> %i", v_min_id, v_max_id);
							
  }

  //::::::::::::::::::::::::::::::::::::  
  //:: reducing vertex id (renumbering)
  //::::::::::::::::::::::::::::::::::::
  for(z=0; z<total_tuples; z++)
  {
    //check if edges[] contains source and target
    if(edges[z].source == start_vertex || edges[z].target == start_vertex)
      ++s_count;
    if(edges[z].source == end_vertex || edges[z].target == end_vertex)
      ++t_count;

    edges[z].source-=v_min_id;
    edges[z].target-=v_min_id;
    DBG("%i - %i", edges[z].source, edges[z].target);      
  }

  DBG("Total %i tuples", total_tuples);

  if(s_count == 0)
  {
    elog(ERROR, "Start vertex was not found.");
    return -1;
  }
      
  if(t_count == 0)
  {
    elog(ERROR, "Target vertex was not found.");
    return -1;
  }
  
  DBG("Calling Bi directional wrapper\n");
        
  start_vertex -= v_min_id;
  end_vertex   -= v_min_id;
  
  v_max_id -= v_min_id;

  ret = bidirsp_wrapper(edges, total_tuples, v_max_id + 1, start_vertex, end_vertex,
                       directed, has_reverse_cost,
                       path, path_count, &err_msg);

  DBG("SIZE %i\n",*path_count);

  //::::::::::::::::::::::::::::::::
  //:: restoring original vertex id
  //::::::::::::::::::::::::::::::::
  for(z=0;z<*path_count;z++)
  {
    //DBG("vetex %i\n",(*path)[z].vertex_id);
    (*path)[z].vertex_id+=v_min_id;
  }

  DBG("ret = %i\n", ret);

  DBG("*path_count = %i\n", *path_count);

  DBG("ret = %i\n", ret);
  
  if (ret < 0)
    {
      //elog(ERROR, "Error computing path: %s", err_msg);
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), 
        errmsg("Error computing path: %s", err_msg)));
    } 
    
  return finish(SPIcode, ret);
}


PG_FUNCTION_INFO_V1(bidir_dijkstra_shortest_path);
Datum
bidir_dijkstra_shortest_path(PG_FUNCTION_ARGS)
{
	
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  path_element_t      *path;
  char *               sql;


  // stuff done only on the first call of the function 
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      int path_count = 0;
      int ret = -1;
      int i;

      // create a function context for cross-call persistence
      funcctx = SRF_FIRSTCALL_INIT();

      // switch to memory context appropriate for multiple function calls
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      // verify that the first 5 args are not NULL
      for (i=0; i<5; i++)
        if(PG_ARGISNULL(i)) {
            elog(ERROR, "turn_restrict_shortest_path(): Argument %i may not be NULL", i+1);
        }
      
      if (PG_ARGISNULL(5))
        sql = NULL;
      else {
        sql = text2char(PG_GETARG_TEXT_P(5));
        if (strlen(sql) == 0)
            sql = NULL;
      }

	  DBG("Calling compute_trsp");

      ret = compute_bidirsp(text2char(PG_GETARG_TEXT_P(0)),
                                   PG_GETARG_INT32(1),
                                   PG_GETARG_INT32(2),
                                   PG_GETARG_BOOL(3),
                                   PG_GETARG_BOOL(4), 
                                   &path, &path_count);
#ifdef DEBUG
	double total_cost = 0;
      DBG("Ret is %i", ret);
      if (ret >= 0) 
        {
          int i;
          for (i = 0; i < path_count; i++) 
            {
         //     DBG("Step %i vertex_id  %i ", i, path[i].vertex_id);
           //   DBG("        edge_id    %i ", path[i].edge_id);
             // DBG("        cost       %f ", path[i].cost);
              total_cost+=path[i].cost;
            }
        }
        DBG("Total cost is: %f",total_cost);
#endif

      // total number of tuples to be returned 
      funcctx->max_calls = path_count;
      funcctx->user_fctx = path;

      funcctx->tuple_desc = 
        BlessTupleDesc(RelationNameGetTupleDesc("pgr_pathResult"));

      MemoryContextSwitchTo(oldcontext);
    }

  // stuff done on every call of the function 
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  path = (path_element_t*) funcctx->user_fctx;

  if (call_cntr < max_calls)    // do when there is more left to send 
    {
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;

      values = palloc(3 * sizeof(Datum));
      nulls = palloc(3 * sizeof(char));

      values[0] = Int32GetDatum(path[call_cntr].vertex_id);
      nulls[0] = ' ';
      values[1] = Int32GetDatum(path[call_cntr].edge_id);
      nulls[1] = ' ';
      values[2] = Float8GetDatum(path[call_cntr].cost);
      nulls[2] = ' ';
		      
      tuple = heap_formtuple(tuple_desc, values, nulls);

      // make the tuple into a datum 
      result = HeapTupleGetDatum(tuple);

      // clean up (this is not really necessary) 
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
    }
  else    // do when there is no more left 
    {
      DBG("Going to free path");
      if (path) free(path);
      SRF_RETURN_DONE(funcctx);
    }
}
