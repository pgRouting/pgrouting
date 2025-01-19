/*PGR-GNU*****************************************************************
File: withPoints_ksp_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2023 Abhinav Jain
Mail: this.abhinav at gmail.com

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

#include "drivers/yen/withPoints_ksp_driver.h"


#include <sstream>
#include <deque>
#include <vector>
#include <string>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/combinations.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"
#include "withPoints/withPoints.hpp"

#include "yen/ksp.hpp"

void
pgr_do_withPointsKsp(
        char *edges_sql,
        char *points_sql,
        char *edges_of_points_sql,
        char *combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        int64_t *start_vid,
        int64_t *end_vid,

        size_t k,
        bool directed,
        bool heap_paths,
        char driving_side,
        bool details,
        Path_rt **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::yen::Pgr_ksp;
    using pgrouting::utilities::get_combinations;
    using pgrouting::pgget::get_points;
    using pgrouting::pgget::get_edges;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    char *hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);


        hint = combinations_sql;
        auto combinations = get_combinations(combinations_sql, starts, ends, true);
        hint = nullptr;

        if (start_vid && end_vid) {
            combinations[*start_vid].insert(*end_vid);
        }

        if (combinations.empty() && combinations_sql) {
            *notice_msg = to_pg_msg("No (source, target) pairs found");
            *log_msg = to_pg_msg(combinations_sql);
            return;
        }

        log << "Combinations size " << combinations.size();

        hint = points_sql;
        auto points = get_points(std::string(points_sql));

        hint = edges_of_points_sql;
        auto edges_of_points = get_edges(std::string(edges_of_points_sql), true, false);

        hint = edges_sql;
        auto edges = get_edges(std::string(edges_sql), true, false);

        if (edges.size() + edges_of_points.size() == 0) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }
        hint = nullptr;

        pgrouting::Pg_points_graph pg_graph(points, edges_of_points,
                true,
                driving_side,
                directed);

        if (pg_graph.has_error()) {
            log << pg_graph.get_log();
            err << pg_graph.get_error();
            *log_msg = to_pg_msg(log);
            *err_msg = to_pg_msg(err);
            return;
        }

        std::deque<Path> paths;
        if (directed) {
            pgrouting::DirectedGraph digraph;
            digraph.insert_edges(edges);
            digraph.insert_edges(pg_graph.new_edges());

            paths = pgrouting::algorithms::Yen(digraph, combinations, k, heap_paths);
        } else {
            pgrouting::UndirectedGraph undigraph;
            undigraph.insert_edges(edges);
            undigraph.insert_edges(pg_graph.new_edges());

            paths = pgrouting::algorithms::Yen(undigraph, combinations, k, heap_paths);
        }


        if (!details) {
            for (auto &path : paths) {
                path = pg_graph.eliminate_details(path);
            }
        }

        auto count(count_tuples(paths));

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No paths found";
            *log_msg = to_pg_msg(notice);
            return;
        }


        *return_tuples = NULL;
        *return_tuples = pgr_alloc(count, (*return_tuples));

        size_t sequence = 0;
        for (const auto &path : paths) {
            if (path.size() > 0)
                path.get_pg_nksp_path(return_tuples, sequence);
        }

        if (count != sequence) {
            (*return_count) = 0;
            notice << "Something went wrong";
            *log_msg = to_pg_msg(notice);
            return;
        }
        (*return_count) = sequence;

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
