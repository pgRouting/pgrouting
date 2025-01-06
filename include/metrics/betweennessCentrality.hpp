/*PGR-GNU*****************************************************************
File: betweennessCentrality.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2024 Arun Thakur
Mail: bedupako12mas at gmail.com

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

#ifndef INCLUDE_METRICS_BETWEENNESSCENTRALITY_HPP_
#define INCLUDE_METRICS_BETWEENNESSCENTRALITY_HPP_
#pragma once

#include <vector>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/betweenness_centrality.hpp>

#include "c_types/iid_t_rt.h"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"

namespace pgrouting {
namespace metrics {

template <class G>
std::vector<double> betweennessCentrality(
        const G &graph
        ) {
    std::vector<double> centrality(boost::num_vertices(graph.graph), 0.0);
    auto centrality_map = boost::make_iterator_property_map(centrality.begin(),
            boost::get(boost::vertex_index, graph.graph));

    CHECK_FOR_INTERRUPTS();

        try {
    boost::brandes_betweenness_centrality(
            graph.graph,
            centrality_map);

    if (boost::num_vertices(graph.graph) > 2) {
        boost::relative_betweenness_centrality(
                graph.graph,
                centrality_map);
    }
        } catch (boost::exception const& ex) {
            (void)ex;
            throw;
        } catch (std::exception &e) {
            (void)e;
            throw;
        } catch (...) {
             throw;
        }

    return centrality;
}


}  // namespace metrics
}  // namespace pgrouting

#endif  // INCLUDE_METRICS_BETWEENNESSCENTRALITY_HPP_
