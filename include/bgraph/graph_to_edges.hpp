/*PGR-GNU*****************************************************************
File: graph_to_edges.hpp

Function's developer:
Copyright (c) 2024 Vicky Vergara
Mail: vicky at gmail.com

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

#ifndef INCLUDE_BGRAPH_GRAPH_TO_EDGES_HPP_
#define INCLUDE_BGRAPH_GRAPH_TO_EDGES_HPP_

#include <vector>
#include <map>
#include <utility>
#include <cstdint>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "cpp_common/edge_t.hpp"
#include "c_types/edge_rt.h"

namespace pgrouting {
namespace b_g {

/** @brief converts a bg to Edges_t
 * @param[in] bg the boost graph
 * @param[in] add_self_loop flag to include self loops on the graph
 * @returns a set of Edges_t that exist on the graph
 *   id, source, target, cost, reverse_cost
 *   source < target
 */
template<typename G>
std::vector<Edge_t> graph_to_existing_edges(const G &bg, bool add_self_loop) {
    std::vector<Edge_t> results;

    std::map<std::pair<int64_t, int64_t>, Edge_t> st_to_edge;
    int64_t count = 0;

    auto bg_edges = boost::edges(bg);
    for (auto e = bg_edges.first; e != bg_edges.second; ++e) {
        auto s = bg[boost::source(*e, bg)].id;
        auto t = bg[boost::target(*e, bg)].id;

        if (s == t && !add_self_loop) continue;

        /*
         * Reverse edge already been added
         */
        if (st_to_edge.find({t, s}) != st_to_edge.end()) {
            st_to_edge[std::pair<int64_t, int64_t>(t, s)].reverse_cost = 1.0;
            continue;
        }

        /*
         * Already been added
         */
        if (st_to_edge.find({s, t}) != st_to_edge.end()) continue;

        st_to_edge[std::pair<int64_t, int64_t>(s, t)] = {++count, s, t, 1, -1};
    }

    for (const auto &st : st_to_edge) {
        results.push_back(st.second);
    }
    return results;
}

}  // namespace b_g
}  // namespace pgrouting

#endif  // INCLUDE_BGRAPH_GRAPH_TO_EDGES_HPP_
