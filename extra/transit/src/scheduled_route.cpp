#include "scheduled_route.h"

#include "transit_graph.hpp"
#include "nonconst_astar_search.hpp"

void populate_next_links(vertex_descriptor u, transit_graph_t &g) {
  bool added;
  edge_descriptor e;
  trip *t;

  int count = fetch_next_links(g[graph_bundle].schema, u, g[u].arrival_time,
      &t);
  if (count == 0) {
    cout << u << " is an absorbing node at time " << g[u].arrival_time << endl;
  }
  for (int i = 0; i < count; i++) {
    tie(e, added) = add_edge(vertex(t[i].src, g), vertex(t[i].dest, g), g);
    g[e].trip_id = t[i].trip_id;
    g[e].transit_cost = t[i].link_cost;
  }
}

struct found_goal {
};

class shortest_time_heuristic: public astar_heuristic<transit_graph_t, int> {
public:
  shortest_time_heuristic(transit_graph_t &g, vertex_descriptor goal) {
    m_st = (unsigned int *) malloc(sizeof(double) * num_vertices(g));
    for(unsigned int i = 0; i < num_vertices(g); i++)
    {
      m_st[i] = 0;
    }
    fetch_shortest_time(g[graph_bundle].schema, goal, m_st);
  }
  int operator ()(vertex_descriptor u) {
    return m_st[u];
  }
  virtual ~shortest_time_heuristic() {
    free(m_st);
  }
private:
  time_seconds_t *m_st;
};

class transit_graph_visitor: public default_astar_visitor {
public:
  transit_graph_visitor(vertex_descriptor goal) :
      m_goal(goal) {
  }
  void examine_vertex(vertex_descriptor u, transit_graph_t &g) {
    cout << "Examining vertex " << u << endl;
    if (u == m_goal)
      throw found_goal();
    populate_next_links(u, g);
  }
  void edge_relaxed(edge_descriptor e, const transit_graph_t &g) {
    cout << "Edge " << g[e].trip_id << " relaxed" << endl;
  }
  void edge_not_relaxed(edge_descriptor e, const transit_graph_t &g) {
    cout << "Edge " << g[e].trip_id << " not relaxed" << endl;
  }
private:
  vertex_descriptor m_goal;
};

int compute_scheduled_route(char *gtfs_schema, int source, int destination,
    time_t query_time, gtfs_path_element_t **path, int *path_count) {
  const unsigned int NUM_NODES = get_max_stop_id(gtfs_schema);
  const time_t MAX_TIME = numeric_limits<time_t>::max();

  transit_graph_t g(NUM_NODES);
  g[graph_bundle].schema = gtfs_schema;

  for (unsigned int i = 0; i < NUM_NODES; i++) {
    g[vertex(i, g)].arrival_time = MAX_TIME;
  }

  g[vertex(source, g)].arrival_time = query_time;

  try {
    astar_search(
        g,
        source,
        shortest_time_heuristic(g, destination),
        visitor(transit_graph_visitor(destination)).weight_map(
            get(&link_t::transit_cost, g)).distance_map(
            get(&stop_t::arrival_time, g)).predecessor_map(
            get(&stop_t::predecessor, g)));
  } catch (found_goal &) {
    cout << "Found goal" << endl;

    cout << "Predecessor map:" << endl;
    for (unsigned int i = 0; i < num_vertices(g); i++) {
      cout << i << " : " << g[vertex(i, g)].predecessor << endl;
    }

    cout << "Distance map:" << endl;
    for (unsigned int i = 0; i < num_vertices(g); i++) {
      cout << i << " : " << g[vertex(i, g)].arrival_time << endl;
    }

    vertex_descriptor v = destination;
    list<vertex_descriptor> changeovers;
    list<vertex_descriptor>::iterator ci, ci_end;

    changeovers.push_front(v);
    do {
      v = g[v].predecessor;
      changeovers.push_front(v);
    } while (g[v].predecessor != v);

    cout << "Changeovers:" << endl;
    for (ci = changeovers.begin(), ci_end = changeovers.end(); ci != ci_end;
        ci++) {
      cout << *ci << endl;
    }
    return 0;
  }
  cout << "Goal not found" << endl;

  return 0;
}
