/*PGR-GNU*****************************************************************
File: withPointsVia_driver.cpp

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo

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

#include "drivers/withPoints/withPointsVia_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <string>

#include "c_types/routes_t.h"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/to_postgres.hpp"
#include "cpp_common/assert.hpp"
#include "dijkstra/dijkstraVia.hpp"
#include "withPoints/withPoints.hpp"


void
pgr_do_withPointsVia(
        const char *edges_sql,
        const char *points_sql,
        const char *edges_of_points_sql,
        ArrayType* viaArr,

        bool directed,
        char driving_side,
        bool details,
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
    using pgrouting::pgget::get_points;
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

        hint = points_sql;
        auto points = get_points(std::string(points_sql));

        hint = edges_of_points_sql;
        auto edges_of_points = get_edges(std::string(edges_of_points_sql), true, false);

        hint = edges_sql;
        auto edges = get_edges(std::string(edges_sql), true, false);

        if (edges.size() + edges_of_points.size() == 0) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = to_pg_msg(edges_sql);
            return;
        }
        hint = nullptr;

        /*
         * processing points
         */
        pgrouting::Pg_points_graph pg_graph(
                points, edges_of_points,
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
            undigraph.insert_edges(pg_graph.new_edges());
            pgrouting::dijkstraVia(
                    undigraph,
                    via,
                    paths,
                    strict,
                    U_turn_on_edge,
                    log);
        }

        if (!details) {
            for (auto &path : paths) path = pg_graph.eliminate_details(path);
        }

        (*return_count) = get_viaRoute(paths, return_tuples);

        if ((*return_count) == 0) {
            *log_msg = to_pg_msg("No paths found");
            return;
        }

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
