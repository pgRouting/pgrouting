/*
 * Traveling Salesman Problem solution algorithm for PostgreSQL
 *
 * Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include "tsp.h"

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"

#include "string.h"
#include "math.h"

#include "fmgr.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


// ------------------------------------------------------------------------

/*
 * Define this to have profiling enabled
 */
//#define PROFILE

#ifdef PROFILE
#include <sys/time.h>

struct timeval prof_tsp, prof_store, prof_extract, prof_total;
long proftime[5];
long profipts1, profipts2, profopts;
#define profstart(x) do { gettimeofday(&x, NULL); } while (0);
#define profstop(n, x) do { struct timeval _profstop;   \
        long _proftime;                         \
        gettimeofday(&_profstop, NULL);                         \
        _proftime = ( _profstop.tv_sec*1000000+_profstop.tv_usec) -     \
                ( x.tv_sec*1000000+x.tv_usec); \
        elog(NOTICE, \
                "PRF(%s) %lu (%f ms)", \
                (n), \
             _proftime, _proftime / 1000.0);    \
        } while (0);

#else

#define profstart(x) do { } while (0);
#define profstop(n, x) do { } while (0);

#endif // PROFILE

// ------------------------------------------------------------------------

Datum tsp(PG_FUNCTION_ARGS);

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

// Apologies for using fixed-length arrays.  But this is an example, not 
// production code ;)
//#define MAX_TOWNS 40

float DISTANCE[MAX_TOWNS][MAX_TOWNS];
float x[MAX_TOWNS],y[MAX_TOWNS];
int total_tuples;


static char *
text2char(text *in)
{
  char *out = (char*)palloc(VARSIZE(in));

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
		  

typedef struct point_columns
{
  int id;
  float8 x;
  float8 y;
} point_columns_t;


static int
fetch_point_columns(SPITupleTable *tuptable, point_columns_t *point_columns)
{
  DBG("Fetching point");

  point_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "source_id");
  point_columns->x = SPI_fnumber(SPI_tuptable->tupdesc, "x");
  point_columns->y = SPI_fnumber(SPI_tuptable->tupdesc, "y");
  if (point_columns->id == SPI_ERROR_NOATTRIBUTE ||
      point_columns->x == SPI_ERROR_NOATTRIBUTE ||
      point_columns->y == SPI_ERROR_NOATTRIBUTE) 
    {
      elog(ERROR, "Error, query must return columns "
           "'source_id', 'x' and 'y'");
      return -1;
    }
    
  DBG("* Point %i [%f, %f]", point_columns->id, point_columns->x, 
      point_columns->y);

  return 0;
}

static void
fetch_point(HeapTuple *tuple, TupleDesc *tupdesc, 
            point_columns_t *point_columns, point_t *point)
{
  Datum binval;
  bool isnull;

  DBG("inside fetch_point\n");

  binval = SPI_getbinval(*tuple, *tupdesc, point_columns->id, &isnull);
  DBG("Got id\n");

  if (isnull)
    elog(ERROR, "id contains a null value");

  point->id = DatumGetInt32(binval);

  DBG("id = %i\n", point->id);

  binval = SPI_getbinval(*tuple, *tupdesc, point_columns->x, &isnull);
  if (isnull)
    elog(ERROR, "x contains a null value");

  point->x = DatumGetFloat8(binval);

  DBG("x = %f\n", point->x);

  binval = SPI_getbinval(*tuple, *tupdesc, point_columns->y, &isnull);

  if (isnull)
    elog(ERROR, "y contains a null value");

  point->y = DatumGetFloat8(binval);

  DBG("y = %f\n", point->y);
}


static int solve_tsp(char* sql, char* p_ids, 
                     int source, path_element_t* path) 
{
  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  int ntuples;

  //todo replace path (vector of path_element_t) with this array
  int ids[MAX_TOWNS];

  point_t *points=NULL;
  point_columns_t point_columns = {id: -1, x: -1, y:-1};

  char *err_msg = NULL;
  int ret = -1;
    
  char *p;
  int   z = 0;
  
  int    tt, cc;
  double dx, dy;
  float  fit=0.0;

  DBG("inside tsp\n");

  //int total_tuples = 0;
  total_tuples = 0;

  p = strtok(p_ids, ",");
  while(p != NULL)
    {
      //      ((path_element_t*)path)[z].vertex_id = atoi(p);
      ids[z]=atoi(p);
      p = strtok(NULL, ",");
      z++;
      if(z >= MAX_TOWNS)
      {
        elog(ERROR, "Number of points exeeds max number.");
        break;
      }
    }
    
  DBG("ZZZ %i\n",z);
  DBG("start tsp\n");
        
  SPIcode = SPI_connect();

  if (SPIcode  != SPI_OK_CONNECT)
    {
      elog(ERROR, "tsp: couldn't open a connection to SPI");
      return -1;
    }

  SPIplan = SPI_prepare(sql, 0, NULL);

  if (SPIplan  == NULL)
    {
      elog(ERROR, "tsp: couldn't create query plan via SPI");
      return -1;
    }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) 
    {
      elog(ERROR, "tsp: SPI_cursor_open('%s') returns NULL", sql);
      return -1;
    }
    
  DBG("Query: %s\n",sql);
  DBG("Query executed\n");

  while (moredata == TRUE)
    {
      SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

      if (point_columns.id == -1)
        {
          if (fetch_point_columns(SPI_tuptable, &point_columns) == -1)
	    return finish(SPIcode, ret);
        }

      ntuples = SPI_processed;

      total_tuples += ntuples;

      DBG("Tuples: %i\n", total_tuples);

      if (!points)
        points = palloc(total_tuples * sizeof(point_t));
      else
        points = repalloc(points, total_tuples * sizeof(point_t));
                                        
      if (points == NULL)
        {
          elog(ERROR, "Out of memory");
	  return finish(SPIcode, ret);
        }

      if (ntuples > 0)
        {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          DBG("Got tuple desc\n");
                
          for (t = 0; t < ntuples; t++) 
            {
              HeapTuple tuple = tuptable->vals[t];
              DBG("Before point fetched\n");
              fetch_point(&tuple, &tupdesc, &point_columns, 
                          &points[total_tuples - ntuples + t]);
              DBG("Point fetched\n");
            }

          SPI_freetuptable(tuptable);
        }
      else
        {
          moredata = FALSE;
        }                                       
    }

  
  DBG("Calling TSP\n");
        
  profstop("extract", prof_extract);
  profstart(prof_tsp);

  DBG("Total tuples: %i\n", total_tuples);

  for(tt=0;tt<total_tuples;++tt)
    {
      //((path_element_t*)path)[tt].vertex_id = points[tt].id;
      ids[tt] = points[tt].id;
      x[tt] = points[tt].x;
      y[tt] = points[tt].y;
  
      DBG("Point at %i: %i [%f, %f]\n",  tt, ids[tt], x[tt], y[tt]);
            
      // ((path_element_t*)path)[tt].vertex_id, x[tt], y[tt]);

      for(cc=0;cc<total_tuples;++cc)
        {
          dx=x[tt]-x[cc]; dy=y[tt]-y[cc];
          DISTANCE[tt][cc] = DISTANCE[cc][tt] = sqrt(dx*dx+dy*dy);
        }
    }

  DBG("DISTANCE counted\n");
  pfree(points);
    
  //ret = find_tsp_solution(total_tuples, DISTANCE, 
  //   path, source, &fit, &err_msg);

  ret = find_tsp_solution(total_tuples, DISTANCE, ids, 
                          source, &fit, err_msg);

  for(tt=0;tt<total_tuples;++tt)
    {
      ((path_element_t*)path)[tt].vertex_id = ids[tt];
    }
    
  DBG("TSP solved!\n");
  DBG("Score: %f\n", fit);

  profstop("tsp", prof_tsp);
  profstart(prof_store);

  DBG("Profile changed and ret is %i", ret);

  if (ret < 0)
    {
      //elog(ERROR, "Error computing path: %s", err_msg);
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), errmsg("Error computing path: %s", err_msg)));
    } 

  return finish(SPIcode, ret);    
}

PG_FUNCTION_INFO_V1(tsp);
Datum
tsp(PG_FUNCTION_ARGS)
{
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  path_element_t        *path;
    
  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL())
    {
      MemoryContext   oldcontext;
      //int path_count;
      int ret=-1;

      // XXX profiling messages are not thread safe
      profstart(prof_total);
      profstart(prof_extract);

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        

      path = (path_element_t *)palloc(sizeof(path_element_t)*MAX_TOWNS);


      ret = solve_tsp(text2char(PG_GETARG_TEXT_P(0)),
                      text2char(PG_GETARG_TEXT_P(1)),
                      PG_GETARG_INT32(2),
                      path);

      /* total number of tuples to be returned */
      DBG("Counting tuples number\n");

      funcctx->max_calls = total_tuples;

      funcctx->user_fctx = path;

      funcctx->tuple_desc = BlessTupleDesc(
                              RelationNameGetTupleDesc("path_result"));
      MemoryContextSwitchTo(oldcontext);
    }

  /* stuff done on every call of the function */
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;

  path = (path_element_t *)funcctx->user_fctx;

  DBG("Trying to allocate some memory\n");
  DBG("call_cntr = %i, max_calls = %i\n", call_cntr, max_calls);

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
    
      values = palloc(3 * sizeof(Datum));
      nulls = palloc(3 * sizeof(char));

      values[0] = Int32GetDatum(path[call_cntr].vertex_id);
      nulls[0] = ' ';
      values[1] = Int32GetDatum(path[call_cntr].edge_id);
      nulls[1] = ' ';
      values[2] = Float8GetDatum(path[call_cntr].cost);
      nulls[2] = ' ';
      
      DBG("Heap making\n");

      tuple = heap_formtuple(tuple_desc, values, nulls);

      DBG("Datum making\n");

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      DBG("VAL: %i\n, %i", values[0], result);
      DBG("Trying to free some memory\n");
    
      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);
        

      SRF_RETURN_NEXT(funcctx, result);
    }
  else    /* do when there is no more left */
    {
      DBG("Ending function\n");
      profstop("store", prof_store);
      profstop("total", prof_total);
      DBG("Profiles stopped\n");

      pfree(path);

      DBG("Path cleared\n");

      SRF_RETURN_DONE(funcctx);
    }
}
