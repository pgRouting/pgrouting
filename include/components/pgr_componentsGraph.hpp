/*PGR-GNU*****************************************************************
File: pgr_componentsGraph.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Maoguang Wang 
Mail: xjtumg1007@gmail.com

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

#ifndef INCLUDE_COMPONENTS_PGR_COMPONENTSGRAPH_HPP_
#define INCLUDE_COMPONENTS_PGR_COMPONENTSGRAPH_HPP_
#pragma once


#include <vector>

#include "cpp_common/pgr_base_graph.hpp"


namespace pgrouting {

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_componentsGraph;

}  // namespace graph

typedef graph::Pgr_componentsGraph <
boost::adjacency_list < boost::vecS, boost::vecS,
    boost::undirectedS,
    Basic_vertex, Basic_edge >,
    Basic_vertex, Basic_edge > ComponentsUndiGraph;

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_componentsGraph : public Pgr_base_graph<G, T_V, T_E> {
 public:
     G graph;

     explicit Pgr_componentsGraph< G, T_V, T_E >(graphType gtype)
         : Pgr_base_graph< G, T_V, T_E >(gtype) {
         }

     template < typename T >
         void insert_edges(const T *edges, int64_t count) {
             insert_edges(std::vector < T >(edges, edges + count));
         }

     template <typename T >
         void insert_edges(const std::vector < T > &edges) {
             for (const auto edge : edges) {
                 graph_add_edge(edge);
             }
         }

 private:
    template < typename T >
    void
    graph_add_edge(const T &edge) {
        bool inserted;
        typename Pgr_base_graph< G, T_V, T_E >::E e;
        if ((edge.cost < 0) && (edge.reverse_cost < 0))
            return;

        /*
         * true: for source
         * false: for target
         */
        auto vm_s = Pgr_base_graph< G, T_V, T_E >::get_V(T_V(edge, true));
        auto vm_t = Pgr_base_graph< G, T_V, T_E >::get_V(T_V(edge, false));

        pgassert((Pgr_base_graph< G, T_V, T_E >::vertices_map).find(edge.source)
                 !=
                 (Pgr_base_graph< G, T_V, T_E >::vertices_map).end());
        pgassert((Pgr_base_graph< G, T_V, T_E >::vertices_map).find(edge.target)
                 !=
                 (Pgr_base_graph< G, T_V, T_E >::vertices_map).end());
        if (edge.cost >= 0) {
            boost::tie(e, inserted) =
                boost::add_edge(vm_s, vm_t, graph);
            graph[e].cost = edge.cost;
            graph[e].id = edge.id;
        } else if (edge.reverse_cost >= 0) {
            boost::tie(e, inserted) =
                boost::add_edge(vm_t, vm_s, graph);
            graph[e].cost = edge.reverse_cost;
            graph[e].id = edge.id;
        }
    }
};

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_PGR_COMPONENTSGRAPH_HPP_
