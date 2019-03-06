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
        : Pgr_base_graph< G, T_V, T_E >(gtype) {
        }

    explicit Pgr_lineGraph< G, T_V, T_E >(const pgrouting::DirectedGraph &digraph)
        : Pgr_base_graph< G, T_V, T_E >(graphType::DIRECTED) {
            insert_vertices(digraph);
            create_edges(digraph);
        }


    friend std::ostream& operator<<(
            std::ostream &log, const Pgr_lineGraph< G, T_V, T_E > &g) {
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

    void insert_vertices(
            const pgrouting::DirectedGraph& digraph) {
        auto es = boost::edges(digraph.graph);
        for (auto eit = es.first; eit != es.second; ++eit) {
            auto edge = *eit;
            Line_vertex vertex({
                    digraph[edge].id,
                    digraph[boost::source(edge, digraph.graph)].id,
                    digraph[boost::target(edge, digraph.graph)].id,
                    digraph[edge].cost,
                    -1});
            add_one_vertex(vertex);
        }
    }



 private:
    template < typename T>
        void
        graph_add_edge(
                const T &source,
                const T &target) {
            bool inserted;
            E e;

            auto vm_s = this->get_V(source);
            auto vm_t = this->get_V(target);

            boost::tie(e, inserted) =
                boost::add_edge(vm_s, vm_t, this->graph);

            this->graph[e].id = this->num_edges();
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
                    boost::out_edges(vertex, digraph.graph);
                    e_outIt != e_outEnd;
                    e_outIt++) {
                /* for( all outgoing edges out from vertex v) */
                for (boost::tie(e_inIt, e_inEnd) =
                        boost::in_edges(vertex, digraph.graph);
                        e_inIt != e_inEnd; e_inIt++) {
                    /*
                       Prevent self-edges from being created in the Line Graph
                       */

                    graph_add_edge(
                            (digraph.graph[*e_inIt]).id,
                            (digraph.graph[*e_outIt]).id);
                }
            }
        }
    }

    V add_one_vertex(
            T_V vertex) {
        auto v =  add_vertex(this->graph);
        this->vertices_map[vertex.id] = v;
        this->graph[v].cp_members(vertex);

        pgassert(boost::num_vertices(this->graph) == this->num_vertices());
        return v;
    }

 private:
    std::map < int64_t, pgr_edge_t > m_edges;

 public:
    std::ostringstream log;
};
}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_LINEGRAPH_PGR_LINEGRAPH_HPP_
