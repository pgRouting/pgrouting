/*PGR-GNU*****************************************************************
File: max_flow_many_to_many_driver.cpp

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

#include "drivers/max_flow/max_flow_driver.h"

#include <sstream>
#include <vector>
#include <set>

#include "max_flow/pgr_maxflow.hpp"

#include "cpp_common/combinations.h"
#include "cpp_common/pgget.hpp"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"

#include "c_types/ii_t_rt.h"

void
pgr_do_max_flow(
        char *edges_sql,
        char *combinations_sql,
        int64_t *sourceVertices, size_t sizeSourceVerticesArr,
        int64_t *sinkVertices, size_t sizeSinkVerticesArr,
        int algorithm,
        bool only_flow,

        Flow_t **return_tuples, size_t *return_count,
        char** log_msg,
        char** notice_msg,
        char **err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    char* hint;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_flow_edges(std::string(edges_sql));

        if (edges.empty()) {
            *notice_msg = pgr_msg("No edges found");
            *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
            return;
        }

        hint = combinations_sql;
        auto combinationsArr = combinations_sql?
            pgrouting::pgget::get_combinations(std::string(combinations_sql)) : std::vector<II_t_rt>();

        auto combinations = combinationsArr.empty()?
            pgrouting::utilities::get_combinations(sourceVertices, sizeSourceVerticesArr,
                    sinkVertices, sizeSinkVerticesArr)
            : pgrouting::utilities::get_combinations(combinationsArr);

        if (combinations.empty()) {
            *notice_msg = pgr_msg("No combinations found");
            *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
            return;
        }
        hint = nullptr;

        std::set<int64_t> sources;
        std::set<int64_t> targets;
        for (const auto &c : combinations) {
            sources.insert(c.first);
            for (const auto &t : c.second) {
                targets.insert(t);
            }
        }

        std::set<int64_t> vertices(sources);
        vertices.insert(targets.begin(), targets.end());

        if (vertices.size() != (sources.size() + targets.size())) {
            *err_msg = pgr_msg("A source found as sink");
            return;
        }

        pgrouting::graph::PgrFlowGraph digraph(
                edges, sources, targets, algorithm);
        // digraph.create_flow_graph(edges, sources, targets, algorithm);

        int64_t max_flow;
        if (algorithm == 1) {
            max_flow = digraph.push_relabel();
        } else if (algorithm == 3) {
            max_flow = digraph.edmonds_karp();
        } else if (algorithm == 2) {
            max_flow = digraph.boykov_kolmogorov();
        } else {
            log << "Unspecified algorithm!\n";
            *err_msg = pgr_msg(log.str().c_str());
            (*return_tuples) = NULL;
            (*return_count) = 0;
            return;
        }


        std::vector<Flow_t> flow_edges;

        if (only_flow) {
            Flow_t edge;
            edge.edge = -1;
            edge.source = -1;
            edge.target = -1;
            edge.flow = max_flow;
            edge.residual_capacity = -1;
            flow_edges.push_back(edge);
        } else {
            flow_edges = digraph.get_flow_edges();
        }
        (*return_tuples) = pgr_alloc(flow_edges.size(), (*return_tuples));
        for (size_t i = 0; i < flow_edges.size(); ++i) {
            (*return_tuples)[i] = flow_edges[i];
        }
        *return_count = flow_edges.size();


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
    } catch (const std::string &ex) {
        *err_msg = pgr_msg(ex.c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
