/*PGR-GNU*****************************************************************
File: metrics_process.cpp

Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510 at gmail.com

Generated with Template by:
Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

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
#include <string>
#include <unordered_map>
#include <cstdint>
#include "cpp_common/assert.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/pgdata_fetchers.hpp"
#include "cpp_common/get_data.hpp"
#include "cpp_common/to_postgres.hpp"

struct Metrics_rt {
    int64_t node;
    double metric_value;
};

std::vector<Metrics_rt>
process_metrics(std::string edges_sql, bool directed, int max_nodes) {
    std::vector<Metrics_rt> result;

    if (directed) {
    pgrouting::DirectedGraph graph;
    auto edges = pgrouting::pgget::get_edges(edges_sql, directed, false);
    graph.insert_edges(edges);
} else {
    pgrouting::UndirectedGraph graph;
    auto edges = pgrouting::pgget::get_edges(edges_sql, directed, false);
    graph.insert_edges(edges);
}


    auto graph_map = pgrouting::graph::get_graph();

    for (const auto& node_pair : graph_map) {
        Metrics_rt record;
        record.node = node_pair.first;
        record.metric_value = static_cast<double>(node_pair.second.size());  
        result.push_back(record);
    }

    return result;
}
