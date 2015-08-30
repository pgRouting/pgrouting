/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/


// #define DEBUG 1
#include <unistd.h>

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#include "fmgr.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "./../../common/src/pgr_types.h"
#include "./../../common/src/postgres_connection.h"
#include "./ksp.h"
#include "./ksp_driver.h"

#ifndef _MSC_VER
Datum kshortest_path(PG_FUNCTION_ARGS);
#else  // _MSC_VER
PGDLLEXPORT Datum kshortest_path(PG_FUNCTION_ARGS);
#endif  // _MSC_VER


#ifndef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

PG_FUNCTION_INFO_V1(kshortest_path);
#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif  // _MSC_VER
kshortest_path(PG_FUNCTION_ARGS) {
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  pgr_path_element3_t      *path;
//  void * toDel;

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      int path_count = 0;
      path = NULL;

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


      compute(
              pgr_text2char(PG_GETARG_TEXT_P(0)), /* SQL  */
              PG_GETARG_INT64(1),   /* source id */
              PG_GETARG_INT64(2),   /* target_id */
              PG_GETARG_INT32(3),   /* number of paths */
              PG_GETARG_BOOL(4),    /* has reverse_cost */
              PG_GETARG_BOOL(5),    /* directed */
              &path,
              &path_count);
//      toDel = path;

      PGR_DBG("Total number of tuples to be returned %i ", path_count);
      PGR_DBG("Return value %i", ret);

      /* total number of tuples to be returned */
      funcctx->max_calls = path_count;
      funcctx->user_fctx = path;

      /* Build a tuple descriptor for our result type */
      if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                            "that cannot accept type record")));

     // funcctx->tuple_desc = BlessTupleDesc(RelationNameGetTupleDesc("__pgr_2i3b2f"));
      funcctx->tuple_desc = tuple_desc;
      MemoryContextSwitchTo(oldcontext);
    }


  /* stuff done on every call of the function */
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  path = (pgr_path_element3_t*) funcctx->user_fctx;

  if (call_cntr < max_calls) {   /* do when there is more left to send */
      HeapTuple    tuple;
      Datum        result;
     /* //Datum values[4];
      //bool nulls[4]; */

      Datum *values;
      bool* nulls;

      values = (Datum *)palloc(7 * sizeof(Datum));
      nulls = (bool *) palloc(7 * sizeof(bool));

      values[0] = Int32GetDatum(call_cntr + 1);
      nulls[0] = false;

      values[1] = Int32GetDatum(path[call_cntr].from);
      nulls[1] = false;

      values[2] = Int32GetDatum(path[call_cntr].seq);
      nulls[2] = false;

      values[3] = Int64GetDatum(path[call_cntr].vertex);
      nulls[3] = false;

      values[4] = Int64GetDatum(path[call_cntr].edge);
      nulls[4] = false;

      values[5] = Float8GetDatum(path[call_cntr].cost);
      nulls[5] = false;

      values[6] = Float8GetDatum(path[call_cntr].tot_cost);
      nulls[6] = false;

      tuple = heap_form_tuple(tuple_desc, values, nulls);

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
  } else {   /* do when there is no more left */
    if (path == (pgr_path_element3_t *)NULL) free(path);
    SRF_RETURN_DONE(funcctx);
  }
}



int compute(char* sql, int64_t start_vertex,
         int64_t end_vertex, int no_paths,
         bool has_rcost, bool directed,
         pgr_path_element3_t **ksp_path, int *path_count) {
  int SPIcode = 0;
  pgr_edge_t *edges = NULL;
  int64_t total_tuples = 0;
  int readCode = 0;

  char *err_msg = (char *)"";
  int ret = -1;

  if (start_vertex == end_vertex) {
    (*path_count) = 0;
    *ksp_path = NULL;
    return  0;
  }

  PGR_DBG("Load data");
  readCode = pgr_get_data(sql, &edges, &total_tuples, has_rcost,
               start_vertex, end_vertex);
  if (readCode == -1) {
    (*path_count) = 0;
    *ksp_path = NULL;
    pfree(edges);
    PGR_DBG("Source or vertex not found from Load data\n");
    return pgr_finish(SPIcode, ret);
  }

  PGR_DBG("Total %ld tuples in query:", total_tuples);
  PGR_DBG("Calling do_pgr_ksp\n");

  ret = do_pgr_ksp(edges, total_tuples,
            start_vertex, end_vertex,
                       no_paths, has_rcost, directed,
                       ksp_path, path_count, &err_msg);

  PGR_DBG("total tuples found %i\n", *path_count);
  PGR_DBG("Exist Status = %i\n", ret);
  PGR_DBG("Returned message = %s\n", err_msg);



  if (ret < 0) {
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
      errmsg("Error computing path: %s", err_msg)));
    }

  pfree(edges);
  return pgr_finish(SPIcode, ret);
}


