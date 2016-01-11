/*PGR

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

*/

#ifndef SRC_COMMON_SRC_BASE_GRAPH_HPP_
#define SRC_COMMON_SRC_BASE_GRAPH_HPP_

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <deque>
#include <vector>
#include <set>
#include <map>
#include <limits>

#include "postgres.h"
#include "./structs.h"

/*! \brief boost::graph simplified to pgRouting needs

This class gives the handling basics of a boost::graph of kind G
where G:
 can be an undirected graph or a directed graph.

Usage:
======

 Given the following types:
~~~~{.c}
 typedef boost::adjacency_list < boost::vecS, boost::vecS,
            boost::undirectedS,
            boost_vertex_t, boost_edge_t > UndirectedGraph;

 typedef boost::adjacency_list < boost::vecS, boost::vecS,
            boost::bidirectionalS,
            boost_vertex_t, boost_edge_t > DirectedGraph;
~~~~

direct usage:
---------------

~~~~{.c}
    Pgr_base_graph < DirectedGraph > digraph(gType, initial_size);
    Pgr_base_graph < UndirectedGraph > undigraph(gType, initial_size);
~~~~

usage by inheritance:
---------------------

~~~~{.c}
 class my_graph: public Pgr_base_graph {
   explicit my_graph(graphType gtype, const int initial_size)
     :Pgr_base_graph< G >(gtype, initial_size) {}
   // the class: my_graph will have the functionality of this class
 }
 
 my_graph < DirectedGraph > digraph(gType, initial_size);
 my_graph < UndirectedGraph > undigraph(gType, initial_size);
~~~~

*/
using namespace std;
template <class G>
class Pgr_base_graph {
 public:
  /** @name Graph related types
  Type      |     boost meaning     |   pgRouting Meaning
:---------: | :-------------------- | :----------------------
   G        | boost::adjacency_list |   Graph
   V        | vertex_descriptor     |   Think of it as local ID of a vertex
   E        | edge_descriptor       |   Think of it as local ID of an edge
   V_i      | vertex_iterator       |   To cycle the vertices of the Graph
   E_i      | edge_iterator         |   To cycle the edges of the Graph
   EO_i     | out_edge_iterator     |   To cycle the out going edges of a vertex
   EI_i     | in_edge_iterator      |   To cycle the in coming edges of a vertex (only in bidirectional graphs)
  */
  //@{
  typedef typename boost::graph_traits < G >::vertex_descriptor V;
  typedef typename boost::graph_traits < G >::edge_descriptor E;
  typedef typename boost::graph_traits < G >::vertex_iterator V_i;
  typedef typename boost::graph_traits < G >::edge_iterator E_i;
  typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
  typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;
  typedef typename std::map< int64_t, std::vector<V> > degree_to_V;
  typedef typename std::map< int64_t, std::vector<V> >::iterator degree_to_V_i;
  typedef typename std::vector<V>::iterator Q_i;
  //@}

  /** @name Id handling related types
  Type      |  Meaning       |   pgRouting Meaning
:---------: | :------------- | :----------------------
   id_to_V  | maps id -> V   | given an id store the V
   V_to_id  | maps V -> id   | given a V store the id
   LI       | Left Iterator  | iterates over id_to_V
   RI       | right Iterator | iterates over V_to_id
  */
  //@{
  typedef typename std::map< int64_t, V > id_to_V;
  typedef typename std::map< V, int64_t > V_to_id;
  typedef typename id_to_V::const_iterator LI;
  typedef typename V_to_id::const_iterator RI;

  //@}

  //! @name The Graph
  //@{
  G graph;                //!< The graph
  int64_t numb_vertices;  //!< number of vertices
  graphType m_gType;      //!< type (DIRECTED or UNDIRECTED)
  //@}
  degree_to_V degree_to_V_map;
  //! @name Id handling
  //@{
  id_to_V  vertices_map;   //!< id -> graph id
  V_to_id  gVertices_map;  //!< graph id -> id
  //@}

  //! @name Graph Modification
  //@{
  //! Used for storing the removed_edges
  std::deque<Edge> removed_edges;
  //@}

  //! @name Used by dijkstra
  //@{
  std::vector<V> predecessors;
  std::vector<float8> distances;
  std::deque<V> nodesInDistance;
  //@}

  //! @name The Graph
  //@{
  //! \brief Constructor
  /*!
    Prepares the _graph_ to be of type _gtype_ with the
    aproximate number of vertices its coing to have as *initial_size*
  */
  explicit Pgr_base_graph< G >(graphType gtype, const int initial_size)
     : graph(initial_size),
       numb_vertices(0),
       m_gType(gtype)
     {}

  //! \brief Inserts *count* edges of type *pgr_edge_t* into the graph
  void graph_insert_data(const Edge *data_edges, int64_t count) {
      for (unsigned int i = 0; i < count; ++i) {
         graph_add_edge(data_edges[i]);
      }
      for ( int64_t i = 0; (unsigned int) i < gVertices_map.size(); ++i )
         graph[i].id = gVertices_map.find(i)->second;
        calculateDegrees();
  }

  //! \brief Disconnects all edges from p_from to p_to
  /*!
    - No edge is disconnected if the vertices id's do not exist in the graph
    - All removed edges are stored for future reinsertion
    - All parallel edges are disconnected (automatically by boost)

    ![disconnect_edge(2,3) on an UNDIRECTED graph](disconnectEdgeUndirected.png)
    ![disconnect_edge(2,3) on a DIRECTED graph](disconnectEdgeDirected.png)

    @param [IN] *p_from* original vertex id of the starting point of the edge
    @param [IN] *p_to*   original vertex id of the ending point of the edge
  */
  void disconnect_edge(int64_t p_from, int64_t p_to) {
      V g_from;
      V g_to;
      Edge d_edge;
      // nothing to do, the vertex doesnt exist
      if (!get_vertex_descriptor(p_from, g_from)) return;
      if (!get_vertex_descriptor(p_to, g_to)) return;
      EO_i out, out_end;
      // store the edges that are going to be removed
      for (boost::tie(out, out_end) = out_edges(g_from, graph);
           out != out_end; ++out) {
            if (target(*out, graph) == g_to) {
                d_edge.id = graph[*out].id;
                d_edge.source = graph[source(*out, graph)].id;
                d_edge.target = graph[target(*out, graph)].id;
                d_edge.cost = graph[*out].cost;
                d_edge.revcost = -1;
                removed_edges.push_back(d_edge);
            }
      }
      // the actual removal
      boost::remove_edge(g_from, g_to, graph);
  }

void remove_vertex(int64_t p_vertex) {
      V g_vertex;
      Edge d_edge;
      // nothing to do, the vertex doesnt exist
      if (!get_vertex_descriptor(p_vertex, g_vertex)) return;
      EO_i out, out_end;
      // store the edges that are going to be removed
      for (boost::tie(out, out_end) = out_edges(g_vertex, graph);
        out != out_end; ++out) {
            d_edge.id = graph[*out].id;
            d_edge.source = graph[source(*out, graph)].id;
            d_edge.target = graph[target(*out, graph)].id;
            d_edge.cost = graph[*out].cost;
            d_edge.revcost = -1;
            removed_edges.push_back(d_edge);
      }

      // special case
      if (m_gType == DIRECTED) {
          EI_i in, in_end;
          for (boost::tie(in, in_end) = in_edges(g_vertex, graph);
            in != in_end; ++in) {
                d_edge.id = graph[*in].id;
                d_edge.source = graph[source(*in, graph)].id;
                d_edge.target = graph[target(*in, graph)].id;
                d_edge.cost = graph[*in].cost;
                d_edge.revcost = -1;
                removed_edges.push_back(d_edge);
          }
      }

      V d_vertex = boost::vertex(vertices_map.find(p_vertex)->second, graph);
      // delete incomming and outgoing edges from the vertex
      boost::clear_vertex(d_vertex, graph);
  }
  //! \brief Disconnects all incomming and outgoing edges from the vertex
  /*!
    boost::graph doesn't recommend th to insert/remove vertices, so a vertex removal is
    simulated by disconnecting the vertex from the graph
    
    - No edge is disconnected if the vertices id's do not exist in the graph
    - All removed edges are stored for future reinsertion
    - All parallel edges are disconnected (automatically by boost)

    ![disconnect_vertex(2) on an UNDIRECTED graph](disconnectVertexUndirected.png)
    ![disconnect_vertex(2) on a DIRECTED graph](disconnectVertexDirected.png)

    @param [IN] *p_vertex* original vertex id of the starting point of the edge
  */
  void disconnect_vertex(int64_t p_vertex) {
      V g_vertex;
      Edge d_edge;
      // nothing to do, the vertex doesnt exist
      if (!get_vertex_descriptor(p_vertex, g_vertex)) return;
      EO_i out, out_end;
      // store the edges that are going to be removed
      for (boost::tie(out, out_end) = out_edges(g_vertex, graph);
        out != out_end; ++out) {
            d_edge.id = graph[*out].id;
            d_edge.source = graph[source(*out, graph)].id;
            d_edge.target = graph[target(*out, graph)].id;
            d_edge.cost = graph[*out].cost;
            d_edge.revcost = -1;
            removed_edges.push_back(d_edge);
      }

      // special case
      if (m_gType == DIRECTED) {
          EI_i in, in_end;
          for (boost::tie(in, in_end) = in_edges(g_vertex, graph);
            in != in_end; ++in) {
                d_edge.id = graph[*in].id;
                d_edge.source = graph[source(*in, graph)].id;
                d_edge.target = graph[target(*in, graph)].id;
                d_edge.cost = graph[*in].cost;
                d_edge.revcost = -1;
                removed_edges.push_back(d_edge);
          }
      }

      V d_vertex = boost::vertex(vertices_map.find(p_vertex)->second, graph);
      // delete incomming and outgoing edges from the vertex
      boost::clear_vertex(d_vertex, graph);
  }
  void calculateDegrees()
    {
      EO_i out, out_end;
        V_i vi;
        int count;
        for (vi = vertices(graph).first; vi != vertices(graph).second; ++vi) {
            count=0;
            for (boost::tie(out, out_end) = out_edges(*vi, graph);
              out != out_end; ++out) {
              count++;
            }
            int64_t id;
            get_vertex_id(*vi,id);
           //cout <<  id << "  " << count << endl; 
            graph[(*vi)].degree=count;
            degree_to_V_map[count].push_back(*vi);
        }
    }



    void print_Vertex_Degree()
    {
      cout << "Printing V_degree map" << endl;
      V_i vi;
      for (vi = vertices(graph).first; vi != vertices(graph).second; ++vi) {
          cout << graph[(*vi)].id << "-->" << graph[(*vi)].degree << endl;
      }
    }

    void print_Degree_Vertices()
    {
      cout << "Printing degree_V map" << endl;
      degree_to_V_i it1;
      Q_i it2;
      for ( it1=degree_to_V_map.begin(); it1!=degree_to_V_map.end(); ++it1)
      {
        cout << it1->first << "-->" ;
        for (it2=it1->second.begin(); it2 !=it1->second.end(); ++it2)
        {
           cout << graph[(*it2)].id << ", ";
        }
        cout << endl;
      }
    }

    void get_degree(int64_t vid,int deg)
    {
      V v;
      get_vertex_descriptor(vid,v);
      deg=graph[v].degree;
    }

  //! \brief Reconnects all edges that were removed
    void restore_graph() {
        while (removed_edges.size() != 0) {
            graph_add_edge(removed_edges[0]);
            removed_edges.pop_front();
        }
    }
  //@}

  //! @name only for stand by program
  //@{
  void print_graph() {
        EO_i out, out_end;
        V_i vi;

        for (vi = vertices(graph).first; vi != vertices(graph).second; ++vi) {
            std::cout << (*vi) << " out_edges(" << graph[(*vi)].id << "):";
            for (boost::tie(out, out_end) = out_edges(*vi, graph);
              out != out_end; ++out) {
              std::cout << ' ' << *out << "=(" << graph[source(*out, graph)].id
                        << ", " << graph[target(*out, graph)].id << ") = "
                        <<  graph[*out].cost <<"\t";
            }
            std::cout << std::endl;
        }
        std::cout << "\n i, distance, predecesor\n"; 
        for (unsigned int i = 0; i < distances.size(); i++) {
            std::cout << i+1 << ", " << distances[i] << ", " << predecessors[i] << "\n";
        }
    }
  //@}


  //returns a vertex_descriptor for a vertex with a particular id
  bool get_vertex_descriptor(int64_t id,V &v)
  {
    LI vertex_ptr = vertices_map.find(id);

      if (vertex_ptr == vertices_map.end())
          return false;

      v = vertex(vertex_ptr->second, graph);
      return true;
  }


  //returns a id for a vertex with a particular vertex_descriptor 
  bool get_vertex_id(V v,int64_t &id)
  {
    RI id_ptr = gVertices_map.find(v);

      if (id_ptr == gVertices_map.end())
          return false;

      id = id_ptr->second;
      return true;
  }


  int64_t
  get_edge_id(const G &graph, V from, V to, float8 &distance) {
        E e;
        EO_i out_i, out_end;
        V v_source, v_target;
        float8 minCost =  std::numeric_limits<float8>::max();
        int64_t minEdge = -1;
        for (boost::tie(out_i, out_end) = boost::out_edges(from, graph);
          out_i != out_end; ++out_i) {
              e = *out_i;
              v_target = target(e, graph);
              v_source = source(e, graph);
              if ((from == v_source) && (to == v_target)
                   && (distance == graph[e].cost))
                     return graph[e].id;
              if ((from == v_source) && (to == v_target)
                   && (minCost > graph[e].cost)) {
                     minCost = graph[e].cost;
                     minEdge = graph[e].id;
              }
        }
        distance = minEdge == -1? 0: minCost;
        return minEdge;
  }


  void
  graph_add_edge(const Edge &edge ) {
      bool inserted;
      LI vm_s, vm_t;
      E e;

      vm_s = vertices_map.find(edge.source);
      if (vm_s == vertices_map.end()) {
        vertices_map[edge.source]=  numb_vertices;
        gVertices_map[numb_vertices++] = edge.source;
        vm_s = vertices_map.find(edge.source);
      }

      vm_t = vertices_map.find(edge.target);
      if (vm_t == vertices_map.end()) {
        vertices_map[edge.target]=  numb_vertices;
        gVertices_map[numb_vertices++] = edge.target;
        vm_t = vertices_map.find(edge.target);
      }

      if (edge.cost >= 0) {
        boost::tie(e, inserted) =
          boost::add_edge(vm_s->second, vm_t->second, graph);
        graph[e].cost = edge.cost;
        graph[e].id = edge.id;
      }

      if (edge.revcost >= 0) {
        boost::tie(e, inserted) =
          boost::add_edge(vm_t->second, vm_s->second, graph);
        graph[e].cost = edge.revcost;
        graph[e].id = edge.id;
      }
    }
};

#endif  // SRC_COMMON_SRC_BASE_GRAPH_HPP_
