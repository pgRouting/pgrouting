/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Refactoring
Copyright (c) 2022 Celia Vriginia Vergara Castillo
Mail: vicky_vergara at hotmail.com

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli@gmail.com

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

#include "cpp_common/undirectedNoCostBG.hpp"

#include <set>

#include <boost/graph/max_cardinality_matching.hpp>

#include "c_types/edge_bool_t.h"


namespace pgrouting {
namespace graph {

UndirectedNoCostsBG::UndirectedNoCostsBG(Edge_bool_t *edges, size_t edges_size) {
    std::set<int64_t> vertices;
    for (size_t i = 0; i < edges_size; ++i) {
        vertices.insert(edges[i].source);
        vertices.insert(edges[i].target);
    }
    for (auto id : vertices) {
        V v = add_vertex(graph);
        id_to_V.insert(std::pair<int64_t, V>(id, v));
        V_to_id.insert(std::pair<V, int64_t>(v, id));
    }

    for (size_t i = 0; i < edges_size; ++i) {
        V v1 = get_boost_vertex(edges[i].source);
        V v2 = get_boost_vertex(edges[i].target);
        E e;
        bool added;
        if (edges[i].going) {
            boost::tie(e, added) = boost::add_edge(v1, v2, graph);
            if (added) {E_to_id.insert(std::pair<E, int64_t>(e, edges[i].id));}
        }
    }
}

}  // namespace graph
}  // namespace pgrouting
