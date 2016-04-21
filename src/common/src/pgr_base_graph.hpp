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

#include "./basic_vertex.h"
#include "./pgr_types.h" // for pgr_edge_t 
#include "./pgr_assert.h"

/*! \brief boost::graph simplified to pgRouting needs

  This class gives the handling basics of a boost::graph of kind G
  where G:
  can be an undirected graph or a directed graph.

Requiremets:
============

A Vertex class
---------------
Members:
  - int64_t id
  - <other members>


with the following functions:

 - cp_members:
   - copyies all the members including id

 - operator<< 

 - And staisfies all of boost requirements
   - has a default constructor
   - its copy constructible
   - etc.


An Edge class
-------------

Members:
  - int64_t id
  - int64_t source
  - int64_t target
  - <other members>

 
with the following functions:
 - cp_members
   - copies all the members excluding
     - source
     - target

 - operator<< 

 - And staisfies all of boost requirements
   - has a default constructor
   - its copy constructible
   - etc.

For directed graphs:
  - boost::bidirectionalS

For undirected graphs:
  - boost::undirectedS


Usage:
======

Creating a type for the following examples we will show the type
used in:
  - pgr_dijkstra & family of functions

~~~~{.c}
typedef typename
graph::Pgr_base_graph <
    boost::adjacency_list < boost::vecS, boost::vecS,
    boost::undirectedS,
    Basic_vertex, Basic_edge >,
    Basic_vertex, Basic_edge >

    UndirectedGraph;

typedef typename graph::Pgr_base_graph <
    boost::adjacency_list < boost::vecS, boost::vecS,
    boost::bidirectionalS,
    Basic_vertex, Basic_edge >,
    Basic_vertex, Basic_edge >
   
    DirectedGraph;
~~~~

Initializing & fill the graph
------------------------------

~~~~{.c}
auto vertices(pgRouting::extract_vertices(edges, total_edges));
vertices = pgRouting::extract_vertices(vertices, new_edges);

pgRouting::DirectedGraph digraph(vertices, gType);
pgRouting::UndirectedGraph undigraph(gType);
~~~~


~~~~{.c}
digraph.graph_insert_data(edges, total_edges);
digraph.graph_insert_data(new_edges);

undigraph.graph_insert_data(edges, total_edges);
undigraph.graph_insert_data(new_edges);
~~~~

*/

namespace pgRouting {

    //forward declararion of Pgr_base_graph
    namespace graph{

        template <class G, typename Vertex, typename Edge>
            class Pgr_base_graph;

    } // namespace graph


     /** @name Grpah types 
      Type      |   pgRouting
      :---------: | :---------------------
      UndirectedGraph | Basic undirected graph
      DirectedGraph | Basic directed graph
      */
    typedef typename graph::Pgr_base_graph <
        boost::adjacency_list < boost::vecS, boost::vecS,
        boost::undirectedS,
        Basic_vertex, Basic_edge >,
        Basic_vertex, Basic_edge > UndirectedGraph;

    typedef typename graph::Pgr_base_graph <
        boost::adjacency_list < boost::vecS, boost::vecS,
        boost::bidirectionalS,
        Basic_vertex, Basic_edge >,
        Basic_vertex, Basic_edge > DirectedGraph;

    namespace graph{

        template <class G, typename T_V, typename T_E>
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
                      LI       | Left Iterator  | iterates over id_to_V
                      */
                    //@{

                    typedef typename std::map< int64_t, V > id_to_V;
                    typedef typename id_to_V::const_iterator LI;

                    //@}

                    //! @name The Graph
                    //@{
                    G graph;                //!< The graph
                    size_t m_num_vertices;  //!< local count.
                    graphType m_gType;      //!< type (DIRECTED or UNDIRECTED)
                    //@}

                    //! @name Id mapping handling
                    //@{

                    // TODO (vicky) THINK is this necessary?
                    // std::vector< T_V > original_vids; //!< ordered original ids
                    id_to_V  vertices_map;   //!< id -> graph id

                    //@}

                    //! @name Graph Modification
                    //@{
                    //! Used for storing the removed_edges
                    std::deque< T_E > removed_edges;
                    //@}



                    //! @name The Graph
                    //@{
                    //! \brief Constructor
                    /*!
                      - Prepares the graph to be of type gtype
                      - inserts the vertices
                      */
                    Pgr_base_graph< G , T_V, T_E >(const std::vector< T_V > &vertices, graphType gtype)
                        : graph(vertices.size()),
                        m_num_vertices(vertices.size()),
                        m_gType(gtype) {
                            pgassert(boost::num_vertices(graph) == num_vertices());
                            pgassert(boost::num_vertices(graph) == vertices.size());

                            size_t i = 0;
                            for (auto vi = boost::vertices(graph).first; vi != boost::vertices(graph).second; ++vi) {
                                vertices_map[vertices[i].id] = (*vi);
                                graph[(*vi)].cp_members(vertices[i++]);
                            }
                        }

                    /*!
                      Prepares the _graph_ to be of type gtype with 0 vertices
                      */
                    explicit Pgr_base_graph< G , T_V, T_E >(graphType gtype)
                        : graph(0),
                        m_num_vertices(0),
                        m_gType(gtype) {
                        }


                    //! \brief Inserts *count* edges of type *pgr_edge_t* into the graph
                    void graph_insert_data(const pgr_edge_t *data_edges, int64_t count) {
                        const std::vector < pgr_edge_t > edges(data_edges, data_edges + count);
                        graph_insert_data(edges);
                    }

                    void graph_insert_data(const std::vector < pgr_edge_t > &data_edges) {
                        pgassert(boost::num_vertices(graph) == num_vertices());
                        if (num_vertices()==0) {
                            auto vertices = pgRouting::extract_vertices(data_edges);
                            add_vertices(vertices);
#if 0
                            for (const auto vertex : vertices) {
                                assert(vertices_map.find(vertex.id) == vertices_map.end());

                                auto v =  add_vertex(graph);
                                vertices_map[vertex.id] =  m_num_vertices++;
                                graph[v].cp_members(vertex);

                                pgassert(boost::num_vertices(graph) == num_vertices());
                            }
#endif
                        }

                        for (const auto edge : data_edges) {
                            graph_add_edge(edge);
                        }
                    }

                private:
                    // void extract_vertices(const pgr_edge_t *data_edges, int64_t count);
#if 0
                    void extract_vertices(const std::vector <pgr_edge_t > &data_edges);
#endif
                    void add_vertices(std::vector< T_V > vids);

                public:



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


                    //! @name edge disconection/reconnection 
                    //@{
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
                    void disconnect_edge(int64_t p_from, int64_t p_to);


                    //! \brief Disconnects the outgoing edges with a particular original id from a vertex
                    /*!

                      - No edge is disconnected if it doesn't exist in the graph
                      - Removed edges are stored for future reinsertion
                      - all outgoing edges with the edge_id are removed if they exist

                      @param [IN] *vertex_id* original vertex
                      @param [IN] *edge_id* original edge_id
                      */
                    void disconnect_out_going_edge(int64_t vertex_id, int64_t edge_id);




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
                    void disconnect_vertex(int64_t p_vertex);


                    //! \brief Reconnects all edges that were removed
                    void restore_graph();

                    //@}

                    //! @name only for stand by program
                    //@{

                    friend std::ostream& operator<<(std::ostream &log, const Pgr_base_graph< G, T_V, T_E > &g) {

                        typename Pgr_base_graph< G, T_V, T_E >::EO_i out, out_end;
                        // typename Pgr_base_graph< G, T_V, T_E >::V_i vi;

#if 0
                        log << "original_vids:";
                        for (const auto vertex : g.original_vids) {
                            log << vertex.id << ", ";
                        }
                        log << "\n";
#endif
                        for (auto vi = vertices(g.graph).first; vi != vertices(g.graph).second; ++vi) {
                            if ((*vi) >= g.m_num_vertices) break;
                            log << (*vi) << ": "  << " out_edges_of(" << g.graph[(*vi)].id << "):";
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

                    //@}


                    bool get_gVertex(int64_t vertex_id, V &gVertex) const;

                public:
                    int64_t get_edge_id(V from, V to, float8 &distance) const;

                    size_t num_vertices() const { return m_num_vertices; }

                    T_V operator[](V v) const {
                        return graph[v];
                    }


                    V getV(const T_V &vertex) {
                        auto vm_s(vertices_map.find(vertex.id));
                        if (vm_s == vertices_map.end()) {
                            auto v =  add_vertex(graph);
                            graph[v].cp_members(vertex);
                            vertices_map[vertex.id] =  m_num_vertices++;
                            vm_s = vertices_map.find(vertex.id);
                            pgassert(vertices_map.find(vertex.id)->second == v);
                            return v;
                        }
                        return vm_s->second;
                    }

                protected:
                    void graph_add_edge(const T_E &edge);
                    void graph_add_edge(const pgr_edge_t &edge);
            };

            template < class G, typename T_V, typename T_E >
                void
                Pgr_base_graph< G, T_V, T_E >::disconnect_edge(int64_t p_from, int64_t p_to) {
                    V g_from;
                    V g_to;
                    T_E d_edge;

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



            template < class G, typename T_V, typename T_E >
                void
                Pgr_base_graph< G, T_V, T_E >::disconnect_out_going_edge(int64_t vertex_id, int64_t edge_id) {
                    Pgr_base_graph< G, T_V, T_E >::V v_from;
                    T_E d_edge;

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
                                removed_edges.push_back(d_edge);
                                boost::remove_edge((*out), graph);
                                change = true;
                                break;
                            }
                        }
                    }
                }


            template < class G, typename T_V, typename T_E >
                void
                Pgr_base_graph< G, T_V, T_E >::disconnect_vertex(int64_t p_vertex) {
                    V g_vertex;
                    T_E d_edge;
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

            template < class G, typename T_V, typename T_E >
                void
                Pgr_base_graph< G, T_V, T_E >::restore_graph() {
                    while (removed_edges.size() != 0) {
                        graph_add_edge(removed_edges[0]);
                        removed_edges.pop_front();
                    }
                }


            template < class G, typename T_V, typename T_E >
                bool
                Pgr_base_graph< G, T_V, T_E >::get_gVertex(int64_t vertex_id, V &gVertex) const {
                    Pgr_base_graph< G, T_V, T_E >::LI vertex_ptr = vertices_map.find(vertex_id);

                    if (vertex_ptr == vertices_map.end())
                        return false;

                    gVertex = vertex(vertex_ptr->second, graph);
                    return true;
                }

            template < class G, typename T_V, typename T_E >
                int64_t
                Pgr_base_graph< G, T_V, T_E >::get_edge_id(V from, V to, float8 &distance) const {
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


            template < class G, typename T_V, typename T_E >
                void
                Pgr_base_graph< G, T_V, T_E >::graph_add_edge(const T_E &edge ) {
                    bool inserted;
                    typename Pgr_base_graph< G, T_V, T_E >::LI vm_s, vm_t;
                    typename Pgr_base_graph< G, T_V, T_E >::E e;

                    vm_s = vertices_map.find(edge.source);
                    if (vm_s == vertices_map.end()) {
                        vertices_map[edge.source]=  m_num_vertices;
                        vm_s = vertices_map.find(edge.source);
                    }

                    vm_t = vertices_map.find(edge.target);
                    if (vm_t == vertices_map.end()) {
                        vertices_map[edge.target]=  m_num_vertices;
                        vm_t = vertices_map.find(edge.target);
                    }

                    if (edge.cost >= 0) {
                        boost::tie(e, inserted) =
                            boost::add_edge(vm_s->second, vm_t->second, graph);
                        graph[e].cp_members(edge);
                    }
                }


            template < class G, typename T_V, typename T_E >
                void
                Pgr_base_graph< G, T_V, T_E >::graph_add_edge(const pgr_edge_t &edge) {
                    bool inserted;
                    typename Pgr_base_graph< G, T_V, T_E >::E e;
                    if ((edge.cost < 0) && (edge.reverse_cost < 0))
                        return;

                    auto vm_s = getV(T_V(edge.source));
                    auto vm_t = getV(T_V(edge.target));

                    pgassert(vertices_map.find(edge.source) != vertices_map.end());
                    pgassert(vertices_map.find(edge.target) != vertices_map.end());
                    if (edge.cost >= 0) {
                        boost::tie(e, inserted) =
                            boost::add_edge(vm_s, vm_t, graph);
                        graph[e].cost = edge.cost;
                        graph[e].id = edge.id;
                        graph[e].first = true;
                    }

                    if (edge.reverse_cost >= 0) {
                        boost::tie(e, inserted) =
                            boost::add_edge(vm_t, vm_s, graph);

                        graph[e].cost = edge.reverse_cost;
                        graph[e].id = edge.id;
                        graph[e].first = false;
                    }
                }

            /******************  PRIVATE *******************/

#if 0
            template < class G, typename T_V, typename T_E >
                void
                Pgr_base_graph< G, T_V, T_E >::extract_vertices(
                        const std::vector <pgr_edge_t > &data_edges) {
                    if (data_edges.empty()) return;

                    std::vector< T_V > vertices;
                    vertices.reserve(data_edges.size() * 2);

                    for (const auto edge : data_edges) {
                        T_V vertex;

                        vertex.id = edge.source;
                        vertices.push_back(vertex);

                        vertex.id = edge.target;
                        vertices.push_back(vertex);

                    }

                    add_vertices(vertices);
                }
#endif
            template < class G, typename T_V, typename T_E >
                void
                Pgr_base_graph< G, T_V, T_E >::add_vertices(
                        std::vector< T_V > vertices) {
                    pgassert(m_num_vertices == 0);
                    for (const auto vertex : vertices) {
                        pgassert(vertices_map.find(vertex.id) == vertices_map.end());

                        auto v =  add_vertex(graph);
                        vertices_map[vertex.id] =  m_num_vertices++;
                        graph[v].cp_members(vertex);

                        pgassert(boost::num_vertices(graph) == num_vertices());
                    }
                    return;
                }
#if 0
                    // pgprecondition(is_sorted(original_ids));
                    // pgprecondition(has_uniques(original_ids));

                    /*
                     *  sort and delete duplicates
                     */
                    std::stable_sort(vertices.begin(), vertices.end(),
                            [](const T_V &lhs, const T_V &rhs)
                            {return lhs.id < rhs.id;});


                    vertices.erase(
                            std::unique(vertices.begin(), vertices.end(),
                                [](const T_V &lhs, const T_V &rhs)
                                {return lhs.id == rhs.id;}), vertices.end()
                            );

                    // pgassert(is_sorted(vertices));
                    // pgassert(has_uniques(vertices));

                    /*
                     * original_ids - vids = vids to be inserted
                     */

                    std::vector< T_V > diff;
                    std::set_difference(
                            vertices.begin(), vertices.end(), 
                            original_vids.begin(), original_vids.end(),
                            std::inserter(diff, diff.begin()),
                            [](const T_V &lhs, const T_V &rhs)
                            {return lhs.id == rhs.id;}
                            );

                    pgassert(boost::num_vertices(graph) == num_vertices());
                    for (const auto vertex : diff) {
                        assert(vertices_map.find(vertex.id) == vertices_map.end());
                        /* not added yet */
                        auto v =  add_vertex(graph);
                        vertices_map[vertex.id] =  m_num_vertices++;
                        graph[v].cp_members(vertex);

                        pgassert(boost::num_vertices(graph) == num_vertices());
                    }

                    /*
                     * insert the newly added vertices into original_vids and sort it.
                     */

                    original_vids.insert(original_vids.end(), diff.begin(), diff.end());


                    std::stable_sort(original_vids.begin(), original_vids.end(),
                            [](const T_V &lhs, const T_V &rhs)
                            {return lhs.id < rhs.id;});

                    for (const auto vertex : original_vids) {
                        pgassert(vertices_map.find(vertex.id) != vertices_map.end());
                    }
                }
#endif

            } // namespace graph
    }  // namespace pgRouting
