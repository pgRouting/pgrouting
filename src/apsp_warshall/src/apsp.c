/*
 * All pairs shortest path - Warshall's algorithm
 *
 * Copyright (c) 2011 Jay Mahadeokar
 *
 * This code was integrated into the pgRoutimg tree 2012 by
 * Stephen Woodbridge from:
 * https://github.com/jay-mahadeokar/pgrouting
 *
 */


#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif
//#include <set>

#include "fmgr.h"

#include "apsp.h"   

//using namespace std;

Datum apsp_warshall(PG_FUNCTION_ARGS);

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

#ifdef PG_MODULE_MAGIC
//PG_MODULE_MAGIC;
#endif

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
  code = SPI_finish();
  if (code  != SPI_OK_FINISH )
  {
    elog(ERROR,"couldn't disconnect from SPI");
    return -1 ;
  }			
  return ret;
}
			  
typedef struct edge_columns 
{
  int id;
  int source;
  int target;
  float8 cost;
  float8 reverse_cost;
} edge_columns_t;


//This probably fetches the edge columns from SPITuple table into the edgeColumns

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

  DBG("columns: id %i source %i target %i cost %i", edge_columns->id, edge_columns->source, edge_columns->target, edge_columns->cost);

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
    //DBG("Fetched Edge.  id: %d \tsource: %i\ttarget: %f\tcost: %i",target_edge->id,target_edge->source,target_edge->target,target_edge->cost);
}



static int compute_apsp_warshall(char* sql, bool directed, 
                                 bool has_reverse_cost, 
                                 apsp_element_t **pair, int *pair_count) 
{
  int i;
  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  int ntuples;
  edge_t *edges = NULL;
  int total_tuples = 0;
  edge_columns_t edge_columns = {.id= -1, .source= -1, .target= -1, 
                                 .cost= -1, .reverse_cost= -1};
  int v_max_id=0;
  int v_min_id=INT_MAX;

  int s_count = 0;
  int t_count = 0;

  char *err_msg;
  int ret = -1;
  register int z;

//  set<int> vertices;
  
  DBG("start compute_apsp_warshall\n");
        
  SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT)
    {
      elog(ERROR, "compute_apsp_warshall: couldn't open a connection to SPI");
      return -1;
    }

  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL)
    {
      elog(ERROR, "compute_apsp_warshall: couldn't create query plan via SPI");
      return -1;
    }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) 
    {
      elog(ERROR, "compute_apsp_warshall: SPI_cursor_open('%s') returns NULL", sql);
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

      DBG("Number of tuples fetched: %i",ntuples);
      
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
//	      vertices.insert(edges[total_tuples - ntuples + t].source);
//	      vertices.insert(edges[total_tuples - ntuples + t].target);
            }
          SPI_freetuptable(tuptable);
        } 
      else 
        {
          moredata = FALSE;
        }
    }
    
#ifdef DEBUG
      
      
          
          for (i = 0; i < total_tuples; i++) 
            {
              DBG("Step %i src_vertex_id  %i ", i, edges[i].source);
              DBG("        dest_vertex_id    %i ", edges[i].target);
              DBG("        cost       %f ", edges[i].cost);
            }
      
#endif
    

  DBG("Calling boost_apsp\n");
        
  //start_vertex -= v_min_id;
  //end_vertex   -= v_min_id;

  ret = boost_apsp(edges, total_tuples, 0,                        //vertices.size()
                       directed, has_reverse_cost,
                       pair, pair_count, &err_msg);
  DBG("Boost message: \n%s",err_msg);
  DBG("SIZE %i\n",*pair_count);

/*  //::::::::::::::::::::::::::::::::
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
  */
  if (ret < 0)
    {
      //elog(ERROR, "Error computing path: %s", err_msg);
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), 
        errmsg("Error computing path: %s", err_msg)));
    } 
    
  return finish(SPIcode, ret);
}


PG_FUNCTION_INFO_V1(apsp_warshall);
Datum
apsp_warshall(PG_FUNCTION_ARGS)
{
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  apsp_element_t      *pair;                         

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL())
    {
      MemoryContext   oldcontext;
      int pair_count = 0;  
      int ret;

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


      ret = compute_apsp_warshall(text2char(PG_GETARG_TEXT_P(0)),
                                  PG_GETARG_BOOL(1),
                                  PG_GETARG_BOOL(2), &pair, &pair_count);                  
#ifdef DEBUG
      DBG("Ret is %i", ret);
      if (ret >= 0) 
        {
          int i;
          for (i = 0; i < pair_count; i++) 
            {
              DBG("Step: %i, source_id: %i, target_id: %i, cost: %f ", i, pair[i].src_vertex_id, pair[i].dest_vertex_id, pair[i].cost);
            }
        }
#endif

      /* total number of tuples to be returned */
      funcctx->max_calls = pair_count;
      funcctx->user_fctx = pair;

      funcctx->tuple_desc = 
        BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

      MemoryContextSwitchTo(oldcontext);
    }

  /* stuff done on every call of the function */
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  pair = (apsp_element_t*) funcctx->user_fctx;

  if (call_cntr < max_calls)    /* do when there is more left to send */
    {
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;

      /* This will work for some compilers. If it crashes with segfault, try to change the following block with this one    
 
      values = palloc(4 * sizeof(Datum));
      nulls = palloc(4 * sizeof(char));
  
      values[0] = call_cntr;
      nulls[0] = ' ';
      values[1] = Int32GetDatum(path[call_cntr].vertex_id);
      nulls[1] = ' ';
      values[2] = Int32GetDatum(path[call_cntr].edge_id);
      nulls[2] = ' ';
      values[3] = Float8GetDatum(path[call_cntr].cost);
      nulls[3] = ' ';
      */
    
      values = palloc(4 * sizeof(Datum));
      nulls = palloc(4 * sizeof(char));

      values[0] = Int32GetDatum(call_cntr);
      nulls[0] = ' ';
      values[1] = Int32GetDatum(pair[call_cntr].src_vertex_id);
      nulls[1] = ' ';
      values[2] = Int32GetDatum(pair[call_cntr].dest_vertex_id);
      nulls[2] = ' ';
      values[3] = Float8GetDatum(pair[call_cntr].cost);
      nulls[3] = ' ';
		      
      tuple = heap_formtuple(tuple_desc, values, nulls);

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
    }
  else    /* do when there is no more left */
    {
      SRF_RETURN_DONE(funcctx);
    }
}

