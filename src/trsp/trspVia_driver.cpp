/*PGR-GNU*****************************************************************
File: trspVia_driver.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo

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

#include "drivers/trsp/trspVia_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>

#include "c_types/routes_t.h"
#include "cpp_common/restriction_t.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/to_postgres.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/rule.hpp"
#include "cpp_common/combinations.hpp"
#include "dijkstra/dijkstraVia.hpp"

#include "trsp/trspHandler.hpp"

namespace {

/** @brief Orders results in terms of the via information */
void
post_process_trspvia(std::deque<pgrouting::Path> &paths, std::vector<int64_t> via) {
    using pgrouting::Path;
    for (auto &p : paths) {
        p.recalculate_agg_cost();
    }

    std::deque<Path> ordered_paths;
    auto u = via.front();
    bool skip = true;
    for (const auto &v : via) {
        if (skip) {
            skip = false; continue;
        }
        /*
         * look for the path (u,v)
         */
        auto path_ptr = std::find_if(
                paths.begin(), paths.end(),
                [&](const Path &path)
                {return (u == path.start_id()) && (v == path.end_id());});

        if (path_ptr == paths.end()) {
            /*
             * TODO path not found
             */
        } else {
            /* path was found */
            ordered_paths.push_back(*path_ptr);
            paths.erase(path_ptr);
        }
        u = v;
    }

    paths = ordered_paths;
}

}  // namespace

void
pgr_do_trspVia(
        const char *edges_sql,
        const char *restrictions_sql,
        ArrayType* viaArr,

        bool directed,
        bool strict,
        bool U_turn_on_edge,
        Routes_t** return_tuples, size_t* return_count,

        char** log_msg,
        char** notice_msg,
        char** err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::pgget::get_intArray;
    using pgrouting::pgget::get_edges;
    using pgrouting::to_postgres::get_viaRoute;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    const char *hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        auto via = get_intArray(viaArr, false);

        hint = edges_sql;
        auto edges = get_edges(std::string(edges_sql), true, false);

        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = to_pg_msg(edges_sql);
            return;
        }
        hint = nullptr;

        hint = restrictions_sql;
        auto restrictions = restrictions_sql?
            pgrouting::pgget::get_restrictions(std::string(restrictions_sql)) : std::vector<Restriction_t>();
        hint = nullptr;

        std::deque<Path> paths;
        if (directed) {
            pgrouting::DirectedGraph digraph;
            digraph.insert_edges(edges);
            pgrouting::dijkstraVia(
                    digraph,
                    via,
                    paths,
                    strict,
                    U_turn_on_edge,
                    log);
        } else {
            pgrouting::UndirectedGraph undigraph;
            undigraph.insert_edges(edges);
            pgrouting::dijkstraVia(
                    undigraph,
                    via,
                    paths,
                    strict,
                    U_turn_on_edge,
                    log);
        }

        size_t count = count_tuples(paths);

        if (count == 0) {
            *log_msg = to_pg_msg("No paths found");
            return;
        }

        /*
         * Exit with the current results when there are no restrictions
         */
        if (restrictions.empty()) {
            (*return_count) = get_viaRoute(paths, return_tuples);
            return;
        }

        std::vector<pgrouting::trsp::Rule> ruleList;
        for (const auto &r : restrictions) {
            if (r.via) ruleList.push_back(pgrouting::trsp::Rule(r));
        }
        hint = nullptr;

        auto new_combinations = pgrouting::utilities::get_combinations(paths, ruleList);

        if (!new_combinations.empty()) {
            pgrouting::trsp::TrspHandler gdef(edges, directed, ruleList);
            auto new_paths = gdef.process(new_combinations);
            paths.insert(paths.end(), new_paths.begin(), new_paths.end());
        }
        post_process_trspvia(paths, via);

        (*return_count) = get_viaRoute(paths, return_tuples);

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
        *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
