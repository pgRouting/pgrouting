/*PGR-GNU*****************************************************************
File: connectedComponentsV_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2017 Celia Virginia Vergara Castillo
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

#include "drivers/components/connectedComponentsV_driver.h"

#include <sstream>
#include <deque>
#include <vector>

#include "components/pgr_components.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"





/************************************************************
  TEXT,
    BIGINT,
    BIGINT,
 ***********************************************************/

template < class G >
static
std::vector<pgr_componentV_t>
pgr_connectedComponentsV(
        G &graph) {
    std::vector<pgr_componentV_t> results;
    Pgr_components< G > fn_components;
    return fn_components.connectedComponentsV(graph);
}


void
do_pgr_connectedComponentsV(
        pgr_edge_t  *data_edges,
        size_t total_edges,
        pgr_componentV_t **return_tuples,
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

        graphType gType = UNDIRECTED;

        std::vector<pgr_componentV_t> results;

        log << "Working with Undirected Graph\n";
        pgrouting::UndirectedGraph undigraph(gType);
        undigraph.insert_edges(data_edges, total_edges);
        results = pgr_connectedComponentsV(
                undigraph);

        auto count = results.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No paths found between start_vid and end_vid vertices";
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        for (size_t i = 0; i < count; i++) {
            *((*return_tuples) + i) = results[i];
        }
        size_t sequence = count;
        (*return_count) = sequence;

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

