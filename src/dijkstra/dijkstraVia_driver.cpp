/*PGR-GNU*****************************************************************
File: dijkstraVia_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers

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

#include "drivers/dijkstra/dijkstraVia_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <string>

#include "c_types/routes_t.h"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"
#include "dijkstra/dijkstraVia.hpp"


namespace  {
void
get_path(
        int route_id,
        int path_id,
        const pgrouting::Path &path,
        Routes_t **postgres_data,
        double &route_cost,
        size_t &sequence) {
    size_t i = 0;
    for (const auto e : path) {
        (*postgres_data)[sequence] = {
            route_id,
            path_id,
            static_cast<int>(i),
            path.start_id(),
            path.end_id(),
            e.node,
            e.edge,
            e.cost,
            e.agg_cost,
            route_cost};
        route_cost += path[i].cost;
        ++i;
        ++sequence;
    }
}


size_t
get_route(
        Routes_t **ret_path,
        std::deque<pgrouting::Path> &paths) {
    size_t sequence = 0;
    int path_id = 1;
    int route_id = 1;
    double route_cost = 0;  // routes_agg_cost
    for (auto &p : paths) {
        p.recalculate_agg_cost();
    }
    for (const auto &path : paths) {
        if (path.size() > 0)
            get_path(route_id, path_id, path, ret_path, route_cost, sequence);
        ++path_id;
    }
    return sequence;
}
}  // namespace

void
pgr_do_dijkstraVia(
        char *edges_sql,
        ArrayType* viaArr,

        bool directed,
        bool strict,
        bool U_turn_on_edge,
        Routes_t** return_tuples,   size_t* return_count,

        char** log_msg,
        char** notice_msg,
        char** err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::pgget::get_intArray;
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



        auto via = get_intArray(viaArr, false);

        hint = edges_sql;
        auto edges = get_edges(std::string(edges_sql), true, false);

        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }
        hint = nullptr;

        std::deque<Path>paths;
        if (directed) {
            pgrouting::DirectedGraph digraph;
            digraph.insert_edges(edges);
            pgrouting::pgr_dijkstraVia(
                    digraph,
                    via,
                    paths,
                    strict,
                    U_turn_on_edge,
                    log);
        } else {
            pgrouting::UndirectedGraph undigraph;
            undigraph.insert_edges(edges);
            pgrouting::pgr_dijkstraVia(
                    undigraph,
                    via,
                    paths,
                    strict,
                    U_turn_on_edge,
                    log);
        }

        size_t count(count_tuples(paths));

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No paths found";
            *log_msg = to_pg_msg(notice);
            return;
        }

        // get the space required to store all the paths
        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "\nConverting a set of paths into the tuples";
        (*return_count) = (get_route(return_tuples, paths));
        (*return_tuples)[count - 1].edge = -2;

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
