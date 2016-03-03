#include "postgres.h"
#include "executor/spi.h"
#include "./connection.h"
#include "./../../common/src/postgres_connection.h"

#include "./../../common/src/debug_macro.h"
#include "./structs.h"
#include "dijkstra_function.h"
#include "funcapi.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif
#include "utils/builtins.h"



static int64_t compute_shortest_path(char* sql,int64_t source,int64_t target,
	Edge** path,int64_t *path_count,bool has_rcost) {
	//int SPIcode = 0;
	pgr_contracted_blob *graphInfo = NULL;
	//int initial_num_edges = 0;

	has_rcost=false;
	//char *err_msg = (char *)"";
	//int ret = -1;

	PGR_DBG("Load data");
	elog(INFO,"Fetching contracted graph.....");
	int readCode = get_contracted_graph(sql,&graphInfo);
	if (readCode == -1) {
		pfree(graphInfo);
		//return finish(SPIcode, ret);
		pgr_SPI_finish();
          return -1;
	}
	elog(INFO,"graphName: %s",graphInfo->contracted_graph_name);
	elog(INFO,"graphBlob: %s",graphInfo->contracted_graph_blob);
	elog(INFO,"removedVertices: %s",graphInfo->removedVertices);
	elog(INFO,"removedEdges: %s",graphInfo->removedEdges);
	elog(INFO,"psuedoEdges: %s",graphInfo->psuedoEdges);

	/*ret = dijkstra_on_contracted(source,target,&graphInfo,has_rcost);
	if (ret < 0) {
		ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
			errmsg("Error running dijkstra: %s", err_msg)));
	}*/

	pfree(graphInfo);
	//return finish(SPIcode, ret);
	pgr_SPI_finish();
          return -1;
}








Datum shortest_path_c(PG_FUNCTION_ARGS);



PG_FUNCTION_INFO_V1(shortest_path_c);
Datum
shortest_path_c(PG_FUNCTION_ARGS) {

 //int SPIcode = 0;
	Edge *edges=NULL;
	Edge *path=NULL;
	int64_t path_size=0;
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
		//fetch the edges and construct the graph
		bool has_rcost=false;
		has_rcost=PG_GETARG_BOOL(3);
		int64_t source=PG_GETARG_INT64(1),target=PG_GETARG_INT64(2);
		compute_shortest_path(pgr_text2char(PG_GETARG_TEXT_P(0)),source,target,
			&edges,&path_size,has_rcost);
		
		//prints the path if the number of edges > 0
		if (path_size>0)
		{
			elog(INFO,"Path Length %ld",path_size);
			int64_t i;
			for (i = 0; i < path_size; ++i)
			{
				elog(INFO,"%ld	|	%ld	|	%ld	|	%f",path[i].id,path[i].source,path[i].target,path[i].cost);
			}
			free(edges);
			free(path);
				//printing it as a buffer
	 			//elog(INFO, "EDGES: %s", buf);
		}
		else
		{
			elog(INFO,"ERROR: %s","No tuples found.");
			free(edges);
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
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;
      values = palloc(4 * sizeof(Datum));
      nulls = palloc(4 * sizeof(char));

      values[0] = Int32GetDatum(ret_path[call_cntr].seq);
      nulls[0] = ' ';
      values[1] = Int64GetDatum(ret_path[call_cntr].source);
      nulls[1] = ' ';
      values[2] = Int64GetDatum(ret_path[call_cntr].target);
      nulls[2] = ' ';
      values[3] = Float8GetDatum(ret_path[call_cntr].cost);
      nulls[3] = ' ';
      //values[4] = Float8GetDatum(ret_path[call_cntr].tot_cost);
      //nulls[4] = ' ';

      tuple = heap_formtuple(tuple_desc, values, nulls);

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
  } else {
      /* do when there is no more left */
      if (ret_path) free(ret_path);
      SRF_RETURN_DONE(funcctx);
  }
}