#include <postgres.h>
#include <fmgr.h>
#include <funcapi.h>
#include <utils/timestamp.h>
#define DEBUG 1
#include "utility.h"
#include "scheduled_route.h"
#define TUPLIMIT 1000
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC
;
#endif

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
    int query_time;

    /* create a function context for cross-call persistence */
    funcctx = SRF_FIRSTCALL_INIT();

    /* switch to memory context appropriate for multiple function calls */
    oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);

    DBG("Before calling compute_scheduled_route\n");
    query_time = PG_GETARG_INT32(3);
    DBG("sizeof c int = %d", sizeof(int));
    DBG("query_time = %d\n", query_time);
    ret = compute_scheduled_route(text2char(PG_GETARG_TEXT_P(0)), // gtfs schema
    PG_GETARG_INT32(1), // source stop id
        PG_GETARG_INT32(2), // destination stop id
        query_time, &path, &path_count);

    DBG("After finishing compute_scheduled_route\n");
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
    DBG("Processing row %d\n", call_cntr);
    HeapTuple tuple;
    Datum result;
    Datum *values;
    char* nulls;

    values = palloc(4 * sizeof(Datum));
    nulls = palloc(4 * sizeof(char));
    DBG("Before call to Int32GetDatum\n");
    values[0] = Int32GetDatum(path[call_cntr].stop_id);
    DBG("After call to Int32GetDatum\n");
    nulls[0] = ' ';
    if (path[call_cntr].trip_id == NULL)
    {
      nulls[1] = 'n';
      values[1] = '\0';
    } else {
      values[1] = PointerGetDatum(char2text(path[call_cntr].trip_id));
      nulls[1] = ' ';
    }
    if (path[call_cntr].waiting_time == -1)
    {
      nulls[2] = 'n';
      values[2] = '\0';
    } else {
      values[2] = Int32GetDatum(path[call_cntr].waiting_time);
      nulls[2] = ' ';
    }
    if (path[call_cntr].travel_time == -1)
    {
      nulls[3] = 'n';
      values[3] = '\0';
    } else {
      values[3] = Int32GetDatum(path[call_cntr].travel_time);
      nulls[3] = ' ';
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
