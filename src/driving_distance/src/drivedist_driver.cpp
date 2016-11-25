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

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif

#include "./drivedist_driver.h"
#include <sstream>
#include <deque>
#include <vector>

#include "./../../common/src/pgr_types.h"
#include "./../../common/src/pgr_assert.h"
#include "../../common/src/pgr_alloc.hpp"
#include "./../../dijkstra/src/pgr_dijkstra.hpp"




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

        if (directedFlag) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_edges);
            paths = pgr_drivingDistance(
                    digraph, start_vertices, distance, equiCostFlag);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_edges);
            paths = pgr_drivingDistance(
                    undigraph, start_vertices, distance, equiCostFlag);
        }

        size_t count(count_tuples(paths));


        if (count == 0) {
            *notice_msg = pgr_msg("No return values was found");
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
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
