/*PGR-GNU*****************************************************************
File: LTDTree_driver.pp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Prakash Tiwari
Mail: 85prakash2017@gmail.com

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




#include <string>
#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include "cpp_common/identifiers.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "c_types/pgr_ltdtree_rt.h"
#include "drivers/dominator/lengauerTarjanDominatorTree_driver.h"
#include "dominator/pgr_lengauerTarjanDominatorTree_driver.hpp"


/************************************************************
  edges_sql TEXT
 ***********************************************************/
void
do_pgr_LTDTree(
        pgr_edge_t  *data_edges,
        size_t total_edges,
        int64_t root_vertex,
        pgr_ltdtree_rt **return_tuples,
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

        std::string logstr;

/***********************Working with graph**************************/

        graphType gType = DIRECTED;
        pgrouting::DirectedGraph digraph(gType);
        digraph.insert_edges(data_edges, total_edges);
        std::vector<pgr_ltdtree_rt> results;
        pgrouting::functions::Pgr_LTDTree<pgrouting::DirectedGraph> fn_LTDTree;
        results = fn_LTDTree.pgr_ltdtree(digraph, root_vertex);

        logstr += fn_LTDTree.get_log();
        log << logstr;

        auto count = results.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No result found";
            *log_msg = log.str().empty()?
                       *log_msg :
                       pgr_msg(log.str().c_str());
            *notice_msg = notice.str().empty()?
                          *notice_msg :
                          pgr_msg(notice.str().c_str());
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
