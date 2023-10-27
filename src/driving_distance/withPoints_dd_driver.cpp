/*PGR-GNU*****************************************************************
File: withPoints_dd_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copyright (c) 2023 Yige Huang
Mail: square1ge at gmail.com
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

#include "dijkstra/drivingDist.hpp"
#include "withPoints/pgr_withPoints.hpp"
#include "c_types/mst_rt.h"

#include "cpp_common/pgr_alloc.hpp"


void
pgr_do_withPointsDD(
        Edge_t          *edges,             size_t total_edges,
        Point_on_edge_t *points_p,          size_t total_points,
        Edge_t          *edges_of_points,   size_t total_edges_of_points,
        int64_t         *start_pidsArr,     size_t s_len,

        double distance,
        char driving_side,

        bool directed,
        bool details,
        bool equiCost,

        MST_rt  **return_tuples,     size_t *return_count,
        char** log_msg,
        char** notice_msg,
        char** err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;

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
        pgassert(start_pidsArr);


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

        std::vector<int64_t> start_vids(start_pidsArr, start_pidsArr + s_len);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque<Path> paths;
        std::vector<std::map<int64_t, int64_t>> depths;

        if (directed) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(edges, total_edges);
            digraph.insert_edges(pg_graph.new_edges());
            paths = pgr_drivingdistance(digraph, start_vids, distance, equiCost, depths, details);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(edges, total_edges);
            undigraph.insert_edges(pg_graph.new_edges());
            paths = pgr_drivingdistance(undigraph, start_vids, distance, equiCost, depths, details);
        }

        if (!details) {
            for (auto &path : paths) {
                pg_graph.eliminate_details_dd(path);
            }
        }


        size_t count(count_tuples(paths));

        if (count == 0) {
            log << "\nNo return values were found";
            *notice_msg = pgr_msg(log.str().c_str());
            return;
        }

        *return_tuples = pgr_alloc(count, (*return_tuples));
        *return_count = collapse_paths(return_tuples, paths);

        for (size_t i = 0; i < count; i++) {
            auto row = (*return_tuples)[i];
            /* given the depth assign the correct depth */
            int64_t depth = -1;
            for (const auto &d : depths) {
                /* look for the correct path */
                auto itr = d.find(row.from_v);
                if (itr == d.end() || !(itr->second == 0)) continue;
                depth = d.at(row.node);
            }
            (*return_tuples)[i].depth = depth;
        }

        /* sort to get depths in order*/
        std::sort((*return_tuples), (*return_tuples) + count,
                [](const MST_rt &l, const MST_rt &r) {return l.agg_cost < r.agg_cost;});
        std::stable_sort((*return_tuples), (*return_tuples) + count,
                [](const MST_rt &l, const MST_rt &r) {return l.depth < r.depth;});
        std::stable_sort((*return_tuples), (*return_tuples) + count,
                [](const MST_rt &l, const MST_rt &r) {return l.from_v < r.from_v;});

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
