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

#ifndef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

Datum function1(PG_FUNCTION_ARGS);

/*******

USE THIS AS A GUIDE

*****/

//TODO change name, parameters, according to the function its being implemented
static int compute_warshall(
	char *sql,
	bool directed,
	bool has_rcost, // this wil be removed
        Matrix_cell_t **matrix,
	size_t *result_tuple_count) {

  int SPIcode = 0;
  pgr_edge_t *edges = NULL;   // id, source, target, cost, reverse_cost
  int64_t total_tuples = 0;


  char *err_msg = (char *)"";
  int ret = -1;

  PGR_DBG("Load sql data");

  int readCode = pgr_get_data(sql, &edges, &total_tuples, has_rcost,
               -1, -1);

  if (readCode == -1 || total_tuples == 0) {
    *matrix = NULL;
    PGR_DBG("No edge tuples found");
    pfree(edges);
    return pgr_finish(SPIcode, ret);
  }



  if (total_tuples == 1) {
    PGR_DBG("One edge with cost == %f and reverse_cost == %f", edges[0].cost, edges[0].reverse_cost );
    PGR_DBG("The soruce == %ld and target == %ld", edges[0].source, edges[0].target);
  }


  PGR_DBG("Total %ld tuples in query:", total_tuples);

  // TODO the function is defined in the function1_driver.h & .cpp
  // fix accordingly
  ret = do_pgr_warshall(edges, 
                        (size_t)total_tuples,
                        directed,                    // no need for rcost flag
                        matrix,
			result_tuple_count,
			&err_msg);

  if (ret < 0) {
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
        errmsg("Error computing path: %s", err_msg)));
  }

  PGR_DBG("total records found %i\n", *result_tuple_count);
  PGR_DBG("Exist Status = %i\n", ret);
  PGR_DBG("Returned message = %s\n", err_msg);

  pfree(edges);
  return pgr_finish(SPIcode, ret);
}

/****
change where indicated
****/

PG_FUNCTION_INFO_V1(pgr_warshall); 
Datum
pgr_warshall(PG_FUNCTION_ARGS) {
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  Matrix_cell_t      *matrix = 0;  //TODO change to appropiate type

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      size_t result_total_count = 0;

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
				  &matrix, &result_total_count); // the return values

      /* total number of tuples to be returned */
      funcctx->max_calls = result_total_count;
      funcctx->user_fctx = matrix;

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
  matrix = (Matrix_cell_t*) funcctx->user_fctx;

  /* do when there is more left to send */
  if (call_cntr < max_calls) {
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;

      values = palloc(5 * sizeof(Datum));
      nulls = palloc(5 * sizeof(char));
      //TODO  modify depending on stored values and result values
      values[0] = Int32GetDatum(call_cntr);
      nulls[0] = ' ';
      values[1] = Int64GetDatum(matrix[call_cntr].from_vid);
      nulls[1] = ' ';
      values[2] = Int64GetDatum(matrix[call_cntr].to_vid);
      nulls[2] = ' ';
      values[3] = Float8GetDatum(matrix[call_cntr].cost);
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
      if (matrix) free(matrix);
      SRF_RETURN_DONE(funcctx);
  }
}

