#include "postgres.h"
#include "executor/spi.h"
#include "../../common/src/connection.h"
#include "../../common/src/structs.h"
#include "contract_function.h"
#include "funcapi.h"
#include "utils/lsyscache.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif
#include "utils/builtins.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

Datum contract_graph(PG_FUNCTION_ARGS);


PG_FUNCTION_INFO_V1(contract_graph);
Datum
contract_graph(PG_FUNCTION_ARGS) {
	// (query,level)
	//int SPIcode = 0;
	Edge *edges=NULL,*final_edges=NULL;
	int initial_num_edges,final_num_edges,num_vertices;
	FuncCallContext *funcctx;
	int call_cntr;
	int max_calls;
	int level;
	TupleDesc tuple_desc;
	PathElement *ret_path = 0;
	//first call of the function
	if (SRF_IS_FIRSTCALL()) {
		MemoryContext   oldcontext;
		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
		
		//fetch the edges and construct the graph
		initial_num_edges=fetch_data(text2char(PG_GETARG_TEXT_P(0)),&edges,&num_vertices);
		final_num_edges=initial_num_edges;
		level=PG_GETARG_INT64(1);
		elog(INFO, "INITIAL EDGE COUNT: %d", initial_num_edges);
		elog(INFO, "INITIAL VERTEX COUNT: %d", num_vertices);
		elog(INFO, "LEVEL: %d", level);
		//prints the path if the number of edges > 0
		if (initial_num_edges>0)
		{
			int i;
			final_num_edges=compute_contracted_graph(edges,num_vertices,initial_num_edges,level,&final_edges);
			elog(INFO, "FINAL EDGE COUNT: %d", final_num_edges);
			elog(INFO,"id	|	source	|	target	|	cost	|");
			for (i = 0; i < final_num_edges; ++i)
			{
				elog(INFO,"%d	|	%d	|	%d	|	%f"
					,final_edges[i].id,final_edges[i].source,final_edges[i].target,final_edges[i].cost);
			}
				//printing it as a buffer
	 			//elog(INFO, "EDGES: %s", buf);
		}
		else
		{
			elog(INFO,"ERROR: %s","No tuples found.");
		}

		/* total number of tuples to be returned */
		funcctx->max_calls = final_num_edges;
		funcctx->user_fctx = final_edges;
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