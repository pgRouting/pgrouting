/*PGR-GNU*****************************************************************
File: sequentialVertexColoring_driver.cpp

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

#include "drivers/graphColoring/sequentialVertexColoring_driver.h"

#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#if 0
#include "graphColoring/pgr_sequentialVertexColoring.hpp"
#endif

/** @file sequentialVertexColoring_driver.cpp
 * @brief Handles actual calling of function in the `pgr_sequentialVertexColoring.hpp` file.
 *
 */

/***********************************************************************
 *
 *   pgr_sequentialVertexColoring(
 *       edges_sql TEXT
 *   );
 *
 ***********************************************************************/

/** @brief Calls the main function defined in the C++ Header file.
 *
 * @param graph      the graph containing the edges
 * @param log        stores the log message
 *
 * @returns results, when results are found
 */

#if 0
template <class G>
std::vector<pgr_vertex_color_rt>
pgr_sequentialVertexColoring(
        G &graph,
        std::string &log) {
    pgrouting::functions::Pgr_sequentialVertexColoring<G> fn_sequentialVertexColoring;
    auto results = fn_sequentialVertexColoring.sequentialVertexColoring(
            graph);
    log += fn_sequentialVertexColoring.get_log();
    return results;
}
#endif

/** @brief Performs exception handling and converts the results to postgres.
 *
 * It first asserts the variables, then builds the graph using the `data_edges`,
 * depending on whether the graph is directed or undirected. It also converts
 * the C types to the C++ types, such as the `rootsArr` to `roots`
 * vector and passes these variables to the template function `pgr_sequentialVertexColoring`
 * which calls the main function defined in the C++ Header file. It also does
 * exception handling.
 * 
 * @param data_edges     the set of edges from the SQL query
 * @param total_edges    the total number of edges in the SQL query
 * @param return_tuples  the rows in the result
 * @param return_count   the count of rows in the result
 * @param log_msg        stores the log message
 * @param notice_msg     stores the notice message
 * @param err_msg        stores the error message
 *
 * @returns void
 */

// TODO (ashish): Use the data_edges and total_edges parameter below.
//                Call the function in HPP file which calls the boost function.

void
do_pgr_sequentialVertexColoring(
        pgr_edge_t  *data_edges,
        size_t total_edges,

        pgr_vertex_color_rt **return_tuples,
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

        std::vector<pgr_vertex_color_rt> results;

        // string variable to store the log messages
        std::string logstr;

        log << logstr;

        // the count of rows in the result
        auto count = results.size();

        // returns directly in case of empty rows in the results
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
