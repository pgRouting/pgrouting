/*PGR-GNU*****************************************************************

file: drivingDist.hpp

Generated with Template by:                                                                                             
Copyright (c) 2023 pgRouting developers                                                                                 
Mail: project AT pgrouting.org   

Copyright (c) 2023 Aryan Gupta
guptaaryan1010 AT gmail.com

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

#include <visitors/dfs_visitor_with_root.hpp>
#include <visitors/edges_order_dfs_visitor.hpp>
#include <boost/graph/filtered_graph.hpp>

#include"c_types/mst_rt.h"



namespace pgrouting {
// TODO(gsoc) namespace algorithm
// TODO(gsoc) convert to functions

template < class G > class Pgr_dijkstra;
// user's functions
// for development

template < class G >
std::deque<Path>
pgr_drivingdistance(
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
          * TODO(gsoc)
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
/* This code has been wrote with reference to pgr_mst.hpp*/
namespace functions {

template <class G>
class ShortestPath_tree{
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::B_G B_G;


 public:
     std::deque<MST_rt> get_depths(
             G&,
             std::deque<Path>);


 private:
     /* Functions */

     template <typename T>
     std::deque<MST_rt> get_results(
             T,
             int64_t,
             const G&);

     std::deque<MST_rt> dfs_order(
             const G&,
             int64_t);

     void get_edges_from_path(
             const G&,
             const Path);


 private:
     /* Member */
     Path m_path;

     struct InSpanning {
         std::set<E> edges;
         bool operator()(E e) const { return edges.count(e); }
         void clear() { edges.clear(); }
     } m_spanning_tree;
};


template <class G>
template <typename T>
std::deque<MST_rt>
ShortestPath_tree<G>::get_results(
        T order,
        int64_t p_root,
        const G &graph) {
    std::deque<MST_rt> results;

    std::vector<int64_t> depth(graph.num_vertices(), 0);
    int64_t root(p_root);

    for (const auto edge : order) {
        auto u = graph.source(edge);
        auto v = graph.target(edge);
        if (depth[u] == 0 && depth[v] != 0) {
            std::swap(u, v);
        }

        if (depth[u] == 0 && depth[v] == 0) {
        if (graph[u].id != root) {
            std::swap(u, v);
        }
            if (!p_root && graph[u].id > graph[v].id) std::swap(u, v);

            root = p_root? p_root: graph[u].id;
            depth[u] = -1;
            results.push_back({
                root,
                    0,
                    graph[u].id,
                    -1,
                    0.0,
                    0.0 });
        }
        depth[v] = depth[u] == -1? 1 : depth[u] + 1;

        int64_t node_id = graph[v].id;
        auto path_index = std::find_if(m_path.begin(), m_path.end(),
                [&node_id](Path_t &path_item)
                {return node_id == path_item.node;});

        if (graph[v].id < 0) depth[v] = depth[u];
        if (graph[v].id > 0) {
            results.push_back({
                root,
                    depth[v],
                    path_index->node,
                    path_index->edge,
                    path_index->cost,
                    path_index->agg_cost
            });
        }
    }
    return results;
}


template <class G>
std::deque<MST_rt>
ShortestPath_tree<G>::dfs_order(const G &graph, int64_t root) {
        boost::filtered_graph<B_G, InSpanning, boost::keep_all>
            mstGraph(graph.graph, m_spanning_tree, {});

        std::deque<MST_rt> results;
        std::vector<E> visited_order;

        using dfs_visitor = visitors::Dfs_visitor_with_root<V, E>;
        if (graph.has_vertex(root)) {
            /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
            CHECK_FOR_INTERRUPTS();
            try {
                boost::depth_first_search(
                        mstGraph,
                        visitor(dfs_visitor(graph.get_V(root), visited_order))
                        .root_vertex(graph.get_V(root)));
            } catch(found_goals &) {
                {}
            } catch (boost::exception const& ex) {
                (void)ex;
                throw;
            } catch (std::exception &e) {
                (void)e;
                throw;
            } catch (...) {
                throw;
            }
            auto result = get_results(visited_order, root, graph);
            results.insert(results.end(), result.begin(), result.end());
        } else {
            results.push_back({root, 0, root, -1, 0.0, 0.0});
        }

        return results;
    }


template <class G>
void
ShortestPath_tree<G>::get_edges_from_path(
         const G& graph,
         const Path path) {
    m_spanning_tree.clear();

    for (size_t i = 0; i < path.size(); i++) {
        if (graph.has_vertex(path[i].node)) {
        auto u = graph.get_V(path[i].node);

        for (size_t j = i+1; j < path.size(); j++) {
            if (graph.has_vertex(path[i].node)) {
            auto v = graph.get_V(path[j].node);
            double cost = path[j].cost;
            auto edge = graph.get_edge(u, v, cost);
            if (graph.target(edge) == v
                    && path[i].agg_cost+cost == path[j].agg_cost
                    && graph[edge].id == path[j].edge) {
                this->m_spanning_tree.edges.insert(edge);
            }
            }
            }
        }
    }
}


template <class G>
std::deque<MST_rt>
ShortestPath_tree<G>::get_depths(
        G &graph,
        std::deque<Path> paths) {
    std::deque<MST_rt> results;

    for (const Path& path : paths) {
        m_path = path;
        get_edges_from_path(graph, path);
        int64_t root = path.start_id();
        auto result = this->dfs_order(graph, root);

        std::sort(result.begin(), result.end(),
                [](const MST_rt &l, const  MST_rt &r)
                {return l.node < r.node;});
        std::stable_sort(result.begin(), result.end(),
                [](const MST_rt &l, const  MST_rt &r)
                {return l.agg_cost < r.agg_cost;});

        results.insert(results.end(), result.begin(), result.end());
    }
    return results;
}


}  // namespace functions
}  // namespace pgrouting


#endif  // INCLUDE_DIJKSTRA_DRIVEDIST_HPP_
