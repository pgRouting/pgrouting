/*PGR-GNU*****************************************************************
File: astar_driver.cpp

Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2023 Celia Virginia Vergara Castillo
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#include "drivers/astar_driver.hpp"

#include <algorithm>
#include <sstream>
#include <deque>
#include <vector>
#include <string>
#include <utility>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/combinations.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/to_postgres.hpp"

#include "astar/astar.hpp"

namespace pgrouting {
namespace drivers {

void do_astar(
        const std::string &edges_sql,
        const std::string &combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        bool directed,
        bool only_cost,
        bool normal,

        int heuristic,
        double factor,
        double epsilon,

        Which which,
        bool &is_matrix,
        Path_rt* &return_tuples, size_t &return_count,
        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    using pgrouting::Path;

    std::string hint = "";

    try {
        if (edges_sql.empty()) {
            err << "Empty edges SQL";
            return;
        }
        if (heuristic > 5 || heuristic < 0) {
            err << "Unknown heuristic";
            log << "Valid values: 0~5";
            return;
        }
        if (factor <= 0) {
            err << "Factor value out of range";
            log << "Valid values: positive non zero";
            return;
        }
        if (epsilon < 1) {
            err << "Epsilon value out of range";
            log << "Valid values: 1 or greater than 1";
            return;
        }

        using pgrouting::pgget::get_edges_xy;
        using pgrouting::utilities::get_combinations;
        using pgrouting::to_postgres::get_tuples;
        using pgrouting::xyUndirectedGraph;
        using pgrouting::xyDirectedGraph;

        using pgrouting::algorithms::astar;

        hint = combinations_sql;
        auto combinations = get_combinations(combinations_sql, starts, ends, normal, is_matrix);
        hint = "";
        log << "is matrix" << is_matrix;

        if (combinations.empty() && !combinations_sql.empty()) {
            notice << "No (source, target) pairs found";
            log << combinations_sql;
            return;
        }

        hint = edges_sql;
        auto edges = get_edges_xy(edges_sql, normal);
        hint = "";

        if (edges.empty()) {
            notice << "No edges found";
            log << edges_sql;
            return;
        }

        xyDirectedGraph digraph;
        xyUndirectedGraph undigraph;

        std::deque<Path> paths;
        if (directed) {
            digraph.insert_edges(edges);
            switch (which) {
                case ASTAR:
                    paths = astar(digraph, combinations, heuristic, factor, epsilon, only_cost);
                    break;
                default:
                    err << "INTERNAL: wrong function call: " << which;
                    return;
            }
        } else {
            undigraph.insert_edges(edges);
            switch (which) {
                case ASTAR:
                    paths = astar(undigraph, combinations, heuristic, factor, epsilon, only_cost);
                    break;
                default:
                    err << "INTERNAL: wrong function call: " << which;
                    return;
            }
        }

        if (!normal) {
            for (auto &path : paths) path.reverse();
        }

        return_count = get_tuples(paths, return_tuples);

        if (return_count == 0) {
            log << "No paths found";
        }
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
