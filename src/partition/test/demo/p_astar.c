#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"

#include <stdio.h>
#include <stdlib.h>
#include <search.h> 
#include "p_astar.h"





Datum p_astar_shortest_path(PG_FUNCTION_ARGS);

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
	float8  cost;
	int s_pid;
	int t_pid;
	
	
} edge_p_astar_columns_t;



static int
fetch_edge_p_astar_columns(SPITupleTable *tuptable,
		edge_p_astar_columns_t *edge_columns)
{
	edge_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "gid");
	edge_columns->source = SPI_fnumber(SPI_tuptable->tupdesc, "source");
	edge_columns->target = SPI_fnumber(SPI_tuptable->tupdesc, "target");
	edge_columns->cost = SPI_fnumber(SPI_tuptable->tupdesc, "cost");
	edge_columns->s_pid = SPI_fnumber(SPI_tuptable->tupdesc, "sgid");
	edge_columns->t_pid= SPI_fnumber(SPI_tuptable->tupdesc, "tgid");
	
	if (edge_columns->id == SPI_ERROR_NOATTRIBUTE ||
			edge_columns->source == SPI_ERROR_NOATTRIBUTE ||
			edge_columns->target == SPI_ERROR_NOATTRIBUTE ||
			edge_columns->cost == SPI_ERROR_NOATTRIBUTE ||
			edge_columns->s_pid == SPI_ERROR_NOATTRIBUTE ||
			edge_columns->t_pid == SPI_ERROR_NOATTRIBUTE) 
	{

		elog(ERROR, "Error, query must return columns "
				"'id', 'source', 'target' ,'cost' ,'sgid' and 'tgid' ");
		return -1;
	}

	if (SPI_gettypeid(SPI_tuptable->tupdesc,
				edge_columns->source) != INT4OID ||
			SPI_gettypeid(SPI_tuptable->tupdesc,
				edge_columns->target) != INT4OID ||
			SPI_gettypeid(SPI_tuptable->tupdesc,
				            edge_columns->s_pid) != INT4OID ||
			      SPI_gettypeid(SPI_tuptable->tupdesc,
				                  edge_columns->t_pid) != INT4OID ||
			SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->cost) != FLOAT8OID) {

		elog(ERROR, "Error, columns 'source', 'target', 's_pid','t_pid' must be of type int4, "
				"'cost' must be of type float8");
		return -1;
	}

	DBG("columns: id %i source %i target %i cost %i s_pid %i t_pid %i",
			edge_columns->id, edge_columns->source,
			edge_columns->target, edge_columns->cost,edge_columns->s_pid,edge_columns->t_pid);

	return 0;
}

static void fetch_edge_p_astar(HeapTuple *tuple, TupleDesc *tupdesc,edge_p_astar_columns_t *edge_columns,
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

	binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->cost, &isnull);
	if (isnull) elog(ERROR, "cost contains a null value");
	target_edge->cost = DatumGetFloat8(binval);


	binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->s_pid, &isnull);
	if (isnull) elog(ERROR, "source x contains a null value");
	target_edge->s_pid = DatumGetInt32(binval);

	binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->t_pid, &isnull);
	if (isnull) elog(ERROR, "source y contains a null value");
	target_edge->t_pid = DatumGetInt32(binval);

}


/* This function is called from main function ,the sql querry here is of type " select c1,c2....cn from table where source_partion_id=var
   or target_partion_id=var" */

static int compute_shortest_path_p_astar(char* sql, int source_vertex_id,
		int target_vertex_id)
{
	int SPIcode;
	void *SPIplan;
	Portal SPIportal;
	bool moredata = TRUE;
	int ntuples;                                 
	edge_p_astar_t *edges = NULL;
	int total_tuples = 0;
                                                                        

	edge_p_astar_columns_t edge_columns = {id: -1, source: -1, target: -1,
		cost: -1, s_pid: -1, t_pid: -1};
	int ret = -1;
	register int z;

	int s_count=0;


	DBG("start shortest_path_p_astar\n");

	SPIcode = SPI_connect();
	if (SPIcode  != SPI_OK_CONNECT) {
		elog(ERROR, "shortest_path_p_astar: couldn't open a connection to SPI");
		return -1;
	}

	SPIplan = SPI_prepare(sql, 0, NULL);
	if (SPIplan  == NULL) {
		elog(ERROR, "shortest_path_p_astar: couldn't create query plan via SPI");
		return -1;
	}

	if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
		elog(ERROR, "shortest_path_p_astar: SPI_cursor_open('%s') returns NULL",
				sql);
		return -1;
	}

	while (moredata == TRUE) {
		SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

		if (edge_columns.id == -1) {
			if (fetch_edge_p_astar_columns(SPI_tuptable, &edge_columns) == -1)
				return finish(SPIcode, ret);
		}

		ntuples = SPI_processed;
		total_tuples += ntuples;
		if (!edges)
			edges = palloc(total_tuples * sizeof(edge_p_astar_t));
		else
			edges = repalloc(edges, total_tuples * sizeof(edge_p_astar_t));

		if (edges == NULL) {
			elog(ERROR, "Out of memory");
			return finish(SPIcode, ret);
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



	SPIcode = SPI_finish();
	if (SPIcode  != SPI_OK_FINISH ) {
		elog(ERROR,"couldn't disconnect from SPI");
		return -1 ;}
	
	DBG("Total %i tuples", total_tuples);

	for(z=0; z<total_tuples; z++) {
//check if edges[] contain source ( as we are fetching the edge belonging to a ceretain partition,target might not be in that partion 
		if(edges[z].source == source_vertex_id ||
				edges[z].target == source_vertex_id)
			++s_count;

		DBG("%i - %i", edges[z].source, edges[z].target);
	}

	DBG("Total %i tuples", total_tuples);

	if(s_count == 0) {
		elog(ERROR, "Start vertex was not found.");
		return -1;
	}

	DBG("Total %i tuples", total_tuples);
/*



                   The wrapper function will be called here which will instantiate the solver class 

                   The solver class then will call load_partiton() function again whenever a partiton needs to be loaded.
		  Thats why the SPI_finish() function is called above so that a connection can be set up with the spi_manager in 
		 the load_partition()  

*/                  
	ret=10;
	

	return ret;	

}


PG_FUNCTION_INFO_V1(p_astar_shortest_path);
	Datum
p_astar_shortest_path(PG_FUNCTION_ARGS)
{
	int ret ;

	ret= compute_shortest_path_p_astar(text2char(PG_GETARG_TEXT_P(0)),
			PG_GETARG_INT32(1),
			PG_GETARG_INT32(2));


			PG_RETURN_INT32(ret);


}




