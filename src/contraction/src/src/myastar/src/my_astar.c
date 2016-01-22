#include "postgres.h"
#include "executor/spi.h"
#include "../../common/src/connection.h"
#include "../../../../../common/src/postgres_connection.h"
#include "../../common/src/structs.h"
#include "my_astar.h"
#include "funcapi.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif
#include "utils/builtins.h"
#define TUPLIMIT 1000

/********************
Functions for with sql:
 id, source, target,x1,y1,x2,y2 cost, revcost(optional) 
************/

Datum shortest_path_astar(PG_FUNCTION_ARGS);



PG_FUNCTION_INFO_V1(shortest_path_astar);
Datum
shortest_path_astar(PG_FUNCTION_ARGS) {

 //int SPIcode = 0;
  Edge *edges=NULL;
  Edge *path=NULL;
  int num_edges,num_vertices,path_size=0;
    FuncCallContext     *funcctx;
    int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  PathElement  *ret_path = 0;
  //first call of the function
  if (SRF_IS_FIRSTCALL()) {
    MemoryContext   oldcontext;
    funcctx = SRF_FIRSTCALL_INIT();
    oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
    bool has_rcost=false;
    has_rcost=PG_GETARG_BOOL(3);
    //fetch the edges and construct the graph
    num_edges=fetch_astar_data(pgr_text2char(PG_GETARG_TEXT_P(0)),&edges,&num_vertices,has_rcost);
    int source=PG_GETARG_INT64(1),target=PG_GETARG_INT64(2);
    elog(INFO, "EDGE COUNT: %d", num_edges);
    elog(INFO, "VERTEX COUNT: %d", num_vertices);
    //prints the path if the number of edges > 0
    if (num_edges>0)
    {
      //char buf[8192];
      //buf[0] = 0;
      //elog(INFO, "COUNT: %d", num_edges);
      int i;
      /*for (i = 0; i < num_edges; ++i)
      {
      //stroing it in a buffer
      //snprintf(buf + strlen (buf), sizeof(buf) - strlen(buf), " %d   %d  %s",edges[i].source,edges[i].dest," | ");
        //elog(INFO, "EDGES: %d %d", edges[i].source,edges[i].target);
      }*/
        path_size=compute_astar(source,target,edges,num_vertices,num_edges,&path);
        elog(INFO,"Path Length %d",path_size);
        elog(INFO,"seq  | source  | target  | cost");
        for (i = 0; i < path_size; ++i)
        {
          elog(INFO,"%d | %d  | %d  | %f",path[i].id,path[i].source,path[i].target,path[i].cost);
        }
        free(edges);
        free(path);
        //printing it as a buffer
        //elog(INFO, "EDGES: %s", buf);
      }
      else
      {
        elog(INFO,"ERROR: %s","No tuples found.");
      }

      /* total number of tuples to be returned */
      funcctx->max_calls = path_size;
      funcctx->user_fctx = path;
      if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
        ereport(ERROR,
          (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
            errmsg("function returning record called in context "
              "that cannot accept type record")));

      funcctx->tuple_desc = tuple_desc;

      MemoryContextSwitchTo(oldcontext);

    }
/* stuff done on every call of the function */
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  ret_path = (PathElement*) funcctx->user_fctx;

  /* do when there is more left to send */
  if (call_cntr < max_calls) {

      /* clean up (this is not really necessary) */

      //SRF_RETURN_NEXT(funcctx, result);
  } else {
      /* do when there is no more left */
      if (ret_path) free(ret_path);
      SRF_RETURN_DONE(funcctx);
  }
}