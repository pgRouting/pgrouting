/*PGR-GNU*****************************************************************
File: allpairs_driver.cpp

Generated with Template by:
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

#include "metrics/bandwidth.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/to_postgres.hpp"
#include "c_types/iid_t_rt.h"

void
do_metrics(
    std::string edges_sql,
    int which,

    IID_t_rt **return_tuples,
    size_t *return_count,
    char ** log_msg,
    char ** err_msg) {
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream err;
    std::string hint;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        using pgrouting::metrics::bandwidth;

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, true);

        if (edges.empty()) {
            throw std::string("No edges found");
        }

        hint = "";

        pgrouting::UndirectedGraph undigraph;
        undigraph.insert_edges(edges);

        uint64_t result;

        if (which == 0) {
            log << "call the function which calculates the bandwidth";
            result = bandwidth(undigraph);
        }

        log << "result = " << result;
#if 0
        if (*return_count == 0) {
            err <<  "No result generated, report this error\n";
            *err_msg = to_pg_msg(err);
            *return_tuples = NULL;
            *return_count = 0;
            return;
        }
#endif
        *log_msg = to_pg_msg(log);
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
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
