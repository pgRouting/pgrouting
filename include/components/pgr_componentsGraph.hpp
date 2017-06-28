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


#include <limits>
#include <algorithm>
#include <vector>

#include "cpp_common/pgr_base_graph.hpp"


namespace pgrouting {

namespace componentsGraph {

template <class G, typename T_V, typename T_E>
class Pgr_componentsGraph;

}

namespace componentsGraph {

template <class G, typename T_V, typename T_E>
class Pgr_componentsGraph : public Pgr_base_graph<G, T_V, T_E> {
 public:
     typedef typename boost::graph_traits < G >::vertex_descriptor V;
     typedef typename boost::graph_traits < G >::edge_descriptor E;
     typedef typename boost::graph_traits < G >::vertex_iterator V_i;
     typedef typename boost::graph_traits < G >::edge_iterator E_i;
     typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
     typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;
     typedef typename boost::graph_traits < G >::degree_size_type
         degree_size_type;

 private:
    template < class G, typename T_V, typename T_E >
    template < typename T >
    void
    Pgr_base_graph< G, T_V, T_E >::graph_add_edge(const T &edge) {
        bool inserted;
        typename Pgr_base_graph< G, T_V, T_E >::E e;
        if ((edge.cost < 0) && (edge.reverse_cost < 0))
            return;

        /*
         * true: for source
         * false: for target
         */
        auto vm_s = get_V(T_V(edge, true));
        auto vm_t = get_V(T_V(edge, false));

        pgassert(vertices_map.find(edge.source) != vertices_map.end());
        pgassert(vertices_map.find(edge.target) != vertices_map.end());
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

}  // namespace componentsGraph
}  // namespace pgrouting

#endif // INCLUDE_COMPONENTS_PGR_COMPONENTSGRAPH_HPP_
