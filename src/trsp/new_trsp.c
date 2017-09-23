/*PGR-GNU*****************************************************************

File: trsp.c

Generated with Template by:
Copyright (c) 2013 pgRouting developers
Mail: project@pgrouting.org

------

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

********************************************************************PGR-GNU*/


#include "c_common/postgres_connection.h"
#include "catalog/pg_type.h"
#include "c_common/debug_macro.h"


#include "drivers/trsp/trsp_driver.h"
#include "c_types/trsp_types.h"
#include "c_types/pgr_edge_t.h"
#include "c_common/edges_input.h"


PGDLLEXPORT Datum turn_restrict_shortest_path_vertex(PG_FUNCTION_ARGS);


typedef struct edge_columns {
  int id;
  int source;
  int target;
  int cost;
  int reverse_cost;
} edge_columns_t;

typedef struct restrict_columns {
  int target_id;
  int via_path;
  int to_cost;
} restrict_columns_t;





/*
 * This function fetches the resturction columns from an SPITupleTable..
 *
*/
static int
fetch_restrict_columns(SPITupleTable *tuptable,
                       restrict_columns_t *restrict_columns) {
  restrict_columns->target_id = SPI_fnumber(tuptable->tupdesc, "target_id");
  restrict_columns->via_path = SPI_fnumber(tuptable->tupdesc, "via_path");
  restrict_columns->to_cost =  SPI_fnumber(tuptable->tupdesc, "to_cost");
  if (restrict_columns->target_id == SPI_ERROR_NOATTRIBUTE ||
      restrict_columns->via_path == SPI_ERROR_NOATTRIBUTE ||
      restrict_columns->to_cost == SPI_ERROR_NOATTRIBUTE) {
    elog(ERROR, "Error, restriction query must return columns "
        "'target_id', 'via_path' and 'to_cost'");
    return -1;
  }

  if (SPI_gettypeid(tuptable->tupdesc,
       restrict_columns->target_id) != INT4OID ||
     SPI_gettypeid(tuptable->tupdesc, restrict_columns->via_path) != TEXTOID ||
     SPI_gettypeid(tuptable->tupdesc, restrict_columns->to_cost) != FLOAT8OID) {
    elog(ERROR, "Error, restriction columns 'target_id' must be of type int4,"
     "'via_path' must be of type text, 'to_cost' must be of type float8");
    return -1;
  }

  return 0;
}

/*
 * To fetch a edge from Tuple.
 *
 */

static void
fetch_restrict(HeapTuple *tuple, TupleDesc *tupdesc,
           restrict_columns_t *restrict_columns, restrict_t *rest) {
  Datum binval;
  bool isnull;
  int t;
  int MAX_RULE_LENGTH = 5;

  for (t = 0; t < MAX_RULE_LENGTH; ++t)
    rest->via[t] = -1;

  binval = SPI_getbinval(*tuple, *tupdesc, restrict_columns->target_id,
       &isnull);
  if (isnull)
    elog(ERROR, "target_id contains a null value");
  rest->target_id = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, restrict_columns->to_cost, &isnull);
  if (isnull)
    elog(ERROR, "to_cost contains a null value");
  rest->to_cost = DatumGetFloat8(binval);
  char *str = DatumGetCString(SPI_getvalue(*tuple, *tupdesc,
       restrict_columns->via_path));

  // PGR_DBG("restriction: %f, %i, %s", rest->to_cost, rest->target_id, str);

  if (str != NULL) {
    char* pch = NULL;
    int ci = 0;

    pch = (char *)strtok(str, " ,");

    while (pch != NULL && ci < MAX_RULE_LENGTH) {
      rest->via[ci] = atoi(pch);
      // PGR_DBG("    rest->via[%i]=%i", ci, rest->via[ci]);
      ci++;
      pch = (char *)strtok(NULL, " ,");
    }
  }
}



static
void compute_trsp(
        char* edges_sql,
        int64_t start_id,
        int64_t end_id,
        bool directed,
        bool has_reverse_cost,
        char* restrict_sql,
        path_element_tt **path,
        size_t *path_count) {
    pgr_SPI_connect();

    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;
    pgr_get_edges(edges_sql, &edges, &total_edges);


    PGR_DBG("Fetching restriction tuples\n");

    SPIPlanPtr SPIplan;
    Portal SPIportal;
    bool moredata = TRUE;
    uint32_t TUPLIMIT = 1000;
    char *err_msg;
    int ret = -1;
    uint32_t ntuples;
    uint32_t total_restrict_tuples = 0;
    restrict_t *restricts = NULL;
    restrict_columns_t restrict_columns = {.target_id = -1, .via_path = -1,
        .to_cost = -1};
    if (restrict_sql == NULL) {
        PGR_DBG("Sql for restrictions is null.");
    } else {
        SPIplan = SPI_prepare(restrict_sql, 0, NULL);
        if (SPIplan  == NULL) {
            elog(ERROR, "turn_restrict_shortest_path: "
                    "couldn't create query plan via SPI");
            return;
        }

        if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) \
                == NULL) {
            elog(ERROR, "turn_restrict_shortest_path:"
                    " SPI_cursor_open('%s') returns NULL", restrict_sql);
            return;
        }

        moredata = TRUE;
        while (moredata == TRUE) {
            SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

            if (restrict_columns.target_id == -1) {
                if (fetch_restrict_columns(SPI_tuptable, &restrict_columns) \
                        == -1) {
                    PGR_DBG("fetch_restrict_columns failed!");
                    pgr_SPI_finish();
                }
            }

            ntuples = SPI_processed;
            total_restrict_tuples += ntuples;

            // DBG("Reading Restrictions: %i", total_restrict_tuples);

            if (ntuples > 0) {
                if (!restricts)
                    restricts = palloc(total_restrict_tuples * sizeof(restrict_t));
                else
                    restricts = repalloc(restricts,
                            total_restrict_tuples * sizeof(restrict_t));

                if (restricts == NULL) {
                    elog(ERROR, "Out of memory");
                    pgr_SPI_finish();
                }

                uint32_t t;
                SPITupleTable *tuptable = SPI_tuptable;
                TupleDesc tupdesc = SPI_tuptable->tupdesc;

                for (t = 0; t < ntuples; t++) {
                    HeapTuple tuple = tuptable->vals[t];
                    fetch_restrict(&tuple, &tupdesc, &restrict_columns,
                            &restricts[total_restrict_tuples - ntuples + t]);
                }
                SPI_freetuptable(tuptable);
            } else {
                moredata = FALSE;
            }
        }
        SPI_cursor_close(SPIportal);
    }

#ifdef DEBUG_OFF
    int t;
    for (t=0; t < total_restrict_tuples; t++) {
        PGR_DBG("restricts: %.2f, %i, %i, %i, %i, %i, %i",
                restricts[t].to_cost, restricts[t].target_id, restricts[t].via[0],
                restricts[t].via[1], restricts[t].via[2], restricts[t].via[3],
                restricts[t].via[4]);
    }
#endif

    PGR_DBG("Total %i restriction tuples", total_restrict_tuples);

    *path = NULL;
    PGR_DBG("Calling trsp_node_wrapper\n");

    ret = trsp_node_wrapper(edges, total_edges,
            restricts, total_restrict_tuples,
            start_id, end_id,
            directed, has_reverse_cost,
            path, path_count, &err_msg);

    PGR_DBG("Message received from inside:");
    PGR_DBG("%s", err_msg);

    PGR_DBG("ret = %i\n", ret);

    PGR_DBG("*path_count = %ld\n", *path_count);

    if (ret < 0) {
        // elog(ERROR, "Error computing path: %s", err_msg);
        ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED),
                    errmsg("Error computing path: %s", err_msg)));
    }

    pgr_SPI_finish();
}



PG_FUNCTION_INFO_V1(turn_restrict_shortest_path_vertex);
PGDLLEXPORT Datum
turn_restrict_shortest_path_vertex(PG_FUNCTION_ARGS) {
    FuncCallContext     *funcctx;
    TupleDesc            tuple_desc;
    path_element_tt      *path;
    char *               sql;


    // stuff done only on the first call of the function
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;
        size_t path_count = 0;

        int ret = -1;
        if (ret == -1) {}  // to avoid warning set but not used

        int i;

        // create a function context for cross-call persistence
        funcctx = SRF_FIRSTCALL_INIT();

        // switch to memory context appropriate for multiple function calls
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

        // verify that the first 5 args are not NULL
        for (i = 0; i < 5; i++)
            if (PG_ARGISNULL(i)) {
                elog(ERROR, "turn_restrict_shortest_path(): "
                        "Argument %i may not be NULL", i+1);
            }

        if (PG_ARGISNULL(5)) {
            sql = NULL;
        } else {
            sql = text_to_cstring(PG_GETARG_TEXT_P(5));
            if (strlen(sql) == 0)
                sql = NULL;
        }

        PGR_DBG("Calling compute_trsp");


        compute_trsp(text_to_cstring(PG_GETARG_TEXT_P(0)),
                PG_GETARG_INT32(1),
                PG_GETARG_INT32(2),
                PG_GETARG_BOOL(3),
                PG_GETARG_BOOL(4),
                sql,
                &path, &path_count);
#ifdef DEBUG
        double total_cost = 0;
        PGR_DBG("Ret is %i", ret);
        if (ret >= 0) {
            int i;
            for (i = 0; i < path_count; i++) {
                // PGR_DBG("Step %i vertex_id  %i ", i, path[i].vertex_id);
                // PGR_DBG("        edge_id    %i ", path[i].edge_id);
                // PGR_DBG("        cost       %f ", path[i].cost);
                total_cost += path[i].cost;
            }
        }
        PGR_DBG("Total cost is: %f", total_cost);
#endif

        // total number of tuples to be returned
#if 1
#if PGSQL_VERSION > 95
        funcctx->max_calls = path_count;
#else
        funcctx->max_calls = (uint32_t)path_count;
#endif
#else
        funcctx->max_calls = path_count;
#endif
        funcctx->user_fctx = path;

        funcctx->tuple_desc =
            BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

        MemoryContextSwitchTo(oldcontext);
    }

    // stuff done on every call of the function
    funcctx = SRF_PERCALL_SETUP();

    tuple_desc = funcctx->tuple_desc;
    path = (path_element_tt*) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        // do when there is more left to send
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        bool* nulls;

        values = palloc(4 * sizeof(Datum));
        nulls = palloc(4 * sizeof(char));

        values[0] = Int32GetDatum(funcctx->call_cntr);
        nulls[0] = false;
        values[1] = Int32GetDatum(path[funcctx->call_cntr].vertex_id);
        nulls[1] = false;
        values[2] = Int32GetDatum(path[funcctx->call_cntr].edge_id);
        nulls[2] = false;
        values[3] = Float8GetDatum(path[funcctx->call_cntr].cost);
        nulls[3] = false;

        tuple = heap_form_tuple(tuple_desc, values, nulls);

        // make the tuple into a datum
        result = HeapTupleGetDatum(tuple);

        // clean up (this is not really necessary)
        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {  // do when there is no more left
        PGR_DBG("Going to free path");
        if (path) free(path);
        SRF_RETURN_DONE(funcctx);
    }
}

