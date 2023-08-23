/*PGR-GNU*****************************************************************
File: drivedist_driver.cpp

Generated with Template by:                                                                                             
Copyright (c) 2023 pgRouting developers                                                                                 
Mail: project AT pgrouting.org   

Copyright (c) 2023 Aryan Gupta
guptaaryan1010 AT gmail.com
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

#include "drivers/driving_distance/drivedist_driver.h"

#include <sstream>
#include <deque>
#include <vector>

#include "dijkstra/drivingDist.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"


void
do_pgr_driving_many_to_dist(
        Edge_t  *data_edges, size_t total_edges,
        int64_t *start_vertex, size_t s_len,
        double distance,
        bool directedFlag,
        bool equiCostFlag,
        bool do_new,
        Path_rt **return_old_tuples, size_t *return_old_count,
        MST_rt  **return_tuples,     size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_alloc;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;

    try {
        pgassert(total_edges != 0);
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert((*return_tuples) == NULL);

        graphType gType = directedFlag? DIRECTED: UNDIRECTED;

        std::deque<Path> paths;
        std::deque<MST_rt> results;
        std::vector<int64_t> start_vertices(start_vertex, start_vertex + s_len);

        auto vertices(pgrouting::extract_vertices(data_edges, total_edges));


        if (directedFlag) {
            pgrouting::DirectedGraph digraph(vertices, gType);

            digraph.insert_edges(data_edges, total_edges, true);

            paths = pgr_drivingdistance(
                    digraph, start_vertices, distance, equiCostFlag, log);
            if (do_new) {
                pgrouting::functions::ShortestPath_tree<pgrouting::DirectedGraph> spt;
                results = spt.get_depths(digraph, paths);
            }
        } else {
            pgrouting::UndirectedGraph undigraph(vertices, gType);

            undigraph.insert_edges(data_edges, total_edges, true);


            paths = pgr_drivingdistance(
                    undigraph, start_vertices, distance, equiCostFlag, log);
            if (do_new) {
                pgrouting::functions::ShortestPath_tree<pgrouting::UndirectedGraph> spt;
                results = spt.get_depths(undigraph, paths);
            }
        }

        if (do_new) {
        size_t count(results.size());


        if (count == 0) {
            log << "\nNo return values were found";
            *notice_msg = pgr_msg(log.str().c_str());
            return;
        }
        *return_tuples = pgr_alloc(count, (*return_tuples));
        for (size_t i = 0; i < count; i++) {
            *((*return_tuples) + i) = results[i];
        }
        (*return_count) = count;
        } else {
        /* old code to be removed on v4 */
        for (auto &path : paths) {
            std::sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    {return l.node < r.node;});
            std::stable_sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    {return l.agg_cost < r.agg_cost;});
        }
        size_t count(count_tuples(paths));

        if (count == 0) {
            *notice_msg = pgr_msg("No return values was found");
            return;
        }
        *return_old_tuples = pgr_alloc(count, (*return_old_tuples));
        *return_old_count = collapse_paths(return_old_tuples, paths);
        }

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
    } catch( ... ) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
