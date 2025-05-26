/*PGR-GNU*****************************************************************
File: undirectedHasCostBG.cpp

Copyright (c) 2021 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2025 Vicky Vergara
Mail: vicky at erosion.dev

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

#include "cpp_common/undirectedHasCostBG.hpp"

#include <utility>
#include <vector>
#include <deque>
#include <string>
#include <limits>

#include "c_types/iid_t_rt.h"
#include "cpp_common/coordinate_t.hpp"
#include "cpp_common/interruption.hpp"
#include <boost/graph/connected_components.hpp>


namespace pgrouting {
namespace graph {

UndirectedHasCostBG::UndirectedHasCostBG(std::vector<IID_t_rt> &distances) {
    /*
     * Inserting vertices
     */
    Identifiers<int64_t> ids;
    for (auto &d : distances) {
        ids += d.from_vid;
        ids += d.to_vid;
        /*
         * Its undirected graph:
         * keeping from_vid < to_vid
         */
        if (d.to_vid > d.from_vid) {
            std::swap(d.to_vid, d.from_vid);
        }
    }

    for (const auto &id : ids) {
        insert_vertex(id);
    }

    /*
     * Inserting edges
     */
    for (const auto &edge : distances) {
        /*
         * skip loops
         */
        if (edge.from_vid == edge.to_vid) continue;

        /*
         * skip negative costs
         */
        if (edge.cost < 0) continue;

        auto v1 = get_boost_vertex(edge.from_vid);
        auto v2 = get_boost_vertex(edge.to_vid);
        auto e_exists = boost::edge(v1, v2, m_graph);
        if (e_exists.second) {
            auto weight = get(boost::edge_weight_t(), m_graph, e_exists.first);
            /*
             * skip duplicated edges with less cost
             */
            if (weight < edge.cost) continue;
            if (edge.cost < weight) {
                /*
                 * substitute edge with new lesser cost found
                 */
                boost::put(boost::edge_weight_t(), m_graph, e_exists.first, edge.cost);
            }
            continue;
        }

        auto add_result = boost::add_edge(v1, v2, edge.cost, m_graph);
        if (!add_result.second) {
            throw std::make_pair(
                    std::string("INTERNAL: something went wrong adding and edge\n"),
                    std::string(__PGR_PRETTY_FUNCTION__));
        }
    }
}


/**
 * The postgres user might inadvertently give duplicate points with the same id
 * the approximation is quite right, for example
 * 1, 3.5, 1
 * 1, 3.499999999999 0.9999999
 * the approximation is quite wrong, for example
 * 2 , 3.5 1
 * 2 , 3.6 1
 * but when the remove_duplicates flag is on, keep only the first row that has the same id
 */
UndirectedHasCostBG::UndirectedHasCostBG(const std::vector<Coordinate_t> &coordinates) {
    /*
     * Inserting edges
     */
    for (size_t i = 0; i < coordinates.size(); ++i) {
        insert_vertex(coordinates[i].id);
        auto u = get_boost_vertex(coordinates[i].id);
        auto ux = coordinates[i].x;
        auto uy = coordinates[i].y;

        /*
         *  undirected, so only need traverse higher coordinates for connections
         */
        for (size_t j = i + 1; j < coordinates.size(); ++j) {
            insert_vertex(coordinates[j].id);
            auto v = get_boost_vertex(coordinates[j].id);

            /*
             * ignoring duplicated coordinates
             */
            if (boost::edge(u, v, m_graph).second) continue;

            auto vx = coordinates[j].x;
            auto vy = coordinates[j].y;

            auto const dx = ux - vx;
            auto const dy = uy - vy;

            /*
             * weight is euclidean distance
             */
            auto add_result = boost::add_edge(u, v, sqrt(dx * dx + dy * dy), m_graph);
            if (!add_result.second) {
                throw std::make_pair(
                        std::string("INTERNAL: something went wrong adding and edge\n"),
                        std::string(__PGR_PRETTY_FUNCTION__));
            }
        }
    }
}


bool
UndirectedHasCostBG::has_vertex(int64_t id) const {
    return m_id_to_V.find(id) != m_id_to_V.end();
}

void
UndirectedHasCostBG::insert_vertex(int64_t id) {
    try {
        if (has_vertex(id)) return;
        auto v = add_vertex(m_id_to_V.size(), m_graph);
        m_id_to_V.insert(std::make_pair(id, v));
        m_V_to_id.insert(std::make_pair(v, id));
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong when inserting a vertex"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }
}

UndirectedHasCostBG::V
UndirectedHasCostBG::get_boost_vertex(int64_t id) const {
    try {
        return m_id_to_V.at(id);
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong when getting the vertex descriptor"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }
}

int64_t
UndirectedHasCostBG::get_vertex_id(V v) const {
    try {
        return m_V_to_id.at(v);
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong when getting the vertex id"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }
}

int64_t
UndirectedHasCostBG::get_edge_id(E e) const {
    try {
        return m_E_to_id.at(e);
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong when getting the edge id"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }
}

}  // namespace graph

}  // namespace pgrouting
