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

int fetch_next_links(const char *schema, int u, time_t arrival_time,
    trip **trips_ref) {
  int i;
  trip *trips;
  int total_processed;
  const char *MAX_WAIT_TIME = "1 hour";
  char sql[1024];
  DBG("Inside fetch_next_links with u = %d and arrival_time = %d", u, (int)arrival_time);
  if (SPI_connect() != SPI_OK_CONNECT)
  {
    elog(ERROR, "Cannot connect to SPI");
  }
  sprintf(
      sql,
      "SELECT st1.trip_id, dest.stop_id_int4,"
          "gtfstime_to_secs(st2.arrival_time) - gtfstime_to_secs(st1.arrival_time)"
          " FROM %1$s.stop_times st1, %1$s.stop_times st2, %1$s.stop_id_map src, %1$s.stop_id_map dest,"
          " %1$s.trips t, %1$s.routes r, %1$s.agency a"
          " WHERE src.stop_id_int4 = %2$d"
          " AND st1.stop_id = src.stop_id_text"
          " AND st2.trip_id = st1.trip_id"
          " AND st2.stop_sequence > st1.stop_sequence"
          " AND dest.stop_id_text = st2.stop_id"
          " AND t.trip_id = st1.trip_id"
          " AND t.route_id = r.route_id"
          " AND r.agency_id = a.agency_id"
          " AND check_service('%1$s', t.service_id, (timestamp with time zone 'epoch'"
          " + %3$d * '1 second'::interval - st1.arrival_time::interval"
          " + '%4$s'::interval) at time zone a.agency_timezone, '%4$s'::interval)",
      schema, u, arrival_time, MAX_WAIT_TIME);

  int ret = SPI_execute(sql, true, 1);
  if (ret != SPI_OK_SELECT)
  {
    elog(ERROR, "Select statement to fetch next links failed.");
  }

  if (SPI_tuptable == NULL)
  {
    return 0;
  }
  total_processed = SPI_processed;
  *trips_ref = trips = (trip *) palloc(sizeof(trip) * total_processed);
  for (i = 0; i < SPI_processed; i++) {

    trips[i].src = u;
    trips[i].trip_id = SPI_getvalue(SPI_tuptable->vals[i],
        SPI_tuptable->tupdesc, 1);
    trips[i].dest =
        DatumGetInt32(SPI_getbinval(SPI_tuptable->vals[i], SPI_tuptable->tupdesc, 2, NULL));
    trips[i].link_cost =
        DatumGetInt32(SPI_getbinval(SPI_tuptable->vals[i], SPI_tuptable->tupdesc, 3, NULL));
    DBG("trips[%d].src = %d", i, trips[i].src);
    DBG("trips[%d].trip_id = %s", i, trips[i].trip_id);
    DBG("trips[%d].dest = %d", i, trips[i].dest);
    DBG("trips[%d].link_cost = %d", i, trips[i].link_cost);
  }
  SPI_finish();
  DBG("fetch_next_links returns %d links", total_processed);
  return total_processed;
}

int fetch_shortest_time(const char *schema, int goal,
    time_seconds_t *shortest_time) {
  char sql[255];
  int total_processed;
  DBG("Inside fetch_shortest_time");
  if (SPI_connect() != SPI_OK_CONNECT)
  {
    elog(ERROR, "Cannot connect to SPI");
  }
  sprintf(
      sql,
      "select source, travel_time from %s.shortest_time_graph where target = %d",
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
