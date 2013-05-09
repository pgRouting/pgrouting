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

// macro to store distance values as DISTANCE[MAX_TOWNS][MAX_TOWNS]
#define D(i,j) DISTANCE[(i)*MAX_TOWNS + j]

DTYPE *DISTANCE;
float *x;
float *y;
int MAX_TOWNS;
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
  //DBG("Fetching point");

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
    
  //DBG("* Point %i [%f, %f]", point_columns->id, point_columns->x, point_columns->y);

  return 0;
}

static void
fetch_point(HeapTuple *tuple, TupleDesc *tupdesc, 
            point_columns_t *point_columns, point_t *point)
{
  Datum binval;
  bool isnull;

  //DBG("inside fetch_point");

  binval = SPI_getbinval(*tuple, *tupdesc, point_columns->id, &isnull);
  //DBG("Got id");

  if (isnull)
    elog(ERROR, "id contains a null value");

  point->id = DatumGetInt32(binval);

  //DBG("id = %i", point->id);

  binval = SPI_getbinval(*tuple, *tupdesc, point_columns->x, &isnull);
  if (isnull)
    elog(ERROR, "x contains a null value");

  point->x = DatumGetFloat8(binval);

  //DBG("x = %f", point->x);

  binval = SPI_getbinval(*tuple, *tupdesc, point_columns->y, &isnull);

  if (isnull)
    elog(ERROR, "y contains a null value");

  point->y = DatumGetFloat8(binval);

  //DBG("y = %f", point->y);
}


static int solve_tsp(char* sql, char* p_ids, 
                     int source, path_element_t** path) 
{
  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  int ntuples;

  //todo replace path (vector of path_element_t) with this array
  int *ids;

  point_t *points=NULL;
  point_columns_t point_columns = {id: -1, x: -1, y:-1};

  char *err_msg = NULL;
  int ret = -1;
    
  char *p;
  int   z = 0;
  int   i;
  
  int    tt, cc;
  double dx, dy;
  float  fit=0.0;
  int got_source = 0;

  DBG("inside tsp");

  //int total_tuples = 0;
  total_tuples = 0;

  /* count the number of towns and allocate memory */
  for (i=0, MAX_TOWNS=1; i<strlen(p_ids); i++)
    if (p_ids[i] == ',') MAX_TOWNS++;

  DBG("MAX_TOWNS=%d", MAX_TOWNS);

  *path = (path_element_t *) palloc( MAX_TOWNS * sizeof(path_element_t) );
  if (! *path) {
    elog(ERROR, "Failed to alloc memory!");
    return -1;
  }

  ids = (int *)palloc( MAX_TOWNS * sizeof(int) );
  if (!ids) {
    elog(ERROR, "Failed to alloc memory!");
    return -1;
  }

  DISTANCE = (DTYPE *) palloc(MAX_TOWNS * MAX_TOWNS * sizeof(DTYPE));
  if (!DISTANCE) {
    elog(ERROR, "Failed to alloc memory!");
    return -1;
  }

  x = (float *) palloc(MAX_TOWNS * sizeof(float));
  y = (float *) palloc(MAX_TOWNS * sizeof(float));
  if (!x || !y) {
    elog(ERROR, "Failed to alloc memory!");
    return -1;
  }

  p = strtok(p_ids, ",");
  while(p != NULL) {
      ids[z]=atoi(p);
      if (ids[z] == source) got_source = 1;
      p = strtok(NULL, ",");
      z++;
      if(z > MAX_TOWNS)
      {
        elog(ERROR, "Number of points exeeds max number.");
        break;
      }
  }

  if (!got_source) {
    elog(ERROR, "tsp: source id not included in list of ids!");
    return -1;
  }
    
  DBG("start tsp");
        
  SPIcode = SPI_connect();

  if (SPIcode  != SPI_OK_CONNECT) {
      elog(ERROR, "tsp: couldn't open a connection to SPI");
      return -1;
  }

  SPIplan = SPI_prepare(sql, 0, NULL);

  if (SPIplan  == NULL) {
      elog(ERROR, "tsp: couldn't create query plan via SPI");
      return finish(SPIcode, -1);
  }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
      elog(ERROR, "tsp: SPI_cursor_open('%s') returns NULL", sql);
      return finish(SPIcode, -1);
  }
    
  DBG("Query: %s",sql);

  while (moredata == TRUE) {
      SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

      if (point_columns.id == -1) {
          if (fetch_point_columns(SPI_tuptable, &point_columns) == -1)
	      return finish(SPIcode, ret);
      }

      ntuples = SPI_processed;

      total_tuples += ntuples;

      //DBG("Tuples: %i", total_tuples);

      if (!points)
        points = palloc(total_tuples * sizeof(point_t));
      else
        points = repalloc(points, total_tuples * sizeof(point_t));
                                        
      if (points == NULL) {
          elog(ERROR, "Out of memory");
	      return finish(SPIcode, ret);
      }

      if (ntuples > 0) {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          //DBG("Got tuple desc");
                
          for (t = 0; t < ntuples; t++) {
              HeapTuple tuple = tuptable->vals[t];
              fetch_point(&tuple, &tupdesc, &point_columns, 
                          &points[total_tuples - ntuples + t]);
          }

          SPI_freetuptable(tuptable);
      }
      else {
          moredata = FALSE;
      }                                       
  }

  
  DBG("Calling TSP");
        
  profstop("extract", prof_extract);
  profstart(prof_tsp);

  DBG("Total tuples: %i", total_tuples);

  for(tt=0; tt < total_tuples; tt++) {
      ids[tt] = points[tt].id;
      x[tt] = points[tt].x;
      y[tt] = points[tt].y;
  
      //DBG("Point at %i: %i [%f, %f]",  tt, ids[tt], x[tt], y[tt]);
            
      for(cc=0; cc < total_tuples; cc++) {
          dx = x[tt] - x[cc];
          dy = y[tt] - y[cc];
          D(tt, cc) = D(cc, tt) = sqrt(dx*dx + dy*dy);
      }
  }

  DBG("DISTANCE computed");
  pfree(points);
    
  ret = find_tsp_solution(total_tuples, DISTANCE, ids, 
                          source, &fit, err_msg);

  if (ret < 0) {
      elog(ERROR, "Error computing path: %s", err_msg);
      return finish(SPIcode, ret);
  }

  for(tt=0; tt < total_tuples; tt++) {
      ((path_element_t*)(*path))[tt].vertex_id = ids[tt];
      ((path_element_t*)(*path))[tt].edge_id   = 0;
      ((path_element_t*)(*path))[tt].cost      = 0;
  }
    
  DBG("TSP solved!");
  DBG("Score: %f", fit);

  profstop("tsp", prof_tsp);
  profstart(prof_store);

  DBG("Profile changed and ret is %i", ret);

  if (ret < 0) {
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
  path_element_t      *path;
    
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
        


      ret = solve_tsp(text2char(PG_GETARG_TEXT_P(0)),
                      text2char(PG_GETARG_TEXT_P(1)),
                      PG_GETARG_INT32(2),
                      &path);

      /* total number of tuples to be returned */
      DBG("Counting tuples number");

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

  DBG("Trying to allocate some memory");
  DBG("call_cntr = %i, max_calls = %i", call_cntr, max_calls);

  if (call_cntr < max_calls)    /* do when there is more left to send */
    {
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;

      /* This will work for some compilers. If it crashes with segfault,
         try to change the following block with this _USE_4 defined */

//#define _USE_4
#ifdef _USE_4 
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
#else
      values = palloc(3 * sizeof(Datum));
      nulls = palloc(3 * sizeof(char));

      values[0] = Int32GetDatum(path[call_cntr].vertex_id);
      nulls[0] = ' ';
      values[1] = Int32GetDatum(path[call_cntr].edge_id);
      nulls[1] = ' ';
      values[2] = Float8GetDatum(path[call_cntr].cost);
      nulls[2] = ' ';
#endif 

      DBG("Heap making");

      tuple = heap_formtuple(tuple_desc, values, nulls);

      DBG("Datum making");

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      DBG("VAL: %i, %i", values[0], result);
      DBG("Trying to free some memory");
    
      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);
        

      SRF_RETURN_NEXT(funcctx, result);
    }
  else    /* do when there is no more left */
    {
      DBG("Ending function");
      profstop("store", prof_store);
      profstop("total", prof_total);
      DBG("Profiles stopped");

      SRF_RETURN_DONE(funcctx);
    }
}
