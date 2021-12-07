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
#include "dijkstra/dijkstra.hpp"


namespace {

void
post_process(std::deque<Path> &paths, bool only_cost, bool normal, size_t n_goals, bool global) {
    paths.erase(std::remove_if(paths.begin(), paths.end(),
                [](const Path &p) {
                    return p.size() == 0;
                }),
                paths.end());
    using difference_type = std::deque<double>::difference_type;

    if (!normal) {
        for (auto &path : paths) path.reverse();
    }

    if (!only_cost) {
        for (auto &p : paths) {
            p.recalculate_agg_cost();
        }
    }

    if (n_goals != (std::numeric_limits<size_t>::max)()) {
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.end_id() < e2.end_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.start_id() < e2.start_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.tot_cost() < e2.tot_cost();
                });
        if (global && n_goals < paths.size()) {
            paths.erase(paths.begin() + static_cast<difference_type>(n_goals), paths.end());
        }
    } else {
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

        /* TODO
         * make a dijkstra, if there are no paths then return (DONE)
         * get set of paths that use a restriction (DOING)
         * use the trsp algorithm to get an alternate path
         */
        graphType gType = directed? DIRECTED: UNDIRECTED;

        auto combinations = total_combinations?
            pgrouting::utilities::get_combinations(combinations_arr, total_combinations)
            : pgrouting::utilities::get_combinations(starts_arr, size_starts_arr, ends_arr, size_ends_arr);

        size_t n = (std::numeric_limits<size_t>::max)();

        std::deque<Path> paths;
        if (directed) {
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_edges);
            paths = pgrouting::dijkstra(
                    digraph,
                    combinations,
                    false, n);
        } else {
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_edges);
            paths = pgrouting::dijkstra(
                    undigraph,
                    combinations,
                    false, n);
        }

        post_process(paths, false, true, n, false);
        size_t count(0);
        count = count_tuples(paths);
        if (ruleList.empty() || count == 0) {
            if (count == 0) {
                (*return_tuples) = NULL;
                (*return_count) = 0;
                notice <<
                    "No paths found";
                *log_msg = pgr_msg(notice.str().c_str());
                return;
            }
            (*return_tuples) = pgr_alloc(count, (*return_tuples));
            (*return_count) = (collapse_paths(return_tuples, paths));
            return;
        }

        std::map<int64_t, std::set<int64_t>> new_combinations;

        for (auto &p : paths) {
            std::deque<int64_t> edgesList(p.size());
            for (const auto &row : p) {
                edgesList.push_back(row.edge);
            }
            for (const auto &r : ruleList) {
                auto ptr = std::find(edgesList.begin(), edgesList.end(), r.precedences().front());
                if (ptr == edgesList.end()) continue;
                /*
                 * And edge on the begining of a rule list was found
                 * Checking if the complete rule applies
                 */

                /*
                 * Suppose the rule is used
                 */
                bool rule_breaker = true;
                for (const auto &e : r.precedences()) {
                    if (*ptr != e) {rule_breaker = false; break;}
                    ++ptr;
                }
                if (rule_breaker) {
                    new_combinations[p.start_id()].insert(p.end_id());
                    p.clear();
                }

            }
        }

        pgrouting::trsp::Pgr_trspHandler gdef(
                data_edges,
                total_edges,
                directed,
                ruleList);

        auto new_paths = gdef.process(new_combinations);
        paths.insert(paths.end(), new_paths.begin(), new_paths.end());
        post_process(paths, false, true, n, false);


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
