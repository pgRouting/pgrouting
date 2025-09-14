/*PGR-GNU*****************************************************************
File: maximum_cardinality_matching_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Ignoring directed flag & works only for undirected graph
Copyright (c) 2022 Celia Vriginia Vergara Castillo
Mail: vicky at erosion.dev

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli at gmail.com

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

#include "drivers/max_flow/maximum_cardinality_matching_driver.h"

#include <sstream>
#include <vector>
#include <string>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"
#include "max_flow/maximumcardinalitymatching.hpp"


void
pgr_do_maximum_cardinality_matching(
    const char *edges_sql,

    int64_t **return_tuples,
    size_t *return_count,

    char** log_msg,
    char** notice_msg,
    char **err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    const char *hint = nullptr;

    try {
        hint = edges_sql;
        auto edges = pgrouting::pgget::get_basic_edges(std::string(edges_sql));

        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }
        hint = nullptr;

        pgrouting::graph::UndirectedNoCostsBG graph(edges);
        auto matched_vertices = pgrouting::flow::maxCardinalityMatch(graph);

        (*return_tuples) = pgr_alloc(matched_vertices.size(), (*return_tuples));
        size_t i {0};
        for (const auto e : matched_vertices) {
            (*return_tuples)[i++] = e;
        }
        *return_count = matched_vertices.size();

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
