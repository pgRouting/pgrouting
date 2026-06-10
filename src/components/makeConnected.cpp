/*PGR-GNU*****************************************************************
File: makeConnected.cpp

Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2020 Himanshu Raj
Mail: raj.himanshu2@gmail.com

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

#include <vector>
#include <set>
#include <map>
#include <cstdint>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/make_connected.hpp>
#include <boost/ref.hpp>

#include "c_types/ii_t_rt.h"
#include "cpp_common/messages.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"

#include "components/makeConnected.hpp"

namespace {

class UpdateNewEdges {
 public:
     using G = typename pgrouting::UndirectedGraph::B_G;
     using V = typename pgrouting::UndirectedGraph::V;

     std::vector<II_t_rt> &new_edges;

     explicit UpdateNewEdges(std::vector<II_t_rt>& edges_out)
         : new_edges(edges_out) {}

     void visit_vertex_pair(V u, V v, G& g) {
         boost::add_edge(u, v, g);
         new_edges.push_back({g[u].id, g[v].id});
     }
};


}  // namespace

namespace pgrouting {
namespace functions {

std::vector<II_t_rt>
makeConnected(pgrouting::UndirectedGraph &graph) {
    std::vector<II_t_rt> results;
    UpdateNewEdges visitor(results);

    /* map which store the indices with their nodes. */
    auto index_map = boost::get(boost::vertex_index, graph.graph);

    CHECK_FOR_INTERRUPTS();
    try {
        boost::make_connected(graph.graph, index_map, visitor);
    } catch (...) {
        throw;
    }

    return results;
}

}  // namespace functions
}  // namespace pgrouting
