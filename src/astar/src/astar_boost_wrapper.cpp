/*PGR-GNU*****************************************************************

 * A* Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2006 Anton A. Patrushev, Orkney, Inc.
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

// Include C header first for windows build issue

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#ifdef unlink
#undef unlink
#endif
#endif


#if 0
extern "C" {
#include "postgres.h"
}
#endif

#include <boost/config.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/astar_search.hpp>

#include <cmath>    // for sqrt
#include "astar.h"

using namespace std;
using namespace boost;

// Maximal number of nodes in the path (to avoid infinite loops)
#define MAX_NODES 100000000

struct Edge
{
  int id;
  double cost;
  //double distance;
};
  
struct Vertex
{
  int id;
  double x;
  double y;
};


struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor
{
    public:
      astar_goal_visitor(Vertex goal) : m_goal(goal) {}
      template <class Graph>
      void examine_vertex(Vertex u, Graph& g) {
        if(u == m_goal)
          throw found_goal();
        num_vertices(g); 
      }
    private:
      Vertex m_goal;
};

// Heuristic function which tells us how far the current node is
// from the target node.
//
// (|dx|+|dy|)/2 is used currently.
template <class Graph, class CostType>
class distance_heuristic : public astar_heuristic<Graph, CostType>
{
    public:
      typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
      distance_heuristic(Graph& g, Vertex goal):m_g(g), m_goal(goal){}
      CostType operator()(Vertex u)
      {
        CostType dx = m_g[m_goal].x - m_g[u].x;
        CostType dy = m_g[m_goal].y - m_g[u].y;
        //You can choose any heuristical function from below
        //return ::max(dx, dy);
        //return ::sqrt(dx * dx + dy * dy)/4;
        //return 0;
        return (::fabs(dx)+::fabs(dy))/2;
      } 
    private:
      Graph& m_g;
      Vertex m_goal;
};


// Adds an edge to the graph.
// Edge id, cost, source and target ids and coordinates are copied also
template <class G, class E>
static void
graph_add_edge(G &graph, int id, int source, int target, 
         double cost, double s_x, double s_y, double t_x, double t_y)
{
  E e;
  bool inserted;
    
  if (cost < 0) // edges are not inserted in the graph if cost is negative
    return;

  tie(e, inserted) = add_edge(source, target, graph);

  graph[e].cost = cost;
  graph[e].id = id;

  typedef typename graph_traits<G>::vertex_descriptor Vertex;
  Vertex s = vertex(source, graph);
  Vertex t = vertex(target, graph);
  graph[s].x=s_x;
  graph[s].y=s_y;
  graph[t].x=t_x;
  graph[t].y=t_y;
}

int 
boost_astar(edge_astar_t *edges, size_t count, 
      int source_vertex_id, int target_vertex_id,
      bool directed, bool has_reverse_cost,
      path_element_t **path, size_t *path_count, char **err_msg)
{
try {

  // FIXME: use a template for the directedS parameters
  typedef adjacency_list < listS, vecS, directedS, Vertex, Edge> graph_t;

  typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
  typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;

  // FIXME: compute this value
  const  auto num_nodes = ((directed && has_reverse_cost ? 2 : 1) * 
          count) + 100;

  graph_t graph(num_nodes);

  // interfers with APPLE build, can it just be commented out?
  //property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, 
  //                   graph);

  for (std::size_t j = 0; j < count; ++j)
  {

      graph_add_edge<graph_t, edge_descriptor>(graph, 
                 edges[j].id, edges[j].source, 
                 edges[j].target, edges[j].cost, 
                 edges[j].s_x, edges[j].s_y, 
                 edges[j].t_x, edges[j].t_y);

      if (!directed || (directed && has_reverse_cost))
      {
        double cost;

        if (has_reverse_cost)
        {
            cost = edges[j].reverse_cost;
        }
        else 
        {
            cost = edges[j].cost;
        }

        graph_add_edge<graph_t, edge_descriptor>(graph, 
               edges[j].id, 
               edges[j].target, 
               edges[j].source, 
               cost, 
               edges[j].t_x, 
               edges[j].t_y, 
               edges[j].s_x, 
               edges[j].s_y);
        }
  }

  std::vector<vertex_descriptor> predecessors(num_vertices(graph));

  vertex_descriptor source_vertex = vertex(source_vertex_id, graph);

  if ((long)source_vertex < 0) 
  {
      *err_msg = (char *) "Source vertex not found";
      return -1;
  }

  vertex_descriptor target_vertex = vertex(target_vertex_id, graph);
  if ((long)target_vertex < 0)
  {
      *err_msg = (char *) "Target vertex not found";
      return -1;
  }

  std::vector<double> distances(num_vertices(graph));

  try {
    // Call A* named parameter interface
    astar_search
      (graph, source_vertex,
       distance_heuristic<graph_t, double>(graph, target_vertex),
       predecessor_map(&predecessors[0]).
       weight_map(get(&Edge::cost, graph)).
       distance_map(&distances[0]).
       visitor(astar_goal_visitor<vertex_descriptor>(target_vertex)));

  } 
  catch(found_goal& fg) {
    // Target vertex found
    vector<vertex_descriptor> path_vect;
    int max = MAX_NODES;
    path_vect.push_back(target_vertex);
  
    while (target_vertex != source_vertex) 
    {
        if (target_vertex == predecessors[target_vertex]) 
        {
            *err_msg = (char *) "No path found";
            return 0;
      
        }
        target_vertex = predecessors[target_vertex];

        path_vect.push_back(target_vertex);
        if (!max--) 
        {
            *err_msg = (char *) "Overflow";
            return -1;
        }
    }

    *path = (path_element_t *) malloc(sizeof(path_element_t) * 
              (path_vect.size() + 1));
    *path_count = path_vect.size();

    for(int64_t i = static_cast<int64_t>(path_vect.size()) - 1, j = 0; i >= 0; i--, j++)
    {
        graph_traits < graph_t >::vertex_descriptor v_src;
        graph_traits < graph_t >::vertex_descriptor v_targ;
        graph_traits < graph_t >::edge_descriptor e;
        graph_traits < graph_t >::out_edge_iterator out_i, out_end;

        (*path)[j].vertex_id = static_cast<int>(path_vect.at(i));

        (*path)[j].edge_id = -1;
        (*path)[j].cost = distances[target_vertex];
        
        if (i == 0) 
        {
            continue;
        }

        v_src = path_vect.at(i);
        v_targ = path_vect.at(i - 1);
        double cost = 99999999.9;
        int edge_id = 0;

        for (tie(out_i, out_end) = out_edges(v_src, graph); 
             out_i != out_end; ++out_i)
        {
            graph_traits < graph_t >::vertex_descriptor targ; // v   set but not used
            e = *out_i;
            // v = source(e, graph);
            targ = target(e, graph);
                                                                
            if (targ == v_targ)
            {
                if (graph[*out_i].cost < cost)
                {
                    edge_id = graph[*out_i].id;
                    cost = graph[*out_i].cost;
                }
            }
        }
        (*path)[j].edge_id = edge_id;
        (*path)[j].cost = cost;
    }

    return EXIT_SUCCESS;
  }
 }
 catch(...) {
    *err_msg = (char *) "Unknown exception caught!";
    return -1;
 }
 *err_msg = (char *) "No path found";
 return 0;
}

