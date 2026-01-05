/*PGR-GNU*****************************************************************
File: topologicalSort.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2019 Hang Wu
mail: nike0good@gmail.com

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

#include "ordering/topologicalSort.hpp"

#include <vector>
#include <algorithm>
#include <string>
#include <cstdint>

#include <boost/config.hpp>

#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/topological_sort.hpp>

namespace pgrouting {
namespace functions {

std::vector<pgrouting::DirectedGraph::V>
topologicalSort(const pgrouting::DirectedGraph &graph) {
    using V = typename pgrouting::DirectedGraph::V;

    std::vector<V> results;

    CHECK_FOR_INTERRUPTS();

    try {
        boost::topological_sort(graph.graph, std::back_inserter(results));
    } catch (boost::exception const& ex) {
        throw std::string("Graph is not DAG");
    } catch (std::exception &e) {
        (void)e;
        throw;
    } catch (...) {
        throw;
    }

    std::reverse(results.begin(), results.end());
    return results;
}

}  // namespace functions
}  // namespace pgrouting
