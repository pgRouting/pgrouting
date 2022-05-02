/*PGR-GNU*****************************************************************
File: edge_disjoint_paths_many_to_many_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#include "drivers/max_flow/edge_disjoint_paths_driver.h"

#include <sstream>
#include <vector>
#include <set>

#include "max_flow/pgr_maxflow.hpp"

#include "cpp_common/combinations.h"
#include "cpp_common/identifiers.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "c_types/ii_t_rt.h"


static
std::vector<Path_rt>
single_execution(
        std::vector<Edge_t> edges,
        int64_t source,
        int64_t target,
        bool directed) {
    std::set<int64_t> set_source_vertices;
    std::set<int64_t> set_sink_vertices;
    set_source_vertices.insert(source);
    set_sink_vertices.insert(target);
    pgrouting::graph::PgrFlowGraph G(
            edges,
            set_source_vertices,
            set_sink_vertices, directed);

    /*
     * boykov_kolmogorov is only for directed graphs
     */
    return G.edge_disjoint_paths();
}

void
do_pgr_edge_disjoint_paths(
    Edge_t *data_edges,
    size_t total_edges,
    II_t_rt *combinations,
    size_t total_combinations,
    int64_t *sources,
    size_t size_source_verticesArr,
    int64_t *sinks,
    size_t size_sink_verticesArr,
    bool directed,
    Path_rt **return_tuples,
    size_t *return_count,
    char** log_msg,
    char** notice_msg,
    char **err_msg) {
    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(data_edges);
        pgassert(total_edges != 0);
        pgassert((sources && sinks) || combinations);
        pgassert((size_source_verticesArr && size_sink_verticesArr) || total_combinations);

        auto combinations_data = total_combinations?
            pgrouting::utilities::get_combinations(combinations, total_combinations)
            : pgrouting::utilities::get_combinations(sources, size_source_verticesArr, sinks, size_sink_verticesArr);

        std::vector<Edge_t> edges(
                data_edges, data_edges + total_edges);

        std::vector<Path_rt> paths;
        for (const auto &c : combinations_data) {
            for (const auto &t : c.second) {
                /*
                 * a source can not be a sink
                 * aka there is no path
                 */
                if (c.first == t) continue;
                auto path = single_execution(
                        edges,
                        c.first,
                        t,
                        directed);
                paths.insert(paths.end(), path.begin(), path.end());
            }
        }

        if (paths.empty()) {
            *return_tuples = nullptr;
            *return_count = 0;
            return;
        }

        /*
         * Initializing the cost
         */
        for (auto &r : paths) {
            r.agg_cost = r.cost = 0;
        }

        /*
         * Calculating the cost
         */
        auto found = paths.size();
        for (const auto &e : edges) {
            for (auto &r : paths) {
                if (r.edge == e.id) {
                    r.cost = (r.node == e.source) ?
                        e.cost : e.reverse_cost;
                    --found;
                }
            }
            if (found == 0) break;
        }

        /*
         * Calculating the agg_cost
         */
        auto prev = paths[0];
        for (auto &r : paths) {
            if (r.seq == 1) {
                r.agg_cost = 0;
            } else {
                r.agg_cost = prev.agg_cost + prev.cost;
            }
            prev = r;
        }

        (*return_tuples) = pgr_alloc(paths.size(), (*return_tuples));
        for (size_t i = 0; i < paths.size(); ++i) {
            (*return_tuples)[i] = paths[i];
        }
        *return_count = paths.size();


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
