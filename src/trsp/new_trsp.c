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
#include "c_types/general_path_element_t.h"
#include "c_common/edges_input.h"
#include "c_common/restrictions_input.h"


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
        char* restrictions_sql,

        int64_t start_id,
        int64_t end_id,
        bool directed,
        bool has_reverse_cost,
        char* restrict_sql,
        General_path_element_t **path,
        size_t *path_count) {
    pgr_SPI_connect();

    pgr_edge_t *edges = NULL;
    size_t total_edges = 0;
    pgr_get_edges(edges_sql, &edges, &total_edges);

    Restriction_t * restrictions = NULL;
    size_t total_restrictions = 0;
    if (!(restrictions_sql == NULL) ) {

        PGR_DBG("GOT THE NEW RESTRICTION QUERY");
        pgr_get_restrictions(restrictions_sql, &restrictions, &total_restrictions);
        PGR_DBG("total restrictions read %d", total_restrictions);

        for (uint64_t i = 0; i < total_restrictions; ++i) {
            PGR_DBG("%d: restriction[%d], %d", i, restrictions[i].id, restrictions[i].cost);
            PGR_DBG("    edges %d", restrictions[i].via_size);

            for (uint64_t j = 0; j < restrictions[i].via_size; ++j) {
                PGR_DBG("edge %d", restrictions[i].via[j]);
            }
        }
    }



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

    ret = do_trsp(
            edges, total_edges,
            restrictions, total_restrictions,

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

    size_t result_count             = 0;
    General_path_element_t  *result_tuples   = NULL;

    // stuff done only on the first call of the function
    if (SRF_IS_FIRSTCALL()) {
        MemoryContext   oldcontext;

#if 0
        int ret = -1;
        if (ret == -1) {}  // to avoid warning set but not used

#endif
        // create a function context for cross-call persistence
        funcctx = SRF_FIRSTCALL_INIT();

        // switch to memory context appropriate for multiple function calls
        oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

#if 0
        char *  restrictions_sql = NULL;
        if (PG_ARGISNULL(6)) {
            restrictions_sql = NULL;
        } else {
            restrictions_sql = text_to_cstring(PG_GETARG_TEXT_P(6));
            if (strlen(restrictions_sql) == 0)
                restrictions_sql = NULL;
        }
#endif

        PGR_DBG("Calling compute_trsp");


        compute_trsp(
                text_to_cstring(PG_GETARG_TEXT_P(0)),
                text_to_cstring(PG_GETARG_TEXT_P(1)),
                PG_GETARG_INT64(2),
                PG_GETARG_INT64(3),
                PG_GETARG_BOOL(4),
                PG_GETARG_BOOL(5),
                text_to_cstring(PG_GETARG_TEXT_P(6)),
                &result_tuples, &result_count);

        //-----------------------------------------------

#if PGSQL_VERSION > 95
        funcctx->max_calls = result_count;
#else
        funcctx->max_calls = (uint32_t)result_count;
#endif

        funcctx->user_fctx = result_tuples;
        if (get_call_result_type(fcinfo, NULL, &tuple_desc)
                != TYPEFUNC_COMPOSITE) {
            ereport(ERROR,
                    (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
                     errmsg("function returning record called in context "
                         "that cannot accept type record")));
        }

        funcctx->tuple_desc = tuple_desc;
        MemoryContextSwitchTo(oldcontext);
    }

    funcctx = SRF_PERCALL_SETUP();

    tuple_desc = funcctx->tuple_desc;
    result_tuples = (General_path_element_t *) funcctx->user_fctx;

    if (funcctx->call_cntr < funcctx->max_calls) {
        // do when there is more left to send
        HeapTuple    tuple;
        Datum        result;
        Datum *values;
        bool* nulls;
        size_t call_cntr = funcctx->call_cntr;


        size_t numb = 8;
        values = palloc(numb * sizeof(Datum));
        nulls = palloc(numb * sizeof(bool));

        size_t i;
        for (i = 0; i < numb; ++i) {
            nulls[i] = false;
        }

        values[0] = Int32GetDatum(call_cntr + 1);
        values[1] = Int32GetDatum(result_tuples[call_cntr].seq);
        values[2] = Int64GetDatum(result_tuples[call_cntr].start_id);
        values[3] = Int64GetDatum(result_tuples[call_cntr].end_id);
        values[4] = Int64GetDatum(result_tuples[call_cntr].node);
        values[5] = Int64GetDatum(result_tuples[call_cntr].edge);
        values[6] = Float8GetDatum(result_tuples[call_cntr].cost);
        values[7] = Float8GetDatum(result_tuples[call_cntr].agg_cost);

        tuple = heap_form_tuple(tuple_desc, values, nulls);

        // make the tuple into a datum
        result = HeapTupleGetDatum(tuple);

        // clean up (this is not really necessary)
        pfree(values);
        pfree(nulls);

        SRF_RETURN_NEXT(funcctx, result);
    } else {  // do when there is no more left
        SRF_RETURN_DONE(funcctx);
    }
}

