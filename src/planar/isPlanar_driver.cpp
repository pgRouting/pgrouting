/*PGR-GNU*****************************************************************
File: isPlanar_driver.cpp

Generated with Template by:
Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Himanshu Raj
Mail: raj.himanshu2@gmail.com

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

#include "drivers/planar/isPlanar_driver.h"

#include <vector>
#include <algorithm>
#include <string>

#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/pgdata_getters.hpp"

#include "planar/boyerMyrvold.hpp"
#include "cpp_common/base_graph.hpp"



bool
pgr_do_isPlanar(
                char *edges_sql,

                char ** log_msg,
                char ** notice_msg,
                char ** err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::pgget::get_edges;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    char* hint = nullptr;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));

        bool result = false;

        hint = edges_sql;
        auto edges = get_edges(std::string(edges_sql), true, true);

        if (edges.empty()) {
            return false;
        }
        hint = nullptr;

        pgrouting::UndirectedGraph undigraph;
        undigraph.insert_edges(edges);
        pgrouting::functions::Pgr_boyerMyrvold<pgrouting::UndirectedGraph> fn_isPlanar;
        result = fn_isPlanar.isPlanar(undigraph);

        pgassert(*err_msg == NULL);
        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
        return result;
    } catch (AssertFailedException &except) {
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
    } catch (std::exception &except) {
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch(...) {
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
    return false;
}
