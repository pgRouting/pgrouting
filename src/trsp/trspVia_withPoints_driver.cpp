/*PGR-GNU*****************************************************************
File: trspVia_withPoints_driver.cpp

Function's developer:
Copyright (c) 2022 Celia Virginia Vergara Castillo

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

#include "drivers/trsp/trspVia_withPoints_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>

#include "dijkstra/dijkstraVia.hpp"
#include "withPoints/pgr_withPoints.hpp"
#include "c_types/routes_t.h"
#include "c_types/restriction_t.h"
#include "cpp_common/rule.h"
#include "cpp_common/combinations.h"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "trsp/pgr_trspHandler.h"


namespace {

/** @brief Orders results in terms of the via information */
void
post_process_trspvia(std::deque<Path> &paths, std::vector<int64_t> via) {
    for (auto &p : paths) {
        p.recalculate_agg_cost();
    }

    std::deque<Path> ordered_paths;
    auto u = via.front();
    bool skip = true;
    for (const auto &v : via) {
        if (skip) {skip = false; continue;}
        /*
         * look for the path (u,v)
         */
        auto path_ptr = std::find_if(
                paths.begin(), paths.end(),
                [&](const Path &path)
                {return (u == path.start_id()) && (v == path.end_id());});

        if (path_ptr == paths.end()) {
            /*
             * TODO path not found
             */
        } else {
            /* path was found */
            ordered_paths.push_back(*path_ptr);
            paths.erase(path_ptr);
        }
        u = v;
    }

    paths = ordered_paths;
}

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
do_trspVia_withPoints(
        Edge_t* edges, size_t total_edges,
        Restriction_t *restrictions, size_t restrictions_size,
        Point_on_edge_t *points_p, size_t total_points,
        Edge_t *edges_of_points, size_t total_edges_of_points,
        int64_t* via_vidsArr, size_t size_via_vidsArr,

        bool directed,

        char driving_side,
        bool details,

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
        pgassert((total_edges + total_edges_of_points) != 0);
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        std::deque<Path>paths;

        graphType gType = directed? DIRECTED: UNDIRECTED;

        /*
         * processing via
         */
        std::vector<int64_t> via_vertices(via_vidsArr, via_vidsArr + size_via_vidsArr);

        /*
         * processing points
         */
        pgrouting::Pg_points_graph pg_graph(
                std::vector<Point_on_edge_t>(
                    points_p,
                    points_p + total_points),
                std::vector< Edge_t >(
                    edges_of_points,
                    edges_of_points + total_edges_of_points),
                true,
                driving_side,
                directed);

        if (pg_graph.has_error()) {
            log << pg_graph.get_log();
            err << pg_graph.get_error();
            *log_msg = pgr_msg(log.str().c_str());
            *err_msg = pgr_msg(err.str().c_str());
            return;
        }

        auto vertices(pgrouting::extract_vertices(edges, total_edges));
        vertices = pgrouting::extract_vertices(vertices, pg_graph.new_edges());

        if (directed) {
            pgrouting::DirectedGraph digraph(vertices, gType);
            digraph.insert_edges(edges, total_edges);
            digraph.insert_edges(pg_graph.new_edges());
            pgrouting::dijkstraVia(
                    digraph,
                    via_vertices,
                    paths,
                    strict,
                    U_turn_on_edge,
                    log);
        } else {
            pgrouting::UndirectedGraph undigraph(vertices, gType);
            undigraph.insert_edges(edges, total_edges);
            undigraph.insert_edges(pg_graph.new_edges());
            pgrouting::dijkstraVia(
                    undigraph,
                    via_vertices,
                    paths,
                    strict,
                    U_turn_on_edge,
                    log);
        }

        if (!details) {
            for (auto &path : paths) path = pg_graph.eliminate_details(path);
        }

        size_t count(count_tuples(paths));

        if (count == 0) {
            notice << "No paths found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        if (restrictions_size == 0) {
            (*return_tuples) = pgr_alloc(count, (*return_tuples));
            (*return_count) = (get_route(return_tuples, paths));
            (*return_tuples)[count - 1].edge = -2;
            return;
        }

        /*
         * When there are turn restrictions
         */
        std::vector<pgrouting::trsp::Rule> ruleList;
        for (size_t i = 0; i < restrictions_size; ++i) {
            if (restrictions[i].via_size == 0) continue;
            ruleList.push_back(pgrouting::trsp::Rule(*(restrictions + i)));
        }

        auto new_combinations = pgrouting::utilities::get_combinations(paths, ruleList);

        if (!new_combinations.empty()) {
            pgrouting::trsp::Pgr_trspHandler gdef(
                    edges,
                    total_edges,
                    pg_graph.new_edges(),
                    directed,
                    ruleList);
            auto new_paths = gdef.process(new_combinations);
            paths.insert(paths.end(), new_paths.begin(), new_paths.end());
        }
        post_process_trspvia(paths, via_vertices);
        if (!details) {
            for (auto &path : paths) path = pg_graph.eliminate_details(path);
        }


        count = count_tuples(paths);

        if (count == 0) {
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
