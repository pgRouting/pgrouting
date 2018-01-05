/*PGR-GNU*****************************************************************
File: withPoints_driver.cpp

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


#include "drivers/driving_distance/withPoints_dd_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>

#include "dijkstra/pgr_dijkstra.hpp"
#include "withPoints/pgr_withPoints.hpp"


#include "cpp_common/pgr_alloc.hpp"


/**********************************************************************/
// CREATE OR REPLACE FUNCTION pgr_withPointsDD(
// edges_sql TEXT,
// points_sql TEXT,
// start_pids anyarray,
// distance FLOAT,
//
// driving_side CHAR -- DEFAULT 'b',
// details BOOLEAN -- DEFAULT false,
// directed BOOLEAN -- DEFAULT true,
// equicost BOOLEAN -- DEFAULT false,


void
do_pgr_many_withPointsDD(
        pgr_edge_t      *edges,             size_t total_edges,
        Point_on_edge_t *points_p,          size_t total_points,
        pgr_edge_t      *edges_of_points,   size_t total_edges_of_points,

        int64_t  *start_pidsArr,    size_t s_len,
        double distance,

        bool directed,
        char driving_side,
        bool details,
        bool equiCost,

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
        pgassert(edges);
        pgassert(points_p);
        pgassert(edges_of_points);
        pgassert(start_pidsArr);


        pgrouting::Pg_points_graph pg_graph(
                std::vector<Point_on_edge_t>(
                    points_p,
                    points_p + total_points),
                std::vector< pgr_edge_t >(
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



        /*
         * storing on C++ containers
         */
        std::vector<int64_t> start_vids(
                start_pidsArr, start_pidsArr + s_len);


        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path >paths;

        if (directed) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(edges, total_edges);
            digraph.insert_edges(pg_graph.new_edges());
            paths = pgr_drivingDistance(
                    digraph, start_vids, distance, equiCost, log);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(edges, total_edges);
            undigraph.insert_edges(pg_graph.new_edges());

            paths = pgr_drivingDistance(
                    undigraph, start_vids, distance, equiCost, log);
        }

        for (auto &path : paths) {
            log << path;

            if (!details) {
                pg_graph.eliminate_details_dd(path);
            }
            log << path;
            std::sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    {return l.node < r.node;});
            std::stable_sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    {return l.agg_cost < r.agg_cost;});
            log << path;
        }

        size_t count(count_tuples(paths));


        if (count == 0) {
            *notice_msg = pgr_msg("No return values was found");
            return;
        }
        *return_tuples = pgr_alloc(count, (*return_tuples));
        *return_count = collapse_paths(return_tuples, paths);
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


