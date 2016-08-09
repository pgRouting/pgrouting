/*PGR-GNU*****************************************************************
File: pgr_contract.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
Mail: 

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
#include <stack>
#include <iostream>
#include <sstream>
#include <deque>
#include <queue>
#include <string>
#include <utility>
#include <functional>
#include <vector>
#include <map>

#include "./pgr_contractionGraph.hpp"
#include "./pgr_deadEndContraction.hpp"
#include "./pgr_linearContraction.hpp"
#include "../../common/src/pgr_assert.h"


template < class G > class Pgr_contract;

template < class G >
void perform_deadEnd(G &graph,
    Identifiers<int64_t> forbidden_vertices,
    std::ostringstream& debug) {
    pgrouting::Pgr_deadEndContraction<G> deadendContractor;
    debug << "Setting forbidden_vertices";
    deadendContractor.setForbiddenVertices(graph, forbidden_vertices
        , debug);

    deadendContractor.calculateVertices(graph, debug);
    try {
        deadendContractor.doContraction(graph, debug);
    }
    catch ( ... ) {
        debug << "Caught unknown expection!\n";
    }
}

#if 1

template < class G >
void perform_linear(G &graph,
    Identifiers<int64_t>& forbidden_vertices,
    std::ostringstream& debug) {
    std::ostringstream linear_debug;
    pgrouting::Pgr_linearContraction<G> linearContractor;
    linearContractor.setForbiddenVertices(graph, forbidden_vertices
        , linear_debug);
    linearContractor.calculateVertices(graph, linear_debug);
    try {
        linearContractor.doContraction(graph, linear_debug);
    }
    catch ( ... ) {
        linear_debug << "Caught unknown expection!\n";
    }
    debug << linear_debug.str().c_str() << "\n";
}
#endif


template < class G >
void pgr_contractGraph(
    G &graph, Identifiers<int64_t> forbidden_vertices,
    int64_t *contraction_order,
    size_t size_contraction_order,
    int64_t max_cycles,
    Identifiers<int64_t> &remaining_vertices,
    std::vector<pgrouting::contraction::Edge> &shortcut_edges,
    std::ostringstream& debug) {
    std::deque<int64_t> contract_order;
    //  push -1 to indicate the start of the queue
    contract_order.push_back(-1);
    for (size_t i = 0; i < size_contraction_order; ++i) {
        contract_order.push_back(contraction_order[i]);
    }
    for (int64_t i = 0; i < max_cycles; ++i) {
        int64_t front = contract_order.front();
        debug << "Starting cycle " << i+1 << "\n";
        contract_order.pop_front();
        contract_order.push_back(front);
        front = contract_order.front();
        while (front != -1) {
            switch (front) {
                case -1:
                debug << "Finished cycle " << i+1 << std::endl;
                break;
                default:
                debug << "contraction "<< front << " asked" << std::endl;
                if (front == 1) {
                    debug << "Graph before dead end contraction" << std::endl;
                    graph.print_graph(debug);
                    debug << "Performing dead end contraction" << std::endl;
                    perform_deadEnd(graph, forbidden_vertices, debug);
                    debug << "Graph after dead end contraction" << std::endl;
                    graph.print_graph(debug);
                } else if (front == 2) {
                    debug << "Graph before linear contraction" << std::endl;
                    graph.print_graph(debug);
                    debug << "Performing linear contraction" << std::endl;
                    perform_linear(graph, forbidden_vertices, debug);
                    debug << "Graph after linear contraction" << std::endl;
                    graph.print_graph(debug);
                }
                contract_order.pop_front();
                contract_order.push_back(front);
                front = contract_order.front();
            }
        }
    }
    graph.get_changed_vertices(remaining_vertices);
    debug << "Printing shortcuts\n";
    for (auto shortcut : graph.shortcuts) {
        debug << shortcut;
        shortcut_edges.push_back(shortcut);
    }
}

bool is_valid_contraction_number(int number) {
    switch (number) {
        case -2:
        return false;
        break;
        case -1:
        return false;
        break;
        case 0:
        return true;
        break;
        case 1:
        return true;
        break;
        default:
        return false;
        break;
    }
}

template < class G >
class Pgr_contract {
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
     // @{
    typedef typename G::V V;
    typedef typename G::E E;
    typedef typename G::V_i V_i;
    typedef typename G::E_i E_i;
    typedef typename G::EO_i EO_i;
    #if 0
    typedef typename G::degree_to_V_i degree_to_V_i;
    #endif
    typedef typename G::EI_i EI_i;
    // @}
    // ! @name Framework related functions
    // @{
    /*! \brief Disconnects all incoming and outgoing edges from the vertex
     *  Calls the disconnect_vertex function of the graph with assertions
     *  @param graph
     *  @param vertex_id
     */
    void disconnectVertex(G &graph, int64_t vertex_id);
    /*! \brief Stores the ids of those vertices forbidden from contraction in a set
     *  Stores them in the set *forbidden_vertices*
     *  @param forbidden_vertices
     *  @param size_forbidden_vertices
     */
    void setForbiddenVertices(int64_t *forbidden_vertices,
    size_t size_forbidden_vertices);

    /*! \brief Stores ids of all the vertices of the graph in a set
     *  Stores them in the set *all_vertices*
     *  @param graph
     */
    void getAllVertices(G &graph);

    /*! \brief Stores the set of ids of those vertices which are not contracted
     *  Stores them in the set *non_contracted*
     */
    void getNonContractedVertices();

    /*! \brief Returns a set of ids of all those vertices adjacent to vertex 
     *  with id *vertex_id* 
     *  Calls the disconnect_vertex function of the graph with assertions
     *  @param graph
     *  @param vertex_id
     */
    Identifiers<int64_t> getAdjacentVertices(G &graph, int64_t vertex_id);
    /*! \brief Writes the string form of identifier class to the *stream*
     *  @param stream
     *  @param identifiers
     */
    void print_identifiers(std::ostringstream& stream, Identifiers<int64_t> identifiers);

    /*! \brief Writes the string form of forbidden vertices to the *stream*
     *  @param stream
     */
    void print_forbidden_vertices(std::ostringstream& stream);

    /*! \brief Writes the string form of all vertices to the *stream*
     *  @param stream
     */
    void print_all_vertices(std::ostringstream& stream);

    /*! \brief Writes the string form of non contracted vertices to the *stream*
     *  @param stream
     */
    void print_non_contracted_vertices(std::ostringstream& stream);

    // @}
    // ! @name Dead end contraction related functions
    // @{

    /*! \brief Checks whether a vertex is a dead end vertex
     *  @param graph
     *  @param vertex_id
     */
    bool is_dead_end(G &graph, int64_t vertex_id) const;

    /*! \brief Stores ids of dead end vertices of the graph in a set
     *  Stores them in the set *dead_end_vertices*
     *  @param graph
     */
    void getDeadEndSet(G &graph);

    /*! \brief Writes the string form of dead end vertices to the *stream*
     *  @param stream
     */
    void print_dead_end_vertices(std::ostringstream& stream);

    // @}

    // bool is_connected(G &graph, V v) const;
    #if 0
    void contract_to_level(
        G &graph,
        int64_t level);
    #endif

    #if 0
    void dead_end_contraction(G &graph);
    void remove_2_degree_vertices(G &graph);


    void calculateDegrees(G &graph);

    void degreeMap(G &graph, std::ostringstream& dmap);

    void getGraphName(std::ostringstream& name, Contraction_type ctype);

    int64_t getGraph_string(G &graph, std::ostringstream& estring);

    void getRemovedE_string(G &graph, std::ostringstream& estring);

    void getRemovedV_string(std::ostringstream& vstring);

    void getPsuedoE_string(std::ostringstream& pstring);

    typedef typename std::map<V, std::deque<Edge> > removed_V;
    typedef typename std::map<V, std::deque<Edge> >::iterator removed_V_i;
    typedef typename std::map<int64_t, std::pair<int64_t, int64_t> > psuedo_E;
    typedef typename std::map<int64_t, std::pair<int64_t, int64_t> >::iterator psuedo_E_i;
    typedef std::map< int, std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t> > > degree_to_V;
    //  typedef std::map< int, std::vector<int64_t> > degree_to_V;
    typedef typename std::vector<V>::iterator Q_i;
    #endif

 private:
    int64_t last_edge_id;
    // ! Used for storing the ids of all vertices of the graph
    Identifiers<int64_t> all;
    // ! Used for storing the ids of dead end vertices of the graph
    Identifiers<int64_t> dead_end;
    // ! Used for storing the ids of vertices of the graph which are not contracted
    Identifiers<int64_t> non_contracted;
    // ! Used for storing the ids of vertices forbidden from contraction
    Identifiers<int64_t> forbidden;
    #if 0
    removed_V removedVertices;
    psuedo_E psuedoEdges;
    degree_to_V degree_to_V_map;
    #endif
    // set of dead_end_vertices;
    // Identifiers<V> dead_end_vertices;
};


/******************** IMPLEMENTATION ******************/
template < class G >
void Pgr_contract< G >::disconnectVertex(G &graph, int64_t vertex_id) {
    pgassert(graph.is_connected(vertex_id));
    pgassert(is_dead_end(vertex_id));
    graph.disconnect_vertex(vertex_id);
    pgassert(!graph.is_connected(vertex_id));
}

template < class G >
void Pgr_contract< G >::setForbiddenVertices(int64_t *forbidden_vertices,
    size_t size_forbidden_vertices ) {
    for (int64_t i = 0; i < size_forbidden_vertices; ++i) {
            forbidden += forbidden_vertices[i];
        }
}

template <class G>
void Pgr_contract< G >::getAllVertices(G &graph) {
    // Identifiers<int64_t> dead_end_vertices;
    V_i vi;
    for (vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
        //  debug << "Checking vertex " << graph.graph[(*vi)].id << '\n';
            all += graph.graph[(*vi)].id;
    }
}

template <class G>
void Pgr_contract< G >::getNonContractedVertices() {
    non_contracted = all - dead_end;
}

template < class G >
Identifiers<int64_t> Pgr_contract< G >::getAdjacentVertices(G &graph, int64_t vertex_id) {
    EO_i out, out_end;
    EI_i in, in_end;
    V v;
    Identifiers<int64_t> adjacent_vertices_set;
    if (!graph.has_vertex(vertex_id)) {
            return adjacent_vertices_set;
    }
    v = graph.get_V(vertex_id);
    for (boost::tie(out, out_end) = out_edges(v, graph.graph);
            out != out_end; ++out) {
            adjacent_vertices_set += graph.graph[target(*out, graph.graph)].id;
        }
    for (boost::tie(in, in_end) = in_edges(v, graph.graph);
            in != in_end; ++in) {
            adjacent_vertices_set += graph.graph[source(*in, graph.graph)].id;
        }
    return adjacent_vertices_set;
}

template <class G>
void Pgr_contract< G >::print_identifiers(std::ostringstream& stream, Identifiers<int64_t> identifiers) {
    stream << identifiers << '\n';
}

template <class G>
void Pgr_contract< G >::print_forbidden_vertices(std::ostringstream& stream) {
    stream << forbidden << '\n';
}

template <class G>
void Pgr_contract< G >::print_all_vertices(std::ostringstream& stream) {
    stream << all << '\n';
}


template <class G>
void Pgr_contract< G >::print_non_contracted_vertices(std::ostringstream& stream) {
    stream << non_contracted << '\n';
}

template < class G >
bool Pgr_contract< G >::is_dead_end(G &graph, int64_t vertex_id) const {
    // debug << "in_degree: " << graph.in_degree(vertex_id) << '\n';
    // debug << "out_degree: " << graph.out_degree(vertex_id) << '\n';
    V v;
    if (!graph.has_vertex(vertex_id)) {
            return false;
    }
    v = graph.get_V(vertex_id);
    if (graph.out_degree(v) == 1 && graph.in_degree(v) == 0) return true;
    if (graph.out_degree(v) == 0 && graph.in_degree(v) == 1) return true;
    if (graph.out_degree(v) == 1 && graph.in_degree(v) == 1) {
        int64_t incoming_edge_id, outgoing_edge_id;
        EO_i out, out_end;
        EI_i in, in_end;
        for (boost::tie(out, out_end) = out_edges(v, graph.graph);
                        out != out_end; ++out) {
            outgoing_edge_id = graph.graph[*out].id;
        }
        for (boost::tie(in, in_end) = in_edges(v, graph.graph);
                        in != in_end; ++in) {
            incoming_edge_id = graph.graph[*in].id;
        }
        if (incoming_edge_id == outgoing_edge_id)
            return true;
        return false;
    }
    return false;
}

template <class G>
void Pgr_contract< G >::getDeadEndSet(G &graph) {
    V_i vi;
    for (vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
        //  debug << "Checking vertex " << graph.graph[(*vi)].id << '\n';
        if (is_dead_end(graph, graph.graph[(*vi)].id)) {
            //  debug << "Adding " << graph.graph[(*vi)].id << "to dead end" << '\n';
            dead_end += graph.graph[(*vi)].id;
        }
    }
}

template <class G>
void Pgr_contract< G >::print_dead_end_vertices(std::ostringstream& stream) {
    stream << dead_end << '\n';
}
