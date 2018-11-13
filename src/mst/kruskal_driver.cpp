/*PGR-GNU*****************************************************************
File: kruskal_driver.cpp

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

#include "drivers/mst/kruskal_driver.h"

#include <sstream>
#include <deque>
#include <vector>

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "mst/pgr_kruskal.hpp"
#include "mst/details.hpp"


void
do_pgr_kruskal(
        pgr_edge_t  *data_edges,
        size_t total_edges,

        int64_t *rootsArr,
        size_t size_rootsArr,

        int order_by,
        int64_t max_depth,
        double distance,

        pgr_mst_rt **return_tuples,
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

        graphType gType = UNDIRECTED;

        std::vector<int64_t> roots(rootsArr, rootsArr + size_rootsArr);
        std::sort(roots.begin(), roots.end());
        roots.erase(
                std::unique(roots.begin(), roots.end()),
                roots.end());
        roots.erase(
                std::remove(roots.begin(), roots.end(), 0),
                roots.end());

        std::vector<pgr_mst_rt> results;

        if (total_edges == 0) {
            results = pgrouting::get_no_edge_graph_result(roots);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_edges);
            log << "The Graph:\n" << undigraph;
            pgrouting::functions::Pgr_kruskal<pgrouting::UndirectedGraph> kruskal;
            results = kruskal(undigraph, roots, order_by, max_depth, distance);
        }

        auto count = results.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No spanning tree found";
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
