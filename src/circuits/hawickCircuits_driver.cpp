/*PGR-GNU*****************************************************************
File: hawickCircuits_driver.cpp

Generated with Template by:
Copyright (c) 2022 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Nitish Chauhan
Mail: nitishchauhan0022 at gmail.com

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


#include "drivers/circuits/hawickCircuits_driver.h"

#include <sstream>
#include <deque>
#include <algorithm>
#include <string>

#include "circuits/hawickcircuits.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

namespace {

/** @brief Calls the main function defined in the C++ Header file.
 *
 * @param graph      the graph containing the edges
 *
 * @returns results, when results are found
 */

template < class G >
std::deque <circuits_rt>
pgr_hawickCircuits(G &graph) {
    pgrouting::functions::pgr_hawickCircuits < G > fn_hawickCircuits;
    auto results = fn_hawickCircuits.hawickCircuits(graph);
    return results;
}
}  // namespace

void
pgr_do_hawickCircuits(
        const char *edges_sql,

        circuits_rt **return_tuples,
        size_t *return_count,

        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    using pgrouting::Path;
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

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);
        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }
        hint = nullptr;

        std::deque <circuits_rt> results;

        pgrouting::DirectedGraph digraph;
        digraph.insert_edges(edges);

        results = pgr_hawickCircuits(digraph);

        auto count = results.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No Circuit found";
            *log_msg = to_pg_msg(notice);
            return;
        }

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
