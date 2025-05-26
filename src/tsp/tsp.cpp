/*PGR-GNU*****************************************************************
File: tsp.cpp

Copyright (c) 2021 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2021 Vicky Vergara
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

#include "tsp/tsp.hpp"

#include <utility>
#include <vector>
#include <deque>
#include <string>
#include <limits>

#include <boost/graph/metric_tsp_approx.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/version.hpp>

#include "cpp_common/identifiers.hpp"
#include "cpp_common/messages.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/interruption.hpp"
#include "cpp_common/undirectedHasCostBG.hpp"

#include "visitors/dijkstra_visitors.hpp"


namespace {

using TSP_graph = pgrouting::graph::UndirectedHasCostBG;
using V = TSP_graph::V;

std::deque<std::pair<int64_t, double>>
start_vid_end_vid_are_fixed(
        std::deque<std::pair<int64_t, double>> tsp_path,
        int64_t start_vid,
        int64_t end_vid) {
    pgassert(tsp_path[0].first == start_vid);
    /*
     * Where is 0?
     */
    auto where = std::find_if(tsp_path.begin(), tsp_path.end(),
            [&](std::pair<int64_t, double>& row){return row.first == 0;});

    /*
     * Move the 0 to be the second value
     * 5,2,1,8,7,10,11,16,6,9,12,17,15,13,14,0,3,4,5
     * to
     * 5 0 3 4 2 1 8 7 10 11 16 6 9 12 17 15 13 14 5
     */
    std::rotate(tsp_path.begin() + 1, where, tsp_path.end() - 1);

    /*
     * delete the 0
     * to
     * 5 3 4 2 1 8 7 10 11 16 6 9 12 17 15 13 14 5
     */
    tsp_path.erase(tsp_path.begin() + 1);

    /*
     * when the second value is not the end_vid, reverse the path
     * - it is the second to last, like the 14 above
     */
    if (tsp_path[0].first != end_vid) std::reverse(tsp_path.begin(), tsp_path.end());
    return tsp_path;
}

double
get_min_cost(
        V u,
        V v,
        TSP_graph &graph) {
    auto the_edge = boost::edge(u, v, graph.graph());
    /*
     * The edge exists, then return the weight it has
     */
    if (the_edge.second) {
        return get(boost::edge_weight, graph.graph())[the_edge.first];
    }

    std::vector<V> predecessors(num_vertices(graph.graph()));
    std::vector<double> distances(num_vertices(graph.graph()), std::numeric_limits<double>::infinity());
    bool found = false;

    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths(
                graph.graph(), u,
                boost::predecessor_map(&predecessors[0])
                .distance_map(&distances[0])
                .visitor(pgrouting::visitors::dijkstra_one_goal_visitor<V>(v)));
    } catch(pgrouting::found_goals &) {
        found = true;
    }
    if (!found) {
        pgassert(false);
        throw std::make_pair(std::string("INTERNAL: graph is incomplete 1"), std::string("Check graph before calling"));
    }

    double agg_cost(0);
    while (v != u) {
        agg_cost += distances[predecessors[v]];
        v = predecessors[v];
    }
    return agg_cost;
}

std::deque<std::pair<int64_t, double>>
eval_tour(TSP_graph& graph, const std::vector<V> &tsp_tour) {
    std::deque<std::pair<int64_t, double>> results;
    auto u = graph.graph().null_vertex();
    for (auto v : tsp_tour) {
        auto cost = (u == graph.graph().null_vertex()) ?
            0 :
            get_min_cost(u, v, graph);
        u = v;
        results.push_back(std::make_pair(graph.get_vertex_id(v), cost));
    }
    return results;
}

double
eval_tour(TSP_graph& graph, std::deque<std::pair<int64_t, double>>& tsp_tour) {
    std::deque<std::pair<int64_t, double>> results;
    auto u = graph.graph().null_vertex();
    double agg_cost(0);
    for (auto &node : tsp_tour) {
        auto v = graph.get_boost_vertex(node.first);
        auto cost = (u == graph.graph().null_vertex()) ?
            0 :
            get_min_cost(u, v, graph);
        u = v;
        node.second = cost;
        agg_cost += cost;
    }
    return agg_cost;
}


std::deque<std::pair<int64_t, double>>
tsp(TSP_graph& graph) {
    std::vector<V> tsp_path;

    CHECK_FOR_INTERRUPTS();
    try {
        boost::metric_tsp_approx_tour(
                graph.graph(),
                back_inserter(tsp_path));
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong while calling boost::metric_tsp_approx_tour"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }

    return eval_tour(graph, tsp_path);
}

std::deque<std::pair<int64_t, double>>
tsp(TSP_graph& graph, int64_t start_vid) {
    std::vector<V> tsp_path;
    /*
     * check that the start_vid
     */
    if (!graph.has_vertex(start_vid)) {
        throw std::make_pair(
                std::string("INTERNAL: Verify start_vid before calling"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }

    auto v = graph.get_boost_vertex(start_vid);

    CHECK_FOR_INTERRUPTS();
    try {
    boost::metric_tsp_approx_tour_from_vertex(
            graph.graph(),
            v,
            back_inserter(tsp_path));
    }  catch (...)  {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong while calling boost::metric_tsp_approx_tour_from_vertex"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }

    return eval_tour(graph, tsp_path);
}



/*
 * fixing (some) crossovers
 */
std::deque<std::pair<int64_t, double>>
crossover_optimize(TSP_graph& graph, std::deque<std::pair<int64_t, double>> result, size_t limit) {
    auto best_cost = eval_tour(graph, result);
    for (size_t i = 1; i < result.size() - limit; ++i) {
        for (size_t j = result.size() - limit; j >= i + 1; --j) {
            auto tmp = result;
            std::reverse(tmp.begin() + static_cast<ptrdiff_t>(i),  tmp.begin() + static_cast<ptrdiff_t>(j));
            auto new_cost = eval_tour(graph, tmp);
            if (new_cost < best_cost) {
                result = tmp;
                best_cost = new_cost;
            }
        }
    }
    return result;
}


/**
 * @param [in] start_vid user's start vertex identifier, 0 when not set
 * @param [in] end_vid user's end vertex identifier, 0 when not set
 * @param [in] max_cycles Upper limit of ccycles
 */
std::deque<std::pair<int64_t, double>>
do_tsp(
        TSP_graph& graph,
        int64_t start_vid,
        int64_t end_vid) {
    /*
     * check that the start_vid, and end_vid exist on the data
     */
    if (start_vid == 0) std::swap(start_vid, end_vid);

    if (start_vid != 0 && !graph.has_vertex(start_vid)) {
        throw std::make_pair(
                std::string("INTERNAL: start_id not found on data"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }

    if (end_vid !=0 && !graph.has_vertex(end_vid)) {
        throw std::make_pair(
                std::string("INTERNAL: end_id not found on data"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }

    std::deque<std::pair<int64_t, double>> result;

    /*
     * just get a tsp result
     */
    if (start_vid == 0) {
        return crossover_optimize(graph, tsp(graph), 1);
    }

    /*
     * the has a start value same as end_vid ignore end_vid
     */
    if ((end_vid == 0) || (start_vid == end_vid)) {
        return crossover_optimize(graph, tsp(graph, start_vid), 1);
    }

    /*
     * have start and end, a dummy node is needed
     */
    auto u = graph.get_boost_vertex(start_vid);
    auto v = graph.get_boost_vertex(end_vid);

    graph.insert_vertex(0);
    auto dummy_node = graph.get_boost_vertex(0);
    boost::add_edge(u, dummy_node, 0, graph.graph());
    boost::add_edge(v, dummy_node, 0, graph.graph());

    for (auto v1 : boost::make_iterator_range(boost::vertices(graph.graph()))) {
        if (v1 == u || v1 == v) continue;
        boost::add_edge(v1, dummy_node, std::numeric_limits<double>::infinity(), graph.graph());
    }

    result = tsp(graph, start_vid);
    result = start_vid_end_vid_are_fixed(result, start_vid, end_vid);

    return crossover_optimize(graph, result, 2);
}

}  // namespace

namespace pgrouting {

std::deque<std::pair<int64_t, double>>
tsp(TSP_graph& graph, int64_t sid, int64_t eid) {
    return do_tsp(graph, sid, eid);
}

}  // namespace pgrouting
