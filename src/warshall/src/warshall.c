/*
 *
 *               2015 Celia Virginia Vergara Castillo
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */




#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./warshall_driver.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

Datum function1(PG_FUNCTION_ARGS);

/*******

USE THIS AS A GUIDE

*****/

//TODO change name, parameters, according to the function its being implemented
static int compute_warshall(
	char* sql,
	bool directed,
	bool has_rcost, // this wil be removed
        pgr_path_element3_t **path, int *path_count) {

  int SPIcode = 0;
  pgr_edge_t *edges = NULL;   // id, source, target, cost, reverse_cost
  int64_t total_tuples = 0;


  char *err_msg = (char *)"";
  int ret = -1;

  PGR_DBG("Load sql data");

  int readCode = pgr_get_data(sql, &edges, &total_tuples, has_rcost,
               -1, -1);

  if (readCode == -1 || total_tuples == 0) {
    *path = noPathFound3(-1, path_count, (*path));
    PGR_DBG("No edge tuples found");
    pfree(edges);
    return pgr_finish(SPIcode, ret);
  }

#if 0
  if (total_tuples == 1 
     && (edges[0].cost < 0 && edges[0].reverse_cost < 0)) {
    PGR_DBG("One edge with cost == %f and reverse_cost == %f", edges[0].cost, edges[0].reverse_cost );
    *path = noPathFound3(-1, path_count, (*path));
    pfree(edges);
    return pgr_finish(SPIcode, ret);
  }
#endif


  if (total_tuples == 1) {
    PGR_DBG("One edge with cost == %f and reverse_cost == %f", edges[0].cost, edges[0].reverse_cost );
    PGR_DBG("The soruce == %ld and target == %ld", edges[0].source, edges[0].target);
#if 0
    if ((edges[0].cost >= 0 && edges[0].source != start_vertex &&  edges[0].target != end_vertex) 
        ||  (edges[0].reverse_cost >= 0 && edges[0].source != end_vertex &&  edges[0].target != start_vertex)) {
      PGR_DBG("There must be a solution or empty for undirected");
    }
    if (edges[0].cost >= 0 && edges[0].source == start_vertex &&  edges[0].target == end_vertex) { 
      PGR_DBG("Solution from source to target");
    }
    if (edges[0].reverse_cost >= 0 && edges[0].target == start_vertex &&  edges[0].source == end_vertex) { 
      PGR_DBG("Solution from target to source");
    }
#endif
  }


  PGR_DBG("Total %ld tuples in query:", total_tuples);

  // TODO the function is defined in the function1_driver.h & .cpp
  // fix accordingly
  ret = do_pgr_warshall(edges, total_tuples,
                        directed,                    // no need for rcost flag
                        path, path_count, &err_msg);

  if (ret < 0) {
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
        errmsg("Error computing path: %s", err_msg)));
  }

  PGR_DBG("total records found %i\n", *path_count);
  PGR_DBG("Exist Status = %i\n", ret);
  PGR_DBG("Returned message = %s\n", err_msg);

  pfree(edges);
  return pgr_finish(SPIcode, ret);
}

/****
change where indicated
****/

PG_FUNCTION_INFO_V1(warshall); 
Datum
warshall(PG_FUNCTION_ARGS) {
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  pgr_path_element3_t  *ret_path = 0;  //TODO change to appropiate type

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      int path_count = 0;

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      /* TODO change the name to match the one above
         the types of the parameters
      */
      compute_warshall(pgr_text2char(PG_GETARG_TEXT_P(0)), // sql
                                  PG_GETARG_BOOL(1),       // directed
                                  PG_GETARG_BOOL(2),       // has_rcost // to be removed
				  &ret_path, &path_count); // the return values

      /* total number of tuples to be returned */
      funcctx->max_calls = path_count;
      funcctx->user_fctx = ret_path;

      funcctx->tuple_desc = BlessTupleDesc(
            RelationNameGetTupleDesc("__pgr_2b1f"));

      MemoryContextSwitchTo(oldcontext);
  }

  /* stuff done on every call of the function */
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  ret_path = (pgr_path_element3_t*) funcctx->user_fctx;

  /* do when there is more left to send */
  if (call_cntr < max_calls) {
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;

      values = palloc(5 * sizeof(Datum));
      nulls = palloc(5 * sizeof(char));
      //TODO  modify depending on stored values and result values
      values[0] = Int32GetDatum(ret_path[call_cntr].seq);
      nulls[0] = ' ';
      values[1] = Int64GetDatum(ret_path[call_cntr].start);
      nulls[1] = ' ';
      values[2] = Int64GetDatum(ret_path[call_cntr].end);
      nulls[2] = ' ';
      values[3] = Float8GetDatum(ret_path[call_cntr].cost);
      nulls[3] = ' ';

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

