#pragma once

#include "transit_data.h"

#include <ctime>
using namespace std;

#include <boost/graph/adjacency_list.hpp>
using namespace boost;

struct stop_t {
  time_t arrival_time;
  unsigned int predecessor; //FIXME: Type should be vertex_descriptor
};

struct link_t {
  char *trip_id;
  time_seconds_t transit_cost;
};

struct transit_t {
  char *schema;
};

typedef adjacency_list<vecS, vecS, directedS, stop_t, link_t, transit_t> transit_graph_t;

typedef typename graph_traits<transit_graph_t>::vertex_descriptor vertex_descriptor;
typedef typename graph_traits<transit_graph_t>::edge_descriptor edge_descriptor;
