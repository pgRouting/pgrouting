/*PGR-GNU*****************************************************************
File: transitiveClosure_driver.cpp
Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org
Function's developer:
Copyright (c) 2019 Hang Wu
mail: nike0good@gmail.com
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

#include "drivers/transitiveClosure/transitiveClosure_driver.h"

#include <string.h>
#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>

#include "transitiveClosure/pgr_transitiveClosure.hpp"

#include "cpp_common/identifiers.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"



template < class G >
static
boost::adjacency_list <>
pgr_transitiveClosure(
        G &graph) {
    Pgr_transitiveClosure< G > fn_transitiveClosure;
    return fn_transitiveClosure.transitiveClosure(graph);
}


template <typename G>
static
void get_postgres_result(
        G &graph,
        transitiveClosure_rt **return_tuples,
        size_t *count) {
    boost::adjacency_list <> TC;
    TC = pgr_transitiveClosure(graph);
    (*count) = boost::num_vertices(TC);
    (*return_tuples) = pgr_alloc((*count), (*return_tuples));
    size_t sequence = 0;
    boost::graph_traits < boost::adjacency_list <> >::vertex_iterator i, end;
    boost::graph_traits < boost::adjacency_list <> >::adjacency_iterator ai, a_end;
    int ii = 1;
    for (boost::tie(i, end) = boost::vertices(TC); i != end; ++i) {
            auto u = *i;
            int64_t* target_array = NULL;
            int64_t uid = graph.graph[u].id;
            boost::tie(ai, a_end) = adjacent_vertices(*i, TC);
            auto adj_siz = a_end - ai;
            target_array = pgr_alloc(adj_siz , target_array);
            int count = 0;
            for (; ai != a_end; ++ai) {
                auto v = *ai;
                int64_t vid = graph.graph[v].id;
                target_array[count++] = vid;
            }

        (*return_tuples)[sequence] = {
            ii++,
            uid,
            target_array,
            count};

         ++sequence;
    }
}



/************************************************************
  edges_sql TEXT
 ***********************************************************/
void
do_pgr_transitiveClosure(
        pgr_edge_t  *data_edges,
        size_t total_edges,
        transitiveClosure_rt **return_tuples,
        size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    try {
        pgassert(total_edges != 0);
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        /*
         * Converting to C++ structures
         */
        std::vector<pgr_edge_t> edges(data_edges, data_edges + total_edges);


        graphType gType = DIRECTED;
        pgrouting::DirectedGraph digraph(gType);
        digraph.insert_edges(data_edges, total_edges);

        get_postgres_result(
                digraph,
                return_tuples,
                return_count);

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
