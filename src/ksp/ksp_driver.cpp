/*PGR-GNU*****************************************************************
File: ksp_driver.cpp

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


#include "drivers/yen/ksp_driver.h"

#include <sstream>
#include <deque>
#include <vector>

#include "yen/pgr_ksp.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "cpp_common/pgr_base_graph.hpp"



void  do_pgr_ksp(
        pgr_edge_t *data_edges,
        size_t total_edges,
        int64_t  start_vid,
        int64_t  end_vid,
        int k,
        bool directed,
        bool heap_paths,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    std::ostringstream err;
    std::ostringstream log;
    std::ostringstream notice;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path > paths;

        if (directed) {
            pgrouting::DirectedGraph digraph(gType);
            Pgr_ksp< pgrouting::DirectedGraph > fn_yen;
            digraph.insert_edges(data_edges, total_edges);
            paths = fn_yen.Yen(digraph, start_vid, end_vid, k, heap_paths);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            Pgr_ksp< pgrouting::UndirectedGraph > fn_yen;
            undigraph.insert_edges(data_edges, total_edges);
            paths = fn_yen.Yen(undigraph, start_vid, end_vid, k, heap_paths);
        }


        auto count(count_tuples(paths));

        if (!(count == 0)) {
            *return_tuples = NULL;
            *return_tuples = pgr_alloc(count, (*return_tuples));

            size_t sequence = 0;
            int route_id = 0;
            for (const auto &path : paths) {
                if (path.size() > 0)
                    path.get_pg_ksp_path(return_tuples, sequence, route_id);
                ++route_id;
            }
        }
        *return_count = count;

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
