/*PGR-GNU*****************************************************************
File: pgr_lineGraph.hpp
Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org
Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com
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

#ifndef INCLUDE_COMPONENTS_PGR_LINEGRAPH_HPP_
#define INCLUDE_COMPONENTS_PGR_LINEGRAPH_HPP_
#pragma once


#include <vector>
#include <set>
#include <utility>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/line_vertex.h"

namespace pgrouting {

namespace graph {
template <class G, typename T_V, typename T_E>
class Pgr_lineGraph;
}  // namespace graph

typedef graph::Pgr_lineGraph <
boost::adjacency_list < boost::vecS, boost::vecS,
    boost::bidirectionalS,
    Line_vertex, Basic_edge >,
    Line_vertex, Basic_edge > LinearDirectedGraph;

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_lineGraph : public Pgr_base_graph<G, T_V, T_E> {
 private:
    std::vector< Line_graph_rt > m_lineGraph_edges;
    int64_t m_num_edges;

    std::vector < int64_t > m_vertex_map;

    void add_vertices(std::vector< T_V > vertices);

    template < typename T >
    void create_edges(const pgrouting::DirectedGraph& digraph, const T vertex);

    template < typename T >
        void graph_add_edge(const T &edge);

 public:
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    typedef typename boost::graph_traits < G >::edge_descriptor E;
    typedef typename boost::graph_traits < G >::vertex_iterator V_i;
    typedef typename boost::graph_traits < G >::edge_iterator E_i;
    typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
    typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;

    std::ostringstream log;

    explicit Pgr_lineGraph< G, T_V, T_E >(graphType gtype)
         : Pgr_base_graph< G, T_V, T_E >(gtype),
           m_num_edges(0) {
         }

    template < typename T >
        void insert_vertices(const T* edges, int64_t count) {
            insert_vertices(std::vector < T >(edges, edges + count));
        }

    template < typename T >
        void insert_vertices(const std::vector < T > &edges) {
            std::vector < Line_vertex > vertices = extract_vertices(edges);
            log << "\nVertices of line graph: \n";
            for (auto vertex: vertices) {
                log << vertex.id << "(" << vertex.source << " - > ";
                log << vertex.target << ")" << vertex.cost << "\n";
            }
            add_vertices(vertices);
        }

    template < typename T >
        std::vector < Line_vertex > extract_vertices(
            const std::vector < T > data_edges);

    void transform(pgrouting::DirectedGraph& digraph) {
        V_i vertexIt, vertexEnd;

        for(boost::tie(vertexIt, vertexEnd) = boost::vertices(digraph.graph);
            vertexIt != vertexEnd; vertexIt++) {
            create_edges(digraph, *vertexIt);
        }
    }

    std::vector< Line_graph_rt > Linegraph() const { return m_lineGraph_edges;}
    int64_t num_edges() const { return m_num_edges; }

    void insert_edges() {
        for (auto edge: m_lineGraph_edges) {
            graph_add_edge(edge);
        }
    }
};

template < class G, typename T_V, typename T_E >
template < typename T>
std::vector < Line_vertex >
Pgr_lineGraph< G, T_V, T_E >::extract_vertices(
        const std::vector < T > data_edges) {
    if (data_edges.empty()) return std::vector< Line_vertex >();

    std::vector< Line_vertex > vertices;

    for (const auto edge : data_edges) {
        Line_vertex vertex(edge);

        if (edge.cost > 0) {
            vertex.id = (++(this->m_num_vertices));
            vertices.push_back(vertex);
            m_vertex_map.push_back( edge.id );
        }

        if (edge.reverse_cost > 0) {
            vertex.id = (++(this->m_num_vertices));
            vertex.cost = edge.reverse_cost;
            std::swap(vertex.source, vertex.target);
            vertices.push_back(vertex);
            m_vertex_map.push_back( edge.id );
        }
    }

    return vertices;
}

template < class G, typename T_V, typename T_E >
template < typename T>
void
Pgr_lineGraph< G, T_V, T_E >::graph_add_edge(const T &edge) {
    bool inserted;
    typename Pgr_base_graph< G, T_V, T_E >::E e;

    auto vm_s = this->get_V(edge.source);
    auto vm_t = this->get_V(edge.target);

    pgassert(this->vertices_map.find(edge.source) != this->vertices_map.end());
    pgassert(this->vertices_map.find(edge.target) != this->vertices_map.end());

    boost::tie(e, inserted) =
        boost::add_edge(vm_s, vm_t, this->graph);

    this->graph[e].id = edge.id;

    if (edge.reverse) {
        boost::tie(e, inserted) =
            boost::add_edge(vm_t, vm_s, this->graph);
        this->graph[e].id = edge.id;
    }
}


template < class G, typename T_V, typename T_E >
template < typename T >
void Pgr_lineGraph< G, T_V, T_E >::create_edges(
        const pgrouting::DirectedGraph& digraph,
        const T vertex) {
    log << digraph.graph[vertex] << "\n";
    EO_i outIt, outEnd;
    EI_i inIt, inEnd;
    std::set < std::pair< T, std::pair < T, T > > > unique;
    for (boost::tie(outIt, outEnd) = boost::out_edges(vertex, digraph.graph);
        outIt != outEnd; outIt++) {
        for (boost::tie(inIt, inEnd) = boost::in_edges(vertex, digraph.graph);
            inIt != inEnd; inIt++) {
            ++m_num_edges;

            log << "\n";
            log << digraph.graph[*inIt].id << " | " << digraph[digraph.source(*inIt)].id << " | " << digraph[digraph.target(*inIt)].id << " | " << digraph.graph[*inIt].cost << "\n";
            log << digraph.graph[*outIt].id << " | " << digraph[digraph.source(*outIt)].id << " | " << digraph[digraph.target(*outIt)].id << " | " << digraph.graph[*outIt].cost << "\n\n";

            auto source_in_edge = digraph.source(*inIt);
            auto target_out_edge = digraph.target(*outIt);

            log << "source = " << digraph[source_in_edge].id << " | target = " << digraph[target_out_edge].id << "\n\n\n";

            if (unique.count( {vertex, {target_out_edge,  source_in_edge} }))
                continue;

            bool is_reverse = false;
            if (boost::edge(target_out_edge, vertex, digraph.graph).second and
                    boost::edge(vertex, source_in_edge, digraph.graph).second)
                is_reverse = true;

            unique.insert( {vertex, {source_in_edge, target_out_edge} });

            m_lineGraph_edges.push_back({
                m_num_edges,
                (digraph.graph[*inIt]).id,
                (digraph.graph[*outIt]).id,
                is_reverse
            });
            log << "\nEdge: ";
            log << "id = " << m_lineGraph_edges.back().id;
            log << " | source = " << m_lineGraph_edges.back().source;
            log << " | target = " << m_lineGraph_edges.back().target;
            log << " | reverse = " << m_lineGraph_edges.back().reverse << "\n";
        }
    }
}

template < class G, typename T_V, typename T_E >
 void
 Pgr_lineGraph< G, T_V, T_E >::add_vertices(
         std::vector< T_V > vertices) {

     for (const auto vertex : vertices) {
         auto v =  add_vertex(this->graph);
         this->graph[v].cp_members(vertex);

         pgassert(boost::num_vertices(this->graph) == this->num_vertices());
     }
     return;
 }

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_PGR_LINEGRAPH_HPP_
