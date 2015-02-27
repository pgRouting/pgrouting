/*
 * kShortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2011 Dave Potts
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#include "fmgr.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "ksp.h" 
#include "KSPDriver.h"

#ifndef _MSC_VER
Datum kshortest_path(PG_FUNCTION_ARGS);
#else // _MSC_VER
PGDLLEXPORT Datum kshortest_path(PG_FUNCTION_ARGS);
#endif // _MSC_VER

#if 0  // change to 1 to leave it as a function (not working)
extern void kspDBG(const char *format, ...)
{
#ifdef DEBUG
	va_list ap;
	char msg[256];
	va_start(ap, format);
	vsnprintf(msg, 256, format, ap);
	va_end(ap);
	elog(NOTICE, msg);
#else 
	;
#endif /* DEBUG */
}

#endif

//#define DEBUG 1
#ifdef DEBUG 
#define kspDBG(format, arg...) \
elog(NOTICE, format , ## arg)
#else
#define kspDBG(format, arg...) do { ; } while (0)
#endif




#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

static char *
text2char(text *in)
{
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

static int ksp_finish(int code, int ret)
{
  code = SPI_finish();
  if (code  != SPI_OK_FINISH )
  {
    elog(ERROR,"couldn't disconnect from SPI");
    return -1 ;
  }			
  return ret;
}
			  
static int
ksp_fetch_edge_columns(SPITupleTable *tuptable, ksp_edge_columns_t *edge_columns, 
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

  kspDBG("columns: id %i source %i target %i cost %f", 
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

      kspDBG("columns: reverse_cost cost %f", edge_columns->reverse_cost);
    }
    
  return 0;
}

void
ksp_fetch_edge(HeapTuple *tuple, TupleDesc *tupdesc, 
           ksp_edge_columns_t *edge_columns, ksp_edge_t *target_edge)
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
}


PG_FUNCTION_INFO_V1(kshortest_path);
#ifndef _MSC_VER
Datum
#else // _MSC_VER
PGDLLEXPORT Datum
#endif // _MSC_VER
kshortest_path(PG_FUNCTION_ARGS)
{
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  ksp_path_element_t      *path;
  void * toDel;

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL())
    {
      MemoryContext   oldcontext;
      int path_count = 0;
      int ret;

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


      ret = compute_kshortest_path(text2char(PG_GETARG_TEXT_P(0)), /* SQL string */
                                  PG_GETARG_INT32(1),             /* source id */
                                  PG_GETARG_INT32(2),             /* target_id */
                                  PG_GETARG_INT32(3),             /* number of paths */
                                  PG_GETARG_BOOL(4), 		  /* has reverse_cost */
				  &path, 
				  &path_count);
      toDel=path;
#if 0
#ifdef DEBUG
      if (ret >= 0) 
        {
          int i;
        
          for (i = 0; i < path_count; i++) 
            {
              kspDBG("Step %i route_id  %d ", i, path[i].route_id);
              kspDBG("        vertex_id  %d ", path[i].vertex_id);
              kspDBG("        edge_id    %d ", path[i].edge_id);
              kspDBG("        cost       %f ", path[i].cost);
            }
        }
#endif
#endif 
     kspDBG("Total number of tuples to be returned %i ", path_count);
	
      /* total number of tuples to be returned */
      funcctx->max_calls = path_count;
      funcctx->user_fctx = path;

      funcctx->tuple_desc = 
        BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult3"));

      MemoryContextSwitchTo(oldcontext);
    }


  /* stuff done on every call of the function */
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  path = (ksp_path_element_t*) funcctx->user_fctx;

  if (call_cntr < max_calls)    /* do when there is more left to send */
    {
      HeapTuple    tuple;
      Datum        result;
     /* //Datum values[4];
      //bool nulls[4]; */

      Datum *values;
      bool* nulls;

      values = (Datum *)palloc(5 * sizeof(Datum));
      nulls =(bool *) palloc(5 * sizeof(bool));


      values[0] = Int32GetDatum(call_cntr);
      nulls[0] = false;
      values[1] = Int32GetDatum(path[call_cntr].route_id);
      nulls[1] = false;
      values[2] = Int32GetDatum(path[call_cntr].vertex_id);
      nulls[2] = false;
      values[3] = Int32GetDatum(path[call_cntr].edge_id);
      nulls[3] = false;
      values[4] = Float8GetDatum(path[call_cntr].cost);
      nulls[4] = false;

      tuple = heap_form_tuple(tuple_desc, values, nulls);

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
    }
  else    /* do when there is no more left */
    {
	free(path);
      SRF_RETURN_DONE(funcctx);
    }
}



int compute_kshortest_path(char* sql, int start_vertex, 
			 int end_vertex, int no_paths, 
			 bool has_reverse_cost, 
			 ksp_path_element_t **ksp_path, int *path_count) 
{

  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  int ntuples;
  ksp_edge_t *edges = NULL;
  int total_tuples = 0;
#ifndef _MSC_VER
  ksp_edge_columns_t edge_columns = {.id= -1, .source= -1, .target= -1, 
                                 .cost= -1, .reverse_cost= -1};
#else // _MSC_VER
  ksp_edge_columns_t edge_columns = {-1, -1, -1, -1, -1};
#endif // _MSC_VER
  int v_max_id=0;
  int v_min_id=INT_MAX;

  int s_count = 0;
  int t_count = 0;

  char *err_msg=(char *)"";
  int ret = -1;
  register int z;


  kspDBG("Starting kshortest_path %s\n",sql);
        
  SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT)
    {
      elog(ERROR, "kshortest_path: couldn't open a connection to SPI");
      return -1;
    }

  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL)
    {
      elog(ERROR, "kshortest_path: couldn't create query plan via SPI");
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
          if (ksp_fetch_edge_columns(SPI_tuptable, &edge_columns, 
                                 has_reverse_cost) == -1)
	    return ksp_finish(SPIcode, ret);
        }

      ntuples = SPI_processed;
      total_tuples += ntuples;
      if (!edges)
        edges = (ksp_edge_t *)palloc(total_tuples * sizeof(ksp_edge_t));
      else
        edges = (ksp_edge_t *)repalloc(edges, total_tuples * sizeof(ksp_edge_t));

      if (edges == NULL) 
        {
          elog(ERROR, "Out of memory");
	    return ksp_finish(SPIcode, ret);	  
        }

      if (ntuples > 0) 
        {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;
                
          for (t = 0; t < ntuples; t++) 
            {
              HeapTuple tuple = tuptable->vals[t];
              ksp_fetch_edge(&tuple, &tupdesc, &edge_columns, 
                         &edges[total_tuples - ntuples + t]);
            }
          SPI_freetuptable(tuptable);
        } 
      else 
        {
          moredata = FALSE;
        }
    }

 
      
  kspDBG("Total %i tuples in query", total_tuples);
  
  kspDBG("Calling doKpaths\n");
        
  ret = doKpaths(edges, total_tuples,
			start_vertex, end_vertex,
                       no_paths, has_reverse_cost,
                       ksp_path, path_count, &err_msg);
  kspDBG("total paths found %i\n",*path_count);
  kspDBG("Exist Status = %i\n", ret);
  kspDBG("Returned message = %s\n", err_msg);

  
  
  if (ret < 0)
    {
      //elog(ERROR, "Error computing path: %s", err_msg);
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), 
      errmsg("Error computing path: %s", err_msg)));
    } 
    
  return ksp_finish(SPIcode, ret);
}

// path gets size big
ksp_path_element_t * get_ksp_memory(int size,ksp_path_element_t *path){
	if(path ==0  ){
		path=malloc(size * sizeof(ksp_path_element_t));
	} else {
		path=realloc(path,size * sizeof(ksp_path_element_t));
	}
	return path;
}

