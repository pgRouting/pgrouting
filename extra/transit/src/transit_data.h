#pragma once

#include <time.h>

typedef unsigned int time_seconds_t;

typedef struct {
  int src;
  int dest;
  char *trip_id;
  time_seconds_t waiting_cost;
  time_seconds_t travel_cost;
} next_link_t;

typedef struct {
  int stop_id;
  char *trip_id;
  int waiting_time;
  int travel_time;
} gtfs_path_element_t;

int get_max_stop_id(const char *schema);
int fetch_next_links(const char *schema, int u, int arrival_time, next_link_t **t);
int fetch_shortest_time(const char *schema, int goal,
    time_seconds_t *shortest_time);

void dbg(const char *format, ...);
