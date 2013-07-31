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


char* vertex_table ,edge_table; // declared globally so that they can help in forming sql querries.  

	static char *
text2char(text *in)
{
	char *out = palloc(VARSIZE(in));

	memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
	out[VARSIZE(in) - VARHDRSZ] = '\0';
	return out;
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


// fetch the cloumns 
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

// fetch values 
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



edge_p_astar_t *
fetch_partition_edges(int vertex_id)
{
	
	
	int SPIcode;
	void *SPIplan;
	Portal SPIportal;
	bool moredata = TRUE;
	int ntuples;                                 
	edge_p_astar_t *edges = NULL;
         total_tuples = 0;
                                                                        
	char *sql_1;                  // this sql querry  will fetch the partion id of the vertex node //

	char *sql_2:                  // this will fetch all the edges from that partion//

	edge_p_astar_columns_t edge_columns = {id: -1, source: -1, target: -1,
		cost: -1, s_pid: -1, t_pid: -1};


	DBG("start shortest_path_p_astar\n");

	SPIcode = SPI_connect();
	if (SPIcode  != SPI_OK_CONNECT) {
		elog(ERROR, "shortest_path_p_astar: couldn't open a connection to SPI");
		return edges;
	}

	/* the first querry will be here , it will be a simple querry which will return the partion id from the vertex_table'*/









     /*   Once we fetch the partition id , all the edges that have thier s_pid or t_pid = partion id of the above will be fetched from the other edge data table */




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
			if (fetch_edge_p_astar_columns(SPI_tuptable, &edge_columns) == -1)
				return edges;
		}

		ntuples = SPI_processed;
		total_tuples += ntuples;
		if (!edges)
			edges = palloc(total_tuples * sizeof(edge_p_astar_t));
		else
			edges = repalloc(edges, total_tuples * sizeof(edge_p_astar_t));

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



	SPIcode = SPI_finish();
	if (SPIcode  != SPI_OK_FINISH ) {
		elog(ERROR,"couldn't disconnect from SPI");
		return edges;}
	
	

	return edges;	

}




PG_FUNCTION_INFO_V1(p_astar_shortest_path);
	Datum
p_astar_shortest_path(PG_FUNCTION_ARGS)
{
	int ret ;

	

	*edge_table=text2char(PG_GETARG_TEXT_P(0));
	*vertex_table=text2char(PG_GETARG_TEXT_P(1));
	
	ret=p_astar_wrapper(PG_GETARG_INT32(2),
			PG_GETARG_INT32(3),&err_msg);

        ret =10;                                            // there will be a lot of things going on here , not yet touched it                      
			                                                    
	PG_RETURN_INT32(ret); // just for testing purpose 
}




