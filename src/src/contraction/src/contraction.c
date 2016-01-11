
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*#include "pagc_api.h"
#include "pagc_std_api.h"
#include "std_pg_hash.h"*/
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

static int compute_contracted_graph(char* sql,int level,Edge **final_edges, 
	int *final_edge_count,char **graphName,
	char **edgeString) {
	int SPIcode = 0;
	Edge *edges = NULL;
	int initial_num_edges = 0;

	bool has_rcost=false;
	char *err_msg = (char *)"";
	int ret = -1;
	int initial_num_vertices=0,final_num_vertices=0;

	PGR_DBG("Load data");
	elog(INFO,"Loading data.....");
	int readCode = fetch_data(sql, &edges, &initial_num_edges, has_rcost);

	if (readCode == -1 || initial_num_edges == 0) {
		pfree(edges);
		return finish(SPIcode, ret);
	}
	elog(INFO,"Contracting.....");
	ret = fetch_contracted_graph(edges, &initial_num_vertices,
		&final_num_vertices,initial_num_edges,final_edge_count,level,
		final_edges, graphName,edgeString);
	elog(INFO,"Initial edge count %d",initial_num_edges);
	elog(INFO,"Initial vertex count %d",initial_num_vertices);
	if (ret < 0) {
		ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
			errmsg("Error contracting the graph: %s", err_msg)));
	}

	PGR_DBG("Final records found %d\n", final_num_edges);
	PGR_DBG("Exist Status = %i\n", ret);
	PGR_DBG("Returned message = %s\n", err_msg);

	pfree(edges);
	return finish(SPIcode, ret);
}

Datum pgr_contractgraph(PG_FUNCTION_ARGS);


PG_FUNCTION_INFO_V1(pgr_contractgraph);
Datum
pgr_contractgraph(PG_FUNCTION_ARGS) {

 //int SPIcode = 0;
	Edge *final_edges=NULL;
	 AttInMetadata       *attinmeta;
	int num_vertices,level=-1;
	FuncCallContext     *funcctx;
	int                  call_cntr;
	int                  max_calls;
	TupleDesc            tuple_desc;
	pgr_contracted_blob *ret_value;
   	//Lens *ret_value=NULL;
	int final_num_edges;
	char *graphName;
	char *edgeString;
	//first call of the function
	if (SRF_IS_FIRSTCALL()) {
		MemoryContext   oldcontext;
		funcctx = SRF_FIRSTCALL_INIT();
		oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
		//fetch the edges and construct the graph
		level=PG_GETARG_INT64(1);
		elog(INFO,"level of contraction : %d",level);
		compute_contracted_graph(text2char(PG_GETARG_TEXT_P(0)),level,&final_edges,
			&final_num_edges,&graphName,&edgeString);
		elog(INFO,"graph name : %s",graphName);
		elog(INFO,"Final edge count : %d",final_num_edges);
		//elog(INFO,"edge string %s",edgeString);
		ret_value=(pgr_contracted_blob*)malloc(sizeof(pgr_contracted_blob));
		ret_value->my_contracted_graph_name=graphName;
		ret_value->contracted_blob=edgeString;
		//elog(INFO,"graphname:- %s,blob:- %s",ret_value->my_contracted_graph_name,ret_value->contracted_blob);
		//char 
		//total_tuples=execq(text2char(PG_GETARG_TEXT_P(0)),limit);
		
		//elog(INFO,"Total tuple %d",total_tuples);
		//ret_value->my_contracted_graph_name=graphName;
		//ret_value->contracted_blob=edgeString;
		
			/* total number of tuples to be returned */
		funcctx->max_calls = 1;
		funcctx->user_fctx = ret_value;
		if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
			ereport(ERROR,
				(errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
					errmsg("function returning record called in context "
						"that cannot accept type record")));
		BlessTupleDesc(tuple_desc);
		funcctx->attinmeta = TupleDescGetAttInMetadata(tuple_desc);

		funcctx->tuple_desc = tuple_desc;
			//elog(INFO,"Finished first ");
		MemoryContextSwitchTo(oldcontext);
	}
/* stuff done on every call of the function */
	//elog(INFO,"Starting per call setup");
	funcctx = SRF_PERCALL_SETUP();
	//elog(INFO,"Starting per call setup");
	call_cntr = funcctx->call_cntr;
	max_calls = funcctx->max_calls;
	tuple_desc = funcctx->tuple_desc;
	attinmeta=funcctx->attinmeta;
	ret_value = (pgr_contracted_blob*) funcctx->user_fctx;
 // ret_value = (Lens*) funcctx->user_fctx;
  /* do when there is more left to send */
	if (call_cntr < max_calls) {
  //	elog(INFO,"call_cntr < max_calls");
		HeapTuple tuple;
		Datum  result;
	//	Datum *values;
		char  **values;
		char *nulls;
      //values = palloc(2*sizeof(Datum));
		int k;
		values = (char **) palloc(2 * sizeof(char *));
		for (k=0; k<2; k++) {
			values[k] = NULL;
		}
		nulls = palloc(2*sizeof(char));
		nulls[0]=' ';
		nulls[1]=' ';
		//values[0]=CStringGetDatum(ret_value->my_contracted_graph_name);
		//values[1]=CStringGetDatum(ret_value->contracted_blob);
		values[0] = ret_value->my_contracted_graph_name   ? pstrdup(ret_value->my_contracted_graph_name) : NULL;
		values[1] = ret_value->contracted_blob  ? pstrdup(ret_value->contracted_blob) : NULL;
      //int l1=ret_value->len1;
      //int l2=ret_value->len2;
    //  elog(INFO,"len1:- %d,len2:- %d",l1,l2);
      //values[0]=Int32GetDatum(l1);
      //values[1]=Int32GetDatum(l2);
		//elog(INFO,"graphname:- %s,blob:- %s",values[0],values[1]);
		//tuple = heap_formtuple(tuple_desc, values, nulls);
		tuple = BuildTupleFromCStrings(attinmeta, values);
		result = HeapTupleGetDatum(tuple);
		SRF_RETURN_NEXT(funcctx, result);
	} else {
      /* do when there is no more left */
		SRF_RETURN_DONE(funcctx);
	}
}