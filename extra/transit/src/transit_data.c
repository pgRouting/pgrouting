#include "transit_data.h"
#define DEBUG 1
#include "utility.h"

// The following headers are incompatible with c++. So,
// they cannot be present in transit_data.h
#include <postgres.h>
#include <executor/spi.h>

int get_max_stop_id(const char *schema) {
  char sql[255];
  int max_stop_id = 0;
  DBG("Inside get_max_stop_id");
  if (SPI_connect() != SPI_OK_CONNECT)
  {
    elog(ERROR, "Cannot connect to SPI");
  }
  sprintf(sql, "select max(stop_id_int4) from %s.stop_id_map", schema);
  int ret = SPI_execute(sql, true, 1);
  if (ret != SPI_OK_SELECT)
  {
    elog(ERROR, "Select statement to retrieve maximum stop id failed.");
  }

  if (SPI_tuptable != NULL)
  {
    bool isNull;
    Datum d = SPI_getbinval(SPI_tuptable->vals[0], SPI_tuptable->tupdesc, 1,
        &isNull);
    max_stop_id = DatumGetInt32(d);
  }

  SPI_finish();
  DBG("Max stop id is %d", max_stop_id);
  return max_stop_id;
}

int fetch_next_links(const char *schema, int u, int arrival_time,
    trip **trips_ref) {
  int i;
  trip *trips;
  int total_processed;
  const char *MAX_WAIT_TIME = "1 hour";
  const int MAX_LINKS_RETURNED = 1000;
  char sql[1024];
  bool isNull;
  HeapTuple heaptuple;
  TupleDesc tupdesc;
  DBG("Inside fetch_next_links with u = %d and arrival_time = %d",
      u, (int)arrival_time);
  if (SPI_connect() != SPI_OK_CONNECT)
  {
    elog(ERROR, "Cannot connect to SPI");
  }
  sprintf(
      sql,
      "SELECT trip_id, destination_stop_id, cost FROM fetch_next_links('%s', %d, %d, '%s')",
      schema, u, arrival_time, MAX_WAIT_TIME);

  int ret = SPI_execute(sql, true, MAX_LINKS_RETURNED);
  DBG("fetch_next_links query returned return value = %d", ret);
  if (ret != SPI_OK_SELECT)
  {
    elog(ERROR, "Select statement to fetch next links failed.");
  }

  if (SPI_tuptable == NULL)
  {
    return 0;
  }
  total_processed = SPI_processed;
  if (total_processed == 0) {
    SPI_finish();
    return total_processed;
  }

  *trips_ref = (trip *) malloc(sizeof(trip) * total_processed); // FIXME: Using palloc hangs indefinitely
  trips = *trips_ref;
  tupdesc = SPI_tuptable->tupdesc;
  for (i = 0; i < total_processed; i++) {
    heaptuple = SPI_tuptable->vals[i];
    trips[i].src = u;
    trips[i].trip_id = text2char(
        DatumGetTextP(SPI_getbinval(heaptuple, tupdesc, 1, &isNull)));
    trips[i].dest = DatumGetInt32(SPI_getbinval(heaptuple, tupdesc, 2, &isNull));
    trips[i].link_cost =
        DatumGetInt32(SPI_getbinval(heaptuple, tupdesc, 3, &isNull));
  }
  SPI_finish();
  DBG("fetch_next_links returns %d links", total_processed);
  return total_processed;
}

int fetch_shortest_time(const char *schema, int goal,
    time_seconds_t *shortest_time) {
  char sql[255];
  int total_processed;
  if (SPI_connect() != SPI_OK_CONNECT)
  {
    elog(ERROR, "Cannot connect to SPI");
  }
  sprintf(
      sql,
      "select source, travel_time from %s.shortest_time_closure where target = %d",
      schema, goal);
  int ret = SPI_execute(sql, true, 0);
  if (ret != SPI_OK_SELECT)
  {
    elog(ERROR, "Select statement to retrieve shortest time heuristic failed.");
  }

  if (SPI_tuptable != NULL)
  {
    bool isNull;
    int i;
    HeapTuple row;
    total_processed = SPI_processed;
    for (i = 0; i < total_processed; i++) {
      int source;
      int travel_time;
      Datum source_d, travel_time_d;

      row = SPI_tuptable->vals[i];
      source_d = SPI_getbinval(row, SPI_tuptable->tupdesc, 1, &isNull);
      if (isNull) {
        elog(ERROR, "Null value for Source id is not valid.");
      }
      travel_time_d = SPI_getbinval(row, SPI_tuptable->tupdesc, 2, &isNull);
      if (isNull) {
        elog(ERROR, "Null value for Travel time is not valid.");
      }
      source = DatumGetInt32(source_d);
      travel_time = DatumGetInt32(travel_time_d);
      shortest_time[source] = travel_time;
    }
  }
  SPI_finish();
  DBG("fetch_shortest_time returns %d records", total_processed);
  return total_processed;
}

void dbg(const char *fmt, ...)
{
  char buff[1024];
  va_list arg;
  va_start(arg, fmt);
  vsprintf(buff, fmt, arg);
  elog(NOTICE, "%s", buff);
  va_end(arg);
}
