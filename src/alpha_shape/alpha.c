/*PGR-GNU*****************************************************************
FILE: alpha.c

Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
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

#include <float.h>
#include <stdint.h>

#include "c_common/postgres_connection.h"


#include "catalog/pg_type.h"
#include "c_common/e_report.h"

#include "drivers/alpha_shape/alpha_driver.h"



/*
 * Define this to have profiling enabled
 */
// #define PROFILE


PGDLLEXPORT Datum alphashape(PG_FUNCTION_ARGS);

#undef DEBUG
#include "c_common/debug_macro.h"

// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000


#if 0
static int
finish(int code, int ret) {
  code = SPI_finish();
  if (code  != SPI_OK_FINISH) {
    elog(ERROR, "couldn't disconnect from SPI");
    return -1;
  }
  return ret;
}
#endif

typedef struct vertex_columns {
  int id;
  int x;
  int y;
} vertex_columns_t;



static int
fetch_vertices_columns(SPITupleTable *tuptable,
                       vertex_columns_t *vertex_columns) {
    if (tuptable) {}  // TODO(vicky) this is unused parameter
  vertex_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
  vertex_columns->x = SPI_fnumber(SPI_tuptable->tupdesc, "x");
  vertex_columns->y = SPI_fnumber(SPI_tuptable->tupdesc, "y");

  if (vertex_columns->id == SPI_ERROR_NOATTRIBUTE ||
      vertex_columns->x == SPI_ERROR_NOATTRIBUTE ||
      vertex_columns->y == SPI_ERROR_NOATTRIBUTE) {
      elog(ERROR, "Error, query must return columns "
           "'id', 'x' and 'y'");
      return -1;
    }

  if (SPI_gettypeid(SPI_tuptable->tupdesc, vertex_columns->id) != INT4OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, vertex_columns->x) != FLOAT8OID ||
      SPI_gettypeid(SPI_tuptable->tupdesc, vertex_columns->y) != FLOAT8OID) {
      elog(ERROR,
           "Error, column 'id' must be of type int4,"
           "'x' and 'y' must be of type float8");
      return -1;
    }

  return 0;
}

static void
fetch_vertex(HeapTuple *tuple, TupleDesc *tupdesc,
             vertex_columns_t *vertex_columns, vertex_t *target_vertex) {
  Datum binval;
  bool isnull;

  binval = SPI_getbinval(*tuple, *tupdesc, vertex_columns->x, &isnull);
  if (isnull)
    elog(ERROR, "x contains a null value");
  target_vertex->x = DatumGetFloat8(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, vertex_columns->y, &isnull);
  if (isnull)
    elog(ERROR, "y contains a null value");
  target_vertex->y = DatumGetFloat8(binval);
}

static void compute_alpha_shape(
        char* sql, float8 alpha,
        vertex_t **res,
        size_t *res_count) {
  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  bool moredata = true;
  size_t ntuples;
  vertex_t *vertices = NULL;
  size_t total_tuples = 0;
#ifndef _MSC_VER
  vertex_columns_t vertex_columns = {.id = -1, .x = -1, .y = -1};
#else   // _MSC_VER
  vertex_columns_t vertex_columns = {-1, -1, -1};
#endif  // _MSC_VER
#if 0
  int ret = -1;
#endif
  PGR_DBG("start alpha_shape\n");

  SPIcode = SPI_connect();
  if (SPIcode  != SPI_OK_CONNECT) {
      elog(ERROR, "alpha_shape: couldn't open a connection to SPI");
      return;
    }

  SPIplan = SPI_prepare(sql, 0, NULL);
  if (SPIplan  == NULL) {
      elog(ERROR, "alpha_shape: couldn't create query plan via SPI");
      return;
    }

  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
      elog(ERROR, "alpha_shape: SPI_cursor_open('%s') returns NULL", sql);
      return;
    }

  while (moredata == true) {
      SPI_cursor_fetch(SPIportal, true, TUPLIMIT);

      if (vertex_columns.id == -1) {
          if (fetch_vertices_columns(SPI_tuptable, &vertex_columns) == -1)
        return;
        }

      ntuples = SPI_processed;
      total_tuples += ntuples;
      if (!vertices)
        vertices = palloc(total_tuples * sizeof(vertex_t));
      else
        vertices = repalloc(vertices, total_tuples * sizeof(vertex_t));

      if (vertices == NULL) {
          elog(ERROR, "Out of memory");
          return;
        }

      if (ntuples > 0) {
          uint32_t t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          for (t = 0; t < ntuples; t++) {
              HeapTuple tuple = tuptable->vals[t];
              fetch_vertex(&tuple, &tupdesc, &vertex_columns,
                           &vertices[total_tuples - ntuples + t]);
            }
          SPI_freetuptable(tuptable);
        } else {
          moredata = false;
        }
    }


  // if (total_tuples < 2) //this was the buggy code of the pgrouting project.
  // TODO(someone): report this as a bug to the pgrouting project
  // the CGAL alpha-shape function crashes
  //    if called with less than three points!!!

  if (total_tuples < 3) {
      if (vertices) pfree(vertices);
      pgr_SPI_finish();
      elog(ERROR, "Less than 3 vertices."
              " Alpha shape calculation needs at least 3 vertices.");
      return;
  }
  if (total_tuples == 1) {
      if (vertices) pfree(vertices);
      pgr_SPI_finish();
      elog(ERROR, "Distance is too short."
              " only 1 vertex for alpha shape calculation."
              " alpha shape calculation needs at least 3 vertices.");
      return;
  }
  if (total_tuples == 2) {
      if (vertices) pfree(vertices);
      pgr_SPI_finish();
      elog(ERROR, "Distance is too short."
              " only 2 vertices for alpha shape calculation."
              " alpha shape calculation needs at least 3 vertices.");
      return;
  }
  if (total_tuples < 3) {
      // elog(ERROR, "Distance is too short ....");
      if (vertices) pfree(vertices);
      pgr_SPI_finish();
      return;
  }

  PGR_DBG("Calling CGAL alpha-shape\n");

  char *err_msg = NULL;
  char* log_msg = NULL;
  char* notice_msg = NULL;

  alpha_shape(vertices, total_tuples, alpha, res, res_count, &err_msg);

  if (err_msg && (*res)) {
      pfree(*res);
      (*res) = NULL;
      (*res_count) = 0;
  }

  pgr_global_report(log_msg, notice_msg, err_msg);

  if (log_msg) pfree(log_msg);
  if (notice_msg) pfree(notice_msg);
  if (err_msg) pfree(err_msg);
  if (vertices) pfree(vertices);
  pgr_SPI_finish();
}

PG_FUNCTION_INFO_V1(alphashape);

PGDLLEXPORT
Datum alphashape(PG_FUNCTION_ARGS) {
  FuncCallContext      *funcctx;
  TupleDesc            tuple_desc;
  vertex_t     *res = NULL;

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
      MemoryContext   oldcontext;
      size_t res_count;


      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

      compute_alpha_shape(text_to_cstring(PG_GETARG_TEXT_P(0)),
                                PG_GETARG_FLOAT8(1), &res, &res_count);

      /* total number of tuples to be returned */
      PGR_DBG("Conting tuples number\n");
#if PGSQL_VERSION > 95
      funcctx->max_calls = res_count;
#else
      funcctx->max_calls = (uint32_t)res_count;
#endif
      funcctx->user_fctx = res;

      PGR_DBG("Total count %lu", res_count);

      if (get_call_result_type(fcinfo, NULL, &tuple_desc) != TYPEFUNC_COMPOSITE)
        ereport(ERROR,
            (errcode(ERRCODE_FEATURE_NOT_SUPPORTED),
            errmsg("function returning record called in context "
                   "that cannot accept type record")));

      funcctx->tuple_desc = BlessTupleDesc(tuple_desc);

      MemoryContextSwitchTo(oldcontext);
    }

  /* stuff done on every call of the function */
  PGR_DBG("Strange stuff doing\n");
  funcctx = SRF_PERCALL_SETUP();

  tuple_desc = funcctx->tuple_desc;
  res = (vertex_t*)funcctx->user_fctx;

  PGR_DBG("Trying to allocate some memory\n");

  if (funcctx->call_cntr < funcctx->max_calls) {
      /* do when there is more left to send */
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      bool* nulls;
      double x;
      double y;

      values = palloc(2 * sizeof(Datum));
      nulls = palloc(2 * sizeof(bool));

      x = res[funcctx->call_cntr].x;
      y = res[funcctx->call_cntr].y;
      if (x == DBL_MAX && y == DBL_MAX) {
        values[0] = 0;
        values[1] = 0;
        nulls[0] = true;
        nulls[1] = true;
      } else {
        values[0] = Float8GetDatum(x);
        values[1] = Float8GetDatum(y);
        nulls[0] = false;
        nulls[1] = false;
      }

      PGR_DBG("Heap making\n");

      tuple = heap_form_tuple(tuple_desc, values, nulls);

      PGR_DBG("Datum making\n");

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      PGR_DBG("Trying to free some memory\n");

      /* clean up (this is not really necessary) */
      pfree(values);
      pfree(nulls);

      SRF_RETURN_NEXT(funcctx, result);
    } else  {
      SRF_RETURN_DONE(funcctx);
    }
}
