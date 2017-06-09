/*PGR-GNU*****************************************************************
File: areaContraction_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Ankur Shukla
Mail: work.ankurshukla@gmail.com

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

#include "drivers/areaContraction/areaContraction_driver.h"

#include <string.h>
#include <sstream>
#include <deque>
#include <vector>

#include "../../contraction/src/pgr_contractionGraph.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"


/************************************************************
TEXT, --edges_sql
ANYARRAY, --border_nodes
directed BOOLEAN DEFAULT true,
 ***********************************************************/
void
do_pgr_areaContraction(
        pgr_edge_t  *data_edges,
        size_t data_edges_size,
        int64_t* borderVertices,
        size_t borderVertices_size,
        bool directed,
        contracted_rt **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(data_edges_size != 0);

/*****************put my code here****************/
        std::vector<pgr_edge_t> edges(data_edges, data_edges + data_edges_size);
        std::vector<int64_t> border(
                borderVertices,
                borderVertices + borderVertices_size);

        /*
         * Extracting vertices of the graph
         */
        Identifiers<int64_t> remaining_vertices;
        std::vector< pgrouting::CH_edge > shortcut_edges;

        graphType gType = directed? DIRECTED: UNDIRECTED;
/************************************************/
        pgassert(*err_msg == NULL);
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
