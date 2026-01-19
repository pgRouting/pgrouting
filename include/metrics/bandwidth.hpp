/*PGR-GNU*****************************************************************
File: bandwidth.hpp

Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510 at gmail.com

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

#ifndef INCLUDE_METRICS_BANDWIDTH_HPP_
#define INCLUDE_METRICS_BANDWIDTH_HPP_
#pragma once

#include <vector>
#include <numeric>
#include <cstdint>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/bandwidth.hpp>

#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"

namespace pgrouting {
namespace metrics {

template <class G>
uint64_t bandwidth(const G &graph) {
        CHECK_FOR_INTERRUPTS();

        try {
        const auto& boost_graph = graph.graph;

        return static_cast<uint64_t>(boost::bandwidth(boost_graph));
    } catch (boost::exception const& ex) {
        (void)ex;
        throw;
    } catch (std::exception &e) {
        (void)e;
        throw;
    } catch (...) {
        throw;
    }
}

}  // namespace metrics
}  // namespace pgrouting

#endif  // INCLUDE_METRICS_BANDWIDTH_HPP_
