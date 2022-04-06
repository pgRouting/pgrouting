/*PGR-GNU*****************************************************************
File: trsp_driver.cpp

Copyright (c) 2017 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2022 Celia Virginia Vergara Castillo
* Added combinations functionality

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

#include <vector>
#include <cstdint>
#include <sstream>
#include <deque>
#include <algorithm>
#include <set>
#include <map>

#include "trsp/pgr_trspHandler.h"
#include "cpp_common/rule.h"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"
#include "c_types/restriction_t.h"

namespace {

void
post_process_trsp(std::deque<Path> &paths, bool sort) {
    paths.erase(std::remove_if(paths.begin(), paths.end(),
                [](const Path &p) {
                return p.size() == 0;
                }),
            paths.end());
    for (auto &p : paths) {
        p.recalculate_agg_cost();
    }

    if (sort) {
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                return e1.end_id() < e2.end_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                return e1.start_id() < e2.start_id();
                });
    }
}

}  // namespace

void
do_trsp(
        Edge_t *edges,
        size_t total_edges,

        Restriction_t *restrictions,
        size_t restrictions_size,

        int64_t  *start_vidsArr,
        size_t size_start_vidsArr,

        int64_t  *end_vidsArr,
        size_t size_end_vidsArr,

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
        pgassert(edges);
        pgassert(*return_tuples == NULL);
        pgassert(*return_count == 0);
        pgassert(*log_msg == NULL);
        pgassert(*notice_msg == NULL);
        pgassert(*err_msg == NULL);

        /*
         * When there are turn restrictions
         */
        std::vector<pgrouting::trsp::Rule> ruleList;
        for (size_t i = 0; i < restrictions_size; ++i) {
            if (restrictions[i].via_size == 0) continue;
            ruleList.push_back(pgrouting::trsp::Rule(*(restrictions + i)));
        }

        std::map<int64_t, std::set<int64_t>> combinations;

        for (size_t i = 0; i < size_start_vidsArr; ++i) {
            for (size_t j = 0; j < size_end_vidsArr; ++j) {
                combinations[start_vidsArr[i]].insert(end_vidsArr[j]);
            }
        }

        pgrouting::trsp::Pgr_trspHandler gdef(
                edges,
                total_edges,
                directed,
                ruleList);
        auto paths = gdef.process(combinations);
        post_process_trsp(paths, true);

        auto count = count_tuples(paths);

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
