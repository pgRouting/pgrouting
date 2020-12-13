/*PGR-GNU*****************************************************************
File: pgr_edwardMoore.hpp
Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org
Copyright (c) 2019 Gudesa Venkata Sai AKhil
Mail: gvs.akhil1997@gmail.com
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

#ifndef INCLUDE_BELLMAN_FORD_PGR_EDWARDMOORE_HPP_
#define INCLUDE_BELLMAN_FORD_PGR_EDWARDMOORE_HPP_
#pragma once

#include<limits>
#include<algorithm>
#include<vector>
#include <deque>


#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"
//******************************************

namespace pgrouting {
namespace functions {

template <class G>
class Pgr_edwardMoore {
 public:
    typedef typename G::V V;
    typedef typename G::E E;
    typedef typename G::B_G B_G;
    typedef typename G::EO_i EO_i;
    typedef typename G::E_i E_i;

    std::deque<Path> edwardMoore(
        G &graph,
        std::vector<int64_t> start_vertex,
        std::vector<int64_t> end_vertex) {
        std::deque<Path> paths;

        for (auto source : start_vertex) {
            std::deque<Path> result_paths = one_to_many_edwardMoore(
                graph,
                source,
                end_vertex);

            paths.insert(
                paths.begin(),
                std::make_move_iterator(result_paths.begin()),
                std::make_move_iterator(result_paths.end()));
        }

        std::sort(paths.begin(), paths.end(),
                  [](const Path &e1, const Path &e2) -> bool {
                      return e1.end_id() < e2.end_id();
                  });
        std::stable_sort(paths.begin(), paths.end(),
                         [](const Path &e1, const Path &e2) -> bool {
                             return e1.start_id() < e2.start_id();
                         });

        return paths;
    }

    // preparation for the parallel arrays
    std::deque<Path> edwardMoore(
        G &graph,
        const std::vector<pgr_combination_t> &combinations) {
        std::deque<Path> paths;

        // group targets per distinct source
        std::map< int64_t, std::vector<int64_t> > vertex_map;
        for (const pgr_combination_t &comb : combinations) {
            std::map< int64_t, std::vector<int64_t> >::iterator it = vertex_map.find(comb.source);
            if (it != vertex_map.end()) {
                it->second.push_back(comb.target);
            } else {
                std::vector<int64_t > targets{comb.target};
                vertex_map[comb.source] = targets;
            }
        }

        for (const auto &start_ends : vertex_map) {
            std::deque<Path> result_paths = one_to_many_edwardMoore(
                graph,
                start_ends.first,
                start_ends.second);

            paths.insert(
                paths.end(),
                std::make_move_iterator(result_paths.begin()),
                std::make_move_iterator(result_paths.end()));
        }

        std::sort(paths.begin(), paths.end(),
                  [](const Path &e1, const Path &e2) -> bool {
                      return e1.end_id() < e2.end_id();
                  });
        std::stable_sort(paths.begin(), paths.end(),
                         [](const Path &e1, const Path &e2) -> bool {
                             return e1.start_id() < e2.start_id();
                         });

        return paths;
    }

 private:
    E DEFAULT_EDGE;

    std::deque<Path> one_to_many_edwardMoore(
        G &graph,
        int64_t start_vertex,
        std::vector<int64_t> end_vertex) {
        std::deque<Path> paths;

        if (graph.has_vertex(start_vertex) == false) {
            return paths;
        }

        std::vector<double> current_cost(graph.num_vertices(), std::numeric_limits<double>::infinity());
        std::vector<bool> isInQ(graph.num_vertices(), false);
        std::vector<E> from_edge(graph.num_vertices());
        std::deque<V> dq;
        DEFAULT_EDGE = from_edge[0];

        auto bgl_start_vertex = graph.get_V(start_vertex);

        current_cost[bgl_start_vertex] = 0;
        isInQ[bgl_start_vertex] = true;
        dq.push_front(bgl_start_vertex);

        while (dq.empty() == false) {
            auto head_vertex = dq.front();

            dq.pop_front();
            isInQ[head_vertex] = false;

            updateVertexCosts(graph, current_cost, isInQ, from_edge, dq, head_vertex);
        }

        for (auto target_vertex : end_vertex) {
            if (graph.has_vertex(target_vertex) == false) {
                continue;
            }

            auto bgl_target_vertex = graph.get_V(target_vertex);

            if (from_edge[bgl_target_vertex] == DEFAULT_EDGE) {
                continue;
            }

            paths.push_front(
                getPath(graph, bgl_start_vertex, target_vertex, bgl_target_vertex, from_edge, current_cost));
        }

        return paths;
    }

    Path getPath(
        G &graph,
        V bgl_start_vertex,
        int64_t target,
        V bgl_target_vertex,
        std::vector<E> &from_edge,
        std::vector<double> &current_cost) {
        auto current_node = bgl_target_vertex;

        Path path = Path(graph[bgl_start_vertex].id, graph[current_node].id);

        path.push_back({target, -1, 0, current_cost[current_node]});

        do {
            E e = from_edge[current_node];
            auto from = graph.source(e);

            path.push_back({graph[from].id, graph[e].id, graph[e].cost, current_cost[from]});

            current_node = from;
        } while (from_edge[current_node] != DEFAULT_EDGE);

        std::reverse(path.begin(), path.end());
        return path;
    }

    void updateVertexCosts(
        G &graph,
        std::vector<double> &current_cost,
        std::vector<bool> &isInQ,
        std::vector<E> &from_edge,
        std::deque<V> &dq,
        V &head_vertex) {
        /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
        CHECK_FOR_INTERRUPTS();

        auto out_edges = boost::out_edges(head_vertex, graph.graph);
        E e;
        EO_i out_i;
        EO_i out_end;
        V v_source, v_target;

        for (boost::tie(out_i, out_end) = out_edges;
             out_i != out_end; ++out_i) {
            e = *out_i;
            v_target = graph.target(e);
            v_source = graph.source(e);
            double edge_cost = graph[e].cost;

            if (std::isinf(current_cost[v_target]) || current_cost[v_source] + edge_cost < current_cost[v_target]) {
                current_cost[v_target] = current_cost[v_source] + edge_cost;

                from_edge[v_target] = e;

                if (isInQ[v_target] == false) {
                    dq.push_back(v_target);
                    isInQ[v_target] = true;
                }
            }
        }
    }
};
}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_BELLMAN_FORD_PGR_EDWARDMOORE_HPP_
