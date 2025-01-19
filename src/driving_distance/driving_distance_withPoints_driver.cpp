/*PGR-GNU*****************************************************************
File: driving_distance_withPoints.cpp

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


#include "drivers/driving_distance/driving_distance_withPoints_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

#include "cpp_common/pgdata_getters.hpp"
#include "dijkstra/drivingDist.hpp"
#include "withPoints/withPoints.hpp"
#include "c_types/mst_rt.h"
#include "cpp_common/combinations.hpp"
#include "cpp_common/alloc.hpp"


void
pgr_do_withPointsDD(
        char *edges_sql,
        char *points_sql,
        char *edges_of_points_sql,
        ArrayType* starts,

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
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::pgget::get_intSet;
    using pgrouting::utilities::get_combinations;
    using pgrouting::pgget::get_points;
    using pgrouting::pgget::get_edges;
    using pgrouting::algorithm::drivingDistance;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    char *hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        auto roots = get_intSet(starts);

        hint = points_sql;
        auto points = get_points(std::string(points_sql));

        hint = edges_of_points_sql;
        auto edges_of_points = get_edges(std::string(edges_of_points_sql), true, false);

        hint = edges_sql;
        auto edges = get_edges(std::string(edges_sql), true, false);

        if (edges.size() + edges_of_points.size() == 0) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }
        hint = nullptr;

        /*
         * processing points
         */
        pgrouting::Pg_points_graph pg_graph(points, edges_of_points,
                true, driving_side, directed);

        if (pg_graph.has_error()) {
            log << pg_graph.get_log();
            err << pg_graph.get_error();
            *log_msg = to_pg_msg(log);
            *err_msg = to_pg_msg(err);
            return;
        }



        std::deque<Path> paths;
        std::vector<std::map<int64_t, int64_t>> depths;

        if (directed) {
            pgrouting::DirectedGraph digraph;
            digraph.insert_edges(edges);
            digraph.insert_edges(pg_graph.new_edges());
            paths = drivingDistance(digraph, roots, distance, equiCost, depths, details);
        } else {
            pgrouting::UndirectedGraph undigraph;
            undigraph.insert_edges(edges);
            undigraph.insert_edges(pg_graph.new_edges());
            paths = drivingDistance(undigraph, roots, distance, equiCost, depths, details);
        }

        if (!details) {
            for (auto &path : paths) pg_graph.eliminate_details_dd(path);
        }


        auto count(count_tuples(paths));


        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No paths found";
            *log_msg = to_pg_msg(notice);
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = (collapse_paths(return_tuples, paths));

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
