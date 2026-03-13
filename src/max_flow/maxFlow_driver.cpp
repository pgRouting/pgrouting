/*PGR-GNU*****************************************************************
File: maxFlow_driver.cpp

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
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

#include "drivers/maxFlow_driver.hpp"

#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <utility>

#include "max_flow/maxflow.hpp"

#include "c_types/flow_t.h"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/to_postgres.hpp"
#include "cpp_common/combinations.hpp"

namespace pgrouting {
namespace drivers {

void
do_maxFlow(
        const std::string &edges_sql,
        const std::string &combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        Which which,
        Flow_t* &return_tuples, size_t &return_count,
        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    std::string hint = "";

    try {
        if (edges_sql.empty()) {
            err << "Empty edges SQL";
            return;
        }

        using pgget::get_flow_edges;
        using utilities::get_combinations;
        using to_postgres::get_tuples;

        hint = combinations_sql;
        auto combinations = get_combinations(combinations_sql, starts, ends, true);
        hint = "";

        if (combinations.empty() && !combinations_sql.empty()) {
            notice << "No (source, target) pairs found";
            log << combinations_sql;
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
            err << "A source found as sink";
            return;
        }

        hint = edges_sql;
        auto edges = get_flow_edges(edges_sql);
        hint = "";

        if (edges.empty()) {
            notice << "No edges found";
            log << edges_sql;
            return;
        }


        pgrouting::graph::PgrFlowGraph digraph(edges, sources, targets, which);

        int64_t max_flow = 0;
        switch (which) {
            case MAXFLOW:
            case PUSHRELABEL:
                max_flow = digraph.push_relabel();
                break;
            case EDMONDSKARP:
                max_flow = digraph.edmonds_karp();
                break;
            case BOYKOV:
                max_flow = digraph.boykov_kolmogorov();
                break;
            default:
                err << "Unknown flow function" << get_name(which);
                return;
        }

        auto flow_edges = which == MAXFLOW? only_maxFlow_result(max_flow) : digraph.get_flow_edges();

        return_count = get_tuples(flow_edges, return_tuples);
    } catch (AssertFailedException &except) {
        err << except.what();
    } catch (const std::pair<std::string, std::string>& ex) {
        err << ex.first;
        log << ex.second;
    } catch (const std::string &ex) {
        err << ex;
        log << hint;
    } catch (std::exception &except) {
        err << except.what();
    } catch (...) {
        err << "Caught unknown exception!";
    }
}

}  // namespace drivers
}  // namespace pgrouting
