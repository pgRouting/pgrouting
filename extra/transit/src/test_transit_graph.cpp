#include "transit_graph.hpp"
#include <boost/lexical_cast.hpp>

#ifdef WANT_MAIN
int main() {
  const unsigned int NUM_STOPS = 10;
  transit_graph_t g;

  for (unsigned int i = 0; i < NUM_STOPS; i++) {
    vertex_descriptor u = add_vertex(g);
    g[u].stop_id = lexical_cast<string>(u);
  }

  add_edge(1, 4, g);
  add_edge(2, 1, g);
  add_edge(2, 3, g);
  add_edge(3, 2, g);
  add_edge(3, 4, g);
  add_edge(4, 1, g);
  add_edge(4, 2, g);
  add_edge(3, 2, g);

  return 0;
}

#endif
