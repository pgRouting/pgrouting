/*
 * Drivedist algorithm for PostgreSQL
 *
 * Copyright (c) 2006 Mario H. Basa, Orkney, Inc.
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
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */

#include <boost/config.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "drivedist.h"

using namespace std;
using namespace boost;


// Maximal number of nodes in the path (to avoid infinite loops)
#define MAX_NODES 1000000

struct Edge
{
  int id;
  float8 cost;
};

struct Vertex
{
  int id;
  int edge_id;
};

template <class G, class E>
static void
graph_add_edge(G &graph, int id, int source, int target, float8 cost)
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
  graph[s].id = source;
  graph[t].id = target;
  graph[s].edge_id = id;
  graph[t].edge_id = id;

}

int
boost_dijkstra_dist(edge_t *edges, unsigned int count, int source_vertex_id,
                    double rdistance, bool directed, bool has_reverse_cost,
                    path_element_t **path, int *path_count, char **err_msg) 
{
  typedef adjacency_list < listS, vecS, directedS, Vertex, Edge > graph_t;
  typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
  typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;

  const unsigned int num_nodes = 1;
  
  graph_t graph( num_nodes );
  
  property_map<graph_t, edge_weight_t>::type weightmap = 
    get(edge_weight, graph);
  
  for (std::size_t j = 0; j < count; ++j)
    {
      graph_add_edge<graph_t, edge_descriptor>
        (graph, edges[j].id, edges[j].source, 
         edges[j].target, edges[j].cost);      
                                        
      if (!directed || (directed && has_reverse_cost))
        {
          if (has_reverse_cost)
            {
              graph_add_edge<graph_t, edge_descriptor>
                (graph, edges[j].id, edges[j].target, 
                 edges[j].source, edges[j].reverse_cost);      
            }
          else 
            {
              graph_add_edge<graph_t, edge_descriptor>
                (graph, edges[j].id, edges[j].target, 
                 edges[j].source, edges[j].cost);      
            }
        }
    }

  vertex_descriptor source_vertex = vertex( source_vertex_id, graph );
  
  std::vector<vertex_descriptor> predecessors(num_vertices(graph));
  std::vector<float8> distances(num_vertices(graph));
  
  dijkstra_shortest_paths(graph, source_vertex,
                          predecessor_map(&predecessors[0])
                          .weight_map(get(&Edge::cost, graph))
                          .distance_map(&distances[0]));

  
  graph_traits < graph_t >::vertex_iterator vi, vend;
  vector<path_element> path_vector;
  int j=0;
  
  for(tie(vi, vend) = vertices(graph); vi != vend; vi++) {
                
    if( (double)distances[*vi] <= rdistance ) {
      
      path_element pe;

      graph_traits<graph_t>::vertex_descriptor s;

      s = vertex(*vi, graph);

      pe.vertex_id = graph[s].id;
      pe.edge_id   = graph[s].edge_id;
      pe.cost      = distances[*vi];
        
      path_vector.push_back( pe );
    }   
  }
  
  if( path_vector.size() == 0 ) {
    *err_msg = (char *)"No path found";
    return 0;          
  }
  
  vector<path_element>::iterator itr;
  *path = (path_element_t *) malloc( sizeof(path_element_t) * 
                                     (path_vector.size() + 1) );
  *path_count = path_vector.size();
  
  for(j=0,itr=path_vector.begin();itr != path_vector.end();itr++,j++) {
    path_element pe = *itr;
    (*path)[j].vertex_id = pe.vertex_id;
    (*path)[j].edge_id   = pe.edge_id;
    (*path)[j].cost      = pe.cost;
  }
  
  return EXIT_SUCCESS;
}

