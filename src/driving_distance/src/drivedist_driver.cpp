/*PGR-GNU*****************************************************************
File: boost_interface_drivedist.cpp

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#include "dijkstra/pgr_dijkstra.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#undef WITH_TIME
#ifdef WITH_TIME
#include <ctime>
#include <chrono>

static
void
start_timing(
        std::time_t &start_t,
        std::chrono::steady_clock::time_point &begin_elapsed,
        clock_t &begin) {
    begin = clock();
    start_t = std::time(NULL);
    begin_elapsed = std::chrono::steady_clock::now();
}

static
void
end_timing(
        const std::time_t start_t,
        std::chrono::steady_clock::time_point& begin_elapsed,
        clock_t &begin,
        std::ostringstream &log) {
    clock_t end = clock();
    double elapsed_secs =
        static_cast<double>(end - begin)
        / static_cast<double>(CLOCKS_PER_SEC);

    std::time_t end_t = std::time(NULL);
    std::chrono::steady_clock::time_point end_elapsed =
        std::chrono::steady_clock::now();

    typedef std::chrono::duration<int, std::milli> millisecs_t;
    millisecs_t duration = std::chrono::duration_cast<millisecs_t>(
            end_elapsed - begin_elapsed);

    log << "Execution started at: " << std::ctime(&start_t);
    log << "Execution ended at:   " << std::ctime(&end_t);
    log << "Elapsed time: "
        << static_cast<double>(duration.count())/static_cast<double>(1000)
        << " Seconds.\n";
    log << "User CPU time: -> " << elapsed_secs << " seconds\n";
}
#endif

void
do_pgr_driving_many_to_dist(
        pgr_edge_t  *data_edges, size_t total_edges,
        int64_t *start_vertex, size_t s_len,
        double distance,
        bool directedFlag,
        bool equiCostFlag,
        General_path_element_t **return_tuples, size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
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
        std::vector<int64_t> start_vertices(start_vertex, start_vertex + s_len);

        auto vertices(pgrouting::extract_vertices(data_edges, total_edges));

#ifdef WITH_TIME
        clock_t begin;
        std::time_t start_t;
        std::chrono::steady_clock::time_point begin_elapsed;
#endif

        if (directedFlag) {
#ifdef WITH_TIME
            start_timing(start_t, begin_elapsed, begin);
            log << "*********Creating graph at time: " << std::ctime(&start_t)
                << "\n";
#endif
            pgrouting::DirectedGraph digraph(vertices, gType);
#ifdef WITH_TIME
            end_timing(start_t, begin_elapsed, begin, log);
#endif

#ifdef WITH_TIME
            start_timing(start_t, begin_elapsed, begin);
            log << "********Inserting edges at time: " << std::ctime(&start_t)
                << "\n";
#endif
            digraph.insert_edges(data_edges, total_edges, true);
#ifdef WITH_TIME
            end_timing(start_t, begin_elapsed, begin, log);
#endif

#ifdef WITH_TIME
            start_timing(start_t, begin_elapsed, begin);
            log << "*******Calling the C++ call to pgr_drivingDistance: "
                << std::ctime(&start_t) << "\n";
#endif
            paths = pgr_drivingDistance(
                    digraph, start_vertices, distance, equiCostFlag, log);
#ifdef WITH_TIME
            end_timing(start_t, begin_elapsed, begin, log);
#endif
        } else {
#ifdef WITH_TIME
            start_timing(start_t, begin_elapsed, begin);
            log << "******Creating graph at time: " << std::ctime(&start_t)
                << "\n";
#endif
            pgrouting::UndirectedGraph undigraph(vertices, gType);
#ifdef WITH_TIME
            end_timing(start_t, begin_elapsed, begin, log);
#endif

#ifdef WITH_TIME
            start_timing(start_t, begin_elapsed, begin);
            log << "*******Inserting edges at time: " << std::ctime(&start_t)
                << "\n";
#endif
            undigraph.insert_edges(data_edges, total_edges, true);
#ifdef WITH_TIME
            end_timing(start_t, begin_elapsed, begin, log);
#endif

#ifdef WITH_TIME
            start_timing(start_t, begin_elapsed, begin);
            log << "*******Calling the C++ call to pgr_drivingDistance: "
                << std::ctime(&start_t) << "\n";
#endif
            paths = pgr_drivingDistance(
                    undigraph, start_vertices, distance, equiCostFlag, log);
#ifdef WITH_TIME
            end_timing(start_t, begin_elapsed, begin, log);
#endif
        }

        size_t count(count_tuples(paths));


        if (count == 0) {
            log << "\nNo return values were found";
            *notice_msg = pgr_msg(log.str().c_str());
            return;
        }
        *return_tuples = pgr_alloc(count, (*return_tuples));
        auto trueCount(collapse_paths(return_tuples, paths));
        *return_count = trueCount;


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
