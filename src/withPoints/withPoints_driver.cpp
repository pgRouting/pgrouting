/*PGR-GNU*****************************************************************
File: one_to_many_withPoints_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail:

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

#include "drivers/withPoints/withPoints_driver.h"

#include <algorithm>
#include <sstream>
#include <deque>
#include <vector>
#include <cassert>


#include "dijkstra/pgr_dijkstra.hpp"
#include "withPoints/pgr_withPoints.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

template < class G >
std::deque< Path >
pgr_dijkstra(
        G &graph,
        std::vector < int64_t > sources,
        std::vector < int64_t > targets,
        bool only_cost,
        bool normal) {
    std::sort(sources.begin(), sources.end());
    sources.erase(
            std::unique(sources.begin(), sources.end()),
            sources.end());

    std::sort(targets.begin(), targets.end());
    targets.erase(
            std::unique(targets.begin(), targets.end()),
            targets.end());

    pgrouting::Pgr_dijkstra< G > fn_dijkstra;
    auto paths = fn_dijkstra.dijkstra(graph, sources, targets, only_cost);

    if (!normal) {
        for (auto &path : paths) {
            path.reverse();
        }
    }
    return paths;
}


// CREATE OR REPLACE FUNCTION pgr_withPoint(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid ANYARRAY,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true

void
do_pgr_withPoints(
        pgr_edge_t *edges, size_t total_edges,
        Point_on_edge_t *points_p, size_t total_points,
        pgr_edge_t *edges_of_points, size_t total_edges_of_points,
        int64_t *start_pidsArr, size_t size_start_pidsArr,
        int64_t *end_pidsArr, size_t size_end_pidsArr,

        char driving_side,
        bool details,
        bool directed,
        bool only_cost,
        bool normal,

        General_path_element_t **return_tuples, size_t *return_count,

        char** log_msg,
        char** notice_msg,
        char** err_msg) {
    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert((*return_count) == 0);
        pgassert(edges || edges_of_points);
        pgassert(points_p);
        pgassert(start_pidsArr);
        pgassert(end_pidsArr);

        pgrouting::Pg_points_graph pg_graph(
                std::vector<Point_on_edge_t>(
                    points_p,
                    points_p + total_points),
                std::vector< pgr_edge_t >(
                    edges_of_points,
                    edges_of_points + total_edges_of_points),
                normal,
                driving_side,
                directed);

        if (pg_graph.has_error()) {
            log << pg_graph.get_log();
            err << pg_graph.get_error();
            *log_msg = pgr_msg(log.str().c_str());
            *err_msg = pgr_msg(err.str().c_str());
            return;
        }


        std::vector<int64_t>
            start_vertices(start_pidsArr, start_pidsArr + size_start_pidsArr);
        std::vector< int64_t >
            end_vertices(end_pidsArr, end_pidsArr + size_end_pidsArr);

        auto vertices(pgrouting::extract_vertices(edges, total_edges));
        vertices = pgrouting::extract_vertices(vertices, pg_graph.new_edges());

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path > paths;

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(vertices, gType);
            digraph.insert_edges(edges, total_edges);
            digraph.insert_edges(pg_graph.new_edges());

            paths = pgr_dijkstra(
                    digraph,
                    start_vertices, end_vertices,
                    only_cost, normal);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(vertices, gType);
            undigraph.insert_edges(edges, total_edges);
            undigraph.insert_edges(pg_graph.new_edges());
            paths = pgr_dijkstra(
                    undigraph,
                    start_vertices, end_vertices,
                    only_cost, normal);
        }

        if (!details) {
            for (auto &path : paths) {
                path = pg_graph.eliminate_details(path);
            }
        }

        /*
         * order paths based on the start_pid, end_pid
         */
        std::sort(paths.begin(), paths.end(),
                [](const Path &a, const Path &b)
                -> bool {
                if (b.start_id() != a.start_id()) {
                return a.start_id() < b.start_id();
                }
                return a.end_id() < b.end_id();
                });

        size_t count(0);
        count = count_tuples(paths);


        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));

        log << "************************************************";
        log << pg_graph.get_log();
        log << "************************************************";

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
