#pragma once

#include <time.h>

typedef unsigned int time_seconds_t;

typedef struct {
  int src;
  int dest;
  char *trip_id;
  time_seconds_t link_cost;
} trip;

typedef struct {
  int stop_id;
  char *trip_id;
} gtfs_path_element_t;

int get_max_stop_id(const char *schema);
int fetch_next_links(const char *schema, int u, time_t arrival_time, trip **t);
int fetch_shortest_time(const char *schema, int goal, time_seconds_t *shortest_time);
