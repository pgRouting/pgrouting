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

#ifndef INCLUDE_LINEGRAPH_PGR_LINEGRAPH_HPP_
#define INCLUDE_LINEGRAPH_PGR_LINEGRAPH_HPP_
#pragma once


#include <vector>
#include <set>
#include <utility>
#include <map>
#include <algorithm>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/line_vertex.h"

namespace pgrouting {

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_lineGraph : public Pgr_base_graph<G, T_V, T_E> {
 public:
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    typedef typename boost::graph_traits < G >::edge_descriptor E;
    typedef typename boost::graph_traits < G >::vertex_iterator V_i;
    typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
    typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;


    explicit Pgr_lineGraph< G, T_V, T_E >(graphType gtype)
        : Pgr_base_graph< G, T_V, T_E >(gtype)
        {
        }

#if 0
    template < typename T >
        void insert_vertices(const T* edges, int64_t count) {
            insert_vertices(std::vector < T >(edges, edges + count));
        }
#endif

    template < typename T >
        void insert_vertices(const std::vector < T > &edges) {
            for (auto &it : edges) {
                m_edges[it.id] = it;
            }
            std::vector<Line_vertex> vertices = extract_vertices();

#if 0
            add_vertices(vertices);
#endif
        }

    void transform(pgrouting::DirectedGraph& digraph) {
        create_edges(digraph);
    }


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

    std::vector< Line_graph_rt >
        get_postgres_results_undirected() {
            std::vector< Line_graph_rt > results;

            typename boost::graph_traits < G >::edge_iterator edgeIt, edgeEnd;
            int64_t count = 0;

            for (boost::tie(edgeIt, edgeEnd) = boost::edges(this->graph);
                    edgeIt != edgeEnd; edgeIt++) {
                E e = *edgeIt;
                auto e_source = this->graph[this->source(e)].vertex_id;
                auto e_target = this->graph[this->target(e)].vertex_id;

                Line_graph_rt edge = {
                    ++count,
                    e_source,
                    e_target,
                    1.0,
                    -1.0
                };
                results.push_back(edge);
            }

            return results;
        }

    std::vector< Line_graph_rt >
        get_postgres_results_directed() {
            std::vector< Line_graph_rt > results;

            typename boost::graph_traits < G >::edge_iterator edgeIt, edgeEnd;
            std::map < std::pair<int64_t, int64_t >, Line_graph_rt > unique;
            int64_t count = 0;

            for (boost::tie(edgeIt, edgeEnd) = boost::edges(this->graph);
                    edgeIt != edgeEnd; edgeIt++) {
                E e = *edgeIt;
                auto e_source = this->graph[this->source(e)].vertex_id;
                auto e_target = this->graph[this->target(e)].vertex_id;

                if (unique.find({e_target, e_source}) != unique.end()) {
                    unique[std::pair<int64_t, int64_t>(e_target,
                            e_source)].reverse_cost = 1.0;
                    continue;
                }
                e_source *= -1;
                e_target *= -1;
                if (unique.find({e_target, e_source}) != unique.end()) {
                    unique[std::pair<int64_t, int64_t>(e_target,
                            e_source)].reverse_cost = 1.0;
                    continue;
                }
                e_source *= -1;
                e_target *= -1;

                Line_graph_rt edge = {
                    ++count,
                    e_source,
                    e_target,
                    1.0,
                    -1.0
                };
                unique[std::pair<int64_t, int64_t>(e_source, e_target)] = edge;
            }
            for (const auto &edge : unique) {
                results.push_back(edge.second);
            }
            return results;
        }

    std::vector<Line_vertex>
        extract_vertices() {
            /*
               m_vertex_map stores a unique id assigned to each vertex of Line Graph.

               In case of a directed edge, either 1 or 2 vertices are to be created in
               the Line Graph for each of the edges.
               Consider the following edge in directed graph:-
               ID = 1 | source = 2 | target = 3 | cost = 10 | reverse_cost = 20
               This creates 2 vertices in Line Graph:-
               1. ID = 1 | source = 2 | target = 3 | cost = 10
               2. ID = 1 | source = 3 | target = 2 | cost = 25
               So, the values stored in m_vertex_map would be:-
               1. {1, 2} = 1(Denoting the edge from 2 - > 3 of cost 10).
               2. {1, 3} = 2(Denoting the edge from 3 - > 2 of cost 25).
               where {key} = value in m_vertex_map.

               In case of undirected edge, either 2 or 4 vertices are to be created in
               the Line Graph for each of the edges.
               Consider the following edge in an undirected graph:-
               ID = 1 | source = 2 | target = 3 | cost = 10 | reverse_cost = 25
               This creates the following 4 vertices in Line Graph:-
               1. ID = 1 | source = 2 | target = 3 | cost = 10
               2. ID = 1 | source = 3 | target = 2 | cost = 10
               3. ID = 1 | source = 3 | target = 2 | cost = 25
               4. ID = 1 | source = 2 | target = 3 | cost = 25
               so, the values stored in m_vertex_map would be:-
               1. {1, 2} = 1(Denoting the edge from 2 - > 3 of cost 10).
               2. {-1, 3} = 2(Denoting the edge from 3 - > 2 of cost 10).
               3. {1, 3} = 3(Deonting the edge from 3 - > 2 of cost 25).
               4. {-1, 2} = 4(Denoting the edge from 2 - > 3 of cost 25).
               where {key} = value in m_vertex_map.
               */
            if (m_edges.empty()) return std::vector<Line_vertex>();

            std::vector<Line_vertex> vertices;

            for (const auto &it : m_edges) {
                auto edge = it.second;
                Line_vertex vertex(edge);

                if (edge.cost > 0) {
                    vertex.id = this->num_vertices() + 1;
                    m_vertex_map[std::pair<int64_t, int64_t>(edge.id, edge.source)] =
                        vertex.id;
                    vertices.push_back(vertex);
                    add_one_vertex(vertex);

                    if (this->m_gType == UNDIRECTED) {
                        vertex.id = this->num_vertices() + 1;
                        m_vertex_map[std::pair<int64_t, int64_t>(-1*edge.id,
                                edge.target)] = vertex.id;
                        std::swap(vertex.source, vertex.target);
                        vertices.push_back(vertex);
                        add_one_vertex(vertex);
                        std::swap(vertex.source, vertex.target);
                    }
                }

                if (edge.reverse_cost > 0) {
                    vertex.id = this->num_vertices() + 1;
                    m_vertex_map[ std::pair<int64_t, int64_t>(edge.id,
                            edge.target) ] = vertex.id;
                    vertex.cost = edge.reverse_cost;
                    vertex.vertex_id *= -1;
                    std::swap(vertex.source, vertex.target);
                    vertices.push_back(vertex);
                    add_one_vertex(vertex);

                    if (this->m_gType == UNDIRECTED) {
                        vertex.id = this->num_vertices() + 1;
                        m_vertex_map[std::pair<int64_t, int64_t>(-1 * edge.id,
                                edge.source)] = vertex.id;
                        std::swap(vertex.source, vertex.target);
                        vertices.push_back(vertex);
                        add_one_vertex(vertex);
                    }
                }
            }
            return vertices;
        }

 private:
    template < typename T>
        void
        graph_add_edge(
                const T &source,
                const T &target,
                int64_t source_in_edge,
                int64_t source_out_edge) {
            bool inserted;
#if 0
            typename Pgr_base_graph< G, T_V, T_E >::E e;
#endif
            E e;

            pgassert(m_vertex_map.find({source, source_in_edge}) !=
                    m_vertex_map.end());
            pgassert(m_vertex_map.find({target, source_out_edge}) !=
                    m_vertex_map.end());

            auto index_source_edge = m_vertex_map[std::pair<int64_t, int64_t>(source,
                    source_in_edge) ];
            auto index_target_edge = m_vertex_map[ std::pair<int64_t, int64_t>(target,
                    source_out_edge) ];

            auto vm_s = this->get_V(index_source_edge);
            auto vm_t = this->get_V(index_target_edge);

            pgassert(this->vertices_map.find(index_source_edge) !=
                    this->vertices_map.end());
            pgassert(this->vertices_map.find(index_target_edge) !=
                    this->vertices_map.end());

            boost::tie(e, inserted) =
                boost::add_edge(vm_s, vm_t, this->graph);

            this->graph[e].id = num_edges(this->graph);
        }

    void create_edges(
            const pgrouting::DirectedGraph& digraph) {
        V_i vertexIt, vertexEnd;
        EO_i e_outIt, e_outEnd;
        EI_i e_inIt, e_inEnd;


        /* for (each vertex v in original graph) */
        for (boost::tie(vertexIt, vertexEnd) = boost::vertices(digraph.graph);
                vertexIt != vertexEnd; vertexIt++) {
            auto vertex = *vertexIt;

           /* for( all incoming edges in to vertex v) */
            for (boost::tie(e_outIt, e_outEnd) =
                    boost::out_edges(vertex, digraph.graph); e_outIt != e_outEnd;
                    e_outIt++) {
                /* for( all outgoing edges out from vertex v) */
                for (boost::tie(e_inIt, e_inEnd) =
                        boost::in_edges(vertex, digraph.graph);
                        e_inIt != e_inEnd; e_inIt++) {

                    /*
                       Prevent self-edges from being created in the Line Graph
                       */
#if 0
                    if (labs(static_cast<long>(digraph.graph[*e_inIt].id))
                            == labs(static_cast<long>(digraph.graph[*e_outIt].id)))
                        continue;
#endif

                    auto source_in_edge = digraph.source(*e_inIt);

                    graph_add_edge(
                            (digraph.graph[*e_inIt]).id,
                            (digraph.graph[*e_outIt]).id,
                            digraph[source_in_edge].id,
                            digraph[vertex].id);
                }
            }
        }
    }

    V add_one_vertex(
            T_V vertex) {
        pgassert(this->vertices_map.find(vertex.id) ==
                this->vertices_map.end());

        auto v =  add_vertex(this->graph);
        this->vertices_map[vertex.id] = v;
        this->graph[v].cp_members(vertex);

        pgassert(boost::num_vertices(this->graph) == this->num_vertices());
        return v;
    }

#if 0
    void add_vertices(
            std::vector< T_V > vertices) {
        for (const auto vertex : vertices) {
            add_one_vertex(vertex);
        }
        return;
    }
#endif

 private:
    std::map < int64_t, pgr_edge_t > m_edges;
    std::map < std::pair< int64_t, int64_t >, int64_t > m_vertex_map;

 public:
    std::ostringstream log;

};

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_LINEGRAPH_PGR_LINEGRAPH_HPP_
