/*PGR-GNU*****************************************************************

File: trsp_core.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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


#include "drivers/trsp/trsp_driver.h"
#include <utility>
#include <vector>
#include <cstdint>
#include <sstream>
#include "trsp/pgr_trspHandler.h"
#include "trsp/rule.h"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"



void
do_trsp(
        pgr_edge_t *edges,
        size_t total_edges,

        Restriction_t *restrictions,
        size_t restrictions_size,

        int64_t start_vertex,
        int64_t end_vertex,

        bool directed,

        General_path_element_t **return_tuples,
        size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    try {
        pgassert(*return_tuples == NULL);
        pgassert(*return_count == 0);
        pgassert(*log_msg == NULL);
        pgassert(*notice_msg == NULL);
        pgassert(*err_msg == NULL);


        std::vector<pgrouting::trsp::Rule> ruleList;
        for (size_t i = 0; i < restrictions_size; ++ i) {
            ruleList.push_back(pgrouting::trsp::Rule(*(restrictions + i)));
        }



        std::deque<Path> paths;

        pgrouting::trsp::Pgr_trspHandler gdef(
                edges,
                total_edges,
                directed,
                ruleList);

        paths.push_back(gdef.process(
                    start_vertex,
                    end_vertex));


        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No paths found";
            *notice_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = collapse_paths(return_tuples, paths);


        return;
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

