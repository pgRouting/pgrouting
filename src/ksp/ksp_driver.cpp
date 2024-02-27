/*PGR-GNU*****************************************************************
File: ksp_driver.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

Copyright (c) 2023 Aniket Agarwal
aniketgarg187 at gmail.com

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


#include "drivers/yen/ksp_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <string>

#include "yen/pgr_ksp.hpp"

#include "cpp_common/combinations.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.hpp"

#include "c_types/ii_t_rt.h"

void pgr_do_ksp(
        char* edges_sql,
        char* combinations_sql,
        ArrayType* starts,
        ArrayType* ends,

        int64_t *start_vid,
        int64_t *end_vid,

        size_t k,
        bool directed,
        bool heap_paths,
        Path_rt **return_tuples,
        size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::utilities::get_combinations;
    using pgrouting::yen::Pgr_ksp;

    std::ostringstream err;
    std::ostringstream log;
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
            *notice_msg = pgr_msg("No (source, target) pairs found");
            *log_msg = pgr_msg(combinations_sql);
            return;
        }

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);

        if (edges.empty()) {
            *notice_msg = pgr_msg("No edges found");
            *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
            return;
        }
        hint = nullptr;

        std::deque<Path>paths;

        if (directed) {
            pgrouting::DirectedGraph graph;
            graph.insert_edges(edges);
            paths = pgrouting::algorithms::Yen(graph, combinations, k, heap_paths);
        } else {
            pgrouting::UndirectedGraph graph;
            graph.insert_edges(edges);
            paths = pgrouting::algorithms::Yen(graph, combinations, k, heap_paths);
        }
        combinations.clear();

        auto count = count_tuples(paths);

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No paths found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = (collapse_paths(return_tuples, paths));

        size_t sequence = 0;
        for (const auto &path : paths) {
            if (path.size() > 0) path.get_pg_nksp_path(return_tuples, sequence);
        }
        *return_count = count;

        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch (const std::string &ex) {
        *err_msg = pgr_msg(ex.c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
