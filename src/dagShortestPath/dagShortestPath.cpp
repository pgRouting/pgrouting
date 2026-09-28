/*PGR-GNU*****************************************************************
File: dagShortestPath.hpp

Copyright (c) 2013-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Sourabh Garg
sourabh.garg.mat@gmail.com

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

#include "dagShortestPath/dagShortestPath.hpp"

#include <deque>
#include <vector>

#include "visitors/dijkstra_visitors.hpp"

namespace {

using G = pgrouting::DirectedGraph;
using V = typename G::V;
using Path = pgrouting::Path;

/** DAG  1 source to many targets */
void
dag_1_to_many(
        G &graph,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        V source,
        const std::set<V> &targets,
        size_t n_goals) {
    CHECK_FOR_INTERRUPTS();
    std::set<V> goals_found;
    try {
        boost::dag_shortest_paths(graph.graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&G::G_T_E::cost, graph.graph))
                .distance_map(&distances[0])
                .distance_inf(std::numeric_limits<double>::infinity())
                .visitor(pgrouting::visitors::dijkstra_many_goal_visitor<V>(targets, n_goals, goals_found)));
    } catch(pgrouting::found_goals &) {
        return;
    } catch (boost::exception const& ex) {
        (void)ex;
        throw;
    } catch (std::exception &e) {
        (void)e;
        throw;
    } catch (...) {
        throw;
    }
}

// used when multiple goals
std::deque<Path>
get_paths(
        const G &graph,
        const std::vector<V> &predecessors,
        const std::vector<double> &distances,
        V source,
        const std::set<V> &targets,
        bool only_cost) {
    std::deque<Path> paths;
    for (const auto target : targets) {
        paths.push_back(Path(
                    graph,
                    source, target,
                    predecessors, distances,
                    only_cost, true));
    }
    return paths;
}

}  // namespace

namespace pgrouting {

/** dag 1 to many */
std::deque<Path>
Pgr_dag::dag(
        G &graph,
        int64_t start_vertex,
        const std::set<int64_t> &end_vertex,
        bool only_cost) {
    std::deque<Path> paths;

    /* precondition */
    if (!graph.has_vertex(start_vertex)) return paths;

    /* adjust predecessors and distances vectors */
    clear();
    size_t n_goals = (std::numeric_limits<size_t>::max)();
    predecessors.resize(graph.num_vertices());
    distances.resize(
            graph.num_vertices(),
            std::numeric_limits<double>::infinity());


    auto v_source(graph.get_V(start_vertex));

    std::set<V> v_targets;
    for (const auto &vertex : end_vertex) {
        if (graph.has_vertex(vertex)) v_targets.insert(graph.get_V(vertex));
    }
    if (v_targets.empty()) return paths;

    dag_1_to_many(graph, predecessors, distances, v_source, v_targets, n_goals);
    paths = ::get_paths(graph, predecessors, distances, v_source, v_targets, only_cost);

    std::stable_sort(paths.begin(), paths.end(),
            [](const Path &e1, const Path &e2)->bool {
            return e1.end_id() < e2.end_id();
            });

    return paths;
}


/** combinations */
std::deque<Path> Pgr_dag::dag(
        G &graph,
        const std::map<int64_t, std::set<int64_t>> &combinations,
        bool only_cost) {
    std::deque<Path> paths;

    for (const auto &c : combinations) {
        auto result_paths = dag(graph, c.first, c.second, only_cost);
        paths.insert(
                paths.end(),
                std::make_move_iterator(result_paths.begin()),
                std::make_move_iterator(result_paths.end()));
    }

    return paths;
}

//@}


void Pgr_dag::clear() {
    predecessors.clear();
    distances.clear();
}




namespace algorithms {

std::deque<pgrouting::Path>
    dagShortestPath(
            pgrouting::DirectedGraph &graph,
            std::map<int64_t, std::set<int64_t>> &combinations,
            bool only_cost) {
        pgrouting::Pgr_dag fn_dag;
        auto paths = fn_dag.dag(graph, combinations, only_cost);
        return paths;
    }
}  // namespace algorithms
}  // namespace pgrouting

