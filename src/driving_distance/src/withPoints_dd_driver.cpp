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


#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif

#include "./withPoints_dd_driver.h"
#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>

#include "./../../dijkstra/src/pgr_dijkstra.hpp"
#include "./../../withPoints/src/pgr_withPoints.hpp"


#include "./../../common/src/pgr_types.h"
#include "./../../common/src/pgr_alloc.hpp"


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

        /*
         * storing on C++ containers
         */
        std::vector<int64_t> start_vids(
                start_pidsArr, start_pidsArr + s_len);

        std::vector<Point_on_edge_t>
            points(points_p, points_p + total_points);

        std::vector<pgr_edge_t> edges_to_modify(
                edges_of_points, edges_of_points + total_edges_of_points);

        log << "start_vids :";
        for (const auto v : start_vids) log << v << ", ";
        /*
         * checking here is easier than on the C code
         */
        int errcode = check_points(points, log);
        if (errcode) {
            *log_msg = strdup(log.str().c_str());
            err << "Unexpected point(s) with same pid"
                << " but different edge/fraction/side combination found.";
            *err_msg = pgr_msg(err.str().c_str());
            return;
        }

        std::vector< pgr_edge_t > new_edges;
        create_new_edges(
                points,
                edges_to_modify,
                driving_side,
                new_edges,
                log);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path >paths;

        if (directed) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(edges, total_edges);
            digraph.insert_edges(new_edges);
            paths = pgr_drivingDistance(
                    digraph, start_vids, distance, equiCost);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(edges, total_edges);
            undigraph.insert_edges(new_edges);
            paths = pgr_drivingDistance(
                    undigraph, start_vids, distance, equiCost);
        }

        for (auto &path : paths) {
            log << path;

            if (!details) {
                eliminate_details_dd(path);
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



// CREATE OR REPLACE FUNCTION pgr_withPointsDD(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid BIGINT,
// end_pid BIGINT,
//
// driving_side CHAR -- DEFAULT 'b',
// details BOOLEAN -- DEFAULT false,
// directed BOOLEAN -- DEFAULT true,
// equicost BOOLEAN -- DEFAULT false,

int
do_pgr_withPointsDD(
        pgr_edge_t  *edges, size_t total_edges,
        Point_on_edge_t  *points_p, size_t total_points,
        pgr_edge_t  *edges_of_points, size_t total_edges_of_points,

        int64_t start_vid,
        double  distance,

        char driving_side,
        bool details,
        bool directed,

        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** err_msg) {
    std::ostringstream log;
    try {
        /*
         * This is the original state
         */
        if (*err_msg) free(err_msg);
        if (*return_tuples) free(return_tuples);
        (*return_count) = 0;

        std::vector< Point_on_edge_t >
            points(points_p, points_p + total_points);

        /*
         * checking here is easier than on the C code
         */
        int errcode = check_points(points, log);
        if (errcode) {
            return errcode;
        }


        std::vector< pgr_edge_t >
            edges_to_modify(
                    edges_of_points, edges_of_points + total_edges_of_points);

        std::vector< pgr_edge_t > new_edges;
        create_new_edges(
                points,
                edges_to_modify,
                driving_side,
                new_edges,
                log);


        graphType gType = directed? DIRECTED: UNDIRECTED;

        Path path;

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(edges, total_edges);
            digraph.insert_edges(new_edges);
            path = pgr_drivingDistance(digraph, start_vid, distance);
        } else {
            log << "Working with undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(edges, total_edges);
            undigraph.insert_edges(new_edges);
            path = pgr_drivingDistance(undigraph, start_vid, distance);
        }



        if (!details) {
            eliminate_details_dd(path);
        }
        std::sort(path.begin(), path.end(),
                [](const Path_t &l, const  Path_t &r)
                {return l.node < r.node;});
        std::stable_sort(path.begin(), path.end(),
                [](const Path_t &l, const  Path_t &r)
                {return l.agg_cost < r.agg_cost;});


        auto count(path.size());

        if (count == 0) {
            return 0;
        }


        *return_tuples = NULL;
        *return_tuples = pgr_alloc(count, (*return_tuples));

        size_t sequence = 0;
        path.get_pg_dd_path(return_tuples, sequence);

        if (count != sequence) {
            return 2;
        }
        (*return_count) = sequence;


#ifndef DEBUG
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif
        return 0;
    } catch ( ... ) {
        log << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
    }
    return 1000;
}
