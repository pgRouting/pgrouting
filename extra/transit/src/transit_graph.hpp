#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "transit_data.h"
#ifdef __cplusplus
}
#endif

#include <ctime>
#include <iostream>
#include <string>
using namespace std;

#include <boost/graph/adjacency_list.hpp>

using namespace boost;

struct stop_t {
  time_t arrival_time;
  double heuristic_cost;
  unsigned int predecessor; //FIXME: Type should be vertex_descriptor
};

struct link_t {
  trip_id_t trip_id;
  double transit_cost;
};

typedef adjacency_list<vecS, vecS, directedS, stop_t, link_t> transit_graph_t;

typedef typename graph_traits<transit_graph_t>::vertex_descriptor vertex_descriptor;
typedef typename graph_traits<transit_graph_t>::edge_descriptor edge_descriptor;
