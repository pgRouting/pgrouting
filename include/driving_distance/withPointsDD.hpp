/*PGR-GNU*****************************************************************
File: withPointsDD.hpp

Generated with Template by:
Copyright (c) 2023 pgRouting developers
Mail: project at pgrouting.org

Function's developer:
Copyright (c) 2023 Yige Huang
Mail: square1ge at gmail.com
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
#ifndef INCLUDE_DRIVING_DISTANCE_WITHPOINTSDD_HPP_
#define INCLUDE_DRIVING_DISTANCE_WITHPOINTSDD_HPP_
#pragma once

#include <algorithm>
#include <deque>
#include <set>
#include <vector>
#include <utility>

#include <visitors/dfs_visitor_with_root.hpp>
#include <visitors/edges_order_dfs_visitor.hpp>
#include <boost/graph/filtered_graph.hpp>

#include"c_types/mst_rt.h"


namespace pgrouting {
namespace functions {

template <class G>
class ShortestPath_tree{
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::B_G B_G;


 public:
     std::deque<MST_rt> get_depths(
             G&,
             pgrouting::Pg_points_graph&,
             std::deque<Path>,
             bool);


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
     bool m_details;
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
            if (graph[u].id != root) std::swap(u, v);
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

        if (!m_details && graph[v].id < 0) depth[v] = depth[u];
        if (m_details || graph[v].id > 0) {
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
        auto u = graph.get_V(path[i].node);

        for (size_t j = i+1; j < path.size(); j++) {
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


template <class G>
std::deque<MST_rt>
ShortestPath_tree<G>::get_depths(
        G &graph,
        pgrouting::Pg_points_graph & points_graph,
        std::deque<Path> paths,
        bool details) {
    m_details = details;
    std::deque<MST_rt> results;

    for (const Path& path : paths) {
        m_path = path;
        get_edges_from_path(graph, path);
        if (!m_details) points_graph.eliminate_details_dd(m_path);

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


#endif  // INCLUDE_DRIVING_DISTANCE_WITHPOINTSDD_HPP_
