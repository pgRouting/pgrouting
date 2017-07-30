/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/exception/diagnostic_information.hpp> 
#include <boost/exception_ptr.hpp> 

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#if 0
#include "./../../common/src/signalhandler.h"
#endif


template < class G > class Pgr_dijkstra;
// user's functions
// for development

template < class G >
std::deque<Path>
pgr_drivingDistance(
        G &graph,
        std::vector< int64_t > start_vids,
        double distance,
        bool equicost) {
    Pgr_dijkstra< G > fn_dijkstra;
    return fn_dijkstra.drivingDistance(graph, start_vids, distance, equicost);
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
            auto path = Path(graph, start_vertex, distance, predecessors, distances);

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
             const std::vector< int64_t > start_vertex,
             double distance,
             bool equicost) {
         if (equicost) {
             auto paths = drivingDistance_with_equicost(
                     graph,
                     start_vertex,
                     distance);
              // pgassertwm(false, log.str().c_str());
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
     //! one to one
     Path dijkstra(
             G &graph,
             int64_t start_vertex,
             int64_t end_vertex,
             bool only_cost = false);

     //! Many to one
     std::deque<Path> dijkstra(
             G &graph,
             const std::vector < int64_t > &start_vertex,
             int64_t end_vertex,
             bool only_cost = false);

     //! Many to Many
     std::deque<Path> dijkstra(
             G &graph,
             const std::vector< int64_t > &start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost = false);

     //! one to Many
     std::deque<Path> dijkstra(
             G &graph,
             int64_t start_vertex,
             const std::vector< int64_t > &end_vertex,
             bool only_cost = false);
     //@}

 private:
     //! Call to Dijkstra  1 source to 1 target
     bool dijkstra_1_to_1(
                 G &graph,
                 V source,
                 V target) {
         try {
             boost::dijkstra_shortest_paths(graph.graph, source,
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph))
                     .distance_map(&distances[0])
                     .visitor(dijkstra_one_goal_visitor(target)));
         } catch(found_goals &) {
             return true;
         } catch (boost::exception const& ex) {
             throw;
         } catch (std::exception &e) {
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
             return true;
         } catch (boost::exception const& ex) {
             throw;
         } catch (std::exception &e) {
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
#if 0
         auto IndexMap = get(boost::vertex_index, graph.graph);
         typedef
             boost::detail::default_color_map_generator<G,  typeid(IndexMap)>
             ColorMapHelper;
         typedef typename ColorMapHelper::type ColorMap;
         ColorMap color =
             ColorMapHelper::build(g, index_map);
#endif
         std::vector<boost::default_color_type> color_map(graph.num_vertices());
         try {
             boost::dijkstra_shortest_paths_no_init(graph.graph, source,
                     &predecessors[0]
                     ,&distances[0]
                     ,get(&G::G_T_E::cost, graph.graph)
                     ,get(boost::vertex_index, graph.graph)
                     ,std::less<double>()
                     ,boost::closed_plus<double>()
                     ,double(0)
                     ,dijkstra_distance_visitor_no_init(
                         log,
                         source,
                         graph.is_undirected(),
                         distance,
                         nodesInDistance,
                         predecessors,
                         distances,
                         color_map)
                     ,boost::make_iterator_property_map(color_map.begin(), boost::get(boost::vertex_index, graph.graph), color_map[0])
                     );
         } catch(found_goals &) {
             return true;
         } catch (boost::exception const& ex) {
             throw;
         } catch (std::exception &e) {
             throw;
         } catch (...) {
             throw;
         }

#if DEBUG
         /*
          * Expensive assertion
          */
         for (V v = 0 ; v < predecessors.size(); ++v) {
             log << "(" << predecessors[v] << "==" << v << ")," ;
             if (v != source) pgassertwm(predecessors[v] == v, log.str().c_str());
         }
#endif
         return true;
     }


     /** @brief to use with driving distance
      *
      * Prepares the execution for a driving distance:
      *
      * @params graph
      * @params start_vertex
      * @params distance
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
         distances.resize(graph.num_vertices());

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
      * @params graph
      * @params start_vertex
      * @params distance
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

#if 1
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
      *   Dont know yet what happens to predecessors
      */
     std::deque< Path > drivingDistance_with_equicost(
             G &graph,
             const std::vector< int64_t > start_vertex,
             double distance) {
         clear();
         log << "Number of edges:" << boost::num_edges(graph.graph) << "\n";

         predecessors.resize(graph.num_vertices());
         distances.resize(graph.num_vertices(), std::numeric_limits<double>::max());

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
             if (!graph.has_vertex(vertex)) {
                 pgassert(false);
                 continue;
             }

             if (execute_drivingDistance_no_init(graph, graph.get_V(vertex), distance)) {
                 pred[i] = predecessors;
             }
             ++i;
         }

#if 0
         for (size_t i = 0; i < start_vertex.size(); ++i) {
             if (start_vertex[i] == 13) {
                 pgassert(!pred[i].empty());
                 size_t count(0);
                 for (V v = 0 ; v < pred[i].size(); ++v) {
                     count += v != predecessors[v]? 1 : 0;
                 }
                 pgassert(count > 0);
             }
         }
#endif

         /*
          * predecessors of vertices in the set are themselfs
          */
         for (const auto &vertex : start_vertex) {
             for (auto &p : pred) {
                 if (!p.empty() & graph.has_vertex(vertex))
                     p[graph.get_V(vertex)] = graph.get_V(vertex);
             }
         }

#if 0
         for (size_t i = 0; i < start_vertex.size(); ++i) {
             if (start_vertex[i] == 13) {
                 pgassert(!pred[i].empty());
                 size_t count(0);
                 for (V v = 0 ; v < pred[i].size(); ++v) {
                     count += v != predecessors[v]? 1 : 0;
                 }
                 pgassert(count > 0);
             }
         }
#endif

         std::deque<Path> paths;

         /*
          * Creating all the result "paths"
          */
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
                 if (pred[i - 1].empty()) continue;


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
             const std::vector< int64_t > start_vertex,
             double distance) {
         std::deque< std::vector< V > > pred;
         std::deque< std::vector< double > > dist;

         // perform the algorithm
         std::deque<Path> paths;
         for (const auto &vertex : start_vertex) {
             if (execute_drivingDistance(graph, vertex, distance)) {
                 auto path = Path(graph, vertex, distance, predecessors, distances);
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


     //! Call to Dijkstra  1 source to many targets
     bool dijkstra_1_to_many(
             G &graph,
             V source,
             const std::vector< V > &targets) {
         try {
             boost::dijkstra_shortest_paths(graph.graph, source,
                     boost::predecessor_map(&predecessors[0])
                     .weight_map(get(&G::G_T_E::cost, graph.graph))
                     .distance_map(&distances[0])
                     .visitor(dijkstra_many_goal_visitor(targets)));
         } catch(found_goals &) {
             return true;
         } catch (boost::exception const& ex) {
             throw;
         } catch (std::exception &e) {
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
     struct found_goals{};  //!< exception for termination
     std::vector< V > predecessors;
     std::vector< double > distances;
     std::deque< V > nodesInDistance;
     std::ostringstream log;
     //@}

     //! @name Stopping classes
     //@{
     //! class for stopping when 1 target is found
     class dijkstra_one_goal_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_one_goal_visitor(V goal) : m_goal(goal) {}
          template <class B_G>
              void examine_vertex(V &u, B_G &g) {
                  if (u == m_goal) throw found_goals();
                  num_edges(g);
              }
      private:
          V m_goal;
     };

     //! class for stopping when all targets are found
     class dijkstra_many_goal_visitor : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_many_goal_visitor(std::vector< V > goals)
              :m_goals(goals.begin(), goals.end()) {}
          template <class B_G>
              void examine_vertex(V u, B_G &g) {
                  auto s_it = m_goals.find(u);
                  if (s_it == m_goals.end()) return;
                  // we found one more goal
                  m_goals.erase(s_it);
                  if (m_goals.size() == 0) throw found_goals();
                  num_edges(g);
              }
      private:
          std::set< V > m_goals;
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
              void examine_vertex(V u, B_G &g) {
                  if (m_dist[u] > m_distance_goal) {
                      throw found_goals();
                  }
                  m_nodes.push_back(u);
                  num_edges(g);
              }

      private:
          double m_distance_goal;
          std::deque< V > &m_nodes;
          std::vector< double > &m_dist;
     };

     //! class for stopping when a distance/cost has being surpassed
     class dijkstra_distance_visitor_no_init : public boost::default_dijkstra_visitor {
      public:
          explicit dijkstra_distance_visitor_no_init(
                  std::ostringstream &p_log,
                  V source,
                  bool undi,
                  double distance_goal,
                  std::deque< V > &nodesInDistance,
                  std::vector< V > &predecessors,
                  std::vector< double > &distances,
                  std::vector<boost::default_color_type> &color_map) :
              log(p_log),
              first(source),
              m_distance_goal(distance_goal),
              m_nodes(nodesInDistance),
              m_predecessors(predecessors),
              m_dist(distances),
              m_color(color_map) {
                  undirected = undi;
                  pgassert(m_nodes.empty());
                  pgassert(m_distance_goal > 0);
              }

          /*
           * TODO 
           * When examine_vertex, and predecessor[u] == u;
           * Remove @b u from the Queue
           * Because anything after that was captured by a previous vertex
           */

          template <class B_G>
          void examine_vertex(V u, B_G &g) {
              if (m_nodes.empty()) first = u;
              if (m_dist[u] > m_distance_goal) {
#if 0
                  if (undirected && g[first].id == 13 && g[u].id != 13)   {
                      for (const auto n : m_nodes) {
                          log << n << ",";
                      }
                      pgassertwm(m_nodes.size() > 4, log.str().c_str());
                  }
#endif
                  log << "THROWING\n";
                  throw found_goals();
              }
#if 1
              log << "\nEXAMINE VERTEX u" << u << "****************";
              log << "\ng[u].id= " << g[u].id ;
              log << "\nm_color[u]" << m_color[u];
              log << "\nm_predecessors[u]" << m_predecessors[u];
              log << "\nm_color[m_predecessors[u]]" << m_color[m_predecessors[u]];
              if (u != first && m_predecessors[u] == u ){
                   m_color[u]=boost::black_color;
              }
              log << "\nm_color[m_predecessors[u]]" << m_color[m_predecessors[u]];
              v_examined = u;
#if 0
              log << "u= " << u ;
              log << "\nPredecesors\n";
              for (auto p : m_predecessors) {
                  log << g[p].id << ",";
              }
              log << "\nfirst= " << first ;
              log << "\ng[first].id= " << g[first].id ;
#endif

#endif
              m_nodes.push_back(u);
              num_edges(g);
          }

#if 1
          template <class B_G>
          void examine_edge(E e, B_G &g) {
              log << "\nEXAMINE EDGE " << e;
              log << "\n\tg[e].target= " << target(e, g);
              log << "\n\tm_color[target(e, g)]" << m_color[target(e, g)];
              log << "\n\tm_predecessors[" << source(e, g) << "]" << m_predecessors[source(e, g)];
              log << "\n\tm_color[source(e, g)]" << m_color[source(e, g)];
              log << "\n\tsource(e, g)" << source(e, g);
              log << "\n\tfirst" << first;
              if (source(e, g) != first && m_predecessors[source(e,g)] == source(e,g)) {
                   m_color[target(e,g)]=boost::black_color;
              }
              log << "\n\tg[e].target= " << g[target(e, g)].id;
              log << "\n\tm_color[target(e, g)]" << m_color[target(e, g)];
#if 0
              log << "e= " << e ;
              log << "\ng[e].id= " << g[e].id ;
              log << "\ng[e].source= " << g[source(e ,g)];
              log << "\nPredecesors\n";
              for (auto p : m_predecessors) {
                  log << "(" << p << "," << g[p].id << "),";
              }
#endif
          }
#endif

          

#if 1
          template <class B_G>
          void edge_relaxed(E e, B_G &g) {
              log << "\n\t\tRELAXED " << e;
              log << "\n\t\tg[e].target= " << g[target(e, g)].id;
              log << "\n\t\tm_color[target(e, g)]" << m_color[target(e, g)];
#if 0
              log << "e= " << e ;
              log << "\ng[e].id= " << g[e].id ;
              log << "\nPredecesors\n";
              for (auto p : m_predecessors) {
                  log << "(" << p << "," << g[p].id << "),";
              }
#endif
          }
#endif

          

#if 1
          template <class B_G>
          void edge_not_relaxed(E e, B_G &g) {
              log << "\n\t\tNOT RELAXED " << e;
              log << "\n\t\tg[e].target= " << g[target(e, g)].id;
              log << "\n\t\texamined vertex" << g[v_examined];
              log << "\n\t\tm_color[target(e, g)]" << m_color[target(e, g)];
              if (source(e, g) != first && m_predecessors[source(e,g)] == source(e,g)) m_color[target(e,g)]=boost::black_color;
              log << "\n\t\tm_color[target(e, g)]" << m_color[target(e, g)];
#if 0
              log << "e= " << e ;
              log << "\ng[e].id= " << g[e].id ;
              log << "\nPredecesors\n";
              for (auto p : m_predecessors) {
                  log << "(" << p << "," << g[p].id << "),";
              }
              if (m_predecessors[target(e,g)] == target(e,g)) m_color[target(e,g)]=boost::gray_color;
              //if (first != u) pgassertwm(m_predecessors[u] != u, log.str().c_str());
#endif
          }
#endif

          

          template <class B_G>
          void finish_vertex(V u, B_G &g) {
              if (m_nodes.empty()) first = u;
              log << "\nFINISH";
              log << "\ng[u].id= " << g[u].id ;
              log << "\nm_color[u]" << m_color[u];
          }
#if 1
          template <class B_G>
          void discover_vertex(V u, B_G &g) {
              if (m_nodes.empty()) first = u;
              log << "\n\t\t\tDISCOVER u" << u;
              log << "\n\t\t\tg[u].id= " << g[u].id ;
              log << "\n\t\t\tm_color[u]" << m_color[u];
              log << "\n\t\t\tm_predecessors[u]" << m_predecessors[u];
              log << "\n\t\t\tm_color[m_predecessors[u]]" << m_color[m_predecessors[u]];
              if (u  != first && m_predecessors[u] == u) {
                   m_color[u]=boost::black_color;
                   log << "\n\t\t\tm_color[u]" << m_color[u];
              }
#if 0
              log << "u= " << u ;
              log << "\ng[u].id= " << g[u].id ;
              log << "\nPredecesors\n";
              for (auto p : m_predecessors) {
                  log << "(" << p << "," << g[p].id << "),";
              }
              log << "\ng[first].id= " << g[first].id ;
              log << "\nm_color[u]" << m_color[u];
#endif
          }
#endif

      private:
          std::ostringstream &log;
          V first;
          double m_distance_goal;
          std::deque< V > &m_nodes;
          std::vector< V > &m_predecessors;
          std::vector< double > &m_dist;
          bool undirected;
          std::vector<boost::default_color_type> &m_color;
          V v_examined;
     };


     //@}
};


/******************** IMPLEMENTTION ******************/





//! Dijkstra 1 to 1
template < class G >
Path
Pgr_dijkstra< G >::dijkstra(
        G &graph,
        int64_t start_vertex,
        int64_t end_vertex,
        bool only_cost) {
    clear();

    // adjust predecessors and distances vectors
    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());


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
template < class G >
std::deque<Path>
Pgr_dijkstra< G >::dijkstra(
        G &graph,
        int64_t start_vertex,
        const std::vector< int64_t > &end_vertex,
        bool only_cost) {
    // adjust predecessors and distances vectors
    clear();

    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());

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
    dijkstra_1_to_many(graph, v_source, v_targets);

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
template < class G >
std::deque<Path>
Pgr_dijkstra< G >::dijkstra(
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
template < class G >
std::deque<Path>
Pgr_dijkstra< G >::dijkstra(
        G &graph,
        const std::vector< int64_t > &start_vertex,
        const std::vector< int64_t > &end_vertex,
        bool only_cost) {
    // a call to 1 to many is faster for each of the sources
    std::deque<Path> paths;
    for (const auto &start : start_vertex) {
        auto r_paths = dijkstra(graph, start, end_vertex, only_cost);
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





#endif  // INCLUDE_DIJKSTRA_PGR_DIJKSTRA_HPP_
