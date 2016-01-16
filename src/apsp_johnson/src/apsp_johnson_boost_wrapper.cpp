/*
 * APSP Johnson algorithm for PostgreSQL
 *
 * Copyright (c) 2011 J Kishore Kumar
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <boost/config.hpp>
#include <boost/version.hpp>
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#if BOOST_VERSION > 103900
#include <boost/property_map/property_map.hpp>
#else
#include <boost/vector_property_map.hpp>
#endif
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>

#include "apsp_johnson.h"

#include <cmath>    // for sqrt
using namespace std;
using namespace boost;

// Maximal number of nodes in the path (to avoid infinite loops)
#define MAX_NODES 100000000

struct Edge {
  int source;
  int target;
  float cost;
};

// Adds an edge to the graph.
template<class G, class E>
static void graph_add_edge(G &graph, int source, int target, float cost) {
  E e;
  bool inserted;

  if (cost < 0.0f) // edges are not inserted in the graph if cost is negative
    return;

  tie(e, inserted) = add_edge(source, target, graph);
  graph[e].source = source;
  graph[e].target = target;
  graph[e].cost = cost;
}

int boost_apsp_johnson(edge_apsp_johnson_t *edges, int count,
    edge_apsp_johnson_t **output_edges, int *output_count, char **err_msg) {
try {
  int i, j;
  int V;
  bool ret;

  vector<edge_apsp_johnson_t>::iterator it;

  // FIXME: use a template for the directedS parameters
  typedef adjacency_list<vecS, vecS, directedS, no_property, Edge> graph_t;

  // typedef graph_traits<graph_t>::vertex_descriptor vertex_descriptor;
  typedef graph_traits<graph_t>::edge_descriptor edge_descriptor;
  // typedef graph_traits<graph_t>::edge_iterator edge_iterator;

  graph_t graph;

  for (int j = 0; j < count; ++j) {

    graph_add_edge<graph_t, edge_descriptor>(graph, edges[j].source,
        edges[j].target, edges[j].cost);
  }

  V = num_vertices(graph);
  float **D = (float **) malloc(sizeof(float *) * V);
  for (i = 0; i < V; i++) {
    D[i] = (float *) malloc(sizeof(float) * V);
  }

  std::vector<float> d(V, (std::numeric_limits<float>::max)());

  graph_traits<graph_t>::edge_iterator ei, eend;
  for (tie(ei, eend) = boost::edges(graph); ei != eend; ei++) {
    dbg("Input Edge %d -> %d cost = %f", graph[*ei].source, graph[*ei].target,
        graph[*ei].cost);
  }

  dbg("Calling johnson_all_pairs_shortest_paths boost library function");
  ret = johnson_all_pairs_shortest_paths(graph, D,
      distance_map(&d[0]).weight_map(get(&Edge::cost, graph)));
  if (ret) {
    dbg("Johnson returned true");
  } else {
    dbg("Johnson returned false");
  }

  vector<edge_apsp_johnson_t> output_vector;

  for (i = 0; i < V; i++) {
    for (j = 0; j < V; j++) {
      if (D[i][j] != std::numeric_limits<float>::max()) {
        dbg("%d -> %d >= %f\n", i, j, D[i][j]);
        edge_apsp_johnson_t new_edge;
        new_edge.source = i;
        new_edge.target = j;
        new_edge.cost = D[i][j];
        output_vector.push_back(new_edge);
      }
    }
  }
  // FIXME: Double copy from Matrix to Vector and Vector to Array. Optimise!
  dbg("output_vector.size() = %d", output_vector.size());
  *output_edges = (edge_apsp_johnson_t *) (malloc(
      sizeof(edge_apsp_johnson_t) * output_vector.size()));

  for (i = 0, it = output_vector.begin(); it < output_vector.end(); it++, i++) {
    (*output_edges)[i] = *it;
  }
  *output_count = i;
  return 0;
 }
 catch(...) {
     *err_msg = (char *) "Unknown exception caught!";
     return -1;
 }
}
