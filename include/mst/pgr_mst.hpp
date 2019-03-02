/*PGR-GNU*****************************************************************
File: pgr_mst.hpp

Copyright (c) 2018 Vicky Vergara
mail: vicky at georepublic dot de

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

#ifndef INCLUDE_MST_PGR_MST_HPP_
#define INCLUDE_MST_PGR_MST_HPP_
#pragma once

#include <visitors/dfs_visitor_with_root.hpp>
#include <visitors/edges_order_bfs_visitor.hpp>
#include <visitors/edges_order_dfs_visitor.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/filtered_graph.hpp>

#include <set>
#include <utility>
#include <string>
#include <vector>

#include "cpp_common/pgr_base_graph.hpp"
#include "mst/details.hpp"

namespace pgrouting {
namespace functions {

template <class G>
class Pgr_mst {
 protected:
     typedef typename G::B_G B_G;
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;


 protected:
     virtual void generate_mst(const G &graph) = 0;

     void clear() {
         this->m_spanning_tree.clear();
         this->m_components.clear();
         this->m_tree_id.clear();
     }

     std::vector<pgr_mst_rt>
     no_order(const G &graph) {
         return no_ordering(graph);
     }

     std::vector<pgr_mst_rt>
     dfs_order(const G &graph) {
         return dfs_ordering(graph);
     }

     std::vector<pgr_mst_rt>
     bfs_order(const G &graph) {
         return bfs_ordering(graph);
     }

     std::vector<pgr_mst_rt> mst(const G &graph) {
         m_suffix = "";
         m_get_component = false;
         m_distance = -1;
         m_max_depth = -1;
         m_roots.clear();

         no_neg_costs(graph);
         this->generate_mst(graph);
         return no_order(graph);
     }


     std::vector<pgr_mst_rt> mstBFS(
             const G &graph,
             std::vector<int64_t> roots,
             int64_t max_depth) {
         m_suffix = "BFS";
         m_get_component = true;
         m_distance = -1;
         m_max_depth = max_depth;
         m_roots = details::clean_vids(roots);

         this->generate_mst(graph);
         return bfs_order(graph);
     }

     std::vector<pgr_mst_rt> mstDFS(
             const G &graph,
             std::vector<int64_t> roots,
             int64_t max_depth) {
         m_suffix = "DFS";
         m_get_component = false;
         m_distance = -1;
         m_max_depth = max_depth;
         m_roots = details::clean_vids(roots);

         this->generate_mst(graph);
         return dfs_order(graph);
     }

     std::vector<pgr_mst_rt> mstDD(
             const G &graph,
             std::vector<int64_t> roots,
             double distance) {
         m_suffix = "DD";
         m_get_component = false;
         m_distance = distance;
         m_max_depth = -1;
         m_roots = details::clean_vids(roots);

         this->generate_mst(graph);
         return dfs_order(graph);
     }

 protected:
     std::vector<int64_t> m_roots;
     bool m_get_component;
     int64_t  m_max_depth;
     double  m_distance;

     struct InSpanning {
         std::set<E> edges;
         bool operator()(E e) const { return edges.count(e); }
         void clear() { edges.clear(); }
     } m_spanning_tree;


     /** m_components[v]:
      *  - is empty (when m_get_component = 0)
      *  - has the component number of vertex v (when m_get_component != 0)
      */
     std::vector<size_t> m_components;

     /**
      * Stores which function is being executed.
      *
      * TODO change to enum
      */
     std::string m_suffix;

     /** m_tree_id[v]:
      *  - is empty (when m_get_component = 0)
      *  - has the component number of vertex v (when m_get_component = 1)
      *  - has the min vertex id that belongs to the component (when m_get_component = 2)
      */
     std::vector<int64_t> m_tree_id;

 private:
     template <typename T>
     std::vector<pgr_mst_rt> get_results(
             T order,
             int64_t p_root,
             const G &graph) {
         std::vector<pgr_mst_rt> results;

         std::vector<double> agg_cost(graph.num_vertices(), 0);
         std::vector<int64_t> depth(graph.num_vertices(), 0);
         int64_t root(p_root);

         for (const auto edge : order) {
             auto u = graph.source(edge);
             auto v = graph.target(edge);
             if (depth[u] == 0 && depth[v] != 0) {
                 std::swap(u, v);
             }

             auto component = m_get_component? m_tree_id[m_components[u]] : 0;
             if (m_suffix != "" && depth[u] == 0 && depth[v] == 0) {
                 if (!m_roots.empty() && graph[u].id != root) std::swap(u, v);
                 if (m_roots.empty() && graph[u].id != component) std::swap(u, v);
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

             agg_cost[v] = agg_cost[u] + graph[edge].cost;
             depth[v] = depth[u] == -1? 1 : depth[u] + 1;

             if ((m_suffix == "")
                     || ((m_suffix == "BFS")   && (m_max_depth >= depth[v]))
                     || ((m_suffix == "DFS")  && m_max_depth >= depth[v])
                     || ((m_suffix == "DD")  && m_distance >= agg_cost[v])) {
                 results.push_back({
                     root,
                         m_suffix != ""? depth[v] : 0,
                         graph[v].id,
                         graph[edge].id,
                         graph[edge].cost,
                         m_suffix != ""? agg_cost[v] : 0.0
                 });
             }
         }
         return results;
     }


     void calculate_component(const G &graph) {
         if (!m_get_component) return;

         m_components.resize(num_vertices(graph.graph));

         /*
          * Calculate connected components
          *
          * Number of components of graph: num_comps components
          */
         auto num_comps = boost::connected_components(
                 graph.graph,
                 &m_components[0]);

         m_tree_id.resize(num_comps, 0);

         for (const auto v : boost::make_iterator_range(vertices(graph.graph))) {
             m_tree_id[m_components[v]] =
                 (m_tree_id[m_components[v]] == 0
                  || m_tree_id[m_components[v]] >= graph[v].id) ?
                 graph[v].id : m_tree_id[m_components[v]];
         }
     }

     std::vector<pgr_mst_rt>
         no_ordering(const G &graph) {
             return get_results(m_spanning_tree.edges, 0, graph);
         }

     std::vector<pgr_mst_rt>
         dfs_forest(const G &graph) {
             boost::filtered_graph<B_G, InSpanning, boost::keep_all>
                 mstGraph(graph.graph, m_spanning_tree, {});
             std::vector<E> visited_order;

             using dfs_visitor = visitors::Edges_order_dfs_visitor<E>;
             try {
                 boost::depth_first_search(mstGraph, visitor(dfs_visitor(visited_order)));
             } catch (boost::exception const& ex) {
                 (void)ex;
                 throw;
             } catch (std::exception &e) {
                 (void)e;
                 throw;
             } catch (...) {
                 throw;
             }

             return get_results(visited_order, 0, graph);
         }


     std::vector<pgr_mst_rt>
         dfs_ordering(const G &graph) {
             boost::filtered_graph<B_G, InSpanning, boost::keep_all>
                 mstGraph(graph.graph, m_spanning_tree, {});

             if (m_roots.empty()) {
                 return dfs_forest(graph);
             } else {
                 std::vector<pgr_mst_rt> results;
                 for (const auto root : m_roots) {
                     std::vector<E> visited_order;

                     using dfs_visitor = visitors::Dfs_visitor_with_root<V, E>;
                     if (graph.has_vertex(root)) {
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
                 }
                 return results;
             }
         }

     std::vector<pgr_mst_rt>
     bfs_ordering(const G &graph) {
         std::vector<pgr_mst_rt> results;
         /*
          * order by bfs
          */
         boost::filtered_graph<B_G, InSpanning, boost::keep_all>
             mst(graph.graph, m_spanning_tree, {});

         calculate_component(graph);

         std::vector<int64_t> roots;
         if (!m_roots.empty()) {
             roots = m_roots;
         } else {
             roots =  m_tree_id;
         }

         using bfs_visitor = visitors::Edges_order_bfs_visitor<E>;
         for (auto root : roots) {
             std::vector<E> visited_order;
             if (graph.has_vertex(root)) {
                 boost::breadth_first_search(mst,
                         graph.get_V(root),
                         visitor(bfs_visitor(visited_order)));

                 auto tree_results = get_results(visited_order, root, graph);
                 results.insert(results.end(), tree_results.begin(), tree_results.end());
             } else {
                 results.push_back({root, 0, root, -1, 0.0, 0.0});
             }
         }

         return results;
     }

     bool no_neg_costs(const G &graph) {
         E_i  ei, ei_end;
         for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ++ei)
             pgassert(graph[*ei].cost > 0);
         return true;
     }
};

}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_MST_PGR_MST_HPP_
