/*PGR-GNU*****************************************************************
File: randomSpanningTree_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2018 Aditya Pratap Singh
Mail: adityapratap.singh28@gmail.com
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

#include "drivers/spanningTree/randomSpanningTree_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <string>


#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"
#include "spanningTree/randomSpanningTree.hpp"

template < class G >
static
std::vector<SpanTree_rt>
pgr_randomSpanningTree(
        G &graph,
        int64_t root_vertex ) {
    Pgr_randomSpanningTree< G > fn_randomSpanningTree;
    return fn_randomSpanningTree.randomSpanningTree(graph, root_vertex);
}


void
pgr_do_randomSpanningTree(
        char *edges_sql,
        int64_t root_vertex,
        bool directed,
        SpanTree_rt **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    char *hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);

        if (edges.empty()) {
            *notice_msg = pgr_msg("No edges found");
            *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str());
            return;
        }
        hint = nullptr;



        std::vector<SpanTree_rt> results;

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(directed);
            digraph.insert_edges(data_edges, total_edges);
            results = pgr_randomSpanningTree(
                    digraph,
                    root_vertex);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(directed);
            undigraph.insert_edges(data_edges, total_edges);
            results = pgr_randomSpanningTree(
                    undigraph,
                    root_vertex);
        }

        auto count = results.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No paths found";
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        for (size_t i = 0; i < count; i++) {
            *((*return_tuples) + i) = results[i];
        }
        (*return_count) = count;

        pgassert(*err_msg == NULL);
        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str());
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str());
        *log_msg = pgr_msg(log.str());
    } catch (const std::string &ex) {
        *err_msg = pgr_msg(ex);
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str());
        *log_msg = pgr_msg(log.str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str());
        *log_msg = pgr_msg(log.str());
    }
}
