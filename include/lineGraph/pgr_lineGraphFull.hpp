/*PGR-GNU*****************************************************************
File: pgr_lineGraphFull.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Anthony Nicola Tasca
Mail: atasca10@gmail.com

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

#ifndef INCLUDE_LINEGRAPH_PGR_LINEGRAPHFULL_HPP_
#define INCLUDE_LINEGRAPH_PGR_LINEGRAPHFULL_HPP_
#pragma once


#include <vector>
#include <set>
#include <utility>
#include <map>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/line_vertex.h"

namespace pgrouting {
namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_lineGraphFull : public Pgr_base_graph<G, T_V, T_E> {
 public:
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    typedef typename boost::graph_traits < G >::edge_descriptor E;
    typedef typename boost::graph_traits < G >::vertex_iterator V_i;
    typedef typename boost::graph_traits < G >::edge_iterator E_i;
    typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
    typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;


    explicit Pgr_lineGraphFull< G, T_V, T_E >(graphType gtype)
        : Pgr_base_graph< G, T_V, T_E >(gtype) {
        }

    Pgr_lineGraphFull< G, T_V, T_E >(const pgrouting::DirectedGraph &digraph)
        : Pgr_base_graph< G, T_V, T_E >(graphType::DIRECTED) {
            apply_transformation(digraph);
            store_edge_costs(digraph);
        }

    friend std::ostream& operator<<(
            std::ostream &log, const Pgr_lineGraphFull< G, T_V, T_E > &g) {
        typename Pgr_base_graph< G, T_V, T_E >::EO_i out, out_end;

        for (auto vi = vertices(g.graph).first;
                vi != vertices(g.graph).second; ++vi) {
            if ((*vi) >= g.num_vertices()) break;
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

    std::vector< Line_graph_full_rt >
        get_postgres_results_directed() {
            std::vector< Line_graph_full_rt > results;

            typename boost::graph_traits < G >::edge_iterator edgeIt, edgeEnd;
            std::map <
                std::pair<int64_t, int64_t >,
                Line_graph_full_rt > unique;
            auto count = 0;
            auto vertex_count = 0;
            std::map < int64_t, int64_t > vertex_id_map;
            std::map < int64_t, int64_t > vertex_id_reverse_map;

            log << "\nPostgres results\n";
            for (boost::tie(edgeIt, edgeEnd) = boost::edges(this->graph);
                    edgeIt != edgeEnd; edgeIt++) {
                E e = *edgeIt;
                auto e_source = this->graph[this->source(e)].vertex_id;
                auto e_target = this->graph[this->target(e)].vertex_id;

                auto target_vertex_edge_pair = m_transformation_map[e_target];
                auto target_vertex_id = target_vertex_edge_pair.first;
                auto target_edge_id = target_vertex_edge_pair.second;
                auto source_vertex_edge_pair = m_transformation_map[e_source];
                auto source_vertex_id = source_vertex_edge_pair.first;
                auto source_edge_id = source_vertex_edge_pair.second;

                int64_t edge_id = 0;
                auto e_cost = 0.0;
                if (source_edge_id == target_edge_id) {
                    e_cost = m_edge_costs[source_edge_id];
                    edge_id = source_edge_id;
                }

                if (vertex_id_map.find(e_source) == vertex_id_map.end()) {
                    if (vertex_id_reverse_map.find(source_vertex_id) ==
                      vertex_id_reverse_map.end()) {
                        vertex_id_map[e_source] = source_vertex_id;
                        vertex_id_reverse_map[source_vertex_id] = e_source;
                    } else {
                        --vertex_count;
                        vertex_id_map[e_source] = vertex_count;
                        vertex_id_reverse_map[vertex_count] = e_source;
                    }
                }

                if (vertex_id_map.find(e_target) == vertex_id_map.end()) {
                    if (vertex_id_reverse_map.find(target_vertex_id) ==
                      vertex_id_reverse_map.end()) {
                        vertex_id_map[e_target] = target_vertex_id;
                        vertex_id_reverse_map[target_vertex_id] = e_target;
                    } else {
                        --vertex_count;
                        vertex_id_map[e_target] = vertex_count;
                        vertex_id_reverse_map[vertex_count] = e_target;
                    }
                }

#if 0
                log << "e_source = " << e_source
                    << " e_target = " << e_target
                    << "\n";
#endif
                Line_graph_full_rt edge = {
                    ++count,
                    vertex_id_map[e_source],
                    vertex_id_map[e_target],
                    e_cost,
                    edge_id
                };

                unique[ std::pair<int64_t, int64_t>(e_source, e_target) ] =
                    edge;
            }
            for (const auto &edge : unique) {
                results.push_back(edge.second);
            }
            return results;
        }

 private:
    void insert_vertex(
            int64_t original_vertex_id,
            int64_t original_edge_id) {
        m_transformation_map[this->num_vertices() + 1] =
            std::pair<int64_t, int64_t>(original_vertex_id, original_edge_id);
        m_vertex_map[std::pair<int64_t, int64_t>(original_vertex_id,
                                                 original_edge_id)] =
            this->num_vertices() + 1;
        auto v =  add_vertex(this->graph);
        this->graph[v].cp_members(original_vertex_id, original_edge_id);
        this->graph[v].vertex_id = this->num_vertices();
        this->vertices_map[this->num_vertices()] = v;
    }

    void store_edge_costs(
            const pgrouting::DirectedGraph &digraph) {
        E_i e_It, e_End;
        for (boost::tie(e_It, e_End) = boost::edges(digraph.graph);
            e_It != e_End; e_It++) {
            m_edge_costs[digraph.graph[*e_It].id] = digraph.graph[*e_It].cost;
        }
    }

    template < typename T>
    void graph_add_edge(
                int64_t _id,
                const T &source,
                const T &target,
                int64_t source_in_edge,
                int64_t source_out_edge) {
            bool inserted;
            E e;

            pgassert(m_vertex_map.find({source, source_in_edge}) !=
                    m_vertex_map.end());
            pgassert(m_vertex_map.find({target, source_out_edge}) !=
                    m_vertex_map.end());

            auto index_source_edge =
                m_vertex_map[ std::pair<int64_t, int64_t>(source,
                                                          source_in_edge) ];
            auto index_target_edge =
                m_vertex_map[ std::pair<int64_t, int64_t>(target,
                                                          source_out_edge) ];

            auto vm_s = this->get_V(index_source_edge);
            auto vm_t = this->get_V(index_target_edge);

            pgassert(this->vertices_map.find(index_source_edge) !=
                    this->vertices_map.end());
            pgassert(this->vertices_map.find(index_target_edge) !=
                    this->vertices_map.end());

            boost::tie(e, inserted) =
                boost::add_edge(vm_s, vm_t, this->graph);

            this->graph[e].id = _id;
        }

    void apply_transformation(
            const pgrouting::DirectedGraph& digraph) {
        V_i vertexIt, vertexEnd;
        EO_i e_outIt, e_outEnd;
        EI_i e_inIt, e_inEnd;

        // For every vertex in the original graph, create a line graph
        // using the edges connected to that vertex
        for (boost::tie(vertexIt, vertexEnd) = boost::vertices(digraph.graph);
                vertexIt != vertexEnd; vertexIt++) {
            V vertex = *vertexIt;
            auto vertex_id = digraph[vertex].id;
            for (boost::tie(e_outIt, e_outEnd) =
                    boost::out_edges(vertex, digraph.graph);
                    e_outIt != e_outEnd; e_outIt++) {
                auto out_edge_id = digraph.graph[*e_outIt].id;
                insert_vertex(vertex_id, out_edge_id);
            }

            for (boost::tie(e_inIt, e_inEnd) =
                    boost::in_edges(vertex, digraph.graph);
                    e_inIt != e_inEnd; e_inIt++) {
                auto in_edge_id = digraph.graph[*e_inIt].id;
                insert_vertex(vertex_id, in_edge_id);

                for (boost::tie(e_outIt, e_outEnd) =
                        boost::out_edges(vertex, digraph.graph);
                        e_outIt != e_outEnd; e_outIt++) {
                    auto out_edge_id = digraph.graph[*e_outIt].id;

                    ++m_num_edges;

                    graph_add_edge(
                            m_num_edges,
                            vertex_id,
                            vertex_id,
                            in_edge_id,
                            out_edge_id);
                }
            }
        }

        // Connect all of the line graphs that were just created using the
        // edges from the original graph
        for (boost::tie(vertexIt, vertexEnd) =
                boost::vertices(digraph.graph);
                vertexIt != vertexEnd; vertexIt++) {
            V vertex = *vertexIt;
            auto vertex_id = digraph[vertex].id;

            for (boost::tie(e_inIt, e_inEnd) =
                    boost::in_edges(vertex, digraph.graph);
                    e_inIt != e_inEnd; e_inIt++) {
                auto source_vertex_id = digraph[digraph.source(*e_inIt)].id;
                auto in_edge_id = digraph.graph[*e_inIt].id;

                ++m_num_edges;

                graph_add_edge(
                        m_num_edges,
                        source_vertex_id,
                        vertex_id,
                        in_edge_id,
                        in_edge_id);
            }
        }
    }

 private:
    int64_t m_num_edges;
    std::map < int64_t, double > m_edge_costs;
    std::map < int64_t, std::pair< int64_t, int64_t > > m_transformation_map;
    std::map < std::pair< int64_t, int64_t >, int64_t > m_vertex_map;

 public:
    std::ostringstream log;
};
}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_LINEGRAPH_PGR_LINEGRAPHFULL_HPP_
