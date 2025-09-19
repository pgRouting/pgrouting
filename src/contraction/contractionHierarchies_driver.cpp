/*PGR-GNU*****************************************************************
File: contractionHierarchies_driver.cpp

Generated with Template by:
Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) Aurélie Bousquet - 2024
Mail: aurelie.bousquet at oslandia.com

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

#include "drivers/contraction/contractionHierarchies_driver.h"

#include <string>
#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/to_postgres.hpp"
#include "contraction/ch_graphs.hpp"
#include "contraction/contractionHierarchies.hpp"

#include "c_types/contractionHierarchies_rt.h"
#include "cpp_common/identifiers.hpp"
#include "cpp_common/alloc.hpp"


void
pgr_contractionHierarchies(
        const char *edges_sql,
        ArrayType* forbidden,
        bool directed,
        contractionHierarchies_rt **return_tuples,
        size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::pgget::get_intArray;
    using pgrouting::pgget::get_edges;
    using pgrouting::functions::contractionHierarchies;
    using pgrouting::to_postgres::graph_to_tuple;

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

        hint = edges_sql;
        auto edges = get_edges(std::string(edges_sql), true, false);
        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }
        hint = nullptr;

        auto forbid = get_intArray(forbidden, true);

        if (directed) {
            using DirectedGraph = pgrouting::graph::CHDirectedGraph;
            DirectedGraph digraph;
            digraph.insert_edges(edges);

            contractionHierarchies(digraph, directed, forbid, log, err);
            graph_to_tuple(digraph, return_tuples, return_count);
        } else {
            using UndirectedGraph = pgrouting::graph::CHUniqueUndirectedGraph;
            UndirectedGraph undigraph;
            undigraph.insert_edges(edges);

            contractionHierarchies(undigraph, directed, forbid, log, err);
            graph_to_tuple(undigraph, return_tuples, return_count);
        }

        pgassert(err.str().empty());
        *log_msg = log.str().empty()?
            *log_msg :
            to_pg_msg(log);
        *notice_msg = notice.str().empty()?
            *notice_msg :
            to_pg_msg(notice);
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
