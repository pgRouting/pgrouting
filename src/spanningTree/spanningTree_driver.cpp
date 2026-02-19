/*PGR-GNU*****************************************************************
File: spanningTree_driver.cpp

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2026 Celia Virginia Vergara Castillo
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

#include "drivers/spanningTree_driver.hpp"

#include <sstream>
#include <deque>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>

#include "c_types/mst_rt.h"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/to_postgres.hpp"
#include "withPoints/withPoints.hpp"

#include "spanningTree/prim.hpp"
#include "spanningTree/kruskal.hpp"
#include "breadthFirstSearch/breadthFirstSearch.hpp"
#include "traversal/depthFirstSearch.hpp"
#include "dijkstra/drivingDist.hpp"

namespace {

template <typename G, typename Func>
std::vector<MST_rt>
process(
        const std::vector<Edge_t> &edges,
        const std::set<int64_t> &roots,
        bool directed,
        int64_t max_depth,
        Func name) {
    G graph;
    graph.insert_edges(edges);
    return name(graph, roots, directed, max_depth);
}

template <typename Func>
std::vector<MST_rt>
process(
        const std::vector<Edge_t> &edges,
        const std::set<int64_t> &roots,
        int64_t max_depth,
        Func name) {
    using pgrouting::UndirectedGraph;
    UndirectedGraph graph;
    graph.insert_edges(edges);
    return name(graph, roots, max_depth);
}

template <typename G, typename Func>
std::deque<pgrouting::Path>
process(
        const std::vector<Edge_t> &edges,
        const std::set<int64_t> &roots,
        double distance,
        bool equicost,
        std::vector<std::map<int64_t, int64_t>> &depths,
        Func name) {
    G graph;
    graph.insert_edges(edges);
    return name(graph, roots, distance, equicost, depths, true);
}

template <typename Func>
std::vector<MST_rt>
process(
        const std::vector<Edge_t> &edges,
        const std::set<int64_t> &roots,
        double distance,
        Func name) {
    using pgrouting::UndirectedGraph;
    UndirectedGraph graph;
    graph.insert_edges(edges);
    return name(graph, roots, distance);
}

template <typename Func>
std::vector<MST_rt>
process(
        const std::vector<Edge_t> &edges,
        const Func name) {
    using pgrouting::UndirectedGraph;
    UndirectedGraph graph;
    graph.insert_edges(edges);
    return name(graph);
}

}  // namespace

namespace pgrouting {
namespace drivers {

void
do_spanningTree(
        const std::string &edges_sql,
        ArrayType* starts,

        bool directed,
        int64_t max_depth,
        double distance,
        bool equiCost,

        Which which,
        MST_rt* &return_tuples, size_t &return_count,
        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    std::string hint = "";

    try {
        if (edges_sql.empty()) {
            err << "Empty edges SQL";
            return;
        }

        using pgget::get_edges;
        using pgget::get_intSet;
        using to_postgres::get_tuples;
        using pgrouting::Path;
        using pgrouting::UndirectedGraph;
        using pgrouting::DirectedGraph;

        using functions::depthFirstSearch;
        using functions::breadthFirstSearch;
        using functions::drivingDistance;

        bool normal = true;
        auto roots = get_intSet(starts);

        std::string enop;
        std::string eofp;
        std::vector<Edge_t> edges;
        std::vector<Edge_t> edges_of_points;
        std::vector<Point_on_edge_t> points;

        hint = edges_sql;
        edges = get_edges(edges_sql, normal, false);
        hint = "";

        if (edges.empty()) {
            notice << "No edges found";
            log << edges_sql;

            if (which == BFS) return;

            auto emptyresults = pgrouting::only_root_result(roots);
            return_count = get_tuples(emptyresults, return_tuples);
            return;
        }
        hint = "";

        std::vector<MST_rt> results;
        std::deque<Path> paths;
        std::vector<std::map<int64_t, int64_t>> depths;

        if (directed) {
            switch (which) {
                case DFS:
                    results = process<DirectedGraph>(edges, roots, directed, max_depth,
                            &depthFirstSearch<DirectedGraph>);
                    break;
                case BFS:
                    results = process<DirectedGraph>(edges, roots, directed, max_depth,
                            &breadthFirstSearch<DirectedGraph>);
                    break;
                case DIJKSTRADD:
                    paths = process<DirectedGraph>(edges, roots, distance, equiCost, depths,
                            &drivingDistance<DirectedGraph>);
                    break;
                default:
                    err << "Unknown function on directed graph " << get_name(which);
                    return;
            }
        } else {
            switch (which) {
            case KRUSKAL:
                results = process(edges, &pgrouting::functions::kruskal);
                break;
            case KRUSKALBFS:
                results = process(edges, roots, max_depth, &pgrouting::functions::kruskalBFS);
                break;
            case KRUSKALDFS:
                results = process(edges, roots, max_depth, &pgrouting::functions::kruskalDFS);
                break;
            case KRUSKALDD:
                results = process(edges, roots, distance, &pgrouting::functions::kruskalDD);
                break;
            case PRIM:
                results = process(edges, &pgrouting::functions::prim);
                break;
            case PRIMBFS:
                results = process(edges, roots, max_depth, &pgrouting::functions::primBFS);
                break;
            case PRIMDFS:
                results = process(edges, roots, max_depth, &pgrouting::functions::primDFS);
                break;
            case PRIMDD:
                results = process(edges, roots, distance, &pgrouting::functions::primDD);
                break;
            case DFS:
                results = process<UndirectedGraph>(edges, roots, directed, max_depth,
                        &depthFirstSearch<UndirectedGraph>);
                break;
            case BFS:
                results = process<UndirectedGraph>(edges, roots, directed, max_depth,
                        &breadthFirstSearch<UndirectedGraph>);
                break;
            case DIJKSTRADD:
                paths = process<UndirectedGraph>(edges, roots, distance, equiCost, depths,
                        &drivingDistance<UndirectedGraph>);
                break;
            default:
                err << "Unknown function on undirected graph " << get_name(which);
                return;
            }
        }

        return_count = get_tuples(results, paths, depths, return_tuples);
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
