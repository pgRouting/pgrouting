//#include <string.h>
#include "transit.h"

int compute_scheduled_route(text *gtfs_schema, text *source_stop_id,
    text *destination_stop_id, TimestampTz query_time,
    gtfs_path_element_t **path, int *path_count);

PG_FUNCTION_INFO_V1(scheduled_route);

Datum scheduled_route(PG_FUNCTION_ARGS) {
  FuncCallContext *funcctx;
  int call_cntr;
  int max_calls;
  TupleDesc tuple_desc;
  gtfs_path_element_t *path = NULL;

  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL()) {
    MemoryContext oldcontext;
    int path_count = 0;
    int ret;

    /* create a function context for cross-call persistence */
    funcctx = SRF_FIRSTCALL_INIT();

    /* switch to memory context appropriate for multiple function calls */
    oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

    ret = compute_scheduled_route(PG_GETARG_TEXT_P(0), // gtfs schema
        PG_GETARG_TEXT_P(1), // source stop id
        PG_GETARG_TEXT_P(2), // destination stop id
        PG_GETARG_TIMESTAMPTZ(3), // max changeovers allowed
        &path, &path_count);

    /* total number of tuples to be returned */
    funcctx->max_calls = path_count;
    funcctx->user_fctx = path;

    funcctx->tuple_desc = BlessTupleDesc(
        RelationNameGetTupleDesc("gtfs_path_result"));

    MemoryContextSwitchTo(oldcontext);
  }

  /* stuff done on every call of the function */
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;
  path = (gtfs_path_element_t *) funcctx->user_fctx;

  if (call_cntr < max_calls) { /* do when there is more left to send */
    HeapTuple tuple;
    Datum result;
    Datum *values;
    char* nulls;

    values = palloc(3 * sizeof(Datum));
    nulls = palloc(3 * sizeof(char));
    values[0] = PointerGetDatum(path[call_cntr].stop_id);
    nulls[0] = ' ';
    if (path[call_cntr].trip_id == NULL)
    {
      nulls[1] = 'n';
      values[1] = '\0';
    } else {
      values[1] = PointerGetDatum(path[call_cntr].trip_id);
      nulls[1] = ' ';
    }
    tuple = heap_formtuple(tuple_desc, values, nulls);

    /* make the tuple into a datum */
    result = HeapTupleGetDatum(tuple);

    /* clean up (this is not really necessary) */
    pfree(values);
    pfree(nulls);
    SRF_RETURN_NEXT(funcctx, result);
  } else { /* do when there is no more left */

    SRF_RETURN_DONE(funcctx);
  }
}

int compute_scheduled_route(text *gtfs_schema, text *source_stop_id,
    text *destination_stop_id, TimestampTz query_time,
    gtfs_path_element_t **path, int *path_count) {
  int SPIcode;
  void *SPIplan;
  Portal SPIportal;
  int ntuples;

  char *sql = "select stop_id, trip_id from forged_output";

  SPIcode = SPI_connect();
  if (SPIcode != SPI_OK_CONNECT)
  {
    elog(ERROR, "couldn't open a connection to SPI");
    return -1;
  }

  SPIplan = SPI_prepare(sql, 0, NULL);

  if (SPIplan == NULL)
  {
    elog(ERROR, "SPI_plan is NULL");
    return -1;
  }
  if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL)
  {
    elog(ERROR, "SPI_cursor_open returns NULL");
    return -1;
  }
  *path_count = 0;
  while (true) {
    SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

    ntuples = SPI_processed;
    DBG("SPI_processed = %i", ntuples);
    *path_count += ntuples;
    if (*path == NULL)
    {
      *path =
          (gtfs_path_element_t *) palloc(*path_count * sizeof(gtfs_path_element_t));
    } else {
      *path = (gtfs_path_element_t *) repalloc(*path,
          *path_count * sizeof(gtfs_path_element_t));
    }
    if (ntuples > 0) {
      int t;
      SPITupleTable *tuptable = SPI_tuptable;
      TupleDesc tupdesc = SPI_tuptable->tupdesc;
      for (t = 0; t < ntuples; t++) {
        HeapTuple tuple = tuptable->vals[t];
        fetch_path(&tuple, &tupdesc, &((*path)[*path_count - ntuples + t]));
      }
      SPI_freetuptable(tuptable);
    } else {
      break;
    }
  }

  return 0;
}

void fetch_path(HeapTuple *tuple, TupleDesc *tupdesc,
    gtfs_path_element_t *path_element) {
  Datum binval;
  bool isnull;
  binval = SPI_getbinval(*tuple, *tupdesc, 1, &isnull);
  if (isnull)
    elog(ERROR, "stop_id contains a null value");
  path_element->stop_id = DatumGetTextP(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, 2, &isnull);
  if (!isnull)
    path_element->trip_id = DatumGetTextP(binval);
  else
    path_element->trip_id = NULL;
}
