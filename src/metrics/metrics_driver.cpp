/*PGR-GNU*****************************************************************
File: metrics_driver.cpp
Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Developer: 
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

#include "drivers/metrics_driver.hpp"

#include <sstream>
#include <vector>
#include <string>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/to_postgres.hpp"
#include "cpp_common/base_graph.hpp"

#include "metrics/bandwidth.hpp"
#include "c_types/bandwidth_rt.h"  

namespace pgrouting::graph {

template <class G>
std::vector<GraphBandwidth_rt>
run_metric(G& graph) {
    std::vector<GraphBandwidth_rt> results;
    int result = pgrouting::metrics::bandwidth(graph);

    GraphBandwidth_rt tuple;
    tuple.id = 1;  // constant as you are computing a graph-level metric
    tuple.bandwidth = result;

    results.push_back(tuple);
    return results;
}

}  // namespace graph


void
do_metrics(
    GraphBandwidth_rt **return_tuples,
    size_t *return_count,
    char **log_msg,
    char **notice_msg,
    char **err_msg,
    std::string edges_sql,
    bool directed) {

    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    const char *hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        hint = edges_sql.c_str();
        auto edges = pgrouting::pgget::get_edges(edges_sql, true, true);

        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint ? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }

        std::vector<GraphBandwidth_rt> results;

        if (directed) {
            log << "Processing directed graph\n";
            pgrouting::DirectedGraph digraph;
            digraph.insert_edges(edges);
            results = pgrouting::graph::run_metric(digraph);
        } else {
            log << "Processing undirected graph\n";
            pgrouting::UndirectedGraph undigraph;
            undigraph.insert_edges(edges);
            results = pgrouting::graph::run_metric(undigraph);
        }

        size_t count = results.size();
        if (count == 0) {
            *return_tuples = nullptr;
            *return_count = 0;
            *log_msg = to_pg_msg(log);
            *notice_msg = to_pg_msg("No results found");
            return;
        }

        *return_tuples = pgr_alloc(count, *return_tuples);
        for (size_t i = 0; i < count; ++i) {
            (*return_tuples)[i] = results[i];
        }

        *return_count = count;
        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);

    } catch (const AssertFailedException &except) {
        *return_tuples = pgr_free(*return_tuples);
        *return_count = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint ? to_pg_msg(hint) : to_pg_msg(log);
    } catch (const std::exception &except) {
        *return_tuples = pgr_free(*return_tuples);
        *return_count = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (...) {
        *return_tuples = pgr_free(*return_tuples);
        *return_count = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
