/*PGR-GNU*****************************************************************

Copyright (c) 2021 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2021 Vicky Vergara
Mail: vicky@georepublic.de

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

#include "cpp_common/identifiers.hpp"
#include "cpp_common/pgr_messages.h"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/interruption.h"

#include "visitors/dijkstra_one_goal_visitor.hpp"


namespace {

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
        pgrouting::algorithm::TSP::V u,
        pgrouting::algorithm::TSP::V v,
        pgrouting::algorithm::TSP::TSP_Graph &graph) {
    using V = pgrouting::algorithm::TSP::V;

    auto the_edge = boost::edge(u, v, graph);
    /*
     * The edge exists, then return the weight it has
     */
    if (the_edge.second) {
        return get(boost::edge_weight, graph)[the_edge.first];
    }

    std::vector<V> predecessors(num_vertices(graph));
    std::vector<double> distances(num_vertices(graph), std::numeric_limits<double>::infinity());
    bool found = false;

    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths(
                graph, u,
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
    /*
     * Saving the calculated value is faster than running the dijkstra again
     */
    boost::add_edge(u, v, agg_cost, graph);
    return agg_cost;
}


}  // namespace


namespace pgrouting {
namespace algorithm {

std::deque<std::pair<int64_t, double>>
TSP::tsp() {
    std::vector<V> tsp_path;

    CHECK_FOR_INTERRUPTS();
    try {
        boost::metric_tsp_approx_tour(
                graph,
                back_inserter(tsp_path));
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong while calling boost::metric_tsp_approx_tour"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }
    pgassert(tsp_path.size() == num_vertices(graph) + 1);

    return eval_tour(tsp_path);
}

std::deque<std::pair<int64_t, double>>
TSP::tsp(int64_t start_vid) {
    std::vector<V> tsp_path;
    /*
     * check that the start_vid
     */
    if (id_to_V.find(start_vid) == id_to_V.end()) {
        throw std::make_pair(
                std::string("INTERNAL: Verify start_vid before calling"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }

    auto v = get_boost_vertex(start_vid);

    CHECK_FOR_INTERRUPTS();
    try {
    boost::metric_tsp_approx_tour_from_vertex(
            graph,
            v,
            back_inserter(tsp_path));
    }  catch (...)  {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong while calling boost::metric_tsp_approx_tour_from_vertex"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }
    pgassert(tsp_path.size() == num_vertices(graph) + 1);

    return eval_tour(tsp_path);
}



/**
 * @param [in] start_vid user's start vertex identifier, 0 when not set
 * @param [in] end_vid user's end vertex identifier, 0 when not set
 * @param [in] max_cycles Upper limit of ccycles
 */
std::deque<std::pair<int64_t, double>>
TSP::tsp(
        int64_t start_vid,
        int64_t end_vid,
        int max_cycles) {
    std::deque<std::pair<int64_t, double>> result;

    if (start_vid == 0) std::swap(start_vid, end_vid);

    /*
     * just get a tsp result
     */
    if (start_vid == 0) return crossover_optimize(tsp(), 1, max_cycles);

    /*
     * the has a start value same as end_vid ignore end_vid
     */
    if ((end_vid == 0) || (start_vid == end_vid)) return crossover_optimize(tsp(start_vid), 1, max_cycles);

    /*
     * check that the start_vid, and end_vid exist on the data
     */
    if (id_to_V.find(start_vid) == id_to_V.end()) {
        throw std::make_pair(
                std::string("INTERNAL: start_id not found on data"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }

    if (id_to_V.find(end_vid) == id_to_V.end()) {
        throw std::make_pair(
                std::string("INTERNAL: end_id not found on data"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }

    auto u = get_boost_vertex(start_vid);
    auto v = get_boost_vertex(end_vid);

    auto dummy_node = add_vertex(num_vertices(graph), graph);
    id_to_V.insert(std::make_pair(0, dummy_node));
    V_to_id.insert(std::make_pair(dummy_node, 0));
    boost::add_edge(u, dummy_node, 0, graph);
    boost::add_edge(v, dummy_node, 0, graph);

    for (auto v1 : boost::make_iterator_range(boost::vertices(graph))) {
        if (v1 == u || v1 == v) continue;
        boost::add_edge(v1, dummy_node, std::numeric_limits<double>::infinity(), graph);
    }

    result = tsp(start_vid);
    result = start_vid_end_vid_are_fixed(result, start_vid, end_vid);

    return crossover_optimize(result, 2, max_cycles);
}


/*
 * fixing (some) crossovers
 */
std::deque<std::pair<int64_t, double>>
TSP::crossover_optimize(std::deque<std::pair<int64_t, double>> result, size_t limit, int cycles) {
    auto best_cost = eval_tour(result);
    for (int k = 0; k < cycles; ++k) {
        bool change(false);
        for (size_t i = 1; i < result.size() - limit; ++i) {
            for (size_t j = result.size() - limit; j >=i + 1; --j) {
                auto tmp = result;
                std::reverse(tmp.begin() + static_cast<ptrdiff_t>(i),  tmp.begin() + static_cast<ptrdiff_t>(j));
                auto new_cost = eval_tour(tmp);
                if (new_cost < best_cost) {
#ifdef DEBUG_TSP
                    log << "\n" << k << ", " << i << "," << j << "\n";
                    for (const auto e : tmp) {
                        log << e.first << "->";
                    }
                    log <<" cost = " << new_cost;
#endif
                    result = tmp;
                    best_cost = new_cost;
                    change = true;
                }
            }
        }
        if (!change) break;
    }
    return result;
}



TSP::TSP(
    IID_t_rt *distances,
    size_t total_distances, bool) {
    /*
     * Inserting vertices
     */
    Identifiers<int64_t> ids;
    for (size_t i = 0; i < total_distances; ++i) {
        ids += distances[i].from_vid;
        ids += distances[i].to_vid;
        /*
         * Its undirected graph:
         * keeping from_vid < to_vid
         */
        if (distances[i].to_vid > distances[i].from_vid) {
            std::swap(distances[i].to_vid, distances[i].from_vid);
        }
    }

    size_t i {0};
    for (const auto id : ids) {
        auto v = add_vertex(i, graph);
        id_to_V.insert(std::make_pair(id, v));
        V_to_id.insert(std::make_pair(v, id));
        ++i;
    }

    /*
     * Inserting edges
     */
    for (size_t i = 0; i < total_distances; ++i) {
        auto edge = distances[i];
        /*
         * skip loops
         */
        if (edge.from_vid == edge.to_vid) continue;
        auto v1 = get_boost_vertex(edge.from_vid);
        auto v2 = get_boost_vertex(edge.to_vid);
        auto e_exists = boost::edge(v1, v2, graph);
        if (e_exists.second) {
            auto weight = get(boost::edge_weight_t(), graph, e_exists.first);
            /*
             * skip duplicated edges with less cost
             */
            if (weight < edge.cost) continue;
            if (edge.cost < weight) {
                /*
                 * substitute edge with new lesser cost found
                 */
                boost::put(boost::edge_weight_t(), graph, e_exists.first, edge.cost);
            }
            continue;
        }

        auto add_result = boost::add_edge(v1, v2, edge.cost, graph);
        if (!add_result.second) {
            throw std::make_pair(
                    std::string("INTERNAL: something went wrong adding and edge\n"),
                    std::string(__PGR_PRETTY_FUNCTION__));
        }
    }

    /*
     * Check data validity
     * - One component
     * Not checking triangle inequality
     */
    std::vector<V> components(boost::num_vertices(graph));
    CHECK_FOR_INTERRUPTS();
    try {
        if (boost::connected_components(graph, &components[0]) > 1) {
            throw std::make_pair(
                    std::string("Graph is not fully connected"),
                    std::string("Check graph before calling"));
        }
    } catch (...) {
        throw;
    }
}


/**
 * The postgres user might inadvertently give duplicate points with the same id
 * the approximation is quite right, for example
 * 1, 3.5, 1
 * 1, 3.499999999999 0.9999999
 * the approximation is quite wrong, for example
 * 2 , 3.5 1
 * 2 , 3.6 1
 * but when the remove_duplicates flag is on, keep only the first row that has the same id
 */
TSP::TSP(Coordinate_t *coordinates,
        size_t total_coordinates,
        bool ) {
    log << "before total_coordinates" << total_coordinates;

    /*
     * keeping the vertex identifiers
     */
    Identifiers<int64_t> ids;
    for (size_t i = 0; i < total_coordinates; ++i) {
        ids += coordinates[i].id;
    }

    /*
     * Inserting vertices
     */
    size_t i{0};
    for (const auto id : ids) {
        auto v = add_vertex(i, graph);
        id_to_V.insert(std::make_pair(id, v));
        V_to_id.insert(std::make_pair(v, id));
        ++i;
    }

    /*
     * Inserting edges
     */
    for (size_t i = 0; i < total_coordinates; ++i) {
        auto u = get_boost_vertex(coordinates[i].id);
        auto ux = coordinates[i].x;
        auto uy = coordinates[i].y;

        /*
         *  undirected, so only need traverse higher coordinates for connections
         */
        for (size_t j = i + 1; j < total_coordinates; ++j) {
            auto v = get_boost_vertex(coordinates[j].id);

            /*
             * ignoring duplicated coordinates
             */
            if (boost::edge(u, v, graph).second) continue;

            auto vx = coordinates[j].x;
            auto vy = coordinates[j].y;

            auto const dx = ux - vx;
            auto const dy = uy - vy;

            /*
             * weight is euclidean distance
             */
            auto add_result = boost::add_edge(u, v, sqrt(dx * dx + dy * dy), graph);
            if (!add_result.second) {
                throw std::make_pair(
                        std::string("INTERNAL: something went wrong adding and edge\n"),
                        std::string(__PGR_PRETTY_FUNCTION__));
            }
        }
    }
}




std::deque<std::pair<int64_t, double>>
TSP::eval_tour(const std::vector<V> &tsp_tour) {
    std::deque<std::pair<int64_t, double>> results;
    auto u = graph.null_vertex();
    for (auto v : tsp_tour) {
        auto cost = (u == graph.null_vertex()) ?
            0 :
            get_min_cost(u, v, graph);
        u = v;
        results.push_back(std::make_pair(get_vertex_id(v), cost));
    }
    pgassert(results.size() == num_vertices(graph) + 1);
    return results;
}

double
TSP::eval_tour(std::deque<std::pair<int64_t, double>>& tsp_tour) {
    std::deque<std::pair<int64_t, double>> results;
    auto u = graph.null_vertex();
    double agg_cost(0);
    for (auto &node : tsp_tour) {
        auto v = get_boost_vertex(node.first);
        auto cost = (u == graph.null_vertex()) ?
            0 :
            get_min_cost(u, v, graph);
        u = v;
        node.second = cost;
        agg_cost += cost;
    }
    return agg_cost;
}

bool
TSP::has_vertex(int64_t id) const {
    return id_to_V.find(id) != id_to_V.end();
}

TSP::V
TSP::get_boost_vertex(int64_t id) const {
    try {
        return id_to_V.at(id);
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong when getting the vertex descriptor"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }
}

int64_t
TSP::get_vertex_id(V v) const {
    try {
        return V_to_id.at(v);
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong when getting the vertex id"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }
}

int64_t
TSP::get_edge_id(E e) const {
    try {
        return E_to_id.at(e);
    } catch (...) {
        throw std::make_pair(
                std::string("INTERNAL: something went wrong when getting the edge id"),
                std::string(__PGR_PRETTY_FUNCTION__));
    }
}



#if Boost_VERSION_MACRO >= 106800
std::ostream& operator<<(std::ostream &log, const TSP& data) {
    log << "Number of Vertices is:" << num_vertices(data.graph) << "\n";
    log << "Number of Edges is:" << num_edges(data.graph) << "\n";
    log << "\n the print_graph\n";
    boost::print_graph(data.graph, boost::get(boost::vertex_index, data.graph), log);
#if 0
    // to print with edge weights:
    for (auto v : boost::make_iterator_range(boost::vertices(data.graph))) {
        for (auto oe : boost::make_iterator_range(boost::out_edges(v, data.graph))) {
            log << "Edge " << oe << " weight " << get(boost::edge_weight, data.graph)[oe] << "\n";
        }
    }
#endif
    return log;
}
#endif

}  // namespace algorithm
}  // namespace pgrouting

