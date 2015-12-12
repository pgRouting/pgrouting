#ifndef GRAPH_MINIMIZER_H_
#define GRAPH_MINIMIZER_H_


#include <deque>
#include <vector>
#include <set>
#include <iostream>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "../../common/myGraph.hpp"
#include "../../common/structs.h"
using namespace std;
using namespace boost;
template < class G >
class Graph_Minimizer
:public My_base_graph<G> {
public:
  //constrictor for this class which inherits the base graph
  explicit Graph_Minimizer(graphType gtype, const int initial_size)
  :My_base_graph<G>(gtype, initial_size) {}

  //initializes the graph with the given edges
  void
  initialize_graph(Edge *data_edges, int64_t count) {
    this->insert_data(data_edges, count);
  }





  



};
#endif