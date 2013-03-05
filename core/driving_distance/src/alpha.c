/*
 * Alpha-Shapes for PostgreSQL
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


#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"

#include "alpha.h"

#include "fmgr.h"


#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/*
 * Define this to have profiling enabled
 */
//#define PROFILE

#ifdef PROFILE
#include <sys/time.h>

/*! \def MAX_NODES
  \brief Maximal number of nodes in the path (to avoid infinite loops)
*/
#define MAX_NODES 1000000

struct timeval prof_alpha, prof_store, prof_extract, prof_total;
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


Datum alphashape(PG_FUNCTION_ARGS);

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
		  

typedef struct vertex_columns 
{
  int id;
  int x;
  int y;

} vertex_columns_t;



static int
fetch_vertices_columns(SPITupleTable *tuptable, 
                       vertex_columns_t *vertex_columns)
{
  vertex_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
  vertex_columns->x = SPI_fnumber(SPI_tuptable->tupdesc, "x");
  vertex_columns->y = SPI_fnumber(SPI_tuptable->tupdesc, "y");

  if (vertex_columns->id == SPI_ERROR_NOATTRIBUTE ||
      vertex_columns->x == SPI_ERROR_NOATTRIBUTE ||
      vertex_columns->y == SPI_ERROR_NOATTRIBUTE) 
    {
      elog(ERROR, "Error, query must return columns "
           "'id', 'x' and 'y'");
      return -1;
    }

  if (SPI_gettypeid(SPI_tuptable->tupdesc, vertex_columns->id) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, vertex_columns->x) != FLOAT8OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, vertex_columns->y) != FLOAT8OID) 
    {
      elog(ERROR, 
           "Error, column 'id' must be of type int4, 'x' and 'y' must be of type float8");
      return -1;
    }
   
  return 0;
}

static void
fetch_vertex(HeapTuple *tuple, TupleDesc *tupdesc, 
             vertex_columns_t *vertex_columns, vertex_t *target_vertex)
{
  Datum binval;
  bool isnull;

  binval = SPI_getbinval(*tuple, *tupdesc, vertex_columns->x, &isnull);
  if (isnull)
    elog(ERROR, "x contains a null value");
  target_vertex->x = DatumGetFloat8(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, vertex_columns->y, &isnull);
  if (isnull)
    elog(ERROR, "y contains a null value");
  target_vertex->y = DatumGetFloat8(binval);
}

static int compute_alpha_shape(char* sql, vertex_t **res, int *res_count) 
{

  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  int ntuples;
  vertex_t *vertices = NULL;
  int total_tuples = 0;
  vertex_columns_t vertex_columns = {id: -1, x: -1, y: -1};
  char *err_msg;
  int ret = -1;

  DBG("start alpha_shape\n");
        
  SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT)
    {
      elog(ERROR, "alpha_shape: couldn't open a connection to SPI");
      return -1;
    }

  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL)
    {
      elog(ERROR, "alpha_shape: couldn't create query plan via SPI");
      return -1;
    }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) 
    {
      elog(ERROR, "alpha_shape: SPI_cursor_open('%s') returns NULL", sql);
      return -1;
    }

  while (moredata == TRUE)
    {
      SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

      if (vertex_columns.id == -1) 
        {
          if (fetch_vertices_columns(SPI_tuptable, &vertex_columns) == -1)
	    return finish(SPIcode, ret);
        }

      ntuples = SPI_processed;
      total_tuples += ntuples;
      if (!vertices)
        vertices = palloc(total_tuples * sizeof(vertex_t));
      else
        vertices = repalloc(vertices, total_tuples * sizeof(vertex_t));

      if (vertices == NULL) 
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
              fetch_vertex(&tuple, &tupdesc, &vertex_columns, 
                           &vertices[total_tuples - ntuples + t]);
            }
          SPI_freetuptable(tuptable);
        } 
      else 
        {
          moredata = FALSE;
        }
    }


  // if (total_tuples < 2) //this was the buggy code of the pgrouting project.
  // TODO: report this as a bug to the pgrouting project
  // the CGAL alpha-shape function crashes if called with less than three points!!!

  if (total_tuples == 0) {
  	  elog(ERROR, "Distance is too short. no vertex for alpha shape calculation. alpha shape calculation needs at least 3 vertices.");
  }
  if (total_tuples == 1) {
	  elog(ERROR, "Distance is too short. only 1 vertex for alpha shape calculation. alpha shape calculation needs at least 3 vertices.");
  }
  if (total_tuples == 2) {
	  elog(ERROR, "Distance is too short. only 2 vertices for alpha shape calculation. alpha shape calculation needs at least 3 vertices.");
  }
  if (total_tuples < 3)
  {
    // elog(ERROR, "Distance is too short ....");
    return finish(SPIcode, ret);
  }

  DBG("Calling CGAL alpha-shape\n");
        
  profstop("extract", prof_extract);
  profstart(prof_alpha);

  ret = alpha_shape(vertices, total_tuples, res, res_count, &err_msg);

  profstop("alpha", prof_alpha);
  profstart(prof_store);

  if (ret < 0)
    {
      //elog(ERROR, "Error computing shape: %s", err_msg);
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), errmsg("Error computing shape: %s", err_msg)));
    } 
  
  return finish(SPIcode, ret);    
}

PG_FUNCTION_INFO_V1(alphashape);

Datum alphashape(PG_FUNCTION_ARGS)
{
  FuncCallContext      *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  vertex_t     *res;
                    
  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL())
    {
      MemoryContext   oldcontext;
      int res_count;
      int ret;
                            
      // XXX profiling messages are not thread safe
      profstart(prof_total);
      profstart(prof_extract);
                                            
      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();
                                                                    
      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      ret = compute_alpha_shape(text2char(PG_GETARG_TEXT_P(0)), 
                                &res, &res_count);

      /* total number of tuples to be returned */
      DBG("Conting tuples number\n");
      funcctx->max_calls = res_count;
      funcctx->user_fctx = res;

      DBG("Total count %i", res_count);

      funcctx->tuple_desc = BlessTupleDesc(
                           RelationNameGetTupleDesc("vertex_result"));

      MemoryContextSwitchTo(oldcontext);
    }

  /* stuff done on every call of the function */
  DBG("Strange stuff doing\n");
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  res = (vertex_t*) funcctx->user_fctx;

  DBG("Trying to allocate some memory\n");

  if (call_cntr < max_calls)    /* do when there is more left to send */
    {
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;

      /* This will work for some compilers. If it crashes with segfault, try to change the following block with this one    

      values = palloc(3 * sizeof(Datum));
      nulls = palloc(3 * sizeof(char));

      values[0] = call_cntr;
      nulls[0] = ' ';
      values[1] = Float8GetDatum(res[call_cntr].x);
      nulls[1] = ' ';
      values[2] = Float8GetDatum(res[call_cntr].y);
      nulls[2] = ' ';
      */
    
      values = palloc(2 * sizeof(Datum));
      nulls = palloc(2 * sizeof(char));

      values[0] = Float8GetDatum(res[call_cntr].x);
      nulls[0] = ' ';
      values[1] = Float8GetDatum(res[call_cntr].y);
      nulls[1] = ' ';
	
      DBG("Heap making\n");

      tuple = heap_formtuple(tuple_desc, values, nulls);

      DBG("Datum making\n");

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      DBG("Trying to free some memory\n");

      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
    }
  else    /* do when there is no more left */
    {
      profstop("store", prof_store);
      profstop("total", prof_total);
#ifdef PROFILE
      elog(NOTICE, "_________");
#endif
      SRF_RETURN_DONE(funcctx);
    }
}
