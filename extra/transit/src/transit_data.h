#pragma once

#include <time.h>
#include <stdlib.h>

typedef char *stop_id_t;
typedef char *trip_id_t;

typedef struct {
  int src;
  int dest;
  trip_id_t trip_id;
  double link_cost;
} trip;

int fetch_next_links(int u, time_t arrival_time, trip **t);
