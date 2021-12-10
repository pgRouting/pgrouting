/*PGR-GNU*****************************************************************
File: dijkstraViaVertex_driver.cpp

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

#include "drivers/trsp/trspVia_driver.h"

#include <sstream>
#include <deque>
#include <vector>

#include "dijkstra/dijkstraVia.hpp"
#include "trsp/pgr_trspHandler.h"

#include "cpp_common/rule.h"
#include "cpp_common/combinations.h"
#include "c_types/edge_t.h"
#include "c_types/restriction_t.h"
#include "c_types/routes_t.h"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"


namespace {

void
get_path(
        int route_id,
        int path_id,
        const Path &path,
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
        std::deque<Path> &paths) {
    size_t sequence = 0;
    int path_id = 1;
    int route_id = 1;
    double route_cost = 0;  // routes_agg_cost
    for (auto &p : paths) {
        p.recalculate_agg_cost();
    }
    for (const Path &path : paths) {
        if (path.size() > 0)
            get_path(route_id, path_id, path, ret_path, route_cost, sequence);
        ++path_id;
    }
    return sequence;
}
}  // namespace

void
do_trspVia(
        Edge_t* data_edges, size_t total_edges,
        Restriction_t* restrictions, size_t total_restrictions,
        int64_t* via_vidsArr, size_t size_via_vidsArr,
        bool directed,
        bool strict,
        bool U_turn_on_edge,
        Routes_t** return_tuples, size_t* return_count,

        char** log_msg,
        char** notice_msg,
        char** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;

    try {
        pgassert(total_edges != 0);
        pgassert(*return_count == 0);
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque<Path> paths;
        std::vector<int64_t> via_vertices(
                via_vidsArr, via_vidsArr + size_via_vidsArr);

        if (directed) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_edges);
            pgrouting::dijkstraVia(
                    digraph,
                    via_vertices,
                    paths,
                    strict,
                    U_turn_on_edge,
                    log);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_edges);
            pgrouting::dijkstraVia(
                    undigraph,
                    via_vertices,
                    paths,
                    strict,
                    U_turn_on_edge,
                    log);
        }

        size_t count(count_tuples(paths));

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            return;
        }

        if (total_restrictions == 0) {
            (*return_tuples) = pgr_alloc(count, (*return_tuples));
            (*return_count) = (get_route(return_tuples, paths));
            (*return_tuples)[count - 1].edge = -2;
            return;
        }

        /*
         * When there are turn restrictions
         */
        std::vector<pgrouting::trsp::Rule> ruleList;
        for (size_t i = 0; i < total_restrictions; ++i) {
            ruleList.push_back(pgrouting::trsp::Rule(*(restrictions + i)));
        }

        auto new_combinations = pgrouting::utilities::get_combinations(paths, ruleList);
        std::deque<Path> new_paths;
        if (!new_combinations.empty()) {
            pgrouting::trsp::Pgr_trspHandler gdef(
                    data_edges,
                    total_edges,
                    directed,
                    ruleList);

            auto new_paths = gdef.process(new_combinations);
            paths.insert(paths.end(), new_paths.begin(), new_paths.end());
            /*
             * order the path based on the vias
             */
            std::deque<Path> orderedPaths;
            auto prev_stop = via_vertices[0];
            for (const auto &stop : via_vertices) {
                if (stop == via_vertices[0]) continue;
                /*
                 * prev_stop, stop
                 */
                auto ptr = std::find_if(
                        paths.begin(), paths.end(),
                        [prev_stop, stop](const Path &p)
                        {return prev_stop == p.start_id() && stop == p.end_id();});
                if (ptr != paths.end()) orderedPaths.push_back(*ptr);
                prev_stop = stop;
            }
        }

        if (strict && paths.size() < via_vertices.size() -1) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = (get_route(return_tuples, paths));
        (*return_tuples)[count - 1].edge = -2;

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
