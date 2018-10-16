/*PGR-GNU*****************************************************************
File: pgr_kruskal.hpp

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

#ifndef INCLUDE_MST_PGR_KRUSKAL_HPP_
#define INCLUDE_MST_PGR_KRUSKAL_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/filtered_graph.hpp>

#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"

namespace pgrouting {

namespace  visitors {

template <class E>
class Bfs_visitor : public boost::default_bfs_visitor {
    public:
        explicit Bfs_visitor(
                std::vector<E> &data) :
            m_data(data)  {}
        template <class B_G>
            void tree_edge(E e, const B_G&) {
                m_data.push_back(e);
            }
    private:
        std::vector<E> &m_data;
};

template <class E>
class Dfs_visitor : public boost::default_dfs_visitor {
    public:
        explicit Dfs_visitor(
                std::vector<E> &data) :
            m_data(data)  {}
        template <typename B_G>
            void tree_edge(E e, const B_G&) {
                m_data.push_back(e);
            }
    private:
        std::vector<E> &m_data;
};

}  // namespace visitors



namespace functions {

template <class G>
class Pgr_kruskal {
 public:
     std::vector<pgr_kruskal_t> operator() (
             G &graph,
             int64_t root,
             int m_order_by);

     std::vector<pgr_kruskal_t> operator() (
             G &graph,
             int m_order_by,
             bool m_get_component);

 private:
     typedef typename G::B_G B_G;
     typedef typename G::V V;
     typedef typename G::E E;

     /* Does all the work */
     std::vector<pgr_kruskal_t> generateKruskal(G &graph);

     /* @brief maps component number with smallest original vertex id */
     void calculate_component(const G &graph);

     /** @brief ordering of the results
      *
      * Orders by
      * - no order
      * - DFS
      * - BFS
      */
     std::vector<pgr_kruskal_t> order_results(const G &graph);

     template <typename T>
     std::vector<pgr_kruskal_t> get_results(
             T order,
             const G &graph);

 private:
     bool m_get_component;
     int  m_order_by;
     int64_t  m_root;
     bool  m_use_root;

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
     /** m_tree_id[v]:
      *  - is empty (when m_get_component = 0)
      *  - has the component number of vertex v (when m_get_component = 1)
      *  - has the min vertex id that belongs to the component (when m_get_component = 2)
      */
     std::vector<int64_t> m_tree_id;
     std::vector<pgr_kruskal_t> m_results;

};

/* IMPLEMENTATION */

template <class G>
template <typename T>
std::vector<pgr_kruskal_t>
Pgr_kruskal<G>::get_results(
        T order,
        const G &graph) {
    std::vector<pgr_kruskal_t> results;
    std::vector<double> agg_cost(graph.num_vertices(), 0);
    std::vector<int64_t> depth(graph.num_vertices(), 0);

    for (const auto edge : order) {
        auto u = graph.source(edge);
        auto v = graph.target(edge);
        if (depth[u] == 0 && depth[v] != 0) {
            std::swap(u, v);
        }

        auto component = m_get_component? m_tree_id[m_components[u]] : 0;
        if (m_order_by && depth[u] == 0 && depth[v] == 0) {
            if (m_use_root && graph[u].id != m_root) std::swap(u, v);
            if (!m_use_root && graph[u].id != component) std::swap(u, v);

            depth[u] = 1;
            results.push_back({
                component,
                m_order_by? depth[u] : 0,
                    graph[u].id,
                    -1,
                    0.0,
                    0.0 });
        }

        agg_cost[v] = agg_cost[u] + graph[edge].cost;
        depth[v] = depth[u] + 1;

        results.push_back({
            component,
                m_order_by? depth[v] : 0,
                graph[v].id,
                graph[edge].id,
                graph[edge].cost,
                m_order_by? agg_cost[v] : 0.0
        });
    }
    return results;
}


template <class G>
void
Pgr_kruskal<G>::calculate_component(const G &graph) {
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



template <class G>
std::vector<pgr_kruskal_t>
Pgr_kruskal<G>::order_results(const G &graph) {
    /*
     * No particular order
     */
    if (m_order_by == 0) {
        return get_results(m_spanning_tree.edges, graph);
    }



    typedef typename G::B_G B_G;
    typedef typename G::E E;

    boost::filtered_graph<B_G, InSpanning, boost::keep_all> mst(graph.graph, m_spanning_tree, {});

    /*
     * order by dfs
     */
    if (m_order_by == 1 ) {
        std::vector<E> visited_order;

        using dfs_visitor = visitors::Dfs_visitor<E>;
        boost::depth_first_search(mst, visitor(dfs_visitor(visited_order)));

        return get_results(visited_order, graph);
    }

    std::vector<int64_t> roots;
    if (m_use_root) {
        roots.push_back(m_root);
    } else {
        roots =  m_tree_id;
    }
    /*
     * order by bfs
     */
    using bfs_visitor = visitors::Bfs_visitor<E>;
    for (auto root : roots) {
        std::vector<E> visited_order;
        boost::breadth_first_search(mst,
                graph.get_V(root),
                visitor(bfs_visitor(visited_order)));

        auto results = get_results(visited_order, graph);
        m_results.insert(m_results.end(), results.begin(), results.end());
    }

    return m_results;
}



template <class G>
std::vector<pgr_kruskal_t>
Pgr_kruskal<G>::generateKruskal(G &graph) {
    m_spanning_tree.clear();
    m_components.clear();
    m_results.clear();
    m_tree_id.clear();

    boost::kruskal_minimum_spanning_tree(
            graph.graph,
            std::inserter(m_spanning_tree.edges, m_spanning_tree.edges.end()),
            boost::weight_map(get(&G::G_T_E::cost, graph.graph)));

    calculate_component(graph);

    m_results = order_results(graph);
    return m_results;
}

template <class G>
std::vector<pgr_kruskal_t>
Pgr_kruskal<G>::operator() (
        G &graph,
        int order_by,
        bool get_component) {
    m_order_by = order_by;
    m_get_component = get_component || (m_order_by == 2);
    m_use_root = false;
    return generateKruskal(graph);
}

template <class G>
std::vector<pgr_kruskal_t>
Pgr_kruskal<G>::operator() (
        G &graph,
        int64_t root,
        int order_by) {
    m_root = root;
    m_order_by = order_by;
    m_get_component = true;
    m_use_root = true;
    return generateKruskal(graph);
}


}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_MST_PGR_KRUSKAL_HPP_
