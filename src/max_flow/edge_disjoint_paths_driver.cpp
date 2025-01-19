/*PGR-GNU*****************************************************************
File: edge_disjoint_paths_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#include "drivers/max_flow/edge_disjoint_paths_driver.h"

#include <sstream>
#include <vector>
#include <set>
#include <string>

#include "max_flow/maxflow.hpp"

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/combinations.hpp"
#include "cpp_common/identifiers.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"

#include "c_types/ii_t_rt.h"


namespace {

std::vector<Path_rt>
single_execution(
        std::vector<Edge_t> edges,
        int64_t source,
        int64_t target,
        bool directed) {
    std::set<int64_t> set_source_vertices;
    std::set<int64_t> set_sink_vertices;
    set_source_vertices.insert(source);
    set_sink_vertices.insert(target);
    pgrouting::graph::PgrFlowGraph G(
            edges,
            set_source_vertices,
            set_sink_vertices, directed);

    /*
     * boykov_kolmogorov is only for directed graphs
     */
    return G.edge_disjoint_paths();
}

}  // namespace

void
pgr_do_edge_disjoint_paths(
    char *edges_sql,
    char *combinations_sql,
    ArrayType *starts,
    ArrayType *ends,
    bool directed,
    Path_rt **return_tuples,
    size_t *return_count,
    char** log_msg,
    char** notice_msg,
    char **err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::utilities::get_combinations;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    char* hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        hint = combinations_sql;
        auto combinations = get_combinations(combinations_sql, starts, ends, true);
        hint = nullptr;

        if (combinations.empty() && combinations_sql) {
            *notice_msg = to_pg_msg("No (source, target) pairs found");
            *log_msg = to_pg_msg(combinations_sql);
            return;
        }

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);
        hint = nullptr;

        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = to_pg_msg(edges_sql);
            return;
        }

        std::vector<Path_rt> paths;
        for (const auto &c : combinations) {
            for (const auto &t : c.second) {
                /*
                 * a source can not be a sink
                 * aka there is no path
                 */
                if (c.first == t) continue;
                auto path = single_execution(
                        edges,
                        c.first,
                        t,
                        directed);
                paths.insert(paths.end(), path.begin(), path.end());
            }
        }

        if (paths.empty()) {
            *return_tuples = nullptr;
            *return_count = 0;
            return;
        }

        /*
         * Initializing the cost
         */
        for (auto &r : paths) {
            r.agg_cost = r.cost = 0;
        }

        /*
         * Calculating the cost
         */
        auto found = paths.size();
        for (const auto &e : edges) {
            for (auto &r : paths) {
                if (r.edge == e.id) {
                    r.cost = (r.node == e.source) ?
                        e.cost : e.reverse_cost;
                    --found;
                }
            }
            if (found == 0) break;
        }

        /*
         * Calculating the agg_cost
         */
        auto prev = paths[0];
        for (auto &r : paths) {
            if (&r == &paths[0] || prev.edge < 0) {
                r.agg_cost = 0;
            } else {
                r.agg_cost = prev.agg_cost + prev.cost;
            }
            prev = r;
        }

        (*return_tuples) = pgr_alloc(paths.size(), (*return_tuples));
        for (size_t i = 0; i < paths.size(); ++i) {
            (*return_tuples)[i] = paths[i];
        }
        *return_count = paths.size();


        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
