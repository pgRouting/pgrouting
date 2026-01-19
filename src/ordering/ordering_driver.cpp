/*PGR-GNU*****************************************************************
File: ordering_driver.cpp
Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Developers:

Copyright (c) 2025 Bipasha Gayary
Mail: bipashagayary at gmail.com

Copyright (c) 2025 Fan Wu
Mail: wifiblack0131 at gmail.com

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

#include "drivers/ordering_driver.hpp"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>
#include <cstdint>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/to_postgres.hpp"

#include "ordering/sloanOrdering.hpp"
#include "ordering/kingOrdering.hpp"
#include "ordering/cuthillMckeeOrdering.hpp"
#include "ordering/topologicalSort.hpp"

template <typename G, typename Func>
void
process(const std::vector<Edge_t> &edges, G &graph, Func ordering,
        size_t &return_count,
        int64_t **return_tuples) {
    using pgrouting::to_postgres::get_vertexId;
    graph.insert_edges(edges);
    auto results = ordering(graph);
    get_vertexId(graph, results, return_count, return_tuples);
}

void
do_ordering(
    const std::string &edges_sql,
    Which which,

    int64_t **return_tuples,
    size_t *return_count,

    char **log_msg,
    char **notice_msg,
    char **err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::pgget::get_edges;
    using pgrouting::UndirectedGraph;
    using pgrouting::DirectedGraph;
    using pgrouting::to_postgres::get_vertexId;

    using pgrouting::functions::sloanOrdering;
    using pgrouting::functions::kingOrdering;
    using pgrouting::functions::cuthillMckeeOrdering;
    using pgrouting::functions::topologicalSort;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    std::string hint = "";

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        hint = edges_sql;
        auto edges = get_edges(edges_sql, true, false);
        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = to_pg_msg(hint);
            *return_tuples = nullptr;
            *return_count = 0;
            return;
        }
        hint = "";


        auto vertices = which == 0 || which == 2?
            pgrouting::extract_vertices(edges)
            : std::vector<pgrouting::Basic_vertex>();

        UndirectedGraph undigraph = which < 11? UndirectedGraph(vertices) : UndirectedGraph();
        DirectedGraph digraph = which >= 11? DirectedGraph(vertices) : DirectedGraph();;


        switch (which) {
            case SLOAN: {
                            process(edges, undigraph, &sloanOrdering, *return_count, return_tuples);
                            break;
                        }
            case CUTCHILL: {
                               process(edges, undigraph, &cuthillMckeeOrdering, *return_count, return_tuples);
                               break;
                           }
            case KING: {
                           process(edges, undigraph, &kingOrdering, *return_count, return_tuples);
                           break;
                       }
            case TOPOSORT: {
                               process(edges, digraph, &topologicalSort, *return_count, return_tuples);
                               break;
                           }
            default:
                           break;
        }

        if ((*return_count) == 0) {
            *notice_msg = to_pg_msg("No results found");
            *return_tuples = nullptr;
            *return_count = 0;
            return;
        }

        pgassert(*err_msg == nullptr);
        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint.empty()? to_pg_msg(hint) : to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        *err_msg = to_pg_msg("Caught unknown exception");
        *log_msg = to_pg_msg(log);
    }
}
