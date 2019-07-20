/*PGR-GNU*****************************************************************
File: johnson_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#include "drivers/allpairs/johnson_driver.h"

#include <sstream>
#include <deque>
#include <vector>

#include "allpairs/pgr_allpairs.hpp"

#include "cpp_common/pgr_assert.h"


void
do_pgr_johnson(
        pgr_edge_t  *data_edges,
        size_t total_tuples,
        bool directed,
        Matrix_cell_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        if (directed) {
            log << "Processing Directed graph\n";
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_tuples);
            pgr_johnson(digraph, *return_count, return_tuples);
        } else {
            log << "Processing Undirected graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_tuples);
            pgr_johnson(undigraph, *return_count, return_tuples);
        }


        if (*return_count == 0) {
            log <<  "No result generated, report this error\n";
            *log_msg = pgr_msg(err.str().c_str());
            *return_tuples = NULL;
            *return_count = 0;
            return;
        }


        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
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
