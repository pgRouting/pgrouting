/*PGR-GNU*****************************************************************
File: max_flow_driver.cpp

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
#include <string>

#include "max_flow/maxflow.hpp"

#include "cpp_common/combinations.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/alloc.hpp"

#include "c_types/ii_t_rt.h"

void
pgr_do_max_flow(
        char *edges_sql,
        char *combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        int algorithm,
        bool only_flow,

        Flow_t **return_tuples, size_t *return_count,
        char** log_msg,
        char** notice_msg,
        char **err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::utilities::get_combinations;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    char* hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        hint = combinations_sql;
        auto combinations = get_combinations(combinations_sql, starts, ends, true);
        hint = nullptr;

        if (combinations.empty() && combinations_sql) {
            *notice_msg = to_pg_msg("No (source, target) pairs found");
            *log_msg = to_pg_msg(combinations_sql);
            return;
        }

        std::set<int64_t> sources;
        std::set<int64_t> targets;
        for (const auto &c : combinations) {
            sources.insert(c.first);
            targets.insert(c.second.begin(), c.second.end());
        }

        std::set<int64_t> vertices(sources);
        vertices.insert(targets.begin(), targets.end());

        if (vertices.size() != (sources.size() + targets.size())) {
            *err_msg = to_pg_msg("A source found as sink");
            return;
        }

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_flow_edges(std::string(edges_sql));
        hint = nullptr;

        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = edges_sql;
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
            *err_msg = to_pg_msg(log);
            (*return_tuples) = NULL;
            (*return_count) = 0;
            return;
        }


        std::vector<Flow_t> flow_edges;

        if (only_flow) {
            Flow_t edge = {-1, -1, -1, max_flow, -1, 0, 0};
            flow_edges.push_back(edge);
        } else {
            flow_edges = digraph.get_flow_edges();
        }
        (*return_tuples) = pgr_alloc(flow_edges.size(), (*return_tuples));
        for (size_t i = 0; i < flow_edges.size(); ++i) {
            (*return_tuples)[i] = flow_edges[i];
        }
        *return_count = flow_edges.size();


        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
