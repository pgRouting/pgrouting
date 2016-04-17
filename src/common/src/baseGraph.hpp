/*PGR-GNU*****************************************************************
 *

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#pragma once


#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <deque>
#include <vector>
#include <set>
#include <map>
#include <limits>

#include "./pgr_types.h"

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

typedef typename boost::adjacency_list < boost::vecS, boost::vecS,
        boost::undirectedS,
        boost_vertex_t, boost_edge_t > UndirectedGraph;

typedef typename boost::adjacency_list < boost::vecS, boost::vecS,
        boost::bidirectionalS,
        boost_vertex_t, boost_edge_t > DirectedGraph;


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

     typedef typename boost::graph_traits < G >::vertices_size_type     vertices_size_type;
     typedef typename boost::graph_traits < G >::edges_size_type        edges_size_type;
     typedef typename boost::graph_traits < G >::degree_size_type       degree_size_type;

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
     size_t m_num_vertices;  //!< number of vertices
     graphType m_gType;      //!< type (DIRECTED or UNDIRECTED)
     //@}

     //! @name Id handling
     //@{
     id_to_V  vertices_map;   //!< id -> graph id
     V_to_id  gVertices_map;  //!< graph id -> id
     //@}

     //! @name Graph Modification
     //@{
     //! Used for storing the removed_edges
     std::deque<boost_edge_t> removed_edges;

     //! Used for storing modified edges because of adding points
     // TODO
#if 0
     std::deque<boost_edge_t> modified_edges;
     std::deque< Point_on_edge > points;
     // map to get wich boost edge was modified
     std::map < int64_t, int64_t >;
#endif
     //@}

#if 0
     void add_point(Point_on_edge &point, int driving) {
         // we have:
         // point.point_id
         // point.edge_id
         // point.fraction
         // Look for the edge in modified edges
         //
         // Driving: 0: doesnt matter (both), 1) right, 2) left
         bool found = false;
         int64_t edge_to_modify = 0;
         for (const auto &edge : modified_edges) {
             if (point.edge_id == edge.id) {
                 found = true;
                 break;
             }
             ++edge_to_modify;
         }

         //was not there so look for it in the graph
         if (!found) {
             E_i edge_ptr, edges_end;
             for (boost::tie(edge_ptr, edges_end) = edges(graph);
                     edge_ptr != edges_end; ++edge_ptr) {
                 if (point.edge_id == edge_ptr->id) {
                     modified_edges.push_back(*edge_ptr);
                     boost::remove_edge(edge_ptr, graph);
                     //delete the edge from the graph
                     found = true;
                     break;
                 }
             }
         }

         // add the point
         int64_t vertex_id = -(points.size() + 1);
         point.vertex_id = vertex_id;
         points.push_back(point);

         // add the vertex
         LI vm_s;
         vm_s = vertices_map.find(vertex_id);
         if (vm_s == vertices_map.end()) {
             vertices_map[vertex_id]=  m_num_vertices;
             gVertices_map[m_num_vertices++] = vertex_id;
             vm_s = vertices_map.find(vertex_id);
         }

         if (!found) {
             // the vertex remains disconnected
             // because the edge was not found
             return;
         }
         
     }
#endif





     //! @name The Graph
     //@{
     //! \brief Constructor
     /*!
       Prepares the _graph_ to be of type _gtype_ with the
       aproximate number of vertices its coing to have as *initial_size*
       */
     explicit Pgr_base_graph< G >(graphType gtype)
         : graph(1),
         m_num_vertices(0),
         m_gType(gtype)
    {}

     //! \brief Inserts *count* edges of type *pgr_edge_t* into the graph
     void graph_insert_data(const pgr_edge_t *data_edges, int64_t count) {
         for (unsigned int i = 0; i < count; ++i) {
             graph_add_edge(data_edges[i]);
         }
         adjust_vertices();
         for ( int64_t i = 0; (unsigned int) i < gVertices_map.size(); ++i )
             graph[i].id = gVertices_map.find(i)->second;
     }

     void graph_insert_data( const std::vector <pgr_edge_t > &data_edges) {
         for (const auto edge : data_edges) {
             graph_add_edge(edge);
         }
         adjust_vertices();
         for ( int64_t i = 0; (unsigned int) i < gVertices_map.size(); ++i )
             graph[i].id = gVertices_map.find(i)->second;
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
         boost_edge_t d_edge;
         // nothing to do, the vertex doesnt exist
         if (!get_gVertex(p_from, g_from)) return;
         if (!get_gVertex(p_to, g_to)) return;
         EO_i out, out_end;
         // store the edges that are going to be removed
         for (boost::tie(out, out_end) = out_edges(g_from, graph);
                 out != out_end; ++out) {
             if (target(*out, graph) == g_to) {
                 d_edge.id = graph[*out].id;
                 d_edge.source = graph[source(*out, graph)].id;
                 d_edge.target = graph[target(*out, graph)].id;
                 d_edge.cost = graph[*out].cost;
                 removed_edges.push_back(d_edge);
             }
         }
         // the actual removal
         boost::remove_edge(g_from, g_to, graph);
     }


     //! @name boost wrappers
     //@{
     //! \brief get the out-degree  of a vertex
     /*!
       \returns 0: The out degree of a vertex that its not in the graph

       @param [IN] *vertex_id* original vertex id
     */
     degree_size_type out_degree(int64_t vertex_id) const{
         V v_from;
         if (!get_gVertex(vertex_id, v_from)) {
             return 0;
         }
         return out_degree(v_from);
     }

     degree_size_type out_degree(V &v) const {
         return boost::out_degree(v, graph);
     }
     //@}


     //! \brief Disconnects the outgoing edges with a particular original id from a vertex
     /*!
       - No edge is disconnected if it doesn't exist in the graph
       - Removed edges are stored for future reinsertion
       - all outgoing edges with the edge_id are removed if they exist

       @param [IN] *vertex_id* original vertex
       @param [IN] *edge_id* original edge_id
       */
     void disconnect_out_going_edge(int64_t vertex_id, int64_t edge_id) {
         V v_from;
         boost_edge_t d_edge;

         // nothing to do, the vertex doesnt exist
         if (!get_gVertex(vertex_id, v_from)) {
             return;
         }

         EO_i out, out_end;
         bool change = true;
         // store the edge that are going to be removed
         while (change) {
             change = false;
             for (boost::tie(out, out_end) = out_edges(v_from, graph);
                     out != out_end; ++out) {
                 if (graph[*out].id  == edge_id) {
                     d_edge.id = graph[*out].id;
                     d_edge.source = graph[source(*out, graph)].id;
                     d_edge.target = graph[target(*out, graph)].id;
                     d_edge.cost = graph[*out].cost;
                     //        d_edge.reverse_cost = -1;
                     removed_edges.push_back(d_edge);
                     boost::remove_edge((*out), graph);
                     change = true;
                     break;
                 }
             }
         }
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
         boost_edge_t d_edge;
         // nothing to do, the vertex doesnt exist
         if (!get_gVertex(p_vertex, g_vertex)) return;
         EO_i out, out_end;
         // store the edges that are going to be removed
         for (boost::tie(out, out_end) = out_edges(g_vertex, graph);
                 out != out_end; ++out) {
             d_edge.id = graph[*out].id;
             d_edge.source = graph[source(*out, graph)].id;
             d_edge.target = graph[target(*out, graph)].id;
             d_edge.cost = graph[*out].cost;
             //        d_edge.reverse_cost = -1;
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
                 //           d_edge.reverse_cost = -1;
                 removed_edges.push_back(d_edge);
             }
         }

         V d_vertex = boost::vertex(vertices_map.find(p_vertex)->second, graph);
         // delete incomming and outgoing edges from the vertex
         boost::clear_vertex(d_vertex, graph);
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

         friend std::ostream& operator<<(std::ostream &log, const Pgr_base_graph< G > &g) {

             typename Pgr_base_graph< G >::EO_i out, out_end;
             typename Pgr_base_graph< G >::V_i vi;

             for (vi = vertices(g.graph).first; vi != vertices(g.graph).second; ++vi) {
                 if ((*vi) >= g.m_num_vertices) continue;
                 log << (*vi) << " out_edges(" << g.graph[(*vi)].id << "):";
                 for (boost::tie(out, out_end) = out_edges(*vi, g.graph);
                         out != out_end; ++out) {
                     log << ' ' << g.graph[*out].id << "=(" << g.graph[source(*out, g.graph)].id
                         << ", " << g.graph[target(*out, g.graph)].id << ") = "
                         <<  g.graph[*out].cost <<"\t";
                 }
                 log << std::endl;
             }
             return log;
         }


     void print_graph(std::ostream &log = std::cout) const {
         EO_i out, out_end;
         V_i vi;

         for (vi = vertices(graph).first; vi != vertices(graph).second; ++vi) {
             if ((*vi) >= m_num_vertices) continue;
             log << (*vi) << " out_edges(" << graph[(*vi)].id << "):";
             for (boost::tie(out, out_end) = out_edges(*vi, graph);
                     out != out_end; ++out) {
                 log << ' ' << graph[*out].id << "=(" << graph[source(*out, graph)].id
                     << ", " << graph[target(*out, graph)].id << ") = "
                     <<  graph[*out].cost <<"\t";
             }
             log << std::endl;
         }
     }
     //@}


     bool get_gVertex(int64_t vertex_id, V &gVertex) const {
         LI vertex_ptr = vertices_map.find(vertex_id);

         if (vertex_ptr == vertices_map.end())
             return false;

         gVertex = vertex(vertex_ptr->second, graph);
         return true;
     }

    public:
     int64_t
         get_edge_id(V from, V to, float8 &distance) const {
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

    public:
     size_t num_vertices() const { return m_num_vertices; }
     void
         adjust_vertices() {
             while (boost::num_vertices(graph) != num_vertices()) {
                 if (boost::num_vertices(graph) > num_vertices()) {
                     boost::remove_vertex(boost::num_vertices(graph), graph);
                 }
             }
         }


     boost_vertex_t operator[](V v) const {
         return graph[v];
     }

    private:


     void
         graph_add_edge(const boost_edge_t &edge ) {
             bool inserted;
             LI vm_s, vm_t;
             E e;

             vm_s = vertices_map.find(edge.source);
             if (vm_s == vertices_map.end()) {
                 vertices_map[edge.source]=  m_num_vertices;
                 gVertices_map[m_num_vertices++] = edge.source;
                 vm_s = vertices_map.find(edge.source);
             }

             vm_t = vertices_map.find(edge.target);
             if (vm_t == vertices_map.end()) {
                 vertices_map[edge.target]=  m_num_vertices;
                 gVertices_map[m_num_vertices++] = edge.target;
                 vm_t = vertices_map.find(edge.target);
             }

             if (edge.cost >= 0) {
                 boost::tie(e, inserted) =
                     boost::add_edge(vm_s->second, vm_t->second, graph);
                 graph[e].cost = edge.cost;
                 graph[e].id = edge.id;
                 graph[e].first = edge.first;
             }
         }

     void
         graph_add_edge(const pgr_edge_t &edge ) {
             bool inserted;
             LI vm_s, vm_t;
             E e;
             if ((edge.cost < 0) && (edge.reverse_cost < 0))
                 return;

             vm_s = vertices_map.find(edge.source);
             if (vm_s == vertices_map.end()) {
                 vertices_map[edge.source]=  m_num_vertices;
                 gVertices_map[m_num_vertices++] = edge.source;
                 vm_s = vertices_map.find(edge.source);
             }

             vm_t = vertices_map.find(edge.target);
             if (vm_t == vertices_map.end()) {
                 vertices_map[edge.target]=  m_num_vertices;
                 gVertices_map[m_num_vertices++] = edge.target;
                 vm_t = vertices_map.find(edge.target);
             }

             if (edge.cost >= 0) {
                 boost::tie(e, inserted) =
                     boost::add_edge(vm_s->second, vm_t->second, graph);
                 graph[e].cost = edge.cost;
                 graph[e].id = edge.id;
                 graph[e].first = true;
             }

             if (edge.reverse_cost >= 0) {
                 boost::tie(e, inserted) =
                     boost::add_edge(vm_t->second, vm_s->second, graph);
                 graph[e].cost = edge.reverse_cost;
                 graph[e].id = edge.id;
                 graph[e].first = false;
             }
         }
};


