/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Celia Virginia Vergara Castillo
Copyright (c) 2022 Celia Virginia Vergara Castillo
Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky at erosion.dev

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
mail: m_attia_sakri at yahoo.com, estebanzimanyi at gmail.com

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

#ifndef INCLUDE_DIJKSTRA_PGR_DIJKSTRA_HPP_
#define INCLUDE_DIJKSTRA_PGR_DIJKSTRA_HPP_
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

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include "c_types/ii_t_rt.h"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"
#include "visitors/dijkstra_visitors.hpp"

namespace detail {

template <typename G, typename V>
std::deque<pgrouting::Path> get_paths(
        const G &graph,
        const std::vector<V> &predecessors,
        const std::vector<double> &distances,
        V source,
        std::set<V> &targets,
        bool only_cost) {
    std::deque<pgrouting::Path> paths;
    for (const auto target : targets) {
        paths.push_back(pgrouting::Path(
                    graph,
                    source, target,
                    predecessors, distances,
                    only_cost, true));
    }
    return paths;
}

template <typename G, typename V>
bool dijkstra_1_to_1(
        G &graph,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        V source,
        V target) {
    CHECK_FOR_INTERRUPTS();
    try {
        boost::dijkstra_shortest_paths(graph.graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&G::G_T_E::cost, graph.graph))
                .distance_map(&distances[0])
                .visitor(pgrouting::visitors::dijkstra_one_goal_visitor<V>(target)));
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

template <typename G, typename V>
bool dijkstra_1_to_many(
        G &graph,
        std::vector<V> &predecessors,
        std::vector<double> &distances,
        V source,
        const std::set<V> &targets,
        size_t n_goals) {
    CHECK_FOR_INTERRUPTS();
    std::set<V> goals_found;
    std::set<V> goals(targets.begin(), targets.end());
    try {
        boost::dijkstra_shortest_paths(graph.graph, source,
                boost::predecessor_map(&predecessors[0])
                .weight_map(get(&G::G_T_E::cost, graph.graph))
                .distance_map(&distances[0])
                .distance_inf(std::numeric_limits<double>::infinity())
                .visitor(pgrouting::visitors::dijkstra_many_goal_visitor<V>(goals, n_goals, goals_found)));
    } catch(pgrouting::found_goals &) {
        for (const auto &g : goals) {
            if (goals_found.find(g) == goals_found.end()) {
                /* goal was not found */
                predecessors[g] = g;
            }
        }
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


//! Dijkstra 1 to many
template <class G>
std::deque<pgrouting::Path> dijkstra(
        G &graph,
        int64_t start_vertex,
        const std::set<int64_t> &end_vertex,
        bool only_cost,
        size_t n_goals) {
    typedef typename G::V V;
    std::vector<V> predecessors(graph.num_vertices());
    std::vector<double> distances(graph.num_vertices(), std::numeric_limits<double>::infinity());

    if (!graph.has_vertex(start_vertex)) return std::deque<pgrouting::Path>();

    auto v_source(graph.get_V(start_vertex));

    std::set<V> s_v_targets;
    for (const auto &vertex : end_vertex) {
        if (graph.has_vertex(vertex)) s_v_targets.insert(graph.get_V(vertex));
    }

    // perform the algorithm
    detail::dijkstra_1_to_many<G, V>(graph, predecessors, distances, v_source, s_v_targets, n_goals);

    // get the results
    auto paths = get_paths(graph,  predecessors, distances, v_source, s_v_targets, only_cost);

    return paths;
}

}  // namespace detail


namespace pgrouting {
// TODO(vicky) namespace algorithm

template < class G > class Pgr_dijkstra;
// user's functions
// for development

template < class G >
std::deque<Path>
pgr_drivingDistance(
        G &graph,
        std::vector< int64_t > start_vids,
        double distance,
        bool equicost,
        std::ostringstream &log) {
    Pgr_dijkstra< G > fn_dijkstra;
    return fn_dijkstra.drivingDistance(
            graph,
            start_vids,
            distance,
            equicost,
            log);
}


/* 1 to 1*/
template <class G>
Path dijkstra(
        G &graph,
        int64_t start_vertex,
        int64_t end_vertex,
        bool only_cost = false) {
    typedef typename G::V V;
    std::vector<V> predecessors(graph.num_vertices());
    std::vector<double> distances(graph.num_vertices(), std::numeric_limits<double>::infinity());

    if (!graph.has_vertex(start_vertex)
            || !graph.has_vertex(end_vertex)) {
        return Path(start_vertex, end_vertex);
    }

    // get the graphs source and target
    auto departure(graph.get_V(start_vertex));
    auto destination(graph.get_V(end_vertex));

    // perform the algorithm
    detail::dijkstra_1_to_1<G, V>(graph, predecessors, distances, departure, destination);

    // get the results
    return Path(
            graph,
            departure, destination,
            predecessors, distances,
            only_cost, true);
}

template <class G>
std::deque<Path> dijkstra(
        G &graph,
        const std::map<int64_t, std::set<int64_t>> &combinations,
        bool only_cost,
        size_t n_goals) {
    std::deque<Path> paths;

    for (const auto &c : combinations) {
        auto r_paths = detail::dijkstra<G>(graph, c.first, c.second, only_cost, n_goals);
        paths.insert(paths.begin(), r_paths.begin(), r_paths.end());
    }

    return paths;
}

//******************************************

template < class G >
class Pgr_dijkstra {
 public:
     typedef typename G::V V;
     typedef typename G::E E;

     //! @name drivingDistance
     //@{
     //! 1 to distance
     Path drivingDistance(
             G &graph,
             int64_t start_vertex,
             double distance) {
        if (execute_drivingDistance(
                graph,
                start_vertex,
                distance)) {
            auto path = Path(graph,
                    start_vertex,
                    distance,
                    predecessors,
                    distances);

            std::sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    {return l.node < r.node;});
            std::stable_sort(path.begin(), path.end(),
                    [](const Path_t &l, const  Path_t &r)
                    {return l.agg_cost < r.agg_cost;});
            return path;
        }

        /* The result is empty */
        Path p(start_vertex, start_vertex);
        p.push_back({start_vertex, -1, 0, 0});
        return p;
     }

     // preparation for many to distance
     std::deque< Path > drivingDistance(
             G &graph,
             const std::vector< int64_t > &start_vertex,
             double distance,
             bool equicost,
             std::ostringstream &the_log) {
         if (equicost) {
             auto paths = drivingDistance_with_equicost(
                     graph,
                     start_vertex,
                     distance);
             the_log << log.str();
             return paths;
         } else {
             return drivingDistance_no_equicost(
                     graph,
                     start_vertex,
                     distance);
         }
     }






     //@}

     //! @name Dijkstra
     //@{


     //! Dijkstra 1 to many
     std::deque<Path> dijkstra(
             G &graph,
             int64_t start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost,
             size_t n_goals) {
         // adjust predecessors and distances vectors
         clear();

         predecessors.resize(graph.num_vertices());
         distances.resize(
                 graph.num_vertices(),
                 std::numeric_limits<double>::infinity());


         // get the graphs source and target
         if (!graph.has_vertex(start_vertex)) return std::deque<Path>();

         auto v_source(graph.get_V(start_vertex));

         std::set< V > s_v_targets;
         for (const auto &vertex : end_vertex) {
             if (graph.has_vertex(vertex)) s_v_targets.insert(graph.get_V(vertex));
         }

         // perform the algorithm
         detail::dijkstra_1_to_many(graph, predecessors, distances, v_source, s_v_targets, n_goals);

         std::deque< Path > paths;
         // get the results // route id are the targets
         paths = detail::get_paths(graph,  predecessors, distances, v_source, s_v_targets, only_cost);

         return paths;
     }

     // preparation for many to 1
     std::deque<Path> dijkstra(
             G &graph,
             const std::vector < int64_t > &start_vertex,
             int64_t end_vertex,
             bool only_cost) {
         std::deque<Path> paths;

         for (const auto &start : start_vertex) {
             paths.push_back(
                     dijkstra(graph, start, end_vertex, only_cost));
         }

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.start_id() < e2.start_id();
                 });
         return paths;
     }


     // preparation for many to many
     std::deque<Path> dijkstra(
             G &graph,
             const std::vector< int64_t > &start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost,
             size_t n_goals = (std::numeric_limits<size_t>::max)()) {
         // a call to 1 to many is faster for each of the sources
         std::deque<Path> paths;

         for (const auto &start : start_vertex) {
             auto r_paths = dijkstra(
                     graph,
                     start, end_vertex,
                     only_cost, n_goals);
             paths.insert(paths.begin(), r_paths.begin(), r_paths.end());
         }

         return paths;
     }

    // preparation for parallel arrays
    std::deque<Path> dijkstra(
            G &graph,
            const std::vector< II_t_rt > &combinations,
            bool only_cost,
            size_t n_goals) {
        // a call to 1 to many is faster for each of the sources
        std::deque<Path> paths;

        // group targets per distinct source
        std::map<int64_t , std::vector<int64_t> > vertex_map;
        for (const II_t_rt &comb : combinations) {
            std::map< int64_t , std::vector<int64_t> >::iterator it = vertex_map.find(comb.d1.source);
            if (it != vertex_map.end()) {
                it->second.push_back(comb.d2.target);
            } else {
                std::vector<int64_t > targets{comb.d2.target};
                vertex_map[comb.d1.source] = targets;
            }
        }

        for (const auto &start_ends : vertex_map) {
            auto r_paths = dijkstra(
                    graph,
                    start_ends.first, start_ends.second,
                    only_cost, n_goals);
            paths.insert(paths.begin(), r_paths.begin(), r_paths.end());
        }

        return paths;
    }


     //@}

 private:
     /** Call to Dijkstra  1 to distance
      *
      * Used on:
      *   1 to distance
      *   many to distance
      *   On the first call of many to distance with equi_cost
      */
     bool dijkstra_1_to_distance(
             G &graph,
             V source,
             double distance) {
         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();
         try {
             boost::dijkstra_shortest_paths(graph.graph, source,
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph))
                     .distance_map(&distances[0])
                     .visitor(visitors::dijkstra_distance_visitor<V>(distance, nodesInDistance, distances)));
         } catch(found_goals &) {
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
     bool dijkstra_1_to_distance_no_init(
             G &graph,
             V source,
             double distance) {
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
                     visitors::dijkstra_distance_visitor_no_init<V, E>(log, source, distance, predecessors, distances,
                         color_map),
                     boost::make_iterator_property_map(
                         color_map.begin(),
                         graph.vertIndex,
                         color_map[0]));
         } catch(found_goals &) {
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
     bool execute_drivingDistance(
             G &graph,
             int64_t start_vertex,
             double distance) {
         clear();

         predecessors.resize(graph.num_vertices());
         distances.resize(
                 graph.num_vertices(),
                 std::numeric_limits<double>::infinity());

         // get source;
         if (!graph.has_vertex(start_vertex)) {
             return false;
         }

         return dijkstra_1_to_distance(
                 graph,
                 graph.get_V(start_vertex),
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

         return dijkstra_1_to_distance_no_init(
                 graph,
                 start_vertex,
                 distance);
     }

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
     std::deque< Path > drivingDistance_with_equicost(
             G &graph,
             const std::vector< int64_t > &start_vertex,
             double distance) {
         clear();
         log << "Number of edges:" << boost::num_edges(graph.graph) << "\n";

         predecessors.resize(graph.num_vertices());
         distances.resize(
                 graph.num_vertices(),
                 std::numeric_limits<double>::infinity());

         /*
          * Vector to store the different predessesors
          * each is of size = graph.num_vertices()
          *
          * TODO(vicky)
          * - figure out less storage if possible
          */
         std::deque< std::vector< V > > pred(start_vertex.size());

         // perform the algorithm
         size_t i = 0;
         for (const auto &vertex : start_vertex) {
             nodesInDistance.clear();
             /*
              * The vertex does not exist
              *   Nothing to do
              */
             if (graph.has_vertex(vertex)) {
                 if (execute_drivingDistance_no_init(
                             graph,
                             graph.get_V(vertex),
                             distance)) {
                     pred[i] = predecessors;
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
                distance);
     }

     /** @brief gets results in form of a container of paths
      *
      * @param [in] graph The graph that is being worked
      * @param [in] start_vertex An array of vertices @b id
      * @param [in] pred an array of predecessors
      * @param [in] distance the max distance
      */
     std::deque< Path > get_drivingDistance_with_equicost_paths(
             G &graph,
             const std::vector< int64_t > &start_vertex,
             std::deque< std::vector< V > > &pred,
             double distance) {
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
             paths.back().push_back({vertex, -1, 0, 0});
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
                 pgassert(edge_id != -1);
                 paths[i - 1].push_back(
                         {graph[d].id,
                         edge_id, cost,
                         distances[d]});
                 break;
             }
         }

         for (auto &path : paths) {
             path.sort_by_node_agg_cost();
         }
         return paths;
     }


     // preparation for many to distance No equicost
     std::deque< Path > drivingDistance_no_equicost(
             G &graph,
             const std::vector< int64_t > &start_vertex,
             double distance) {
         // perform the algorithm
         std::deque<Path> paths;
         for (const auto &vertex : start_vertex) {
             if (execute_drivingDistance(graph, vertex, distance)) {
                 auto path = Path(
                         graph,
                         vertex,
                         distance,
                         predecessors,
                         distances);
                 path.sort_by_node_agg_cost();
                 paths.push_back(path);
             } else {
                 Path p(vertex, vertex);
                 p.push_back({vertex, -1, 0, 0});
                 paths.push_back(p);
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

}  // namespace pgrouting


#endif  // INCLUDE_DIJKSTRA_PGR_DIJKSTRA_HPP_
