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
#include "./dijkstraViaVertices_driver.h"

#ifndef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


/*******

USE THIS AS A GUIDE

*****/

//TODO change name, parameters, according to the function its being implemented
static int compute_pgr_dijkstra_via_vertices (
	char* sql,
	int64_t *via_vertex,
	int v_len,
        bool directed,
        bool has_rcost,
        pgr_path_element3_t **path,
	int *path_count) {

  int SPIcode = 0;
  pgr_edge_t *edges = NULL;
  int64_t total_tuples = 0;


  char *err_msg = (char *)"";
  int ret = -1;

  PGR_DBG("Load edges");
  int readCode = pgr_get_data(sql, &edges, &total_tuples, has_rcost,
               -1, -1);

  if (readCode == -1 || total_tuples == 0) {
    *path = noPathFound3(-1, path_count, (*path));
    PGR_DBG("No edge tuples found");
    pfree(edges);
    return pgr_finish(SPIcode, ret);
  }

  if (total_tuples == 1 
     && (edges[0].cost < 0 && edges[0].reverse_cost < 0)) {
    PGR_DBG("One edge with cost == %f and reverse_cost == %f", edges[0].cost, edges[0].reverse_cost );
    *path = noPathFound3(-1, path_count, (*path));
    pfree(edges);
    return pgr_finish(SPIcode, ret);
  }

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


  PGR_DBG("Total %ld edges in query:", total_tuples);

  ret = do_pgr_dijkstra_via_vertices(
	edges, total_tuples,
        via_vertex, v_len,
        directed,
	//return data
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


#ifndef _MSC_VER
Datum pgr_dijkstra_via_vertices(PG_FUNCTION_ARGS);
#else  // _MSC_VER
PGDLLEXPORT Datum pgr_dijkstra_via_vertices(PG_FUNCTION_ARGS);
#endif  // _MSC_VER


PG_FUNCTION_INFO_V1(pgr_dijkstra_via_vertices);


#ifndef _MSC_VER
Datum
#else  // _MSC_VER
PGDLLEXPORT Datum
#endif
pgr_dijkstra_via_vertices(PG_FUNCTION_ARGS) {
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  pgr_path_element3_t  *ret_path = 0;  //TODO change to appropiate type I think this is the appropiate

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      int path_count = 0;

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      int64_t* verticesArr;
      int num;

      PGR_DBG("Load array of via vertices");
      verticesArr = (int64_t*) pgr_get_bigIntArray(&num, PG_GETARG_ARRAYTYPE_P(1));
      PGR_DBG("verticesArr size %d ", num);

      /* TODO change the name to match the one above
         the types of the parameters
      */
      compute_pgr_dijkstra_via_vertices(pgr_text2char(
				 PG_GETARG_TEXT_P(0)), //the sql
				 verticesArr, num,
                                 PG_GETARG_BOOL(2),    // directed
                                 PG_GETARG_BOOL(3),   // has_rcost
				 // the return values  and count
				 &ret_path,
				 &path_count);

      /* total number of tuples to be returned */
      funcctx->max_calls = path_count;
      funcctx->user_fctx = ret_path;

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
  ret_path = (pgr_path_element3_t*) funcctx->user_fctx;

  /* do when there is more left to send */
  if (call_cntr < max_calls) {
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;
      values = palloc(7 * sizeof(Datum));
      nulls = palloc(7 * sizeof(char));
      // route_id, seq, from, to cost 
      values[0] = Int32GetDatum(ret_path[call_cntr].seq);
      nulls[0] = ' ';
      values[1] = Int64GetDatum(ret_path[call_cntr].from);
      nulls[1] = ' ';
      values[2] = Int64GetDatum(ret_path[call_cntr].to);
      nulls[2] = ' ';
      values[3] = Int64GetDatum(ret_path[call_cntr].vertex);
      nulls[3] = ' ';
      values[4] = Int64GetDatum(ret_path[call_cntr].edge);
      nulls[4] = ' ';
      values[5] = Float8GetDatum(ret_path[call_cntr].cost);
      nulls[5] = ' ';
      values[6] = Float8GetDatum(ret_path[call_cntr].tot_cost);
      nulls[6] = ' ';


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

