
#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <search.h>

#include "p_astar.h"

//-------------------------------------------------------------------------

/*
 * Define this to have profiling enabled
 */
//#define PROFILE
/*

#ifdef PROFILE
#include <sys/time.h>

struct timeval prof_astar, prof_store, prof_extract, prof_total;
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

*/
//-------------------------------------------------------------------------

Datum pastar(PG_FUNCTION_ARGS);

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

static int
finish(int code, int ret)
{
  code = SPI_finish();
  if (code  != SPI_OK_FINISH ) {
      elog(ERROR,"couldn't disconnect from SPI");
      return -1 ;
  }

  return ret;
}

typedef struct edge_p_astar_columns
{
  int id;
  int source;
  int target;
  int s_pid;
  int t_pid;
  int cost;
  int reverse_cost;
  int s_x;
  int s_y;
  int t_x;
  int t_y;
} edge_p_astar_columns_t;

char table_name[100]="";
static int
fetch_edge_p_astar_columns(SPITupleTable *tuptable,
             edge_p_astar_columns_t *edge_columns,
             bool has_reverse_cost)
{
  edge_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
  edge_columns->source = SPI_fnumber(SPI_tuptable->tupdesc, "source");
  edge_columns->target = SPI_fnumber(SPI_tuptable->tupdesc, "target");
  edge_columns->cost = SPI_fnumber(SPI_tuptable->tupdesc, "cost");
  edge_columns->s_pid = SPI_fnumber(SPI_tuptable->tupdesc, "s_pid");
  edge_columns->t_pid = SPI_fnumber(SPI_tuptable->tupdesc, "t_pid");
  
  if (edge_columns->id == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->source == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->target == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->s_pid == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->t_pid== SPI_ERROR_NOATTRIBUTE ||
      edge_columns->cost == SPI_ERROR_NOATTRIBUTE) {

      elog(ERROR, "Error, query must return columns "
            "'id', 'source', 'target','s_pid','t_pid' and 'cost'");
      return -1;
  }

  if (SPI_gettypeid(SPI_tuptable->tupdesc,edge_columns->source) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->target) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->s_pid) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->t_pid) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->cost) != FLOAT8OID) {

      elog(ERROR, "Error, columns 'source', 'target','s_pid' ,'t_pid' must be of type int4, "
            "'cost' must be of type float8");
      return -1;
    }

  DBG("columns: id %i source %i target %i cost %i s_pid %i t_pid %i",
      edge_columns->id, edge_columns->source,
      edge_columns->target, edge_columns->cost
      edge_columns->s_pid,edge_columns->t_pid);

  if (has_reverse_cost) {
      edge_columns->reverse_cost = SPI_fnumber(SPI_tuptable->tupdesc,
                           "reverse_cost");

      if (edge_columns->reverse_cost == SPI_ERROR_NOATTRIBUTE) {
          elog(ERROR, "Error, reverse_cost is used, but query did't return "
               "'reverse_cost' column");
          return -1;
      }

      if (SPI_gettypeid(SPI_tuptable->tupdesc,
            edge_columns->reverse_cost) != FLOAT8OID) {
          elog(ERROR, "Error, columns 'reverse_cost' must be of type float8");
          return -1;
      }

      DBG("columns: reverse_cost cost %i", edge_columns->reverse_cost);
  }

  edge_columns->s_x = SPI_fnumber(SPI_tuptable->tupdesc, "x1");
  edge_columns->s_y = SPI_fnumber(SPI_tuptable->tupdesc, "y1");
  edge_columns->t_x = SPI_fnumber(SPI_tuptable->tupdesc, "x2");
  edge_columns->t_y = SPI_fnumber(SPI_tuptable->tupdesc, "y2");

  if (edge_columns->s_x == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->s_y == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->t_x == SPI_ERROR_NOATTRIBUTE ||
      edge_columns->t_y == SPI_ERROR_NOATTRIBUTE) {

      elog(ERROR, "Error, query must return columns "
       "'x1', 'x2', 'y1' and 'y2'");
      return -1;
  }

  DBG("columns: x1 %i y1 %i x2 %i y2 %i",
      edge_columns->s_x, edge_columns->s_y,
      edge_columns->t_x,edge_columns->t_y);

  return 0;
}

static void
fetch_edge_p_astar(HeapTuple *tuple, TupleDesc *tupdesc,
         edge_p_astar_columns_t *edge_columns,
         edge_p_astar_t *target_edge)
{
  Datum binval;
  bool isnull;

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->id, &isnull);
  if (isnull) elog(ERROR, "id contains a null value");
  target_edge->id = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->source, &isnull);
  if (isnull) elog(ERROR, "source contains a null value");
  target_edge->source = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->target, &isnull);
  if (isnull) elog(ERROR, "target contains a null value");
  target_edge->target = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->s_pid, &isnull);
  if (isnull) elog(ERROR, "s_pid contains a null value");
  target_edge->s_pid = DatumGetInt32(binval);
  
  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->t_pid, &isnull);
  if (isnull) elog(ERROR, "t_pid contains a null value");
  target_edge->t_pid = DatumGetInt32(binval);
  
  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->cost, &isnull);
  if (isnull) elog(ERROR, "cost contains a null value");
  target_edge->cost = DatumGetFloat8(binval);

  if (edge_columns->reverse_cost != -1) {
      binval = SPI_getbinval(*tuple, *tupdesc,
                 edge_columns->reverse_cost, &isnull);
      if (isnull)
        elog(ERROR, "reverse_cost contains a null value");
      target_edge->reverse_cost =  DatumGetFloat8(binval);
  }

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->s_x, &isnull);
  if (isnull) elog(ERROR, "source x contains a null value");
  target_edge->s_x = DatumGetFloat8(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->s_y, &isnull);
  if (isnull) elog(ERROR, "source y contains a null value");
  target_edge->s_y = DatumGetFloat8(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->t_x, &isnull);
  if (isnull) elog(ERROR, "target x contains a null value");
  target_edge->t_x = DatumGetFloat8(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->t_y, &isnull);
  if (isnull) elog(ERROR, "target y contains a null value");
  target_edge->t_y = DatumGetFloat8(binval);
}


edge_p_astar_t *
fetch_partition_edges(int pid,int *ptuples,bool has_reverse_cost)
{
  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  int ntuples;
  edge_p_astar_t *edges = NULL;
  int total_tuples = 0;


  edge_p_astar_columns_t edge_columns = {.id= -1, .source= -1, .target= -1,.s_pid=-1, .t_pid=-1,
                       .cost= -1, .reverse_cost= -1,
                       .s_x= -1, .s_y= -1, .t_x= -1, .t_y= -1};

  char sql[300]=""; 
  
  if(has_reverse_cost)
  {

       strcpy(sql,"select id,source,target,s_pid,t_pid,cost,reverse_cost,x1,y1,x2,y2 from ");
       strcat(sql,table_name);
       char abc[150]="";
       sprintf (abc, " where s_pid=%d or t_pid=%d",pid,pid);
       strcat(sql,abc); 
  }
  
  else
  {
       strcpy(sql,"select id,source,target,s_pid,t_pid,cost,x1,y1,x2,y2 from ");
       strcat(sql,table_name);
       char abc[150]="";
       sprintf (abc, " where s_pid=%d or t_pid=%d",pid,pid);
       strcat(sql,abc); 
 }


  
  SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT) {
      elog(ERROR, "shortest_path_p_astar: couldn't open a connection to SPI");
      return edges;
  }

  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL) {
      elog(ERROR, "shortest_path_p_astar: couldn't create query plan via SPI");
      return edges;
  }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
      elog(ERROR, "shortest_path_p_astar: SPI_cursor_open('%s') returns NULL",
            sql);
      return edges;
  }

  while (moredata == TRUE) {
      SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

      if (edge_columns.id == -1) {
          if (fetch_edge_p_astar_columns(SPI_tuptable, &edge_columns,
                           has_reverse_cost) == -1)
            return edges;
      }

      ntuples = SPI_processed;
      total_tuples += ntuples;
      if (!edges)
        edges = (edge_p_astar_t *)malloc(total_tuples * sizeof(edge_p_astar_t));
      else
        edges = (edge_p_astar_t *)realloc(edges, total_tuples * sizeof(edge_p_astar_t));

      if (edges == NULL) {
          elog(ERROR, "Out of memory");
          return edges;
      }

      if (ntuples > 0) {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          for (t = 0; t < ntuples; t++) {
              HeapTuple tuple = tuptable->vals[t];
              fetch_edge_p_astar(&tuple, &tupdesc, &edge_columns,
                       &edges[total_tuples - ntuples + t]);
          }
          SPI_freetuptable(tuptable);
      }
      else {
          moredata = FALSE;
      }
  }


  DBG("Total %i tuples", total_tuples);


//  profstop("extract", prof_extract);
//  profstart(prof_astar);

  SPIcode=SPI_finish();
   if (SPIcode  != SPI_OK_FINISH ) {
               elog(ERROR,"couldn't disconnect from SPI");
	       return edges;}

      *ptuples=total_tuples;
      return edges;



//  profstop("astar", prof_astar);
//  profstart(prof_store);

}


PG_FUNCTION_INFO_V1(pastar);
Datum
pastar(PG_FUNCTION_ARGS)
{
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  path_element_t      *path;
  char *err_msg;

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      int path_count = 0;
      int ret;

      // XXX profiling messages are not thread safe
    //  profstart(prof_total);
     // profstart(prof_extract);

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


      strcpy(table_name,text2char(PG_GETARG_TEXT_P(0)));

      ret = p_astar_wrapper(PG_GETARG_INT32(1),
                    PG_GETARG_INT32(3),
                    PG_GETARG_INT32(2),
		    PG_GETARG_INT32(4),
                    PG_GETARG_BOOL(5),
                    &path, &path_count,&err_msg);

#ifdef DEBUG
      DBG("Ret is %i", ret);
      if (ret >= 0) {
          int i;
          for (i = 0; i < path_count; i++) {
              DBG("Step # %i vertex_id  %i ", i, path[i].vertex_id);
              DBG("        edge_id    %i ", path[i].edge_id);
              DBG("        cost       %f ", path[i].cost);
          }
      }
#endif

      /* total number of tuples to be returned */
      DBG("Conting tuples number\n");
      funcctx->max_calls = path_count;
      funcctx->user_fctx = path;

      DBG("Path count %i", path_count);

      funcctx->tuple_desc =
            BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

      MemoryContextSwitchTo(oldcontext);
  }

  /* stuff done on every call of the function */
  DBG("Strange stuff doing\n");

  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  path = (path_element_t*) funcctx->user_fctx;

  DBG("Trying to allocate some memory\n");

  if (call_cntr < max_calls) {   /* do when there is more left to send */
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;

      values = palloc(4 * sizeof(Datum));
      nulls = palloc(4 * sizeof(char));

      values[0] = Int32GetDatum(call_cntr);
      nulls[0] = ' ';
      values[1] = Int32GetDatum(path[call_cntr].vertex_id);
      nulls[1] = ' ';
      values[2] = Int32GetDatum(path[call_cntr].edge_id);
      nulls[2] = ' ';
      values[3] = Float8GetDatum(path[call_cntr].cost);
      nulls[3] = ' ';

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
  else {   /* do when there is no more left */
      DBG("Freeing path");
      if (path) free(path);

/*      profstop("store", prof_store);
//      profstop("total", prof_total);
#ifdef PROFILE
      elog(NOTICE, "_________");
#endif
*/
      SRF_RETURN_DONE(funcctx);
  }
}
