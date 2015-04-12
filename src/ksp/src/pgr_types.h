#ifndef PGR_TYPES_H
#define PGR_TYPES_H

#include "postgres.h"

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
} ;

enum graphType { UNDIRECTED= 0, DIRECTED};

#endif // PGR_TYPES_H
