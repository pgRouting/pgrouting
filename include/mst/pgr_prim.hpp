/*PGR-GNU*****************************************************************
File: pgr_prim.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Aditya Pratap Singh
adityapratap.singh28@gmail.com

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

#ifndef INCLUDE_MST_PGR_PRIM_HPP_
#define INCLUDE_MST_PGR_PRIM_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/filtered_graph.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_assert.h"

#include "mst/visitors.hpp"
#include "mst/details.hpp"


//******************************************

namespace pgrouting {
namespace {

template <class V>
double prim_aggegrateCost(
        const std::vector<V> &predecessors,
        const std::vector<double> &distances,
        V root_vertex,
        V find_node) {
    double agg_cost = 0;

    for (V i = find_node; i != root_vertex;) {
        auto parent =  predecessors[i];
        auto cost = distances[i] - distances[root_vertex];

        agg_cost += cost;
        i = parent;
    }
    return agg_cost;
}

}  // namespace visitors

namespace functions {

template <class G>
class Pgr_prim {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::B_G B_G;

     std::vector<pgr_mst_rt> operator() (
                 G &graph,
                 int64_t root_vertex,
                 bool use_root);

     std::vector<pgr_mst_rt> prim(G &graph);

     std::vector<pgr_mst_rt> primBFS(
             G &graph,
             std::vector<int64_t> roots,
             int64_t max_depth);

     std::vector<pgr_mst_rt> primDFS(
             G &graph,
             std::vector<int64_t> roots,
             int64_t max_depth);

     std::vector<pgr_mst_rt> primDD(
             G &graph,
             std::vector<int64_t> roots,
             double distance);


 private:
     // Functions
     void clear() {
         data.clear();
         predecessors.clear();
         distances.clear();
     }

     void resize(const G &graph);

     std::vector<pgr_mst_rt> generatePrim(
             const G &graph,
             int64_t root_vertex );

     std::vector<pgr_mst_rt> generatePrim(
             const G &graph);

     void calculate_component(const G &graph);
     std::vector< pgr_mst_rt > disconnectedPrim(const G &graph);

 private:
     // Member
     std::vector<V> predecessors;
     std::vector<double> distances;
     std::vector<V> data;

     std::string m_suffix;
     bool m_get_component;
     int  m_order_by;
     std::vector<int64_t> m_roots;
     int64_t  m_max_depth;
     double  m_distance;
     std::vector<pgr_mst_rt> m_results;
     std::set<V> m_unassigned;
     std::set<V> m_assigned;
     struct InSpanning {
         std::set<E> edges;
         bool operator()(E e) const { return edges.count(e); }
         void clear() { edges.clear(); }
     } m_spanning_tree;
     std::vector<E> m_added_order;

     /** @brief ordering of the results
      *
      * Orders by
      * - no order
      * - DFS
      * - BFS
      */
     std::vector<pgr_mst_rt> order_results(const G &graph);

     std::vector<size_t> m_components;
     std::vector<int64_t> m_tree_id;
     template <typename T>
     std::vector<pgr_mst_rt> get_results(
             T order,
             int64_t root,
             const G &graph);

};


template <class G>
void
Pgr_prim<G>::resize(const G &graph) {
    predecessors.resize(graph.num_vertices());
    distances.resize(graph.num_vertices());
}


template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::generatePrim(
        const G &graph) {
    for (auto r : m_roots) {
        auto result = generatePrim(graph, r);
        m_results.insert(m_results.end(), result.begin(), result.end());
    }
    return m_results;
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::generatePrim(
        const G &graph,
        int64_t root_vertex ) {
    clear();
    resize(graph);
    auto v_root(graph.get_V(root_vertex));

    using prim_visitor = visitors::Prim_visitor<V>;
    boost::prim_minimum_spanning_tree(
            graph.graph,
            &predecessors[0],
            boost::distance_map(&distances[0]).
            weight_map(get(&G::G_T_E::cost, graph.graph))
            .root_vertex(v_root)
            .visitor(prim_visitor(data)));

    std::vector<pgr_mst_rt> results;

    for (const auto v : data) {
        /*
         * its not a tree, its a forest
         * - v is not on current tree
         */
        if (isinf(distances[v])) continue;
        m_unassigned.erase(v);


        auto u = predecessors[v];

        /*
         * Not a valid edge
         */
        if (u == v) continue;

        auto cost = distances[u] - distances[v];
        auto edge = graph.get_edge(u, v, cost);
        m_added_order.push_back(edge);
    }
    return std::vector<pgr_mst_rt>();
}


template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::disconnectedPrim(const G &graph) {
    size_t totalNodes = num_vertices(graph.graph);

    m_unassigned.clear();
    for (V v = 0; v < totalNodes; ++v) {
            m_unassigned.insert(m_unassigned.end(), v);
    }

    std::vector<pgr_mst_rt> results;
    while (!m_unassigned.empty()) {
        auto root = *m_unassigned.begin();
        m_unassigned.erase(m_unassigned.begin());
        auto tmpresults = generatePrim(
                graph,
                graph.graph[root].id);
        results.insert(results.end(), tmpresults.begin(), tmpresults.end());
    }

    m_results = order_results(graph);
    return m_results;
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::prim(
        G &graph) {
    m_suffix = "";
    m_order_by = 0;
    m_get_component = false;
    m_distance = -1;
    m_max_depth = -1;
    m_roots.clear();

    return disconnectedPrim(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primBFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
    m_suffix = "BFS";
    m_order_by = 2;
    m_get_component = true;
    m_distance = -1;
    m_max_depth = max_depth;
    m_roots = details::clean_vids(roots);

    return m_roots.empty()? disconnectedPrim(graph)
        : generatePrim(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primDFS(
        G &graph,
        std::vector<int64_t> roots,
        int64_t max_depth) {
    m_suffix = "DFS";
    m_order_by = 1;
    m_get_component = false;
    m_distance = -1;
    m_max_depth = max_depth;
    m_roots = details::clean_vids(roots);

    return m_roots.empty()? disconnectedPrim(graph)
        : generatePrim(graph);
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::primDD(
        G &graph,
        std::vector<int64_t> roots,
        double distance) {
    m_suffix = "DD";
    m_order_by = 1;
    m_get_component = false;
    m_distance = distance;
    m_max_depth = -1;
    m_roots = details::clean_vids(roots);

    return generatePrim(graph);
}


template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::operator() (
        G &graph,
        int64_t root_vertex,
        bool use_root) {
    if (!use_root) {
        return disconnectedPrim(graph);
    }

    if (!graph.has_vertex(root_vertex)) {
        return std::vector<pgr_mst_rt>();
    }

    return generatePrim(
            graph,
            root_vertex);
}

template <class G>
template <typename T>
std::vector<pgr_mst_rt>
Pgr_prim<G>::get_results(
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
        if (m_order_by && depth[u] == 0 && depth[v] == 0) {
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
                    m_order_by? depth[v] : 0,
                    graph[v].id,
                    graph[edge].id,
                    graph[edge].cost,
                    m_order_by? agg_cost[v] : 0.0
            });
        }
    }
    return results;
}

template <class G>
std::vector<pgr_mst_rt>
Pgr_prim<G>::order_results(const G &graph) {
    /*
     * No particular order
     */
    if (m_order_by == 0) {
        return get_results(m_added_order, 0, graph);
    }



    typedef typename G::B_G B_G;
    typedef typename G::E E;
    typedef typename G::V V;

    m_spanning_tree.edges.insert(m_added_order.begin(), m_added_order.end());
    boost::filtered_graph<B_G, InSpanning, boost::keep_all>
        mst(graph.graph, m_spanning_tree, {});

    /*
     * order by dfs
     */
    if (m_roots.empty() && m_order_by == 1) {
        std::vector<E> visited_order;

        using dfs_visitor = visitors::Dfs_visitor<E>;
        boost::depth_first_search(mst, visitor(dfs_visitor(visited_order)));

        return get_results(visited_order, 0, graph);
    }
    if (!m_roots.empty() && m_order_by == 1) {
        std::vector<pgr_mst_rt> results;
        for (const auto root : m_roots) {
            std::vector<E> visited_order;

            using dfs_visitor = visitors::Dfs_visitor_with_root<V, E>;
            if (graph.has_vertex(root)) {
                try {
                    boost::depth_first_search(
                            mst,
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

    /*
     * order by bfs
     */
    calculate_component(graph);

    std::vector<int64_t> roots;
    if (!m_roots.empty()) {
        roots = m_roots;
    } else {
        roots =  m_tree_id;
    }

    using bfs_visitor = visitors::Bfs_visitor<E>;
    for (auto root : roots) {
        std::vector<E> visited_order;
        if (graph.has_vertex(root)) {
            boost::breadth_first_search(mst,
                    graph.get_V(root),
                    visitor(bfs_visitor(visited_order)));

            auto results = get_results(visited_order, root, graph);
            m_results.insert(m_results.end(), results.begin(), results.end());
        } else {
            m_results.push_back({root, 0, root, -1, 0.0, 0.0});
        }
    }

    return m_results;
}

template <class G>
void
Pgr_prim<G>::calculate_component(const G &graph) {
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



}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_MST_PGR_PRIM_HPP_
