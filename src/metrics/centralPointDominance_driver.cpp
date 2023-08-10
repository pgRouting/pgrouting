/*PGR-GNU*****************************************************************
File: centralPointDominance_driver.cpp
Generated with Template by:

Copyright (c) 2023 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2023 Sanskar Bhushan
Mail: sbdtu5498@gmail.com
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

#include "drivers/metrics/centralPointDominance_driver.h"

#include <sstream>
#include <vector>

#include "metrics/pgr_centralPointDominance.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "c_types/d_rt.h"
#include "c_types/edge_t.h"

/** @file centralPointDominance_driver.cpp
 * @brief Handles actual calling of function in the `pgr_centralPointDominance.hpp` file.
 *
 */

/***********************************************************************
 *
 *   pgr_centralPointDominance( edges_sql TEXT );
 *
 ***********************************************************************/

/** @brief Calls the main function defined in the C++ Header file.
 *
 * Also sorts the root vertices in an increasing order,
 * and removes the duplicated vertices. Then calls the function
 * defined in the C++ Header file - `pgr_centralPointDominance.hpp`
 *
 * @param graph      the graph containing the edges
 * @param log        stores the log message
 *
 * @returns central_point_dominance, when central_point_dominance are found
 */

template < class G >
D_rt
pgr_centralPointDominance(G &graph) {
    pgrouting::centrality::PgrCentralityGraph < G > fn_centralPointDominance;
    auto central_point_dominance = fn_centralPointDominance.centralPointDominance(graph);
    return central_point_dominance;
}

/** @brief Performs exception handling and converts the central_point_dominance to postgres.
 *
 * @pre log_msg is empty
 * @pre notice_msg is empty
 * @pre err_msg is empty
 * @pre return_tuples is empty
 * @pre return_count is 0
 *
 * It builds the undirected graph using the `data_edges` variable.  
 * Then, it passes the required variables to the template function
 * `pgr_centralPointDominance` which calls the main function
 * defined in the C++ Header file. It also does exception handling.
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

void
do_pgr_centralPointDominance(
        Edge_t  *data_edges,
        size_t total_edges,

        bool directed,

        D_rt **return_tuples,
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

        D_rt central_point_dominance;

        if (directed) {
            pgrouting::centrality::PgrCentralityGraph<pgrouting::DirectedGraph> G(data_edges, total_edges);
            central_point_dominance = G.get_central_point_dominance();
        } else {
            pgrouting::centrality::PgrCentralityGraph<pgrouting::UndirectedGraph> G(data_edges, total_edges);
            central_point_dominance = G.get_central_point_dominance();
        }

        auto count = 1;

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No traversal found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(1, (*return_tuples));
        (*return_tuples)[0] = central_point_dominance;

        *return_count = 1;

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
