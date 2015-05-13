#ifndef PGR_TYPES_H
#define PGR_TYPES_H

#include "postgres.h"

typedef struct edge_astar
{
  int id;
  int source;
  int target;
  float8 cost;
  float8 reverse_cost;
  float8 s_x;
  float8 s_y;
  float8 t_x;
  float8 t_y;
} edge_astar_t;


typedef struct path_element
{
    int64_t vertex_id;
    int64_t edge_id;
    float8 cost;
} path_element_t;

typedef struct path_element3
{
    int route_id;
    int64_t vertex_id;
    int64_t edge_id;
    float8 cost;
} pgr_path_element3_t;

typedef struct {
  int64_t id;
  int64_t source;
  int64_t target;
  float8 cost;
  float8 reverse_cost;
} pgr_edge_t;

typedef struct {
  int seq;
  int64_t source;
  int64_t edge;
  float8 cost;
} pgr_path_t;

struct boost_vertex_t {
  int64_t id;
};

struct boost_edge_t{
  int64_t id;
  float8 cost;
  int64_t source_id;
  int64_t target_id;
};

enum graphType { UNDIRECTED= 0, DIRECTED};

#endif // PGR_TYPES_H
