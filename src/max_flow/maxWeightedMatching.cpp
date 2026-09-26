/*PGR-GNU*****************************************************************
File: maxWeightedMatching.cpp

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

#include "max_flow/maxWeightedMatching.hpp"

#include <vector>
#include <algorithm>
#include <utility>

#include <boost/graph/maximum_weighted_matching.hpp>

#include "c_types/iid_t_rt.h"
#include "cpp_common/undirectedHasCostBG.hpp"
#include "cpp_common/interruption.hpp"


namespace pgrouting {
namespace functions {

Identifiers<int64_t>
maxWeightedMatch(pgrouting::graph::UndirectedHasCostBG &graph) {
    using G = pgrouting::graph::UndirectedHasCostBG::TSP_Graph;
    using V = pgrouting::graph::UndirectedHasCostBG::V;
    using E = pgrouting::graph::UndirectedHasCostBG::E;

    std::vector<V> mate_map(boost::num_vertices(graph.graph()));
    Identifiers<int64_t> match;

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

    /*
     * Check for each vertex:
     * 1) The vertex does not have a match
     * 2) prevent double output of the edge
     */
    for (const auto &v2 : mate_map) {
        auto v1 = static_cast<V>(&v2 - &mate_map[0]);

        if (v2 == boost::graph_traits<G>::null_vertex()) continue;
        if (v1 >= v2) continue;

        E e;
        bool exists = false;
        boost::tie(e, exists) = boost::edge(v1, v2, graph.graph());
        if (!exists) throw;

        match += graph.get_edge_id(e);
    }

    return match;
}

}  // namespace functions
}  // namespace pgrouting
