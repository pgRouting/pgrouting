/*PGR-GNU*****************************************************************
File: shortestPath_driver.cpp

Copyright (c) 2025-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Copying this file (or a derivative) within pgRouting code add the following:

Generated with Template by:
Copyright (c) 2025-2026 pgRouting developers
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

#include "drivers/shortestPath_driver.hpp"

#include <algorithm>
#include <sstream>
#include <deque>
#include <vector>
#include <limits>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <cstdint>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/combinations.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/to_postgres.hpp"

#include "dijkstra/dijkstra.hpp"
#include "bellman_ford/edwardMoore.hpp"
#include "bdDijkstra/bdDijkstra.hpp"
#include "dagShortestPath/dagShortestPath.hpp"
#include "bellman_ford/bellman_ford.hpp"
#include "max_flow/maxflow.hpp"
#include "traversal/binaryBreadthFirstSearch.hpp"
#include "yen/yen.hpp"

namespace {

void
post_process(std::deque<pgrouting::Path> &paths, bool only_cost, bool normal, size_t n_goals, bool global) {
    using pgrouting::Path;
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

namespace pgrouting {
namespace drivers {

void
do_shortestPath(
        const std::string &edges_sql,
        const std::string &combinations_sql,
        ArrayType *starts,
        ArrayType *ends,

        bool directed,
        bool only_cost,
        bool normal,

        int64_t n_goals,
        bool global,

        /* for ksp */
        int k,
        bool heap_paths,
        int64_t *start_vid,
        int64_t *end_vid,

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

        if ((which == KSP || which == OLDKSP) && k <= 0) {
            err << "Invalid value for k";
            return;
        }

        using pgrouting::pgget::get_edges;
        using pgrouting::utilities::get_combinations;
        using pgrouting::to_postgres::get_tuples;
        using pgrouting::UndirectedGraph;
        using pgrouting::DirectedGraph;

        using pgrouting::algorithms::dijkstra;
        using pgrouting::algorithms::bdDijkstra;
        using pgrouting::algorithms::edwardMoore;
        using pgrouting::algorithms::dagShortestPath;
        using pgrouting::functions::bellmanFord;
        using pgrouting::functions::edgeDisjoint;
        using functions::binaryBreadthFirstSearch;
        using pgrouting::algorithms::Yen;

        hint = combinations_sql;
        auto combinations = get_combinations(combinations_sql, starts, ends, normal, is_matrix);
        hint = "";


        if (which == OLDKSP && start_vid && end_vid) {
            combinations[*start_vid].insert(*end_vid);
        }

        size_t K{static_cast<size_t>(k)};

        if (combinations.empty() && !combinations_sql.empty()) {
            notice << "No (source, target) pairs found";
            log << combinations_sql;
            return;
        }

        hint = edges_sql;
        auto edges = get_edges(edges_sql, normal, false);
        hint = "";

        if (edges.empty()) {
            notice << "No edges found";
            log << edges_sql;
            return;
        }

        size_t n = n_goals <= 0? (std::numeric_limits<size_t>::max)() : static_cast<size_t>(n_goals);

        DirectedGraph digraph;
        UndirectedGraph undigraph;

        if (which == EDGEDISJOINT) {
            auto results = edgeDisjoint(edges, combinations, directed);
            return_count = get_tuples(results, edges, return_tuples);
            return;
        } else if (directed) {
            if (which == DAGSP) {
                digraph.insert_no_edge_cycle(edges);
            } else if (which == KSP || which == OLDKSP) {
                digraph.insert_min_edges_no_parallel(edges);
            } else {
                digraph.insert_edges(edges);
            }

            switch (which) {
                case DIJKSTRA: {
                    auto paths = dijkstra(digraph, combinations, only_cost, n);
                    post_process(paths, only_cost, normal, n, global);
                    return_count = get_tuples(paths, return_tuples);
                    break;
                    }
                case BDDIJKSTRA:
                    return_count = get_tuples(bdDijkstra(digraph, combinations, only_cost), return_tuples);
                    break;
                case EDWARDMOORE:
                    return_count = get_tuples(edwardMoore(digraph, combinations), return_tuples);
                    break;
                case DAGSP:
                    return_count = get_tuples(dagShortestPath(digraph, combinations, only_cost), return_tuples);
                    break;
                case BELLMANFORD:
                    return_count = get_tuples(bellmanFord(digraph, combinations, only_cost), return_tuples);
                    break;
                case BINARYBFS:
                    return_count = get_tuples(binaryBreadthFirstSearch(digraph, combinations), return_tuples);
                    break;
                case OLDKSP:
                case KSP:
                    return_count = get_tuples(Yen(digraph, combinations, K, heap_paths), return_tuples);
                    break;
                default:
                    err << "INTERNAL: wrong function call: " << which;
                    return;
            }
        } else {
            if (which == KSP || which == OLDKSP) {
                undigraph.insert_min_edges_no_parallel(edges);
            } else {
                undigraph.insert_edges(edges);
            }

            switch (which) {
                case DIJKSTRA: {
                    auto paths = dijkstra(undigraph, combinations, only_cost, n);
                    post_process(paths, only_cost, normal, n, global);
                    return_count = get_tuples(paths, return_tuples);
                    break;
                    }
                case BDDIJKSTRA:
                    return_count = get_tuples(bdDijkstra(undigraph, combinations, only_cost), return_tuples);
                    break;
                case EDWARDMOORE:
                    return_count = get_tuples(edwardMoore(undigraph, combinations), return_tuples);
                    break;
                case BELLMANFORD:
                    return_count = get_tuples(bellmanFord(undigraph, combinations, only_cost), return_tuples);
                    break;
                case BINARYBFS:
                    return_count = get_tuples(binaryBreadthFirstSearch(undigraph, combinations), return_tuples);
                    break;
                case OLDKSP:
                case KSP:
                    return_count = get_tuples(Yen(undigraph, combinations, K, heap_paths), return_tuples);
                    break;
                default:
                   err << "INTERNAL: wrong function call: " << which;
                   return;
            }
        }

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
