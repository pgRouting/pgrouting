/*
 * Shooting* Shortest path algorithm for PostgreSQL
 *
 * Copyright (c) 2007 Anton A. Patrushev, Orkney, Inc.
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
#include <iostream>
#include <fstream>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/vector_property_map.hpp>
#include <shooting_star_search.hpp>

#include "shooting_star.h"

#include <math.h>    // for sqrt

using namespace std;
using namespace boost;

// Maximal number of nodes in the path (to avoid infinite loops)
#define MAX_NODES 1000000

struct Edge
{
  int id;
  int source;
  int target;
  float8 cost;
  float8 distance;
  float8 rank;
  std::map< int, std::pair<float8, std::vector<int> >, std::less<int> > adjacent_edges;

  std::vector< int > history;

  default_color_type color;

  adjacency_list_traits<vecS, vecS, directedS>::edge_descriptor predecessor;
};
	
struct Vertex
{
  int id;
  float8 x;
  float8 y;

  default_color_type color;
  
  float8 cost;
  
};


struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Edge>
class shooting_star_goal_visitor : public boost::default_shooting_star_visitor
{
public:
  shooting_star_goal_visitor(Edge goal) : m_goal(goal) {}

  template <class Graph>
  void examine_edge(Edge e, Graph& g) {
    if(g[e].id == g[m_goal].id)
      throw found_goal();
  }
  template <class Graph>
  void finish_edge(Edge e, Graph& g) {}
private:
  Edge m_goal;
};


template <class Graph, class CostType>
class distance_heuristic : public astar_heuristic<Graph, CostType>
{
public:
  typedef typename graph_traits<Graph>::edge_descriptor Edge;
  distance_heuristic(Graph& g, Edge goal):m_g(g), m_goal(goal){}
  CostType operator()(Edge e)
  {
    CostType dx = m_g[target(m_goal, m_g)].x - m_g[target(e, m_g)].x;
    CostType dy = m_g[target(m_goal, m_g)].y - m_g[target(e, m_g)].y;
    //You can choose any heuristical function from below
    //return ::max(dx, dy);
    //return ::sqrt(dx * dx + dy * dy)/2;
    //return 0;
    return (::fabs(dx)+::fabs(dy))/2;
  } 
private:
  Graph& m_g;
  Edge m_goal;
};


template <class G, class E>
static void
graph_add_edge(G &graph, int id, int source, int target, 
	       float8 cost, float8 s_x, float8 s_y, float8 t_x, float8 t_y, 
	       std::map< int, std::pair<float8, vector<int> >, std::less<int> > adjacent_edges)
{

  E e;
  bool inserted;

  if (cost < 0) // edges are not inserted in the graph if cost is negative
    return;

  tie(e, inserted) = add_edge(source, target, graph);
  
  graph[e].cost = cost;
  graph[e].id = id;
  
  graph[e].source = source;
  graph[e].target = target;
  
  graph[e].adjacent_edges = adjacent_edges;
  
  typedef typename graph_traits<G>::vertex_descriptor Vertex;
  Vertex s = vertex(source, graph);
  Vertex t = vertex(target, graph);

  graph[s].id=source;
  graph[t].id=target;

  graph[s].x=s_x;
  graph[s].y=s_y;
  graph[t].x=t_x;
  graph[t].y=t_y;

}

int 
boost_shooting_star(edge_shooting_star_t *edges_array, unsigned int count, 
	    int source_edge_id, int target_edge_id,
	    bool directed, bool has_reverse_cost,
	    path_element_t **path, int *path_count, char **err_msg, int e_max_id)
{

  typedef adjacency_list<vecS, vecS, directedS, Vertex, Edge> graph_t;

  typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
  typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;

  const unsigned int num_nodes = /*1*/count*2;
  
  int z;
  int src, trg, offset;
  
  graph_t graph(num_nodes);

  std::map< int, std::pair<float8, vector<int> >, std::less<int> > adjacent_edges;

  std::map< int, int, std::less<int> > vertices;

  offset = 0;
  
  for (std::size_t j = 0; j < count; ++j)
  {
    //Vertex ids renumbering moved here
    
    src = edges_array[j].source;
    trg = edges_array[j].target;

    if(vertices[src]==NULL)
    //if (vertices.find(edges_array[j].source) == vertices.end())
    {
      vertices[src]=j+offset;
      edges_array[j].source=j+offset;
      
    }
    else
    {
      edges_array[j].source=vertices[src];
    }

    if(vertices[trg]==NULL)
    //if (vertices.find(edges_array[j].target) == vertices.end())    
    {
      offset++;
  
      vertices[trg]=j+offset;
      edges_array[j].target=j+offset;
    }
    else
    {
      edges_array[j].target=vertices[trg];
    }
    
    adjacent_edges[edges_array[j].id].first = edges_array[j].to_cost;

    for(z=0; z<MAX_RULE_LENGTH;++z)
    {
      if(edges_array[j].rule[z] > 0)
      {
        adjacent_edges[edges_array[j].id].second.push_back(edges_array[j].rule[z]);
      }
    }

    if((j < count-1 && edges_array[j].id != edges_array[j+1].id)||(j==count-1))
    {
      graph_add_edge<graph_t, edge_descriptor>(graph, 
					       edges_array[j].id, edges_array[j].source, 
					       edges_array[j].target, edges_array[j].cost, 
					       edges_array[j].s_x, edges_array[j].s_y, 
					       edges_array[j].t_x, edges_array[j].t_y, adjacent_edges);

      if (!directed || (directed && has_reverse_cost))
      {
        float8 cost;
		
        if (has_reverse_cost)
        {
          cost = edges_array[j].reverse_cost;
        }
        else
        {
          cost = edges_array[j].cost;
        }

        graph_add_edge<graph_t, edge_descriptor>(graph,
	                                       edges_array[j].id+e_max_id, edges_array[j].target, 
					       edges_array[j].source, cost, 
					       edges_array[j].s_x, edges_array[j].s_y, 
					       edges_array[j].t_x, edges_array[j].t_y, adjacent_edges);
      }

    adjacent_edges.clear();
    
    }
  }
    
  std::map< int, edge_descriptor, std::less<int> > predecessors;
  
  edge_descriptor source_edge;
  edge_descriptor target_edge;
  
  bool source_found = false, target_found = false;
  
  graph_traits<graph_t>::edge_iterator ei, ei_end;
    
  for(tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei) 
  {
    if(graph[*ei].id == source_edge_id)
    {
      source_edge = *ei;
      source_found = true;
      break;
    }
  }

  if (!source_found) 
  {
    *err_msg = "Source edge not found";
    return -1;
  }

  for(tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei) 
  {
    if(graph[*ei].id == target_edge_id)
    {
      target_edge = *ei;
      target_found = true;
      break;
    }
  }

  if (!target_found)
  {
    *err_msg = "Target edge not found";
    return -1;
  }

  std::vector<float8> distances(num_edges(graph));
  
  std::vector<default_color_type> edge_colors(num_edges(graph), color_traits<default_color_type>::white());

  property_map<graph_t, std::vector< int > Edge::*>::type history = get(&Edge::history, graph);

  property_map<graph_t, int Edge::*>::type edge_index = get(&Edge::id, graph);

  property_map<graph_t, float8 Edge::*>::type rank = get(&Edge::rank, graph);
  property_map<graph_t, float8 Edge::*>::type distance = get(&Edge::distance, graph);

  try 
  {
    shooting_star_search
      (graph, source_edge,
       distance_heuristic<graph_t, float>(graph, target_edge),
       weight_map(get(&Edge::cost, graph)).
       weight_map2(get(&Edge::adjacent_edges, graph)).
       vertex_color_map(get(&Vertex::color, graph)).
       edge_color_map(get(&Edge::color, graph)).
       visitor(shooting_star_goal_visitor<edge_descriptor>(target_edge)),
       edge_index,
       distance, rank,
       history, 
       predecessors, e_max_id
       );

  } 
  catch(found_goal fg) {
  
    vector<edge_descriptor> path_vect;
    int max = MAX_NODES;

    path_vect.push_back(target_edge);
    
    while (target_edge != source_edge) 
      {
        if ((target_edge == predecessors[graph[target_edge].id]) && (predecessors[graph[target_edge].id] != source_edge))
	{
          *err_msg = "No path found";
          return -1;
	    
	}
  
	target_edge = predecessors[graph[target_edge].id];

        if(target_edge == predecessors[graph[target_edge].id] )
          continue;

        path_vect.push_back(target_edge);

        if (!max--) 
	  {
            *err_msg = "Overflow";
            return -1;
	  }
      }

    *path = (path_element_t *) malloc(sizeof(path_element_t) * 
				      (path_vect.size() + 1));
    *path_count = path_vect.size();

    for(int i = path_vect.size() - 1, j = 0; i >= 0; i--, j++)
    {
      graph_traits < graph_t >::edge_descriptor e;

      e = path_vect.at(i);

      if(graph[e].id > e_max_id)
        graph[e].id -= e_max_id;

      (*path)[j].edge_id = graph[e].id;
      (*path)[j].cost = graph[e].cost;
      (*path)[j].vertex_id = source(e, graph);
    }

    return EXIT_SUCCESS;
  }

}

