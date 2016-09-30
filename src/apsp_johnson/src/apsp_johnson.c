/*
 * APSP Johnson algorithm for PostgreSQL
 *
 * Copyright (c) 2011 J Kishore Kumar
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
#include <stdio.h>
#include <stdlib.h>
#include "apsp_johnson.h"

//-------------------------------------------------------------------------

PGDLLEXPORT Datum apsp_johnson(PG_FUNCTION_ARGS);

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

static char *
text2char(text *in) {
  char *out = palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

static int finish(int code, int ret) {
  code = SPI_finish();
  if (code != SPI_OK_FINISH) {
    elog(ERROR, "couldn't disconnect from SPI");
    return -1;
  }

  return ret;
}

static int fetch_edge_apsp_columns(SPITupleTable *tuptable,
    edge_apsp_johnson_t *edge_columns) {
  edge_columns->source = SPI_fnumber(SPI_tuptable->tupdesc, "source");
  edge_columns->target = SPI_fnumber(SPI_tuptable->tupdesc, "target");
  edge_columns->cost = SPI_fnumber(SPI_tuptable->tupdesc, "cost");
  if (edge_columns->source == SPI_ERROR_NOATTRIBUTE
      || edge_columns->target == SPI_ERROR_NOATTRIBUTE
      || edge_columns->cost == SPI_ERROR_NOATTRIBUTE)
      {
    elog(ERROR, "Error, query must return columns "
        "'source', 'target' and 'cost'");
    return -1;
  }

  if (SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->source) != INT4OID
      || SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->target) != INT4OID
      || SPI_gettypeid(SPI_tuptable->tupdesc, edge_columns->cost) != FLOAT8OID)
      {
    elog(ERROR, "Error, columns 'source', 'target' must be of type int4, "
        "'cost' must be of type float8");
    return -1;
  }

  DBG("columns: source %i target %i cost %lf",
      edge_columns->source, edge_columns->target, edge_columns->cost);
  return 0;
}

static void fetch_edge_apsp_johnson(HeapTuple *tuple, TupleDesc *tupdesc,
    edge_apsp_johnson_t *edge_columns, edge_apsp_johnson_t *target_edge) {
  Datum binval;
  bool isnull;

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->source, &isnull);
  if (isnull)
    elog(ERROR, "source contains a null value");
  target_edge->source = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->target, &isnull);
  if (isnull)
    elog(ERROR, "target contains a null value");
  target_edge->target = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->cost, &isnull);
  if (isnull)
    elog(ERROR, "cost contains a null value");
  target_edge->cost = DatumGetFloat8(binval);
}

int compute_apsp_johnson(char* sql, edge_apsp_johnson_t **output_edges,
    int *output_count) {

  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = TRUE;
  int ntuples;
  edge_apsp_johnson_t *edges = NULL;
  int total_tuples = 0;

  int v_max_id = 0;
  int v_min_id = INT_MAX;

  edge_apsp_johnson_t edge_columns = { .source= -1, .target= -1, .cost= -1 };
  char *err_msg;
  int ret = -1;
  register int z;

  DBG("start apsp-johnson\n");

  SPIcode = SPI_connect();
  if (SPIcode != SPI_OK_CONNECT)
  {
    elog(ERROR, "apsp-johnson: couldn't open a connection to SPI");
    return -1;
  }

  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan == NULL)
  {
    elog(ERROR, "apsp-johnson: couldn't create query plan via SPI");
    return -1;
  }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL)
  {
    elog(ERROR, "apsp-johnson: SPI_cursor_open('%s') returns NULL", sql);
    return -1;
  }

  while (moredata == TRUE) {
    SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

    if (edge_columns.cost == -1) {
      if (fetch_edge_apsp_columns(SPI_tuptable, &edge_columns) == -1)
        return finish(SPIcode, ret);
    }

    ntuples = SPI_processed;
    total_tuples += ntuples;
    if (!edges)
      edges = palloc(total_tuples * sizeof(edge_apsp_johnson_t));
    else
      edges = repalloc(edges, total_tuples * sizeof(edge_apsp_johnson_t));

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
        fetch_edge_apsp_johnson(&tuple, &tupdesc, &edge_columns,
            &edges[total_tuples - ntuples + t]);
      }
      SPI_freetuptable(tuptable);
    } else {
      moredata = FALSE;
    }
  }

  //defining min and max vertex id

  DBG("Total %i tuples", total_tuples);

  for (z = 0; z < total_tuples; z++) {
    if (edges[z].source < v_min_id)
      v_min_id = edges[z].source;

    if (edges[z].source > v_max_id)
      v_max_id = edges[z].source;

    if (edges[z].target < v_min_id)
      v_min_id = edges[z].target;

    if (edges[z].target > v_max_id)
      v_max_id = edges[z].target;

    DBG("%i <-> %i", v_min_id, v_max_id);

  }

  //::::::::::::::::::::::::::::::::::::
  //:: reducing vertex id (renumbering)
  //::::::::::::::::::::::::::::::::::::
  for (z = 0; z < total_tuples; z++) {

    edges[z].source -= v_min_id;
    edges[z].target -= v_min_id;
    DBG("%i - %i", edges[z].source, edges[z].target);
  }

  DBG("Total %i tuples", total_tuples);

  DBG("Calling boost_apsp_johnson <%i>\n", total_tuples);

  // calling C++ apsp_johnson function
  ret = boost_apsp_johnson(edges, total_tuples, output_edges, output_count,
      &err_msg);

  DBG("SIZE %i\n", *output_count);

  DBG("ret =  %i\n", ret);

  //::::::::::::::::::::::::::::::::
  //:: restoring original vertex id
  //::::::::::::::::::::::::::::::::
  for (z = 0; z < *output_count; z++) {
    //DBG("vertex %i\n",(*path)[z].vertex_id);
    (*output_edges)[z].source += v_min_id;
    (*output_edges)[z].target += v_min_id;
  }

  if (ret < 0) {
    //elog(ERROR, "Error computing path: %s", err_msg);
    ereport(
        ERROR,
        (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), errmsg("Error computing path: %s", err_msg)));
  }
  return finish(SPIcode, ret);
}

PG_FUNCTION_INFO_V1(apsp_johnson);
PGDLLEXPORT Datum apsp_johnson(PG_FUNCTION_ARGS) {
  FuncCallContext *funcctx;
  int call_cntr;
  int max_calls;
  TupleDesc tuple_desc;
  edge_apsp_johnson_t *output_edges;

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
    MemoryContext oldcontext;
    int output_count = 0;
#ifdef DEBUG
    int ret;
#endif

    /* create a function context for cross-call persistence */
    funcctx = SRF_FIRSTCALL_INIT();

    /* switch to memory context appropriate for multiple function calls */
    oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

#ifdef DEBUG
    ret =
#endif
    compute_apsp_johnson(text2char(PG_GETARG_TEXT_P(0)), &output_edges,
        &output_count);

#ifdef DEBUG
    DBG("Ret is %i", ret);
    if (ret >= 0) {
      int i;
      for (i = 0; i < output_count; i++) {
        DBG("Step # %i source  %i ", i, output_edges[i].source);
        DBG("        target    %i ", output_edges[i].target);
        DBG("        cost       %f ", output_edges[i].cost);
      }
    }
#endif

    /* total number of tuples to be returned */
    DBG("Conting tuples number\n");
    funcctx->max_calls = output_count;
    funcctx->user_fctx = output_edges;

    DBG("Output count %i", output_count);

    funcctx->tuple_desc = BlessTupleDesc(RelationNameGetTupleDesc("pgr_costResult"));

    MemoryContextSwitchTo(oldcontext);
  }

  /* stuff done on every call of the function */
  DBG("Strange stuff doing\n");

  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  output_edges = (edge_apsp_johnson_t*) funcctx->user_fctx;

  DBG("Trying to allocate some memory\n");

  if (call_cntr < max_calls) /* do when there is more left to send */
  {
    HeapTuple tuple;
    Datum result;
    Datum *values;
    bool* nulls;

    values = palloc(4 * sizeof(Datum));
    nulls = palloc(4 * sizeof(bool));

    values[0] = Int32GetDatum(call_cntr);
    nulls[0] = false;
    values[1] = Int32GetDatum(output_edges[call_cntr].source);
    nulls[1] = false;
    values[2] = Int32GetDatum(output_edges[call_cntr].target);
    nulls[2] = false;
    values[3] = Float8GetDatum(output_edges[call_cntr].cost);
    nulls[3] = false;

    DBG("Heap making\n");

    tuple = heap_form_tuple(tuple_desc, values, nulls);

    DBG("Datum making\n");

    /* make the tuple into a datum */
    result = HeapTupleGetDatum(tuple);

    DBG("Trying to free some memory\n");

    /* clean up (this is not really necessary) */
    pfree(values);
    pfree(nulls);

    SRF_RETURN_NEXT(funcctx, result);
  } else /* do when there is no more left */
  {
    SRF_RETURN_DONE(funcctx);
  }
}

#ifdef DEBUG
void dbg(const char *fmt, ...) //FIXME:Refactor to a common place
    {
      char buff[1024];
      va_list arg;
      va_start(arg, fmt);
      vsprintf(buff, fmt, arg);
      elog(NOTICE, "%s", buff);
      va_end(arg);
    }
#else
void dbg(const char *fmt, ...)
{
  return;
}
#endif
