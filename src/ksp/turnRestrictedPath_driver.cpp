/*PGR-GNU*****************************************************************
File: turnRestrictedPath_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
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

#include "drivers/yen/turnRestrictedPath_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <string>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/rule.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/combinations.hpp"
#include "cpp_common/restriction_t.hpp"
#include "cpp_common/basePath_SSEC.hpp"
#include "yen/turnRestrictedPath.hpp"

namespace {

template < class G >
std::deque<pgrouting::Path>
pgr_dijkstraTR(
        G &graph,
        const std::vector<pgrouting::trsp::Rule> &restrictions,
        int64_t source,
        int64_t target,
        std::string& log,
        size_t k,
        bool heap_paths,
        bool stop_on_first,
        bool strict) {
    using pgrouting::yen::Pgr_turnRestrictedPath;
    Pgr_turnRestrictedPath<G> fn_TRSP;

    auto paths = fn_TRSP.turnRestrictedPath(graph,
                    restrictions,
                    source,
                    target,
                    k,
                    heap_paths,
                    stop_on_first,
                    strict);

    log += fn_TRSP.get_log();
    return paths;
}

}  // namespace

void
pgr_do_turnRestrictedPath(
        const char *edges_sql,
        const char *restrictions_sql,
        const char *combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        size_t k,
        bool directed,
        bool heap_paths,
        bool stop_on_first,
        bool strict,

        Path_rt **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::utilities::get_combinations;
    using pgrouting::yen::Pgr_turnRestrictedPath;
    using pgrouting::trsp::Rule;
    using pgrouting::pgget::get_restrictions;
    using pgrouting::pgget::get_edges;

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



        hint = combinations_sql;
        auto combinations = get_combinations(combinations_sql, starts, ends, true);
        hint = nullptr;

        if (combinations.empty() && combinations_sql) {
            *notice_msg = to_pg_msg("No (source, target) pairs found");
            *log_msg = to_pg_msg(combinations_sql);
            return;
        }

        hint = edges_sql;
        auto edges = get_edges(std::string(edges_sql), true, false);

        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }
        hint = nullptr;

        hint = restrictions_sql;
        auto restrictions = restrictions_sql?
            get_restrictions(std::string(restrictions_sql))
            : std::vector<Restriction_t>();
        hint = nullptr;

        std::vector<pgrouting::trsp::Rule> ruleList;
        for (const auto &r : restrictions) {
            ruleList.push_back(Rule(r));
        }

        std::deque<Path> paths;
        std::string logstr;
        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph;
            Pgr_turnRestrictedPath < pgrouting::DirectedGraph > fn_TRSP;
            digraph.insert_edges(edges);
            log << digraph;
            paths = pgr_dijkstraTR(digraph,
                    ruleList,

                    combinations.begin()->first,
                    *(combinations.begin()->second.begin()),

                    logstr,
                    k,
                    heap_paths,
                    stop_on_first,
                    strict);
        } else {
            log << "TODO Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph;
            Pgr_turnRestrictedPath < pgrouting::UndirectedGraph > fn_TRSP;
            undigraph.insert_edges(edges);
            paths = pgr_dijkstraTR(undigraph,
                    ruleList,

                    combinations.begin()->first,
                    *(combinations.begin()->second.begin()),

                    logstr,

                    k,
                    heap_paths,
                    stop_on_first,
                    strict);
        }

        log << logstr;

        auto count(count_tuples(paths));
        log << "\nCount = " << count;

        if (!(count == 0)) {
            *return_tuples = NULL;
            *return_tuples = pgr_alloc(count, (*return_tuples));

            size_t sequence = 0;
            int route_id = 0;
            for (const auto &path : paths) {
                if (path.size() > 0) {
                    path.get_pg_turn_restricted_path(
                            return_tuples,
                            sequence,
                            route_id);
                }
                log << "the agg cost" << (*return_tuples)[0].agg_cost;
                ++route_id;
            }
        }
        *return_count = count;


        pgassert(*err_msg == NULL);
        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
        pgassert(!log.str().empty());
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
