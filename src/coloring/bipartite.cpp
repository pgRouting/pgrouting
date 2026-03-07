/*PGR-GNU*****************************************************************
File: bipartite.cpp

Generated with Template by:
Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Prakash Tiwari
Mail: 85prakash2017@gmail.com

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

#include "coloring/bipartite.hpp"

#include <vector>
#include <cstdint>

#include <boost/property_map/property_map.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>
#include <boost/graph/bipartite.hpp>

#include "cpp_common/interruption.hpp"

namespace {

std::vector<II_t_rt> get_bipartition(pgrouting::UndirectedGraph &graph) {
    using V_i = pgrouting::UndirectedGraph::V_i;

    std::vector<II_t_rt> results;
    std::vector<boost::default_color_type> partition(graph.num_vertices());
    auto partition_map =
        make_iterator_property_map(partition.begin(), boost::get(boost::vertex_index, graph.graph));

    CHECK_FOR_INTERRUPTS();
    try {
        boost::is_bipartite(graph.graph, boost::get(boost::vertex_index, graph.graph), partition_map);
    } catch (boost::exception const& ex) {
        throw;
    } catch (std::exception &e) {
        throw;
    } catch (...) {
        throw;
    }

    V_i v, vend;
    for (boost::tie(v, vend) = vertices(graph.graph); v != vend; ++v) {
        int64_t vid = graph[*v].id;
        boost::get(partition_map, *v) ==
            boost::color_traits <boost::default_color_type>::white() ?
            results.push_back({{vid}, {0}}) : results.push_back({{vid}, {1}});
    }
    return results;
}

}  // namespace

namespace pgrouting {
namespace functions {

std::vector<II_t_rt> pgr_bipartite(pgrouting::UndirectedGraph &graph ){
    bool bipartite = boost::is_bipartite(graph.graph);
    return (bipartite)? get_bipartition(graph) : std::vector<II_t_rt>();
}

}  // namespace functions
}  // namespace pgrouting
