/*PGR-GNU*****************************************************************
File: maxWeightedMatching.hpp

Copyright (c) 2025-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2026 Mayur Galhate
Mail: galhatemayur at gmail.com

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

#ifndef INCLUDE_MAX_FLOW_MAXWEIGHTEDMATCHING_HPP_
#define INCLUDE_MAX_FLOW_MAXWEIGHTEDMATCHING_HPP_
#pragma once

#include <vector>
#include <algorithm>
#include <utility>

#include <boost/graph/maximum_weighted_matching.hpp>

#include "c_types/iid_t_rt.h"
#include "cpp_common/undirectedHasCostBG.hpp"
#include "cpp_common/interruption.hpp"


namespace pgrouting {
namespace flow {

inline std::vector<IID_t_rt>
maximumWeightedMatch(pgrouting::graph::UndirectedHasCostBG &graph) {
    using G = pgrouting::graph::UndirectedHasCostBG::TSP_Graph;
    using V = pgrouting::graph::UndirectedHasCostBG::V;
    using E = pgrouting::graph::UndirectedHasCostBG::E;

    std::vector<V> mate_map(boost::num_vertices(graph.graph()));

    CHECK_FOR_INTERRUPTS();
    try {
        boost::maximum_weighted_matching(graph.graph(), &mate_map[0]);
    } catch (boost::exception const &ex) {
        (void)ex;
        throw;
    } catch (std::exception &e) {
        (void)e;
        throw;
    } catch (...) {
        throw;
    }

    std::vector<IID_t_rt> results;

    for (const auto &v2 : mate_map) {
        auto v1 = static_cast<V>(&v2 - &mate_map[0]);

        if (v2 == boost::graph_traits<G>::null_vertex()) continue;
        if (v1 >= v2) continue;

        E e;
        bool exists = false;
        boost::tie(e, exists) = boost::edge(v1, v2, graph.graph());
        if (!exists) throw;

        int64_t src = graph.get_vertex_id(v1);
        int64_t tgt = graph.get_vertex_id(v2);

        if (src > tgt) std::swap(src, tgt);

        IID_t_rt row;
        row.from_vid = src;
        row.to_vid   = tgt;
        row.cost     = boost::get(boost::edge_weight_t(), graph.graph(), e);
        results.push_back(row);
    }

    std::sort(results.begin(), results.end(),
        [](const IID_t_rt &a, const IID_t_rt &b) {
            return a.to_vid < b.to_vid;
        });

    return results;
}

}  // namespace flow
}  // namespace pgrouting

#endif  // INCLUDE_MAX_FLOW_MAXWEIGHTEDMATCHING_HPP_
