/*PGR-GNU*****************************************************************

File: trsp_driver.cpp

Copyright (c) 2017 pgRouting developers
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
#include <algorithm>
#include "trsp/pgr_trspHandler.h"
#include "cpp_common/rule.h"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/combinations.h"
#include "c_types/restriction_t.h"
#include "c_types/ii_t_rt.h"
#include "drivers/dijkstra/dijkstra_driver.h"



void
do_trsp(
        Edge_t *data_edges,
        size_t total_edges,

        Restriction_t *restrictions,
        size_t restrictions_size,

        II_t_rt  *combinations_arr,
        size_t total_combinations,

        int64_t  *starts_arr,
        size_t size_starts_arr,

        int64_t  *ends_arr,
        size_t size_ends_arr,

        bool directed,

        Path_rt **return_tuples,
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
        for (size_t i = 0; i < restrictions_size; ++i) {
            ruleList.push_back(pgrouting::trsp::Rule(*(restrictions + i)));
        }

        if (ruleList.empty()) {
                    /* TODO if all the edges on the ruleList
                     * are not on the edges set, then the dijkstra
                     * has to be done also */
            do_dijkstra(
                    data_edges,
                    total_edges,

                    combinations_arr,
                    total_combinations,
                    starts_arr,
                    size_starts_arr,
                    ends_arr,
                    size_ends_arr,

                    directed,
                    false,  // only_cost,
                    true,   // normal,
                    0,      //n_goals,
                    false,  //global,

                    return_tuples,
                    return_count,
                    log_msg,
                    notice_msg,
                    err_msg);
            return;
        }

        auto combinations = total_combinations?
            pgrouting::utilities::get_combinations(combinations_arr, total_combinations)
            : pgrouting::utilities::get_combinations(starts_arr, size_starts_arr, ends_arr, size_ends_arr);

        pgrouting::trsp::Pgr_trspHandler gdef(
                data_edges,
                total_edges,
                directed,
                ruleList);

        auto paths = gdef.process(combinations);


        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = collapse_paths(return_tuples, paths);


        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());

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

