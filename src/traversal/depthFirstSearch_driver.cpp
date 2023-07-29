/*PGR-GNU*****************************************************************
File: depthFirstSearch_driver.cpp

Generated with Template by:
Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Ashish Kumar
Mail: ashishkr23438@gmail.com
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

#include "drivers/traversal/depthFirstSearch_driver.h"

#include <vector>
#include <algorithm>
#include <string>

#include "cpp_common/pggetdata.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "traversal/pgr_depthFirstSearch.hpp"

/** @file depthFirstSearch_driver.cpp
 * @brief Handles actual calling of function in the `pgr_depthFirstSearch.hpp` file.
 *
 */

namespace {

/** @brief Calls the main function defined in the C++ Header file.
 *
 * Also sorts the root vertices in an increasing order,
 * and removes the duplicated vertices. Then calls the function
 * defined in the C++ Header file - `pgr_depthFirstSearch.hpp`
 *
 * @param graph      the graph containing the edges
 * @param roots      the root vertices
 * @param directed   whether the graph is directed or undirected
 * @param max_depth  the maximum depth of traversal
 *
 * @returns results, when results are found
 */

template < class G >
std::vector < MST_rt >
pgr_depthFirstSearch(
        G &graph,
        std::vector < int64_t > roots,
        bool directed,
        int64_t max_depth) {
    std::sort(roots.begin(), roots.end());
    roots.erase(
            std::unique(roots.begin(), roots.end()),
            roots.end());

    pgrouting::functions::Pgr_depthFirstSearch < G > fn_depthFirstSearch;
    auto results = fn_depthFirstSearch.depthFirstSearch(graph, roots, directed, max_depth);
    return results;
}

}  // namespace

void
pgr_do_depthFirstSearch(
        char *edges_sql,

        int64_t *rootsArr,
        size_t size_rootsArr,

        bool directed,
        int64_t max_depth,

        MST_rt **return_tuples,
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
    char *hint;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);
        if (edges.size() == 0) {
            *notice_msg = pgr_msg("No edges found");
            *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
            return;
        }
        hint = nullptr;

        std::vector < int64_t > roots(rootsArr, rootsArr + size_rootsArr);

        std::vector < MST_rt > results;

        graphType gType = directed ? DIRECTED : UNDIRECTED;

        if (directed) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(edges);

            results = pgr_depthFirstSearch(
                    digraph,
                    roots,
                    directed,
                    max_depth);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(edges);

            results = pgr_depthFirstSearch(
                    undigraph,
                    roots,
                    directed,
                    max_depth);
        }

        auto count = results.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No traversal found";
            *log_msg = pgr_msg(notice.str().c_str());
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
    } catch (const std::string &ex) {
        *err_msg = pgr_msg(ex.c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
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
