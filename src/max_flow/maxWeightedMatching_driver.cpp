/*PGR-GNU*****************************************************************
File: maxWeightedMatching_driver.cpp

Copyright (c) 2025-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2026 Mayur Galhate
Mail: galhatemayur at gmail.com

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

#include "drivers/maxWeightedMatching_driver.hpp"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "c_types/iid_t_rt.h"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

#include "max_flow/maxWeightedMatching.hpp"


namespace pgrouting {
namespace drivers {

void do_maxWeightedMatching(
        const std::string &edges_sql,
        IID_t_rt* &return_tuples,
        size_t &return_count,
        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    using pgrouting::pgr_alloc;

    std::string hint = "";
    return_tuples = nullptr;
    return_count  = 0;

    try {
        if (edges_sql.empty()) {
            err << "Empty edges SQL";
            return;
        }

        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(edges_sql, false, false);

        if (edges.empty()) {
            notice << "No edges found";
            log << edges_sql;
            return;
        }
        hint = "";

        std::vector<IID_t_rt> matrix;
        matrix.reserve(edges.size());
        for (const auto &e : edges) {
            IID_t_rt row;
            row.from_vid = e.source;
            row.to_vid   = e.target;
            row.cost     = e.cost;
            matrix.push_back(row);
        }

        pgrouting::graph::UndirectedHasCostBG graph(matrix);

        auto matched_pairs = pgrouting::flow::maximumWeightedMatch(graph);

        auto count = matched_pairs.size();

        if (count == 0) {
            log << "No matching found";
            return;
        }

        double agg = 0.0;
        return_tuples = pgr_alloc(count, return_tuples);
        for (size_t i = 0; i < count; i++) {
            agg += matched_pairs[i].cost;
            matched_pairs[i].cost = agg;
            return_tuples[i]      = matched_pairs[i];
        }
        return_count = count;
    } catch (AssertFailedException &except) {
        err << except.what();
    } catch (const std::pair<std::string, std::string> &ex) {
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
