#include "transit_data.h"

int fetch_next_links(int u, time_t arrival_time, trip **trips_ref) {
  int count;
  trip *trips;
  //TODO: Query database and retrieve actual trips
  // Following is hardcoded data.
  switch (u) {
  case 1:
    if (arrival_time == 0) //Trip-1
        {
      count = 2;
      *trips_ref = trips = (trip *) malloc(sizeof(trip) * count);
      trips[0].src = 1;
      trips[0].dest = 2;
      trips[0].link_cost = 15;
      trips[0].trip_id = "E(1->2)";

      trips[1].src = 1;
      trips[1].dest = 3;
      trips[1].link_cost = 25;
      trips[1].trip_id = "E(1->3)";
    } else
      return 0;
    break;
  case 2:
    if (arrival_time == 15) {
      count = 1;
      *trips_ref = trips = (trip *) malloc(sizeof(trip) * count);
      trips[0].src = 2;
      trips[0].dest = 3;
      trips[0].link_cost = 15;
      trips[0].trip_id = "E(2->3)";
    } else
      return 0;
    break;
  case 3:
    return 0;
  }
  return count;
}
