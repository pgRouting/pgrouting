/*PGR-GNU*****************************************************************
File: prim.cpp

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Aditya Pratap Singh
Mail: adityapratap.singh28@gmail.com

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

#include "spanningTree/prim.hpp"

#include <vector>
#include <set>

namespace pgrouting {
namespace functions {

std::vector<MST_rt>
prim(pgrouting::UndirectedGraph &graph) {
    using Prim = pgrouting::algorithms::Pgr_prim<pgrouting::UndirectedGraph>;
    Prim prim;
    return prim.prim(graph);
}

std::vector<MST_rt>
primBFS(pgrouting::UndirectedGraph &graph, const std::set<int64_t>& roots, int64_t max_depth) {
    using Prim = pgrouting::algorithms::Pgr_prim<pgrouting::UndirectedGraph>;
    Prim prim;
    return prim.primBFS(graph, roots, max_depth);
}
std::vector<MST_rt>
primDFS(pgrouting::UndirectedGraph &graph, const std::set<int64_t>& roots, int64_t max_depth) {
    using Prim = pgrouting::algorithms::Pgr_prim<pgrouting::UndirectedGraph>;
    Prim prim;
    return prim.primDFS(graph, roots, max_depth);
}

std::vector<MST_rt>
primDD(pgrouting::UndirectedGraph &graph, const std::set<int64_t>& roots, double distance) {
    using Prim = pgrouting::algorithms::Pgr_prim<pgrouting::UndirectedGraph>;
    Prim prim;
    return prim.primDD(graph, roots, distance);
}

}  // namespace functions
}  // namespace pgrouting
