#include "scheduled_route.h"

#include "transit_graph.hpp"
#include "nonconst_astar_search.hpp"

#include <stdio.h>

void populate_next_links(vertex_descriptor u, transit_graph_t &g) {
  bool added;
  edge_descriptor e;
  trip *t;

  dbg("arrival time in populate_next_links = %d", g[u].arrival_time);
  int count = fetch_next_links(g[graph_bundle].schema, u, g[u].arrival_time,
      &t);
  if (count == 0) {
    dbg("%d is an absorbing node at time %d", u, g[u].arrival_time);
  } else {
    for (int i = 0; i < count; i++) {
      tie(e, added) = add_edge(t[i].src, t[i].dest, g);
      g[e].trip_id = t[i].trip_id;
      g[e].transit_cost = t[i].link_cost;
    }
    free(t);
    dbg("Successfully populated next links of %d", u);
  }
}

struct found_goal {
};

class shortest_time_heuristic: public astar_heuristic<transit_graph_t, int> {
public:
  shortest_time_heuristic(transit_graph_t &g, vertex_descriptor goal) {
    m_st = (unsigned int *) malloc(sizeof(double) * num_vertices(g)); //FIXME: Use palloc if possible
    for (unsigned int i = 0; i < num_vertices(g); i++) {
      m_st[i] = 0;
    }
    fetch_shortest_time(g[graph_bundle].schema, goal, m_st);
  }
  int operator ()(vertex_descriptor u) {
    return m_st[u];
  }
  virtual ~shortest_time_heuristic() {
    //free(m_st); //FIXME: Adding this line hangs the server(futex_wait_queue_me if it helps) indefinitely
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
    dbg("Examining vertex %d", u);
    if (u == m_goal) {
      throw found_goal();
    }
    populate_next_links(u, g);
  }
  void edge_relaxed(edge_descriptor e, transit_graph_t &g) {
    dbg("Edge %s relaxed", g[e].trip_id);
    vertex_descriptor v = target(e, g);
    g[v].predecessor_trip_id = g[e].trip_id;
  }
  void edge_not_relaxed(edge_descriptor e, const transit_graph_t &g) {
    //dbg("Edge %s not relaxed", g[e].trip_id);
  }
private:
  vertex_descriptor m_goal;
};

int compute_scheduled_route(char *gtfs_schema, int source, int destination,
    time_t query_time, gtfs_path_element_t **path_ref, int *path_count) {
  gtfs_path_element_t *path;
  int i;
  const unsigned int NUM_NODES = get_max_stop_id(gtfs_schema);
  //const time_t MAX_TIME = numeric_limits<time_t>::max();
  const int MAX_TIME = numeric_limits<int>::max();
  dbg("MAX_TIME = %d", MAX_TIME);

  dbg("sizeof c++ int = %d\n", sizeof(int));
  dbg("query_time inside compute_scheduled_route = %d", query_time);

  transit_graph_t g(NUM_NODES);
  g[graph_bundle].schema = gtfs_schema;

  for (unsigned int i = 0; i < NUM_NODES; i++) {
    g[vertex(i, g)].predecessor = vertex(i, g);
    g[vertex(i, g)].arrival_time = MAX_TIME;
  }

  g[vertex(source, g)].arrival_time = query_time;

  try {
    astar_search_no_init(g, source, shortest_time_heuristic(g, destination),
        transit_graph_visitor(destination), // visitor
        get(&stop_t::predecessor, g), // predecessor
        get(&stop_t::cost, g), // cost
        get(&stop_t::arrival_time, g), // distance
        get(&link_t::transit_cost, g), // weight
        get(&stop_t::color, g), // color
        get(vertex_index, g), // index
        std::less<time_t>(), // compare
        closed_plus<int>(), // combine
        numeric_limits<time_t>::max(), // inf
        0 // zero
        );
  } catch (found_goal &) {
    dbg("Found goal");

    vertex_descriptor v = destination;
    list<vertex_descriptor> changeovers;
    list<vertex_descriptor>::iterator ci, ci_end;

    changeovers.push_front(v);
    do {
      v = g[v].predecessor;
      changeovers.push_front(v);
    } while (g[v].predecessor != v);

    dbg("Changeovers:");
    for (ci = changeovers.begin(), ci_end = changeovers.end(); ci != ci_end;
        ci++) {
      dbg("%d", *ci);
    }
    *path_count = changeovers.size();
    *path_ref = (gtfs_path_element_t *) malloc(
        sizeof(gtfs_path_element_t) * *path_count); // FIXME: Try using palloc
    path = *path_ref;
    ci = changeovers.begin();
    path[0].stop_id = *ci;
    ci++;
    for (i = 1; i < *path_count; i++, ci++) {
      vertex_descriptor v = *ci;
      path[i - 1].trip_id = g[v].predecessor_trip_id;
      path[i].stop_id = v;
    }
    path[i - 1].trip_id = NULL;
    return 0;
  }
  dbg("Goal not found");
  *path_count = 0;
  return 0;
}
