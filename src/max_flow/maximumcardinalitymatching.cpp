/*PGR-GNU*****************************************************************
File:maximumcardinalitymatching.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Refactoring
Copyright (c) 2022 Celia Vriginia Vergara Castillo
Mail: vicky_vergara at hotmail.com

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#include "max_flow/maximumcardinalitymatching.hpp"
#include <vector>
#include <utility>
#include <set>
#include <algorithm>

#include <boost/graph/max_cardinality_matching.hpp>
#include "cpp_common/interruption.hpp"

namespace pgrouting {
namespace flow {


std::set<int64_t>
maxCardinalityMatch(pgrouting::graph::UndirectedNoCostsBG &graph) {
    using G = pgrouting::graph::UndirectedNoCostsBG::G;
    using V = pgrouting::graph::UndirectedNoCostsBG::V;
    using E = pgrouting::graph::UndirectedNoCostsBG::E;

    std::vector<V> mate_map(boost::num_vertices(graph()));
    std::set<int64_t> match;

    CHECK_FOR_INTERRUPTS();
    try {
        edmonds_maximum_cardinality_matching(graph(), &mate_map[0]);
    } catch (boost::exception const& ex) {
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
        bool exists;
        boost::tie(e, exists) = boost::edge(v1, v2, graph());
        if (!exists) throw;

        match.insert(graph.get_edge_id(e));
    }

    return match;
}

}  // namespace flow
}  // namespace pgrouting
