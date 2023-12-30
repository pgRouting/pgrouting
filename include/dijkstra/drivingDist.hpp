/*PGR-GNU*****************************************************************

file: drivingDist.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2022 Celia Virginia Vergara Castillo
Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
mail: m_attia_sakri at yahoo.com, estebanzimanyi at gmail.com

Copyright (c) 2023 Aryan Gupta
guptaaryan1010 AT gmail.com

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

#ifndef INCLUDE_DIJKSTRA_DRIVEDIST_HPP_
#define INCLUDE_DIJKSTRA_DRIVEDIST_HPP_
#pragma once

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>
#include <map>
#include <numeric>
#include <utility>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"
#include "visitors/dijkstra_visitors.hpp"

namespace bg_detail {

/** @brief Dijkstra 1 to distance
 *
 * Used on:
 *   1 to distance
 *   many to distance
 *   On the first call of many to distance with equi_cost
 */
template <typename G, typename V>
bool dijkstra_1_to_distance(
        G &graph,
        V source,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        std::deque<V> &nodesInDistance,
        double distance) {
    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths(graph.graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&G::G_T_E::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(pgrouting::visitors::dijkstra_distance_visitor<V>(distance, nodesInDistance, distances)));
    } catch(pgrouting::found_goals &) {
        /*No op*/
    } catch (boost::exception const&) {
        throw;
    } catch (std::exception&) {
        throw;
    } catch (...) {
        throw;
    }
    return true;
}

/** Call to Dijkstra  1 to distance no init
 *
 * Used on:
 *   On the subsequent calls of many to distance with equi_cost
 */
template <typename G, typename V>
bool dijkstra_1_to_distance_no_init(
        G &graph,
        V source,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        double distance) {
    typedef typename G::E E;
    pgassert(predecessors.size() == graph.num_vertices());
    pgassert(distances.size() == graph.num_vertices());
    distances[source] = 0;
    std::vector<boost::default_color_type> color_map(graph.num_vertices());
    /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths_no_init(graph.graph, source,
                make_iterator_property_map(
                    predecessors.begin(),
                    graph.vertIndex),
                make_iterator_property_map(
                    distances.begin(),
                    graph.vertIndex),
                get(&G::G_T_E::cost, graph.graph),
                graph.vertIndex,
                std::less<double>(),
                boost::closed_plus<double>(),
                static_cast<double>(0),
                pgrouting::visitors::dijkstra_distance_visitor_no_init<V, E>(source, distance, predecessors, distances,
                    color_map),
                boost::make_iterator_property_map(
                    color_map.begin(),
                    graph.vertIndex,
                    color_map[0]));
    } catch(pgrouting::found_goals &) {
        return true;
    } catch (boost::exception const& ex) {
        (void)ex;
        throw;
    } catch (std::exception &e) {
        (void)e;
        throw;
    } catch (...) {
             throw;
         }

         return true;
     }

}  // namespace bg_detail


namespace detail {

/* notation:
 * node: vertex or point
 * vertex: node with id >= 0
 * point: node with id < 0
 */
template <typename G, typename V>
void remove_details(const G &graph,
             std::vector<double> &distances,
             std::vector<V>      &predecessors) {
    /*
     * find all the points that are predecessors
     */
    std::set<V> node_with_predecessor_point;
    for (V v = 0; v < predecessors.size() ; ++v) {
        /*
         * skipping unreachable nodes and or initial node
         * skipping predecessors that are vertices
         */
        if (predecessors[v] == v) continue;
        if (graph[predecessors[v]].id >= 0) continue;
        node_with_predecessor_point.insert(v);
    }

    /*
     * Compact all nodes that have predecessor point
     */
    for (const auto node : node_with_predecessor_point) {
        /*
         * Cycle predecessors
         * u -> v  cost to arrive to v is distances[v]
         */
        auto v = node;
        auto u = predecessors[v];
        pgassert(graph[u].id < 0);

        /*
         * while u is a point and its predecessor is not itself
         */
        while (graph[u].id < 0 && predecessors[u] != u) {
            CHECK_FOR_INTERRUPTS();
            pgassert(graph[u].id < 0);
            pgassert(distances[v] !=  std::numeric_limits<double>::infinity());
            v = u;
            u = predecessors[v];
        }

        /* the vertex (or initial point) that is a predecesor of p */
        predecessors[node] = u;
    }
}

/** @brief gets results in form of a container of paths with depth
 *
 * @param [in] graph The graph that is being worked
 * @param [in] root The starting node
 * @param [in] distances An array of vertices @b id
 * @param [in] predecessors an array of predecessors
 * @param [in] distance the max distance
 */
template <typename G, typename V>
std::map<int64_t, int64_t> get_depth(
        const G &graph,
        V root,
        std::vector<double> &distances,
        std::vector<V>      &predecessors,
        double distance,
        bool details) {
    std::map<int64_t, int64_t> depth;
    if (predecessors.empty()) return depth;
    if (predecessors.size() != distances.size()) return depth;
    depth[graph[root].id] = 0;

    std::set<V> vertices;
    vertices.insert(root);

    if (!details) {
        remove_details(graph, distances, predecessors);
    }

    /* cycle depth max depth can be number of nodes*/
    for (int64_t d = 1; static_cast<size_t>(d) < graph.num_vertices() ; ++d) {
        /*
         * there is no more to search for
         */
        if (vertices.empty()) break;

        /*
         * One next cycle these vertices have the next depth
         */
        std::set<V> vertices_next;
        std::set<V> point_vertices;


        for (const auto v : vertices) {
            /*
             * Cycle predecessors looking for vertices on the depth d
             * v -> p
             */
            for (V p = 0; p < graph.num_vertices() ; ++p) {
                /*
                 * Sikiping unassigned predecessors
                 * Sikiping distances greater than the one asked for
                 */
                if (predecessors[p] == p) continue;
                if (!(distances[p] <= distance)) continue;
                if (!(predecessors[p] == v)) continue;


                /* found */
                depth[graph[p].id] = d;
                vertices_next.insert(p);
            }
        }
        vertices = vertices_next;
    }
    return depth;
}

/** @brief to use with driving distance
 *
 * Prepares the execution for a driving distance:
 *
 * @param graph
 * @param root
 * @param distance
 *
 * Results are kept on predecessor & distances
 *
 * @returns bool  @b True when results are found
 */
template <typename G, typename V>
bool execute_drivingDistance(
        G &graph,
        int64_t root,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        std::deque<V> &nodesInDistance,
        double distance) {

    // get root;
    if (!graph.has_vertex(root)) {
        return false;
    }

    predecessors.clear();
    distances.clear();
    nodesInDistance.clear();
    predecessors.resize(graph.num_vertices());
    distances.resize(
            graph.num_vertices(),
            std::numeric_limits<double>::infinity());


    return bg_detail::dijkstra_1_to_distance(
            graph,
            graph.get_V(root),
            predecessors,
            distances,
            nodesInDistance,
            distance);
}


/** @brief to use with driving distance
 *
 * Prepares the execution for a driving distance:
 *
 * @param graph
 * @param start_vertex
 * @param distance
 *
 * Results are kept on predecessor & distances
 *
 * @returns bool  @b True when results are found
 */
bool execute_drivingDistance_no_init(
        G &graph,
        V start_vertex,
        double distance) {
    pgassert(predecessors.size() == graph.num_vertices());
    pgassert(distances.size() == graph.num_vertices());

    std::iota(predecessors.begin(), predecessors.end(), 0);

    return bg_detail::dijkstra_1_to_distance_no_init(
            graph,
            start_vertex,
            predecessors, distances,
            distance);
}

}  // namespace detail


namespace pgrouting {
// TODO(v4) convert to functions

namespace algorithm {
template < class G > class Pgr_dijkstra;
}  // namespace algorithm

template <class G>
std::deque<Path>
pgr_drivingdistance(
        G &graph,
        const std::vector<int64_t> &start_vids,
        double distance,
        bool equicost,
        std::vector<std::map<int64_t, int64_t>> &depths,
        bool details) {
    algorithm::Pgr_dijkstra<G> fn_dijkstra;
    return fn_dijkstra.drivingDistance(graph, start_vids, distance, equicost, depths, details);
}

namespace algorithm {

template < class G >
class Pgr_dijkstra {
 public:
     typedef typename G::V V;
     typedef typename G::E E;

     // preparation for many to distance
     std::deque<Path> drivingDistance(
             G &graph,
             const std::vector<int64_t> &start_vertex,
             double distance,
             bool equicost,
             std::vector<std::map<int64_t, int64_t>> &depths,
             bool details) {
         if (equicost) {
             return drivingDistance_with_equicost(
                     graph,
                     start_vertex,
                     depths,
                     distance, details);
         } else {
             return drivingDistance_no_equicost(
                     graph,
                     start_vertex,
                     depths,
                     distance, details);
         }
     }






     //@}


 private:


     /* preparation for many to distance with equicost
      *
      * Idea:
      *   The distances vector does not change
      *   The predecessors vector does not change
      *   The first @b valid execution is done normally:
      *     - The distances will have:
      *       - inf
      *       - values < distance
      *       - values > distance
      *   Subsequent @b valid executions
      *       - will not change the:
      *         - values < distance
      *   Don't know yet what happens to predecessors
      */
     std::deque<Path> drivingDistance_with_equicost(
             G &graph,
             const std::vector<int64_t> &start_vertex,
             std::vector<std::map<int64_t, int64_t>> &depths,
             double distance, bool details) {
         clear();
         log << "Number of edges:" << boost::num_edges(graph.graph) << "\n";

         depths.resize(start_vertex.size());
         predecessors.resize(graph.num_vertices());
         distances.resize(
                 graph.num_vertices(),
                 std::numeric_limits<double>::infinity());

         /*
          * Vector to store the different predessesors
          * each is of size = graph.num_vertices()
          *
          * TODO(gsoc)
          * - figure out less storage if possible
          */
         std::deque< std::vector<V>> pred(start_vertex.size());
         std::deque< std::vector<V>> nodetailspred(start_vertex.size());

         // perform the algorithm
         size_t i = 0;
         for (const auto &vertex : start_vertex) {
             nodesInDistance.clear();
             /*
              * The vertex does not exist
              *   Nothing to do
              */
             if (!(graph.has_vertex(vertex))) continue;

             if (execute_drivingDistance_no_init(
                         graph, graph.get_V(vertex), distance)) {
                 pred[i] = predecessors;
                 depths[i] = detail::get_depth(graph, graph.get_V(vertex), distances, predecessors, distance, details);
                 if (!details) {
                     nodetailspred[i] = predecessors;
                 }
             }
             ++i;
         }


         /*
          * predecessors of vertices in the set are themselves
          */
         for (const auto &vertex : start_vertex) {
             for (auto &p : pred) {
                 if (!p.empty() && graph.has_vertex(vertex))
                     p[graph.get_V(vertex)] = graph.get_V(vertex);
             }
         }


        return get_drivingDistance_with_equicost_paths(
                graph,
                start_vertex,
                pred,
                nodetailspred,
                distance, details);
     }

     /** @brief gets results in form of a container of paths
      *
      * @param [in] graph The graph that is being worked
      * @param [in] start_vertex An array of vertices @b id
      * @param [in] pred an array of predecessors
      * @param [in] distance the max distance
      * @pre one predecessor per root
      */
     std::deque< Path > get_drivingDistance_with_equicost_paths(
             G &graph,
             const std::vector<int64_t> &start_vertex,
             std::deque<std::vector<V>> &pred,
             std::deque<std::vector<V>> &nodetailspred,
             double distance, bool details) {
         /*
          * precondition
          */
         pgassert(start_vertex.size() == pred.size());


         /*
          * Creating all the result "paths"
          */
         std::deque<Path> paths;
         for (const auto vertex : start_vertex) {
             paths.push_back(Path(vertex, vertex));
             paths.back().push_back({vertex, -1, 0, 0, vertex});
         }

         /*
          *  Ciclying the distances:
          *  To which vertex do they belong to?
          */
         for (V d = 0; d < distances.size(); ++d) {
             /*
              * Sikiping distances greater than the one asked for
              */
             if (!(distances[d] <= distance)) continue;

             for (auto i = start_vertex.size(); i > 0; --i) {
                 /*
                  * The vertex does not exist on the graph
                  */
                 if (pred[i - 1].empty()) break;


                 /*
                  * The predecessor = current then
                  *  its unreachable to this vertex
                  */
                 if (pred[i - 1][d] == d) continue;

                 auto cost = distances[d] - distances[pred[i - 1][d]];
                 auto edge_id = graph.get_edge_id(pred[i - 1][d], d, cost);
                 int64_t pred_node =  details? graph[pred[i - 1][d]].id : graph[nodetailspred[i - 1][d]].id;
                 pgassert(edge_id != -1);
                 paths[i - 1].push_back(
                         {graph[d].id,
                         edge_id,
                         details? cost : distances[d] - distances[nodetailspred[i - 1][d]],
                         distances[d], pred_node});
                 break;
             }
         }

         for (auto &path : paths) {
             path.sort_by_node_agg_cost();
         }
         return paths;
     }


     // preparation for many to distance No equicost
     std::deque<Path> drivingDistance_no_equicost(
             G &graph,
             const std::vector< int64_t > &start_vertex,
             std::vector<std::map<int64_t, int64_t>> &depths,
             double distance, bool details) {
         // perform the algorithm
         std::deque<Path> paths;
         for (const auto &vertex : start_vertex) {
             if (detail::execute_drivingDistance(graph, vertex, predecessors, distances, nodesInDistance, distance)) {
                 auto path = Path(
                         graph,
                         vertex,
                         distance,
                         predecessors,
                         distances);
                 path.sort_by_node_agg_cost();
                 auto root = graph.get_V(vertex);
                 depths.push_back(detail::get_depth(graph, root, distances, predecessors, distance, details));
                 /*
                  * When details are not wanted update costs
                  */
                 if (!details) {
                     for (auto &pathstop : path) {
                         auto node = graph.get_V(pathstop.node);

                         /* skip points */
                         if (graph[node].id < 0) continue;

                         pathstop.cost = distances[node] - distances[predecessors[node]];
                     }
                     log << "Updated costs of path " << path;
                 }
                     paths.push_back(path);

             } else {
                 Path p(vertex, vertex);
                 p.push_back({vertex, -1, 0, 0, vertex});
                 paths.push_back(p);
                 std::map<int64_t, int64_t> m;
                 m[vertex] = 0;
                 depths.push_back(m);
             }
         }
         return paths;
     }


     void clear() {
         predecessors.clear();
         distances.clear();
         nodesInDistance.clear();
     }

     //! @name members
     //@{
     std::vector<V> predecessors;
     std::vector<double> distances;
     std::deque<V> nodesInDistance;
     std::ostringstream log;
     //@}
};

}  // namespace algorithm
}  // namespace pgrouting


#endif  // INCLUDE_DIJKSTRA_DRIVEDIST_HPP_
