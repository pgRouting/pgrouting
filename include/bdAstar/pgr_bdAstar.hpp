/*PGR-GNU*****************************************************************
File: pgr_bdAstar.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
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

#ifndef INCLUDE_BDASTAR_PGR_BDASTAR_HPP_
#define INCLUDE_BDASTAR_PGR_BDASTAR_HPP_
#pragma once


#include "cpp_common/pgr_bidirectional.hpp"

#include <string>
#include <queue>
#include <utility>
#include <vector>
#include <limits>
#include <functional>

#include "cpp_common/basePath_SSEC.hpp"

namespace pgrouting {
namespace bidirectional {

template < typename G >
class Pgr_bdAstar : public Pgr_bidirectional<G> {
    typedef typename Pgr_bidirectional<G>::V V;
    typedef typename Pgr_bidirectional<G>::E E;
    typedef typename Pgr_bidirectional<G>::Cost_Vertex_pair Cost_Vertex_pair;

    using Pgr_bidirectional<G>::graph;
    using Pgr_bidirectional<G>::m_log;
    using Pgr_bidirectional<G>::v_source;
    using Pgr_bidirectional<G>::v_target;

    using Pgr_bidirectional<G>::backward_predecessor;
    using Pgr_bidirectional<G>::backward_queue;
    using Pgr_bidirectional<G>::backward_finished;
    using Pgr_bidirectional<G>::backward_cost;
    using Pgr_bidirectional<G>::backward_edge;

    using Pgr_bidirectional<G>::forward_predecessor;
    using Pgr_bidirectional<G>::forward_queue;
    using Pgr_bidirectional<G>::forward_finished;
    using Pgr_bidirectional<G>::forward_cost;
    using Pgr_bidirectional<G>::forward_edge;


    using Pgr_bidirectional<G>::bidirectional;


 public:
    explicit Pgr_bdAstar(G &pgraph) :
        Pgr_bidirectional<G>(pgraph),
        m_heuristic(5),
        m_factor(1.0) {
        m_log << "pgr_bdAstar constructor\n";
    }

    ~Pgr_bdAstar() = default;

    Path pgr_bdAstar(V start_vertex, V end_vertex,
            int heuristic,
            double factor,
            double epsilon,
            bool only_cost) {
        m_log << "pgr_bdAstar\n";
        v_source = start_vertex;
        v_target = end_vertex;
        m_heuristic = heuristic;
        m_factor = factor * epsilon;

        if (v_source == v_target) {
            return Path(v_source, v_target);
        }
        return bidirectional(only_cost);
    }

    using Pgr_bidirectional<G>::log;
    using Pgr_bidirectional<G>::clean_log;

 private:
    void explore_forward(const Cost_Vertex_pair &node) {
        typename G::EO_i out, out_end;

        auto current_cost = node.first;
        auto current_node = node.second;

        for (boost::tie(out, out_end) = out_edges(current_node, graph.graph);
                out != out_end; ++out) {
            auto edge_cost = graph[*out].cost;
            auto next_node = graph.adjacent(current_node, *out);

            if (forward_finished[next_node]) continue;

            if (edge_cost + current_cost < forward_cost[next_node]) {
                forward_cost[next_node] = edge_cost + current_cost;
                forward_predecessor[next_node] = current_node;
                forward_edge[next_node] = graph[*out].id;
                forward_queue.push({
                        forward_cost[next_node]
                            + heuristic(next_node, v_target),
                        next_node});
            }
        }
        forward_finished[current_node] = true;
    }

    void explore_backward(const Cost_Vertex_pair &node) {
        typename G::EI_i in, in_end;

        auto current_cost = node.first;
        auto current_node = node.second;

        for (boost::tie(in, in_end) = in_edges(current_node, graph.graph);
                in != in_end; ++in) {
            auto edge_cost = graph[*in].cost;
            auto next_node = graph.adjacent(current_node, *in);

            if (backward_finished[next_node]) continue;

            if (edge_cost + current_cost < backward_cost[next_node]) {
                backward_cost[next_node] = edge_cost + current_cost;
                backward_predecessor[next_node] = current_node;
                backward_edge[next_node] = graph[*in].id;
                backward_queue.push({
                        backward_cost[next_node]
                            + heuristic(next_node, v_source),
                        next_node});
            }
        }
        backward_finished[current_node] = true;
    }


    double heuristic(V v, V u) {
        if (m_heuristic == 0) return 0;

        double dx = graph[v].x() - graph[u].x();
        double dy = graph[v].y() - graph[u].y();
        double current;

        switch (m_heuristic) {
            case 0:
                current = 0;
                break;
            case 1:
                current = std::fabs((std::max)(dx, dy)) * m_factor;
                break;
            case 2:
                current = std::fabs((std::min)(dx, dy)) * m_factor;
                break;
            case 3:
                current = (dx * dx + dy * dy) * m_factor * m_factor;
                break;
            case 4:
                current = std::sqrt(dx * dx + dy * dy) * m_factor;
                break;
            case 5:
                current = (std::fabs(dx) + std::fabs(dy)) * m_factor;
                break;
            default:
                current = 0;
        }
        return current;
    }

 private:
    int m_heuristic;
    double m_factor;
};

}  // namespace bidirectional
}  // namespace pgrouting

#endif  // INCLUDE_BDASTAR_PGR_BDASTAR_HPP_
