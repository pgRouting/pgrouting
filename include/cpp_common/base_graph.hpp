/*PGR-GNU*****************************************************************
File: base_graph.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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

/** @file */

#ifndef INCLUDE_CPP_COMMON_BASE_GRAPH_HPP_
#define INCLUDE_CPP_COMMON_BASE_GRAPH_HPP_
#pragma once

#include <deque>
#include <vector>
#include <set>
#include <map>
#include <limits>
#include <string>
#include <cstdint>

#include <boost/graph/iteration_macros.hpp>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include "cpp_common/basic_vertex.hpp"
#include "cpp_common/xy_vertex.hpp"
#include "cpp_common/basic_edge.hpp"

#include "cpp_common/assert.hpp"

namespace pgrouting {

/** @brief boost::graph simplified to pgRouting needs
  This class gives the handling basics of a boost::graph of kind G
  where G:
  can be an undirected graph or a directed graph.
Requiremets:
============
A vertex class T_V
------------------
Current Available vertex classes:
- Basic_vertex
- XY_vertex
An edge class T_E
-----------------
Current Available edge classes:
- Basic_edge
extract_vertices function
-------------------------
Data obtained from postgresql is stored in
A C array of Edge_t type.
~~~~{.c}
std::vector< T_V >
extract_vertices(Edge_t *, size_t)
~~~~
Data obtained from postgresql is stored in
o a vector container.
~~~~{.c}
std::vector< T_V >
extract_vertices(std::vector< Edge_t >)
~~~~
Boost Graph
-------------
The code is prepared to be used for:
- boost::adjacency_list graph type
- boost::undirectedS when the graph is UNDIRECTED
- boost::bidirectionalS when the graph is DIRECTED
~~~~{.c}
boost::adjacency_list
< boost::vecS,  // not tested with other values
boost::vecS,  // not tested with other values
boost::undirectedS,  // Using UNDIRECTED
Basic_vertex,  // the vertex class
Basic_edge >   // the edge class
~~~~
Example Usage:
=============
For this example we will use:
- Basic_vertex
- Basic_edge
- Edge_t
Create Graph type
-----------------
~~~~{.c}
typedef typename
graph::Pgr_base_graph <
boost::adjacency_list <
boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    Basic_vertex,
    Basic_edge >,
    Basic_vertex,
    Basic_edge >
    DirectedGraph;
~~~~
Initializing the graph
------------------------------
Graph initialization is for seting the Vertices of the graph.
//TODO discuss if also the edges
Vector of unique vertices of the graph
~~~~{.c}
size_t total_edges;
Edge_t *my_edges = NULL;
pgr_get_edges(edges_sql, &my_edges, &total_tuples, true, false, &err_msg);
std::vector< Basic_Vertex > vertices(pgrouting::extract_vertices(my_edges));
~~~~
There are several ways to initialize the graph
~~~~{.c}
// 1. Initializes an empty graph
pgrouting::DirectedGraph digraph(true);
// 2. Initializes a graph based on the vertices
pgrouting::DirectedGraph digraph(
    verices,
    true);
vertices.clear();
3. Initializes a graph based on the extracted vertices
pgrouting::DirectedGraph digraph(
    pgrouting::extract_vertices(my_edges, total_edges);
    true);
4. Initializes a graph based on the extracted vertices
pgrouting::DirectedGraph digraph(
    pgrouting::extract_vertices(my_edges);
    true);
~~~~
1. Initializes an empty graph
  - vertices vector size is 0
2. Initializes a graph based on the vertices:
  - vertices vector size is vertices.size()
  - the vertices are inserted
  - vertices container can be clared to free memory
3. Initializes a graph based on the vertices extracted
  - from edges stored on a C array
  - the vertices are inserted
4. Initializes a graph based on the vertices extracted
  - from edges stored on a vector
  - the vertices are inserted
Fill the graph
---------------------
After initializing the graph with the vertices, the edges can be added.
~~~~{.c}
// inserting edges from a C array
digraph.insert_edges(my_edges, total_edges);
// adding more edges to the graph from a vector container
digraph.insert_edges(new_edges);
~~~~
*/

namespace graph {
template <class G, typename Vertex, typename Edge, bool t_directed>
class Pgr_base_graph;

}  // namespace graph


/** @name Graph types
  Type      |   pgRouting
  :---------: | :---------------------
  UndirectedGraph | Basic undirected graph
  UndirectedUniqueGraph | Undirected graph with uniqueness of edges
  DirectedGraph | Basic directed graph
  xyUndirectedGraph | X & Y values stored on the vertex
  xyDirectedGraph | X & Y values stored on the vertex
  */
/**@{*/
typedef graph::Pgr_base_graph <
boost::adjacency_list < boost::vecS, boost::vecS,
    boost::undirectedS,
    Basic_vertex, Basic_edge >,
    Basic_vertex, Basic_edge, false > UndirectedGraph;

typedef graph::Pgr_base_graph <
boost::adjacency_list < boost::vecS, boost::vecS,
    boost::bidirectionalS,
    Basic_vertex, Basic_edge >,
    Basic_vertex, Basic_edge , true> DirectedGraph;

typedef graph::Pgr_base_graph <
boost::adjacency_list < boost::listS, boost::vecS,
    boost::undirectedS,
    XY_vertex, Basic_edge >,
    XY_vertex, Basic_edge , false> xyUndirectedGraph;

typedef graph::Pgr_base_graph <
boost::adjacency_list < boost::listS, boost::vecS,
    boost::bidirectionalS,
    XY_vertex, Basic_edge >,
    XY_vertex, Basic_edge , true> xyDirectedGraph;

/**@}*/


namespace graph {

template <typename G, typename T_V, typename T_E, bool t_directed>
class Pgr_base_graph {
 public:
     /** @name Graph related types
       Type      |     boost meaning     |   pgRouting meaning
       :---------: | :-------------------- | :----------------------
       G        | boost::adjacency_list |   Graph
       V        | vertex_descriptor     |   Think of it as local ID of a vertex
       E        | edge_descriptor       |   Think of it as local ID of an edge
       V_i      | vertex_iterator       |   To cycle the vertices of the Graph
       E_i      | edge_iterator         |   To cycle the edges of the Graph
       EO_i     | out_edge_iterator     |   To cycle the out going edges of a vertex
       EI_i     | in_edge_iterator      |   To cycle the in coming edges of a vertex (only in bidirectional graphs)
       */
     /**@{*/
     using B_G = G;
     using G_T_E = T_E;
     using G_T_V = T_V;
     using V = typename boost::graph_traits<G>::vertex_descriptor;
     using E = typename boost::graph_traits<G>::edge_descriptor;
     using V_i = typename boost::graph_traits<G>::vertex_iterator;
     using E_i = typename boost::graph_traits<G>::edge_iterator;
     using EO_i = typename boost::graph_traits<G>::out_edge_iterator;
     using EI_i = typename boost::graph_traits<G>::in_edge_iterator;

     using vertices_size_type = typename boost::graph_traits<G>::vertices_size_type;
     using edges_size_type = typename boost::graph_traits<G>::edges_size_type;
     using degree_size_type = typename boost::graph_traits<G>::degree_size_type;
     using vertex_index_t = typename boost::property_map<G, boost::vertex_index_t>::type;

     /**@}*/

     /** @name Other types
       Type      |  Meaning       |   pgRouting Meaning
       :---------: | :------------- | :----------------------
       IndexMap | maps V -> id |  given a V store the id
       id_to_V  | maps id -> V   | given an id store the V
       */
     /**@{*/

     using IndexMap = std::map<V, size_t>;
     using id_to_V = typename std::map<int64_t, V>;

     /**@}*/


     /** @name Graph constructors */
     /**@{*/
     /** @brief Constructor */
     /**
       - Prepares the graph to be of type gtype
       - inserts the vertices
       - The vertices must be checked (if necessary)  before calling the constructor
       */
     explicit Pgr_base_graph(
             const std::vector<T_V> &vertices)
         : graph(vertices.size()),
         m_is_directed(t_directed),
         vertIndex(boost::get(boost::vertex_index, graph)),
         propmapIndex(mapIndex) {
             // This code does not work with contraction
             size_t i = 0;
             for (auto vi = boost::vertices(graph).first;
                     vi != boost::vertices(graph).second; ++vi) {
                 vertices_map[vertices[i].id] = (*vi);
                 graph[(*vi)].cp_members(vertices[i]);
                 // put(propmapIndex, *vi, num_vertices());
                 pgassert(vertIndex[*vi] == i);
                 ++i;
             }

             std::ostringstream log;
             for (auto iter = vertices_map.begin();
                     iter != vertices_map.end();
                     iter++) {
                 log << "Key: "
                     << iter->first <<"\tValue:" << iter->second << "\n";
             }
             // pgassert(mapIndex.size() == vertices.size());
         }

     /**
       Prepares the _graph_ to be of type gtype with 0 vertices
       */
     explicit Pgr_base_graph()
         : graph(0),
         m_is_directed(t_directed),
         vertIndex(boost::get(boost::vertex_index, graph)),
         propmapIndex(mapIndex) {
         }
     /**@}*/

     /** @name Edges inserters */
     /**@{*/
     template <typename T> void insert_edges_neg(const std::vector<T> &edges) {
         insert_edges(edges, false);
     }


     /** @brief Inserts *count* edges of type *Edge_t* into the graph
        The set of edges should not have an illegal vertex defined
        When the graph is empty calls:
        - @b extract_vertices
        and throws an exception if there are illegal vertices.
        When developing:
          - if an illegal vertex is found an exception is thrown
          - That means that the set of vertices should be checked in the
            code that is being developed
        No edge is inserted when there is an error on the vertices
        @param edges
        @param normal
      */
     template <typename T>
     void insert_edges(const std::vector<T> &edges, bool normal = true) {
         for (const auto &edge : edges) {
             graph_add_edge(edge, normal);
         }
     }


     template <typename T> void insert_min_edges_no_parallel(const std::vector<T> &edges) {
         for (const auto edge : edges) {
             graph_add_min_edge_no_parallel(edge);
         }
     }

     template <typename T> void insert_negative_edges(const std::vector<T> &edges, bool normal = true) {
         for (const auto edge : edges) {
             graph_add_neg_edge(edge, normal);
         }
     }
     /**@}*/

     /** @name in & out degree */
     /**@{*/
     /** @brief get the out-degree  of a vertex
       @returns 0: The out degree of a vertex that its not in the graph
       @param [in] vertex_id original vertex id
      */
     degree_size_type out_degree(int64_t vertex_id) const {
         if (!has_vertex(vertex_id)) {
             return 0;
         }
         auto v = get_V(vertex_id);
         auto d = out_degree(v);
         return d;
     }

     degree_size_type in_degree(int64_t vertex_id) const {
         if (!has_vertex(vertex_id)) {
             return 0;
         }
         return is_directed()?
             in_degree(get_V(vertex_id))
             :  out_degree(get_V(vertex_id));
     }

     /** @brief in degree of a vertex
      *
      * - when its undirected there is no "concept" of in degree
      *   - out degree is returned
      * - on directed in degree of vertex is returned
      * @param [in] v vertex descriptor
      */
     degree_size_type in_degree(V &v) const {
         return is_directed()?
             boost::in_degree(v, graph) :
             boost::out_degree(v, graph);
     }

     /** @brief out degree of a vertex
      *
      * regardles of undirected or directed graph
      * - out degree is returned
      */
     degree_size_type out_degree(V &v) const {
         return boost::out_degree(v, graph);
     }
     /**@}*/


     /** @name Get vertex descriptor */
     /**@{*/
     /** @brief get the vertex descriptor of the vertex
       When the vertex does not exist
       - creates a new vetex
       @return V: The vertex descriptor of the vertex
       */
     V get_V(const T_V &vertex) {
         auto vm_s(vertices_map.find(vertex.id));
         if (vm_s == vertices_map.end()) {
             auto v =  add_vertex(graph);
             graph[v].cp_members(vertex);
             vertices_map[vertex.id] =  v;
             put(propmapIndex, v, num_vertices());
             return v;
         }
         return vm_s->second;
     }

     /** @brief get the vertex descriptor of the vid
       Call has_vertex(vid) before calling this function
       @param[in] vid vertex identifier
       @return V: The vertex descriptor of the vertex
       */
     V get_V(int64_t vid) const {
         if (!has_vertex(vid)) throw std::string("Call to ") + __PRETTY_FUNCTION__ + "without checking with has_vertex";
         return vertices_map.find(vid)->second;
     }

     /** @brief get source edge
       @param[in] e edge descriptor
       @return V vertex descriptor
       */
     V source(E e) const {return boost::source(e, graph);}

     /** @brief get source edge
       @param[in] e edge descriptor
       @return V vertex descriptor
       */
     V target(E e) const {return boost::target(e, graph);}

     /** @brief get source edge
       @param[in] v vertex descriptor
       @param[in] e edge descriptor
       @return V vertex descriptor
       */
     V adjacent(V v, E e) const {
         pgassert(is_source(v, e) || is_target(v, e));
         return is_source(v, e)? target(e) : source(e);
     }
     /**@}*/


     /** @name to be or not to be */
     /**@{*/
     /** @brief does vertex exist in the graph?
       @param[in] vid vertex identifier
       @return true: when vid is in the graph
       @return false: when vid is not in the graph
      */
     bool has_vertex(int64_t vid) const {
         return vertices_map.find(vid) != vertices_map.end();
     }
     /** @brief is the graph directed?
       @return true: when the graph is directed
       @return false: when the graph is not directed
      */
     bool is_directed() const {return m_is_directed;}
     /** @brief is the graph undirected?
       @return true: when the graph is undirected
       @return false: when the graph is not undirected
      */
     bool is_undirected() const {return !m_is_directed;}
     /** @brief is v a source in the edge?
       @param[in] v vertex descriptor
       @param[in] e edge descriptor
       @return true: when v is source on the edge
       @return false: when v is not source on the edge
      */
     bool is_source(V v, E e) const {return v == source(e);}
     /** @brief is v a target in the edge?
       @param[in] v vertex descriptor
       @param[in] e edge descriptor
       @return true: when v is target on the edge
       @return false: when v is not target on the edge
      */
     bool is_target(V v, E e) const {return v == target(e);}

     /**@}*/

     /** @name get original edge/vertex data */
     /**@{*/
     /** @brief get the edge
       @param[in] e edge descriptor
       @return The edge information (allows modification)
      */
     T_E& operator[](E e) {return graph[e];}

     /** @brief get the edge
       @param[in] e edge descriptor
       @return The edge information (does not allow modification)
      */
     const T_E& operator[](E e) const {return graph[e];}

     /** @brief get the vertex
       @param[in] v vertex descriptor
       @return The vertex information (allows modification)
      */
     T_V& operator[](V v) {return graph[v];}

     /** @brief get the vertex
       @param[in] v vertex descriptor
       @return The vertex information (does not allow modification)
      */
     const T_V& operator[](V v) const {return graph[v];}

     /** @brief get the edge id of from--to with the indicated distance
       @param[in] from vertex descriptor
       @param[in] to vertex descriptor
       @param[in] weight edge's weight to match when several options are found
       @return The edge identifier of edge from--to with @b weight
       @return OR the edge identifier of edge from--to with @b min weight
      */
     int64_t get_edge_id(V from, V to, double &weight) const {
         EO_i out_i, out_end;
         double minCost =  (std::numeric_limits<double>::max)();
         int64_t minEdge = -1;
         for (boost::tie(out_i, out_end) = boost::out_edges(from, graph); out_i != out_end; ++out_i) {
             auto e = *out_i;
             auto v_target = target(e);
             auto v_source = source(e);
             if ((from == v_source) && (to == v_target) && (weight == graph[e].cost)) return graph[e].id;
             if ((from == v_source) && (to == v_target) && (minCost > graph[e].cost)) {
                 minCost = graph[e].cost;
                 minEdge = graph[e].id;
             }
         }
         weight = minEdge == -1? 0: minCost;
         return minEdge;
     }
     /**@}*/



     /** @name only for debug */
     /**@{*/
     friend std::ostream& operator<<(
             std::ostream &log, const Pgr_base_graph<G, T_V, T_E, t_directed> &g) {
         typename Pgr_base_graph<G, T_V, T_E, t_directed>::EO_i out, out_end;

         auto vs = boost::vertices(g.graph);
         for (auto vi = vs.first; vi != vs.second; ++vi) {
             if ((*vi) >= g.num_vertices()) break;
             log << (*vi) << ": " << " out_edges_of(" << g.graph[(*vi)] << "):";
             auto es = boost::out_edges(*vi, g.graph);
             for (auto out = es.first; out != es.second; ++out) {
                 log << ' '
                     << g.graph[*out].id << "=("
                     << g[g.source(*out)].id << ", "
                     << g[g.target(*out)].id << ") = "
                     << g.graph[*out].cost <<"\t";
             }
             log << std::endl;
         }
         return log;
     }
     /**@}*/


     /** @name Get edge descriptor */
     /**@{*/
     E get_edge(V from, V to, double &distance) const {
         E e;
         EO_i out_i, out_end;
         V v_source, v_target;
         double minCost =  (std::numeric_limits<double>::max)();
         E minEdge;
         bool valid = false;
         for (boost::tie(out_i, out_end) = boost::out_edges(from, graph);
                 out_i != out_end; ++out_i) {
             e = *out_i;
             if (!valid) {
                 minEdge = e;
                 valid = true;
             }
             v_target = target(e);
             v_source = source(e);
             if ((from == v_source) && (to == v_target)
                     && (distance == graph[e].cost)) {
                 return e;
             }
             if ((from == v_source) && (to == v_target)
                     && (minCost > graph[e].cost)) {
                 minCost = graph[e].cost;
                 minEdge = e;
             }
         }
         return minEdge;
     }
     /**@}*/

     /** @name How big */
     /**@{*/
     size_t num_vertices() const { return boost::num_vertices(graph);}
     size_t num_edges() const { return boost::num_edges(graph);}
     /**@}*/

     /** @name edge disconection/reconnection */
     /**@{*/
     /** @brief Disconnects all edges from p_from to p_to
       - No edge is disconnected if the vertices id's do not exist in the graph
       - All removed edges are stored for future reinsertion
       - All parallel edges are disconnected (automatically by boost)
       ![disconnect_edge(2,3) on an UNDIRECTED graph](disconnectEdgeUndirected.png)
       ![disconnect_edge(2,3) on a DIRECTED graph](disconnectEdgeDirected.png)
       @param [in] p_from vertex identifier of the starting point of the edge
       @param [in] p_to   vertex identifier of the ending point of the edge
       */
     void disconnect_edge(int64_t p_from, int64_t p_to) {
         /* nothing to do, a vertex doesn't exist */
         if (!has_vertex(p_from) || !has_vertex(p_to)) return;

         EO_i out, out_end;
         auto u(get_V(p_from));
         auto v(get_V(p_to));

         for (boost::tie(out, out_end) = out_edges(u, graph); out != out_end; ++out) {
             if (target(*out) == v) {
                 removed_edges.push_back(get_edge_info(*out));
                 break;
             }
         }
         boost::remove_edge(u, v, graph);
     }

     /** @brief Disconnects the outgoing edges of a vertex

       - No edge is disconnected if it doesn't exist in the graph
       - Removed edges are stored for future reinsertion
       - all outgoing edges with the edge_id are removed if they exist
       @param [in] vid original vertex
       @param [in] eid original edge_id
       */
     void disconnect_out_going_edge(int64_t vid, int64_t eid) {
         /* nothing to do, the vertex doesn't exist */
         if (!has_vertex(vid)) return;

         auto v(get_V(vid));
         EO_i out, out_end;
         bool change = true;
         while (change) {
             change = false;
             for (boost::tie(out, out_end) = out_edges(v, graph); out != out_end; ++out) {
                 if (graph[*out].id  == eid) {
                     removed_edges.push_back(get_edge_info(*out));
                     boost::remove_edge((*out), graph);
                     change = true;
                     break;
                 }
             }
         }
     }

     /** @brief Disconnects all incoming and outgoing edges from @b vertex
       @param [in] vid vertex identifier
       */
     void disconnect_vertex(int64_t vid) {
         if (!has_vertex(vid)) return;
         disconnect_vertex(get_V(vid));
     }

     /** @brief Disconnects all incoming and outgoing edges from the vertex

       boost::graph doesn't recommend th to insert/remove vertices, so a vertex removal is
       simulated by disconnecting the vertex from the graph
       - No edge is disconnected if the vertices id's do not exist in the graph
       - All removed edges are stored for future reinsertion
       - All parallel edges are disconnected (automatically by boost)
       ![disconnect_vertex(2) on an UNDIRECTED graph](disconnectVertexUndirected.png)
       ![disconnect_vertex(2) on a DIRECTED graph](disconnectVertexDirected.png)
       @param [in] v vertex descriptor
       */
     void disconnect_vertex(V v) {
         EO_i out, out_end;
         for (boost::tie(out, out_end) = out_edges(v, graph); out != out_end; ++out) {
                     removed_edges.push_back(get_edge_info(*out));
         }

         /* special case */
         if (m_is_directed) {
             EI_i in, in_end;
             for (boost::tie(in, in_end) = in_edges(v, graph); in != in_end; ++in) {
                     removed_edges.push_back(get_edge_info(*in));
             }
         }

         /* delete incoming and outgoing edges from the vertex */
         boost::clear_vertex(v, graph);
     }

     /** @brief Reconnects all edges that were removed */
     void
         restore_graph() {
             while (removed_edges.size() != 0) {
                 restore_edge(removed_edges[0]);
                 removed_edges.pop_front();
             }
         }

 private:
     void restore_edge(const T_E &edge) {
         E e;

         auto u = get_V(edge.source);
         auto v = get_V(edge.target);
         if (edge.cost >= 0) {
             bool inserted;
             boost::tie(e, inserted) = boost::add_edge(u, v, graph);
             graph[e].cp_members(edge);
         }
     }

     /* @brief builds up info that will be deleted
      * @param[in] e edge descriptor
      * @return edge data
      */
     T_E get_edge_info(const E &e) const {
         T_E d_edge;
         d_edge.id = graph[e].id;
         d_edge.source = graph[source(e)].id;
         d_edge.target = graph[target(e)].id;
         d_edge.cost = graph[e].cost;
         return d_edge;
     }
     /**@}*/

     template <typename T> void graph_add_edge(const T &edge, bool normal) {
         bool inserted;
         E e;
         if ((edge.cost < 0) && (edge.reverse_cost < 0)) return;

         /*
          * true: for source
          * false: for target
          */
         auto vm_s = get_V(T_V(edge, true));
         auto vm_t = get_V(T_V(edge, false));

         pgassert(vertices_map.find(edge.source) != vertices_map.end());
         pgassert(vertices_map.find(edge.target) != vertices_map.end());
         if (edge.cost >= 0) {
             boost::tie(e, inserted) =
                 boost::add_edge(vm_s, vm_t, graph);
             graph[e].cost = edge.cost;
             graph[e].id = edge.id;
         }

         if (edge.reverse_cost >= 0
                 && (m_is_directed || (!m_is_directed && edge.cost != edge.reverse_cost))) {
             boost::tie(e, inserted) =
                 boost::add_edge(vm_t, vm_s, graph);

             graph[e].cost = edge.reverse_cost;
             graph[e].id = normal? edge.id : -edge.id;
         }
     }

     template <typename T> void graph_add_min_edge_no_parallel(const T &edge) {
         bool inserted;
         E e;
         if ((edge.cost < 0) && (edge.reverse_cost < 0)) return;

         /*
          * true: for source
          * false: for target
          */
         auto vm_s = get_V(T_V(edge, true));
         auto vm_t = get_V(T_V(edge, false));

         pgassert(vertices_map.find(edge.source) != vertices_map.end());
         pgassert(vertices_map.find(edge.target) != vertices_map.end());
         if (edge.cost >= 0) {
             E e1;
             bool found;
             boost::tie(e1, found) = boost::edge(vm_s, vm_t, graph);
             if (found) {
                 if (edge.cost < graph[e1].cost) {
                     graph[e1].cost = edge.cost;
                     graph[e1].id = edge.id;
                 }
             } else {
                 boost::tie(e, inserted) = boost::add_edge(vm_s, vm_t, graph);
                 graph[e].cost = edge.cost;
                 graph[e].id = edge.id;
             }
         }

         if (edge.reverse_cost >= 0
                 && (m_is_directed || (!m_is_directed && edge.cost != edge.reverse_cost))) {
             E e1;
             bool found;
             boost::tie(e1, found) = boost::edge(vm_t, vm_s, graph);
             if (found) {
                 if (edge.reverse_cost < graph[e1].cost) {
                     graph[e1].cost = edge.reverse_cost;
                     graph[e1].id = edge.id;
                 }
             } else {
                 boost::tie(e, inserted) =
                     boost::add_edge(vm_t, vm_s, graph);

                 graph[e].cost = edge.reverse_cost;
                 graph[e].id = edge.id;
             }
         }
     }

     /**
       Add edges with negative cost(either cost or reverse_cost or both)
       Reading them into graph as positive cost ( edge_cost = (-1)* edge_negative_cost) [L931 & L941]
       To Do: Read and apply edges with negative cost in function as it is
       */
     template <typename T> void graph_add_neg_edge(const T &edge, bool normal = true) {
         bool inserted;
         E e;

         auto vm_s = get_V(T_V(edge, true));
         auto vm_t = get_V(T_V(edge, false));

         pgassert(vertices_map.find(edge.source) != vertices_map.end());
         pgassert(vertices_map.find(edge.target) != vertices_map.end());

         boost::tie(e, inserted) = boost::add_edge(vm_s, vm_t, graph);
         if (edge.cost < 0) {
             /* reading negative edges as positive */
             graph[e].cost = (-0.5)*edge.cost;
         } else {
             graph[e].cost = edge.cost;
         }
         graph[e].id = edge.id;

         if (m_is_directed
                 || (!m_is_directed && edge.cost > edge.reverse_cost)) {
             boost::tie(e, inserted) =
                 boost::add_edge(vm_t, vm_s, graph);
             if (edge.reverse_cost < 0) {
                 /* reading negative edges as positive */
                 graph[e].cost = (-0.5)*edge.reverse_cost;
             } else {
                 graph[e].cost = edge.reverse_cost;
             }

             graph[e].id = normal? edge.id : -edge.id;
         }
     }

 public:
     /** @name The Graph */
     /**@{*/
     G graph;                /**< The graph */
     id_to_V  vertices_map;   /**< id -> graph id */

 private:
     bool m_is_directed;      /**< type (DIRECTED or UNDIRECTED) */
     /**@}*/

     /** @name Id mapping handling */
     /**@{*/
     vertex_index_t vertIndex;
     IndexMap mapIndex;
     boost::associative_property_map<IndexMap> propmapIndex;
     /**@}*/

     /** @name Graph Modification */
     /**@{*/
     /** Used for storing the removed_edges */
     std::deque<T_E> removed_edges;
     /**@}*/
};

}  // namespace graph
}  // namespace pgrouting
#endif  // INCLUDE_CPP_COMMON_BASE_GRAPH_HPP_
