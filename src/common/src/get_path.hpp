/*PGR-GNU*****************************************************************

File: get_path.hpp 

Copyright (c) 2016 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#pragma once

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif


#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <deque>
#include <vector>

#include "./../../common/src/basePath_SSEC.hpp"
#include "./../../common/src/pgr_base_graph.hpp"


/*! @brief returns a "path" from v_source to v_target
 
  @returns empty path, when there is no path
*/
template < typename G , typename V>
Path get_path( 
        const G &graph,
        const V v_source,
        const V v_target,
        const std::vector<V> &predecessors,
        const std::vector<double> &distances,
        bool normal = true) {
    Path path(graph.graph[v_source].id, graph.graph[v_target].id);
    
    // no path was found
    if (v_target == predecessors[v_target]) {
        return path;
    }

    /*
     * set the target
     */
    auto target = v_target;

    /*
     * the last stop is the target
     */
    path.push_front(
            {graph.graph[target].id, -1,
            0,  distances[target]});

    /*
     * get the path
     */
    while (target != v_source) {
        /*
         * done when the predecesor of the target is the target
         */
        if (target == predecessors[target]) break;

        /*
         * Inserting values in the path
         */
        auto cost = distances[target] - distances[predecessors[target]];
        auto vertex_id = graph.graph[predecessors[target]].id;
        auto edge_id = normal?
            graph.get_edge_id(predecessors[target], target, cost)
            : graph.get_edge_id(target, predecessors[target], cost);

        path.push_front({
                vertex_id,
                edge_id,
                cost,
                distances[target] - cost});
        target = predecessors[target];
    }

    return path;
}


/*! @brief returns a "path" containing the only the cost

  @returns empty path, when there is no path
  */

template < typename G , typename V>
Path
get_cost(
        const G &graph,
        const V v_source,
        const V v_target,
        const std::vector<V> &predecessors,
        const std::vector<double> &distances) {
    Path path(graph.graph[v_source].id, graph.graph[v_target].id);

    if (v_target != predecessors[v_target]) {
        path.push_front(
                {graph.graph[v_target].id,
                -1,
                distances[v_target],
                distances[v_target]});
    }
    return path;
}

