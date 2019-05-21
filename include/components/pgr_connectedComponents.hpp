/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2017 Maoguang Wang
Mail: xjtumg1007@gmail.com

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

#ifndef INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/connected_components.hpp>

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "cpp_common/pgr_base_graph.hpp"
#include "components/componentsResult.h"

namespace pgrouting {
namespace algorithms {

/**
 * works for undirected graph
 **/
std::vector<pgr_components_rt>
pgr_connectedComponents(pgrouting::UndirectedGraph &graph) {
    size_t totalNodes = num_vertices(graph.graph);

    // perform the algorithm
    std::vector< int > components(totalNodes);
    auto num_comps = boost::connected_components(graph.graph, &components[0]);

    // get the results
    std::vector< std::vector< int64_t > > results;
    results.resize(num_comps);
    for (size_t i = 0; i < totalNodes; i++)
        results[components[i]].push_back(graph[i].id);

    return detail::componentsResult(results);
}

}  // namespace algorithms
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
