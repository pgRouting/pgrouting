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
#include <map>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/line_vertex.h"
#include "dijkstraTRSP/restriction.h"

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
    int64_t m_num_edges;
    std::map < int64_t, pgr_edge_t > m_edges;
    std::map < std::pair< int64_t, int64_t >, int64_t > m_vertex_map;

    void add_vertices(std::vector< T_V > vertices);

    void create_edges(const pgrouting::DirectedGraph& digraph,
            std::vector< Line_graph_rt >& line_graph_edges);

    template < typename T >
        void graph_add_edge(const T &edge, int64_t, int64_t);

    template < typename T >
    void disconnect_edge(const T& from, const T& to);

    template < typename T >
    void get_ids(std::vector< T >& restrictions);

    void create_virtual_vertex(int64_t id);
    void create_virtual_edge(
            int64_t source_id,
            int64_t source_vertex,
            int64_t target_id,
            int64_t target_vertex
        );

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
            for (auto &it: edges)
                m_edges[it.id] = it;
            std::vector < Line_vertex > vertices = extract_vertices();
            log << "\nVertices of line graph: \n";
            for (auto vertex: vertices) {
                log << vertex.id << "(" << vertex.source << " - > ";
                log << vertex.target << ")" << vertex.cost << "\n";
            }
            add_vertices(vertices);
        }
    template < typename T >
        std::vector< Restriction > remove_restricted_edges(std::vector< T >& restrictions) {
            get_ids(restrictions);
            std::vector< T > remaining;
            for (const auto &r: restrictions) {
                if (r.restriction_size() > 2) {
                    remaining.push_back(r);
                    continue;
                }
                disconnect_edge(r.restrict_edges()[0], r.restrict_edges()[1]);
            }
            return remaining;
        }

    std::vector < Line_vertex > extract_vertices();

    std::vector< Line_graph_rt > transform(pgrouting::DirectedGraph& digraph);
    void create_virtual_vertices();

    int64_t num_edges() const { return m_num_edges; }

    friend std::ostream& operator<<(
            std::ostream &log, const Pgr_lineGraph< G, T_V, T_E > &g) {
        typename Pgr_base_graph< G, T_V, T_E >::EO_i out, out_end;

        for (auto vi = vertices(g.graph).first;
                vi != vertices(g.graph).second; ++vi) {
            if ((*vi) >= g.m_num_vertices) break;
            log << (*vi) << ": " << " out_edges_of(" << g.graph[(*vi)] << "):";
            for (boost::tie(out, out_end) = out_edges(*vi, g.graph);
                    out != out_end; ++out) {
                log << ' '
                    << g.graph[*out].id << "=("
                    << g[g.source(*out)].id << ", "
                    << g[g.target(*out)].id << ")\t";
            }
            log << std::endl;
        }
        return log;
    }
};

template < class G, typename T_V, typename T_E >
template < typename T >
void
Pgr_lineGraph< G, T_V, T_E >::get_ids(std::vector< T >& restrictions) {
    for (auto &r: restrictions) {
        auto restrict_edges = r.restrict_edges();
        std::vector < int64_t > temp;

        pgassert(m_edges.find(restrict_edges[0]) != m_edges.end());
        auto prev = m_edges[restrict_edges[0]];

        for (auto i = 1; i < (int64_t)restrict_edges.size(); i++) {
            pgassert(m_edges.find(restrict_edges[i]) != m_edges.end());
            auto cur = m_edges[restrict_edges[i]];

            if (prev.target == cur.target) {
                std::swap(cur.source, cur.target);
                std::swap(cur.cost, cur.reverse_cost);
            }

            if(prev.source == cur.source) {
                std::swap(prev.source, prev.target);
                std::swap(prev.cost, prev.reverse_cost);
            }

            if(prev.source == cur.target) {
                std::swap(prev.source, prev.target);
                std::swap(prev.cost, prev.reverse_cost);
                std::swap(cur.source, cur.target);
                std::swap(cur.cost, cur.reverse_cost);
            }

            pgassert(m_vertex_map.find( {prev.id, prev.source} ) != m_vertex_map.end());
            pgassert(m_vertex_map.find( {cur.id, cur.source} ) != m_vertex_map.end());

            if (temp.empty()) {
                temp.push_back( m_vertex_map[ {prev.id, prev.source} ] );
            }

            temp.push_back( m_vertex_map[ {cur.id, cur.source} ] );
            prev = cur;
        }
        r.clear();
        for (const auto &it: temp) r.restrict_edges(it);
    }
}

template < class G, typename T_V, typename T_E >
template < typename T >
void
Pgr_lineGraph< G, T_V, T_E >::disconnect_edge(const T& from, const T& to) {

    pgassert(this->vertices_map.find(from) != this->vertices_map.end());
    pgassert(this->vertices_map.find(to) != this->vertices_map.end());

    auto vm_s = this->get_V(from);
    auto vm_t = this->get_V(to);

    boost::remove_edge(vm_s, vm_t, this->graph);
}

template < class G, typename T_V, typename T_E >
void
Pgr_lineGraph< G, T_V, T_E >::create_virtual_vertex(int64_t id) {
    ++(this->m_num_vertices);
    auto v = add_vertex(this->graph);
    this->vertices_map[this->m_num_vertices] = v;
    this->graph[v].cp_members(this->m_num_vertices, id);
    m_vertex_map[ {id, -1} ] = this->m_num_vertices;
    pgassert(boost::num_vertices(this->graph) == this->num_vertices());
}

template < class G, typename T_V, typename T_E >
void
Pgr_lineGraph< G, T_V, T_E >::create_virtual_edge(
        int64_t source_id,
        int64_t source_vertex,
        int64_t target_id,
        int64_t target_vertex) {
    bool inserted;
    typename Pgr_base_graph< G, T_V, T_E >::E e;

    if (source_id < 0) source_id *= -1;
    if (target_id < 0) target_id *= -1;

    pgassert(m_vertex_map.find( {source_id, source_vertex} ) !=
        m_vertex_map.end());
    pgassert(m_vertex_map.find( {target_id, target_vertex} ) !=
            m_vertex_map.end());

    auto index_source_edge = m_vertex_map[ {source_id, source_vertex} ];
    auto index_target_edge = m_vertex_map[ {target_id, target_vertex} ];

    auto vm_s = this->get_V(index_source_edge);
    auto vm_t = this->get_V(index_target_edge);

    boost::tie(e, inserted) =
        boost::add_edge(vm_s, vm_t, this->graph);

    ++m_num_edges;
    this->graph[e].id = m_num_edges;
}

template < class G, typename T_V, typename T_E >
void
Pgr_lineGraph< G, T_V, T_E >::create_virtual_vertices() {
    V_i vertexIt, vertexEnd;
    boost::tie(vertexIt, vertexEnd) = boost::vertices(this->graph);
    for (;vertexIt != vertexEnd; vertexIt++) {
        auto vertex = this->graph[*vertexIt];
        if (!m_vertex_map.count( {vertex.source, -1} )) {
            create_virtual_vertex(vertex.source);
        }
        if(!m_vertex_map.count( {vertex.target, -1} )) {
            create_virtual_vertex(vertex.target);
        }

        pgassert(m_vertex_map.find( {vertex.source, -1} ) !=
            m_vertex_map.end());
        pgassert(m_vertex_map.find( {vertex.target, -1} ) !=
                m_vertex_map.end());

        create_virtual_edge(vertex.source, -1, vertex.vertex_id, vertex.source);
        create_virtual_edge(vertex.vertex_id, vertex.source, vertex.target, -1);
    }
}

template < class G, typename T_V, typename T_E >
std::vector< Line_graph_rt >
Pgr_lineGraph< G, T_V, T_E >::transform(pgrouting::DirectedGraph& digraph) {
    std::vector< Line_graph_rt > line_graph_edges;

    create_edges(digraph, line_graph_edges);

    log << "\nEdges: \n";
    for (auto it: line_graph_edges) {
        log << "id = " << it.id;
        log << " | source = " << it.source;
        log << " | target = " << it.target;
        log << " | cost = " << it.cost;
        log << " | reverse_cost = " << it.reverse_cost << "\n";
    }
    return line_graph_edges;
}

template < class G, typename T_V, typename T_E >
std::vector < Line_vertex >
Pgr_lineGraph< G, T_V, T_E >::extract_vertices() {
    if (m_edges.empty()) return std::vector< Line_vertex >();

    std::vector< Line_vertex > vertices;

    log << "\nEdges of original graph\n";
    for (const auto &it : m_edges) {
        auto edge = it.second;
        Line_vertex vertex(edge);

        log << "ID: " << edge.id;
        log << "| source: " << edge.source;
        log << "| target: " << edge.target;
        log << "| cost: " << edge.cost;
        log << "| reverse_cost: " << edge.reverse_cost << "\n\n";

        if (edge.cost > 0) {
            vertex.id = (++(this->m_num_vertices));
            vertices.push_back(vertex);
            m_vertex_map[ {edge.id, edge.source} ] = this->m_num_vertices;
        }

        if (edge.reverse_cost > 0) {
            vertex.id = (++(this->m_num_vertices));
            vertex.cost = edge.reverse_cost;
            vertex.vertex_id *= -1;
            std::swap(vertex.source, vertex.target);
            vertices.push_back(vertex);
            m_vertex_map[ {edge.id, edge.target} ] = this->m_num_vertices;
        }
    }

    for (auto it: m_vertex_map) {
        log << it.first.first << " | " << it.first.second << " | " << it.second << "\n";
    }

    return vertices;
}

template < class G, typename T_V, typename T_E >
template < typename T>
void
Pgr_lineGraph< G, T_V, T_E >::graph_add_edge(
        const T &edge,
        int64_t source_in_edge,
        int64_t source_out_edge) {

    bool inserted;
    typename Pgr_base_graph< G, T_V, T_E >::E e;

    pgassert(m_vertex_map.find( {edge.source, source_in_edge} ) !=
        m_vertex_map.end());
    pgassert(m_vertex_map.find( {edge.target, source_out_edge} ) !=
            m_vertex_map.end());

    auto index_source_edge = m_vertex_map[ {edge.source, source_in_edge} ];
    auto index_target_edge = m_vertex_map[ {edge.target, source_out_edge} ];

    log << "\nsource_in_edge = " << source_in_edge << " | "
        << "source_out_edge = " << source_out_edge << " | "
        << "index_source_edge = " << index_source_edge << " | "
        << "index_target_edge = " << index_target_edge << " | "
        << "edge.source = " << edge.source << " | "
        << "edge.target = " << edge.target << "\n";

    auto vm_s = this->get_V(index_source_edge);
    auto vm_t = this->get_V(index_target_edge);

    pgassert(this->vertices_map.find(index_source_edge) != this->vertices_map.end());
    pgassert(this->vertices_map.find(index_target_edge) != this->vertices_map.end());

    boost::tie(e, inserted) =
        boost::add_edge(vm_s, vm_t, this->graph);

    this->graph[e].id = edge.id;
}

template < class G, typename T_V, typename T_E >
void
Pgr_lineGraph< G, T_V, T_E >::create_edges(
        const pgrouting::DirectedGraph& digraph,
        std::vector< Line_graph_rt >& line_graph_edges) {

    V_i vertexIt, vertexEnd;
    EO_i outIt, outEnd;
    EI_i inIt, inEnd;

    std::map < std::pair< int64_t, int64_t >, Line_graph_rt > unique;

    for(boost::tie(vertexIt, vertexEnd) = boost::vertices(digraph.graph);
            vertexIt != vertexEnd; vertexIt++) {
        V vertex = *vertexIt;

        for (boost::tie(outIt, outEnd) = boost::out_edges(vertex, digraph.graph);
                outIt != outEnd; outIt++) {
            for (boost::tie(inIt, inEnd) = boost::in_edges(vertex, digraph.graph);
                    inIt != inEnd; inIt++) {

                log << "\n";
                log << digraph.graph[*inIt].id << " | " << digraph[digraph.source(*inIt)].id << " | " << digraph[digraph.target(*inIt)].id << " | " << digraph.graph[*inIt].cost << "\n";
                log << digraph.graph[*outIt].id << " | " << digraph[digraph.source(*outIt)].id << " | " << digraph[digraph.target(*outIt)].id << " | " << digraph.graph[*outIt].cost << "\n\n";

                auto source_in_edge = digraph.source(*inIt);

                log << "source = " << digraph[source_in_edge] << " | mid = " << digraph[vertex] << "\n\n\n";

                if (digraph.graph[*inIt].id == digraph.graph[*outIt].id)
                    continue;

                Line_graph_rt edge = {
                    -1,
                    (digraph.graph[*inIt]).id,
                    (digraph.graph[*outIt]).id,
                    1.0,
                    -1.0
                };

                auto temp_source = edge.source;
                auto temp_target = edge.target;

                pgassert(m_edges.find(edge.source) != m_edges.end());
                pgassert(m_edges.find(edge.target) != m_edges.end());
                auto orig_edge1 = m_edges[edge.source];
                auto orig_edge2 = m_edges[edge.target];
                log << "\n" << orig_edge1.source << " | " << orig_edge1.target << "\n";
                log << "\n" << orig_edge2.source << " | " << orig_edge2.target << "\n";
                if (orig_edge1.target != orig_edge2.source) {
                    if (orig_edge1.target == orig_edge2.target) temp_target *= -1;
                    if (orig_edge1.source == orig_edge2.source) temp_source *= -1;
                    if (orig_edge1.source == orig_edge2.target) {
                        temp_source *= -1;
                        temp_target *= -1;
                    }
                }

                log << "\ntemp_source: " << temp_source << "\n";
                log << "\ntemp_target: " << temp_target << "\n";
                log << "\nKeys in unique: \n";
                for(const auto &it: unique)
                    log << it.first.first << " || " << it.first.second << "\n";

                if (unique.find( {temp_target, temp_source} ) ==
                        unique.end() and
                        unique.find( {temp_target*-1, temp_source*-1}) ==
                        unique.end()) {
                    log << "\nUnique\n";
                    edge.id = ++m_num_edges;
                    graph_add_edge(
                        edge,
                        digraph[source_in_edge].id,
                        digraph[vertex].id
                    );
                    edge.source = temp_source;
                    edge.target = temp_target;
                    log << "-------------------- " << edge.source << " | " << edge.target << "\n";
                    unique[ {edge.source, edge.target} ] = edge;
                    continue;
                }
                if (unique.find( {temp_target*-1, temp_source*-1}) !=
                        unique.end()) {
                    temp_source *= -1;
                    temp_target *= -1;
                }

                log << "\nNot Unique\n";
                edge.id = unique[ {temp_target, temp_source} ].id;
                graph_add_edge(
                    edge,
                    digraph[source_in_edge].id,
                    digraph[vertex].id
                );
                unique[ {temp_target, temp_source} ].reverse_cost = 1.0;
            }
        }
    }

    for(const auto &edge: unique)
        line_graph_edges.push_back(edge.second);
}

template < class G, typename T_V, typename T_E >
 void
 Pgr_lineGraph< G, T_V, T_E >::add_vertices(
         std::vector< T_V > vertices) {

     for (const auto vertex : vertices) {
         pgassert(this->vertices_map.find(vertex.id) == this->vertices_map.end());

         auto v =  add_vertex(this->graph);
         this->vertices_map[vertex.id] = v;
         this->graph[v].cp_members(vertex);

         pgassert(boost::num_vertices(this->graph) == this->num_vertices());
     }
     return;
 }

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_PGR_LINEGRAPH_HPP_
