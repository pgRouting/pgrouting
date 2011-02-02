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

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <shooting_star_search.hpp>

#include "shooting_star.h"

#include <cmath>    // for sqrt

using namespace std;
using namespace boost;

struct Edge
{
  int id;
  int source;
  int target;
  float8 cost;
  float distance;
  float rank;
  std::map< int, vector< std::pair<float, std::vector<int> > >, std::less<int> > adjacent_edges;
  default_color_type color;
  
  std::size_t index;
};
	
struct Vertex
{
  int id;
  float8 x;
  float8 y;
};

// exception for termination
template <class Edge>
class found_goal 
{
  public:
    found_goal() {}
    found_goal(Edge target) : target_edge(target) {}
    found_goal(const found_goal &fg) {}
    ~found_goal() {}
    Edge get_target()
    {
      return target_edge;
    }
  private:
    Edge target_edge;
};


// visitor that terminates when we find the goal
template <class Edge>
class shooting_star_goal_visitor : public boost::default_shooting_star_visitor
{
public:
  shooting_star_goal_visitor(Edge goal, int max_id) : m_goal(goal){}
  shooting_star_goal_visitor(const shooting_star_goal_visitor &gv) : m_goal(gv.m_goal){}
  ~shooting_star_goal_visitor(){}

  template <class Graph>
  void examine_edge(Edge e, Graph& g, int e_max_id) 
  {
    if( g[e].id == g[m_goal].id || g[e].id == g[m_goal].id + e_max_id )
    {
      throw found_goal<Edge>(e);
    }
  }
  template <class Graph>
  void finish_edge(Edge e, Graph& g) {}
private:
  Edge m_goal;
  int e_max_id;
};

// Heuristic function
template <class Graph, class CostType>
class distance_heuristic
{
public:
  typedef typename graph_traits<Graph>::edge_descriptor Edge;
  distance_heuristic(Graph& g, Edge goal):m_g(g), m_goal(goal){}
  CostType operator()(Edge e)
  {
    CostType dx = m_g[source(m_goal, m_g)].x - m_g[source(e, m_g)].x;
    CostType dxt = m_g[target(m_goal, m_g)].x - m_g[target(e, m_g)].x;
    CostType dy = m_g[source(m_goal, m_g)].y - m_g[source(e, m_g)].y;
    CostType dyt = m_g[target(m_goal, m_g)].y - m_g[target(e, m_g)].y;
    //You can choose any heuristical function from below
    //return ::max(dx, dy);
    //return ::sqrt(dx * dx + dy * dy)/2;
    //return 0;
    //return (min(::fabs(dx),::fabs(dxt))+min(::fabs(dy),::fabs(dyt)))/2;
    return sqrt(pow(min(::fabs(dx),::fabs(dxt)),2)+pow(min(::fabs(dy),::fabs(dyt)),2))/2;
  } 
private:
  Graph& m_g;
  Edge m_goal;
};


// Adds an edge to the graph
// Also copies all attributes and adjacent edges
template <class G, class E>
static void
graph_add_edge(G &graph, int index,
               int id, int source, int target, 
	       float8 cost, float8 s_x, float8 s_y, float8 t_x, float8 t_y, 
	       std::map< int, vector< std::pair<float, vector<int> > >, std::less<int> > adjacent_edges)
{

  E e;
  bool inserted;

  if (cost < 0) // edges are inserted as unpassable if cost is negative
    cost = MAX_COST;

  tie(e, inserted) = add_edge(source, target, graph);

  graph[e].cost = cost;
  graph[e].id = id;
  
  graph[e].source = source;
  graph[e].target = target;
  
  graph[e].adjacent_edges = adjacent_edges;

  graph[e].rank = 0;
  graph[e].distance = 0;
  
  graph[e].index = index;

  
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

// Main Shooting* function.
// It renumbers vertices, fills the graph with edges,
// calculates a route and return a result.
int 
boost_shooting_star(edge_shooting_star_t *edges_array, unsigned int count, 
	    int source_edge_id, int target_edge_id,
	    bool directed, bool has_reverse_cost,
	    path_element_t **path, int *path_count, char **err_msg, int e_max_id)
{

  typedef adjacency_list<vecS, vecS, directedS, Vertex, Edge> graph_t;

  typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
  typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;

  const unsigned int num_nodes = count*2;
  
  int z;
  int src, trg, offset, rule_num;
  
  graph_t graph(num_nodes);

  std::map< int, vector< std::pair<float, vector<int> > >, std::less<int> > adjacent_edges;

  std::map< int, int, std::less<int> > vertices;
  
  vector<int> rule;

  offset = 1;
  rule_num = 0;

  for (std::size_t j = 0; j < count; ++j)
  {
    //Vertex ids renumbering moved here
    src = edges_array[j].source;
    trg = edges_array[j].target;
    
    if(vertices[src]==0)
    {
      vertices[src]=j+offset;
    }
    edges_array[j].source=vertices[src];
    
    if(vertices[trg]==0)
    {
      offset++;      
      vertices[trg]=j+offset;
    }
    edges_array[j].target=vertices[trg];
    
    for(z=0; z<MAX_RULE_LENGTH;++z)
    {
      if(edges_array[j].rule[z] > 0)
      {
        rule.push_back(edges_array[j].rule[z]);
      }
    }

    if(edges_array[j].to_cost > 0)
    {
      adjacent_edges[edges_array[j].id].push_back(std::pair<float8, vector<int> > (edges_array[j].to_cost, rule) );
      rule.clear();
    }

    if((j < count-1 && edges_array[j].id != edges_array[j+1].id)||(j==count-1))
    {
      graph_add_edge<graph_t, edge_descriptor>(graph, j,
					       edges_array[j].id, edges_array[j].source, 
					       edges_array[j].target, edges_array[j].cost, 
					       edges_array[j].s_x, edges_array[j].s_y, 
					       edges_array[j].t_x, edges_array[j].t_y, adjacent_edges);
    
      // if the edge is not directed or if it is directed and has reverse cost
      if (!directed || (directed && has_reverse_cost))
      {
        float8 cost, reverse_cost;
		
        if (has_reverse_cost)
        {
          cost = edges_array[j].cost;
		  reverse_cost = edges_array[j].reverse_cost;
		  
		  //If chosen source/target edge's cost is high, take the edge for opposite direction
		  if(cost > reverse_cost)
		  {
			  if(edges_array[j].id == source_edge_id)
				source_edge_id += e_max_id;
			  else if(edges_array[j].id == target_edge_id)
				target_edge_id += e_max_id;
		  }
        }
        else
        {
          cost = edges_array[j].cost;
        }


      if(adjacent_edges[edges_array[j].id].size() > 0)
      {
	    adjacent_edges[edges_array[j].id+e_max_id].assign( adjacent_edges[edges_array[j].id].begin(), adjacent_edges[edges_array[j].id].end() );
	    adjacent_edges.erase(edges_array[j].id);
      }


        graph_add_edge<graph_t, edge_descriptor>(graph, j,
	                                       edges_array[j].id+e_max_id, edges_array[j].target, 
					       edges_array[j].source, cost, 
					       edges_array[j].s_x, edges_array[j].s_y, 
					       edges_array[j].t_x, edges_array[j].t_y, adjacent_edges);
      }

    adjacent_edges.clear();
    rule_num = 0;
    }
    else
    {
      rule_num++;
    }
  }
  
  
  edge_descriptor source_edge;
  edge_descriptor target_edge;
  
  bool source_found = false, target_found = false;
  
  graph_traits<graph_t>::edge_iterator ei, ei_end;

  for(tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei) 
  {
    if(graph[*ei].id == source_edge_id || graph[*ei].id == source_edge_id - e_max_id)
    {
      source_edge = *ei;
      source_found = true;
      break;
    }

  }

  if (!source_found) 
  {
    *err_msg = (char *) "Source edge not found";
    return -2;
  }


  for(tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei) 
  {
    if(graph[*ei].id == target_edge_id || graph[*ei].id == target_edge_id - e_max_id)
    {
      target_edge = *ei;
      target_found = true;
      break;
    }
  }


  if (!target_found)
  {
    *err_msg = (char *) "Target edge not found";
    return -3;
  }

  property_map<graph_t, std::size_t Edge::*>::type edge_index = get(&Edge::index, graph);

  std::map< int, edge_descriptor, std::less<int> > predecessors;
  
  property_map<graph_t, float Edge::*>::type rank = get(&Edge::rank, graph);
  property_map<graph_t, float Edge::*>::type distance = get(&Edge::distance, graph);

  try 
  {
    //calling Shooting* search
    shooting_star_search
      (graph, source_edge,
       distance_heuristic<graph_t, float>(graph, target_edge),
       weight_map(get(&Edge::cost, graph)).
       weight_map2(get(&Edge::adjacent_edges, graph)).
       edge_color_map(get(&Edge::color, graph)).
       visitor(shooting_star_goal_visitor<edge_descriptor>(target_edge, e_max_id)),
       edge_index,
       distance, rank,
       predecessors, e_max_id
       );

  } 
  catch(found_goal<edge_descriptor> &fg) 
  {
  
    vector<edge_descriptor> path_vect;
    int max = MAX_NODES;
    
    target_edge = fg.get_target();

    path_vect.push_back(target_edge);
    
    while (target_edge != source_edge) 
      {
        
        if ((target_edge == predecessors[graph[target_edge].id]) && (predecessors[graph[target_edge].id] != source_edge))
	{
          *err_msg = (char *) "No path found";
          return -1;
	    
	}
  
	target_edge = predecessors[graph[target_edge].id];
	
	//Check if we have u-turns within same edge at the beginning
	if( !(abs(graph[predecessors[graph[target_edge].id]].id - graph[target_edge].id) == e_max_id && (target_edge != source_edge || predecessors[graph[target_edge].id] != source_edge)) )
    {   
        path_vect.push_back(target_edge);
	}
	
	// This check was made to be sure that we can
	// restore the path from the target edge within
	// MAX_NODE iterations.
	// Sometimes it doesn't work properly and search exits here
	// even if the target edge was reached.

        if (!max--) 
	  {
            *err_msg = (char *) "No path found";
            return -1;
	  }	  
      }

    *path = (path_element_t *) malloc(sizeof(path_element_t) * 
				      (path_vect.size() + 1));
    *path_count = path_vect.size();
    
    int start_from = path_vect.size() - 1;
    
    for(int i = start_from, j = 0; i >= 0; i--, j++)
    {
      float cost;
	  graph_traits < graph_t >::edge_descriptor e;

      e = path_vect.at(i);
      
      if(graph[e].id > e_max_id)
      {
        graph[e].id -= e_max_id;
      }
      
      (*path)[j].edge_id = graph[e].id;
      (*path)[j].cost = graph[e].cost;
      (*path)[j].vertex_id = source(e, graph);
    }

    return EXIT_SUCCESS;
  }

  *err_msg = (char *) "Target was not reached";
  return 0;

}

