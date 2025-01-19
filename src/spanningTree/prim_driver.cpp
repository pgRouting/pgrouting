/*PGR-GNU*****************************************************************
File: prim_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Aditya Pratap Singh
Mail: adityapratap.singh28@gmail.com
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

#include "drivers/spanningTree/prim_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <string>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

#include "c_types/mst_rt.h"

#include "spanningTree/prim.hpp"
#include "spanningTree/details.hpp"


void
pgr_do_prim(
        char *edges_sql,
        ArrayType* starts,

        char* fn_suffix,

        int64_t max_depth,
        double distance,

        MST_rt **return_tuples,
        size_t *return_count,

        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::pgget::get_intArray;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    char *hint = nullptr;

    try {
        // NOLINTBEGIN(clang-analyzer-cplusplus.NewDelete)
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        // NOLINTEND(clang-analyzer-cplusplus.NewDelete)

        auto roots = get_intArray(starts, false);

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);
        hint = nullptr;

        std::string suffix(fn_suffix);

        std::vector<MST_rt> results;

        pgrouting::UndirectedGraph undigraph;
        undigraph.insert_min_edges_no_parallel(edges);
        pgrouting::functions::Pgr_prim<pgrouting::UndirectedGraph> prim;

        if (edges.empty()) {
            results = pgrouting::details::get_no_edge_graph_result(roots);
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = to_pg_msg(edges_sql);
        } else {
            if (suffix == "") {
                results = prim.prim(undigraph);
            } else if (suffix == "BFS") {
                results = prim.primBFS(undigraph, roots, max_depth);
            } else if (suffix == "DFS") {
                results = prim.primDFS(undigraph, roots, max_depth);
            } else if (suffix == "DD") {
                results = prim.primDD(undigraph, roots, distance);
            } else {
                err << "Unknown Prim function";
                *err_msg = to_pg_msg(err);
                return;
            }
        }

        auto count = results.size();

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        for (size_t i = 0; i < count; i++) {
            *((*return_tuples) + i) = results[i];
        }
        (*return_count) = count;

        pgassert(*err_msg == NULL);
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
