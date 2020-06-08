/*PGR-GNU*****************************************************************
File: boyerMyrvold_driver.cpp

Generated with Template by:
Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Himanshu Raj
Mail: raj.himanshu2@gmail.com

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

// #include "drivers/breadthFirstSearch/breadthFirstSearch_driver.h"
#include "drivers/planarGraph/boyerMyrvold_driver.h"

#include <vector>
#include <algorithm>
#include <string>

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"
#include "c_types/pgr_edge_t.h"

#include "cpp_common/pgr_base_graph.hpp"
// #include "breadthFirstSearch/pgr_breadthFirstSearch.hpp"

void
do_pgr_boyerMyrvold(
                pgr_edge_t  *data_edges,
                size_t total_edges,
                // bool directed,

                pgr_edge_t **return_tuples,
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
        pgassert(total_edges != 0);

        // graphType gType = directed? DIRECTED: UNDIRECTED;

        // log << "Inserting vertices into a c++ vector structure";
        // std::vector<int64_t>
        //     start_vertices(start_vidsArr, start_vidsArr + size_start_vidsArr);

        std::vector<pgr_edge_t> results;
        std::string logstr;
        // if (directed) {
        //     log << "Working with directed Graph\n";
        //     pgrouting::DirectedGraph digraph(gType);
        //     digraph.insert_edges(data_edges, total_edges);
        //
        // } else {
        //     log << "Working with Undirected Graph\n";
        //     pgrouting::UndirectedGraph undigraph(gType);
        //     undigraph.insert_edges(data_edges, total_edges);
        // }

        auto count = results.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No traversal found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "\nConverting a set of traversals into the tuples";
        for (size_t i = 0; i < count; i++) {
            *((*return_tuples) + i) = results[i];
        }
        (*return_count) = count;

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
