/*PGR-GNU*****************************************************************
File: makeBiconnectedPlanar.cpp

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2026 Mohit Rawat
Mail: mohit25rawat at gmail.com

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

#include "planar/makeBiconnectedPlanar.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <cstdint>
#include <cstddef>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/make_biconnected_planar.hpp>

#include "c_types/ii_t_rt.h"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"

namespace {

using G = pgrouting::UndirectedGraph;
using E = typename G::E;
using E_i = typename G::E_i;

struct planar_visitor {
    std::vector<II_t_rt>& m_results;
    G& m_graph;

    planar_visitor(std::vector<II_t_rt>& results, G& graph)
        : m_results(results), m_graph(graph) {}

    /*
     * boost::make_biconnected_planar() only reports the pairs it wants
     * added; it never adds them itself when an explicit visitor is given
     */
    template <typename Vertex, typename BGraph>
    void visit_vertex_pair(Vertex u, Vertex v, BGraph&) {
        m_results.push_back({m_graph[u].id, m_graph[v].id});
    }
};

}  // namespace

namespace pgrouting {
namespace functions {

/** @brief generate the missing edges to have biconnected graphs
 *
 * Disconnected graphs are handled by boost::boyer_myrvold_planarity_test()
 * and boost::make_biconnected_planar() directly, so no per component
 * sub-graph is built
 */
std::vector<II_t_rt>
makeBiconnectedPlanar(pgrouting::UndirectedGraph &graph) {
    CHECK_FOR_INTERRUPTS();

    E_i ei, ei_end;
    std::map<E, size_t> edge_id_map;
    size_t edge_count = 0;
    for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ++ei) {
        edge_id_map[*ei] = edge_count++;
    }
    boost::associative_property_map<std::map<E, size_t>>
        e_index(edge_id_map);

    typedef std::vector<typename boost::graph_traits<
        typename G::B_G>::edge_descriptor> vec_t;
    std::vector<vec_t> embedding(boost::num_vertices(graph.graph));

    /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
    CHECK_FOR_INTERRUPTS();

    bool is_planar = boost::boyer_myrvold_planarity_test(
        boost::boyer_myrvold_params::graph = graph.graph,
        boost::boyer_myrvold_params::embedding = &embedding[0]);

    if (!is_planar) {
        throw std::string("Graph is not planar");
    }

    std::vector<II_t_rt> results;
    planar_visitor vis(results, graph);

    /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
    CHECK_FOR_INTERRUPTS();
    boost::make_biconnected_planar(graph.graph, &embedding[0], e_index, vis);

    for (auto &edge : results) {
        if (edge.d1 > edge.d2) {
            std::swap(edge.d1, edge.d2);
        }
    }
    std::sort(results.begin(), results.end(), [](const II_t_rt &a, const II_t_rt &b) {
        if (a.d1 != b.d1) return a.d1 < b.d1;
        return a.d2 < b.d2;
    });

    return results;
}

}  // namespace functions
}  // namespace pgrouting
