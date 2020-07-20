/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
mail: m_attia_sakr@yahoo.com, estebanzimanyi@gmail.com

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

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "c_types/pgr_combination_t.h"

#if BOOST_VERSION_OK
#include <boost/graph/dijkstra_shortest_paths.hpp>
#else
#include "boost/dijkstra_shortest_paths.hpp"
#endif

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>
#include <map>
#include <numeric>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"
#include "visitors/dijkstra_one_goal_visitor.hpp"

#if 0
#include "./../../common/src/signalhandler.h"
#endif

namespace pgrouting {

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
template < class G >
Path
pgr_dijkstra(
        G &graph,
        int64_t source,
        int64_t target,
        bool only_cost = false) {
    Pgr_dijkstra< G > fn_dijkstra;
    return fn_dijkstra.dijkstra(graph, source, target, only_cost);
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

     //! Dijkstra 1 to 1
     Path dijkstra(
             G &graph,
             int64_t start_vertex,
             int64_t end_vertex,
             bool only_cost = false) {
         clear();

         // adjust predecessors and distances vectors
         predecessors.resize(graph.num_vertices());
         distances.resize(
                 graph.num_vertices(),
                 std::numeric_limits<double>::infinity());


         if (!graph.has_vertex(start_vertex)
                 || !graph.has_vertex(end_vertex)) {
             return Path(start_vertex, end_vertex);
         }

         // get the graphs source and target
         auto v_source(graph.get_V(start_vertex));
         auto v_target(graph.get_V(end_vertex));

         // perform the algorithm
         dijkstra_1_to_1(graph, v_source, v_target);

         // get the results
         return Path(
                 graph,
                 v_source, v_target,
                 predecessors, distances,
                 only_cost, true);
     }






     //! Dijkstra 1 to many
     std::deque<Path> dijkstra(
             G &graph,
             int64_t start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost,
             size_t n_goals = (std::numeric_limits<size_t>::max)()) {
         // adjust predecessors and distances vectors
         clear();

         predecessors.resize(graph.num_vertices());
         distances.resize(
                 graph.num_vertices(),
                 std::numeric_limits<double>::infinity());


         // get the graphs source and target
         if (!graph.has_vertex(start_vertex))
             return std::deque<Path>();
         auto v_source(graph.get_V(start_vertex));

         std::set< V > s_v_targets;
         for (const auto &vertex : end_vertex) {
             if (graph.has_vertex(vertex)) {
                 s_v_targets.insert(graph.get_V(vertex));
             }
         }

         std::vector< V > v_targets(s_v_targets.begin(), s_v_targets.end());
         // perform the algorithm
         dijkstra_1_to_many(graph, v_source, v_targets, n_goals);

         std::deque< Path > paths;
         // get the results // route id are the targets
         paths = get_paths(graph, v_source, v_targets, only_cost);

         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.end_id() < e2.end_id();
                 });

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

         std::sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.end_id() < e2.end_id();
                 });
         std::stable_sort(paths.begin(), paths.end(),
                 [](const Path &e1, const Path &e2)->bool {
                 return e1.start_id() < e2.start_id();
                 });
         return paths;
     }

    // preparation for parallel arrays
    std::deque<Path> dijkstra(
            G &graph,
            const std::vector< pgr_combination_t > &combinations,
            bool only_cost,
            size_t n_goals = (std::numeric_limits<size_t>::max)()) {
        // a call to 1 to many is faster for each of the sources
        std::deque<Path> paths;

        // group targets per distinct source
        std::map<int64_t , std::vector<int64_t> > vertex_map;
        for (const pgr_combination_t &comb : combinations) {
            std::map< int64_t , std::vector<int64_t> >::iterator it = vertex_map.find(comb.source);
            if (it != vertex_map.end()) {
                it->second.push_back(comb.target);
            } else {
                std::vector<int64_t > targets{comb.target};
                vertex_map[comb.source] = targets;
            }
        }

        for (const auto &start_ends : vertex_map) {
            auto r_paths = dijkstra(
                    graph,
                    start_ends.first, start_ends.second,
                    only_cost, n_goals);
            paths.insert(paths.begin(), r_paths.begin(), r_paths.end());
        }
        vertex_map.clear();
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.end_id() < e2.end_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.start_id() < e2.start_id();
                });

        return paths;
    }

     //@}

 private:
     //! Call to Dijkstra  1 source to 1 target
     bool dijkstra_1_to_1(
                 G &graph,
                 V source,
                 V target) {
         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();
         try {
             boost::dijkstra_shortest_paths(graph.graph, source,
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph))
                     .distance_map(&distances[0])
                     .visitor(visitors::dijkstra_one_goal_visitor<V>(target)));
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
                     .visitor(dijkstra_distance_visitor(
                             distance,
                             nodesInDistance,
                             distances)));
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
                     dijkstra_distance_visitor_no_init(
                         log,
                         source,
                         distance,
                         predecessors,
                         distances,
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

#if 0
         /*
          * Expensive assertion
          */
         for (V v = 0 ; v < predecessors.size(); ++v) {
             log << "(" << predecessors[v] << "==" << v << "),";
             if (v != source) {
                 pgassertwm(predecessors[v] == v, log.str().c_str());
             }
         }
#endif
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

#if 0
         /*
          * Expensive assertion
          */
         for (V i = 0 ; i < predecessors.size(); ++i) {
             pgassert(i == predecessors[i]);
         }
#endif

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


     //! Dijkstra  1 source to many targets
     bool dijkstra_1_to_many(
             G &graph,
             V source,
             const std::vector< V > &targets,
             size_t n_goals = (std::numeric_limits<size_t>::max)()) {
         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();
         try {
             boost::dijkstra_shortest_paths(graph.graph, source,
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph))
                     .distance_map(&distances[0])
                     .distance_inf(std::numeric_limits<double>::infinity())
                     .visitor(dijkstra_many_goal_visitor(targets, n_goals)));
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


     void clear() {
         predecessors.clear();
         distances.clear();
         nodesInDistance.clear();
     }




     // used when multiple goals
     std::deque<Path> get_paths(
             const G &graph,
             V source,
             std::vector< V > &targets,
             bool only_cost) const {
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



     //! @name members
     //@{
#if 0
     struct found_goals{};  //!< exception for termination
#endif
     std::vector< V > predecessors;
     std::vector< double > distances;
     std::deque< V > nodesInDistance;
     std::ostringstream log;
     //@}


     //! @name Stopping classes
     //@{
     //! class for stopping when all targets are found
     class dijkstra_many_goal_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_many_goal_visitor(
                  const std::vector< V > &goals,
                  size_t n_goals) :
              m_goals(goals.begin(), goals.end()),
              m_n_goals(n_goals)   {}
          template <class B_G>
              void examine_vertex(V u, B_G &) {
                  auto s_it = m_goals.find(u);
                  if (s_it == m_goals.end()) return;

                  // found one more goal
                  m_goals.erase(s_it);

                  // all goals found
                  if (m_goals.size() == 0) throw found_goals();

                  // number of requested goals found
                  --m_n_goals;
                  if (m_n_goals == 0) throw found_goals();
              }

      private:
          std::set< V > m_goals;
          size_t m_n_goals;
     };


     //! class for stopping when a distance/cost has being surpassed
     class dijkstra_distance_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_distance_visitor(
                  double distance_goal,
                  std::deque< V > &nodesInDistance,
                  std::vector< double > &distances) :
              m_distance_goal(distance_goal),
              m_nodes(nodesInDistance),
              m_dist(distances) {
                  pgassert(m_nodes.empty());
                  pgassert(m_distance_goal > 0);
              }
          template <class B_G>
              void examine_vertex(V u, B_G &) {
                  if (m_dist[u] > m_distance_goal) {
                      throw found_goals();
                  }
                  m_nodes.push_back(u);
              }

      private:
          double m_distance_goal;
          std::deque< V > &m_nodes;
          std::vector< double > &m_dist;
     };

     //! class for stopping when a distance/cost has being surpassed
     class dijkstra_distance_visitor_no_init
         : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_distance_visitor_no_init(
                  std::ostringstream &p_log,
                  V source,
                  double distance_goal,
                  std::vector< V > &predecessors,
                  std::vector< double > &distances,
                  std::vector<boost::default_color_type> &color_map) :
              log(p_log),
              first(source),
              m_distance_goal(distance_goal),
              m_num_examined(0),
              m_predecessors(predecessors),
              m_dist(distances),
              m_color(color_map) {
                  pgassert(m_num_examined == 0);
                  pgassert(m_distance_goal > 0);
              }

          template <class B_G>
          void examine_vertex(V u, B_G &) {
              if ( 0 == m_num_examined++) first = u;
              if (m_dist[u] > m_distance_goal) {
                  throw found_goals();
              }
              if (u != first && m_predecessors[u] == u) {
                   m_color[u] = boost::black_color;
              }
          }

          template <class B_G>
          void examine_edge(E e, B_G &g) {
              if (source(e, g) != first
                      && m_predecessors[source(e, g)] == source(e, g)) {
                   m_color[target(e, g)] = boost::black_color;
              }
          }


          template <class B_G>
          void edge_relaxed(E, B_G &) {
          }



          template <class B_G>
          void edge_not_relaxed(E e, B_G &g) {
              if (source(e, g) != first
                      && m_predecessors[source(e, g)] == source(e, g)) {
                  m_color[target(e, g)] = boost::black_color;
              }
          }


          template <class B_G>
          void finish_vertex(V, B_G &) {
          }


          template <class B_G>
          void discover_vertex(V u, B_G &) {
              if (u  != first && m_predecessors[u] == u) {
                   m_color[u] = boost::black_color;
              }
          }

      private:
          std::ostringstream &log;
          V first;
          double m_distance_goal;
          size_t m_num_examined;
          std::vector< V > &m_predecessors;
          std::vector< double > &m_dist;
          std::vector<boost::default_color_type> &m_color;
     };
};

}  // namespace pgrouting


#endif  // INCLUDE_DIJKSTRA_PGR_DIJKSTRA_HPP_
