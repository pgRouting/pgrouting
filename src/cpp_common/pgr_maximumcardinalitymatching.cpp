/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Removing directed flag & works only for undirected graph
Copyright (c) 2022 Celia Vriginia Vergara Castillo
Mail: vicky at georepublic.mail

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

#include "max_flow/pgr_maximumcardinalitymatching.hpp"
#include <vector>
#include <utility>
#include <set>
#include <algorithm>

#include <boost/graph/max_cardinality_matching.hpp>

#include "c_types/edge_bool_t_rt.h"


namespace pgrouting {
namespace flow {

MaxCardinalityMatch::MaxCardinalityMatch(Edge_bool_t_rt *data_edges, size_t total_tuples) {
    std::set<int64_t> vertices;
    for (size_t i = 0; i < total_tuples; ++i) {
        vertices.insert(data_edges[i].source);
        vertices.insert(data_edges[i].target);
    }
    for (int64_t id : vertices) {
        V v = add_vertex(boost_graph);
        id_to_V.insert(std::pair<int64_t, V>(id, v));
        V_to_id.insert(std::pair<V, int64_t>(v, id));
    }
    bool added;

    for (size_t i = 0; i < total_tuples; ++i) {
        V v1 = get_boost_vertex(data_edges[i].source);
        V v2 = get_boost_vertex(data_edges[i].target);
        E e1;
        if (data_edges[i].going) {
            boost::tie(e1, added) = boost::add_edge(v1, v2, boost_graph);
            E_to_id.insert(std::pair<E, int64_t>(e1, data_edges[i].id));
        }
    }
}

std::vector<int64_t>
MaxCardinalityMatch::get_matched_vertices() {

    std::vector<V> mate_map(boost::num_vertices(boost_graph));
    std::vector<int64_t> matched_vertices;
    edmonds_maximum_cardinality_matching(boost_graph,
            &mate_map[0]);

    V_it vi, vi_end;
    E e;
    bool exists;

    /*
     * Check for each vertex:
     * 1) It is matched with a non-null vertex
     * 2) An edge exists from this vertex to his mate
     * 3) The vertices have not been matched already
     * (this last point prevents having double output with reversed
     * source and target)
     */
    for (boost::tie(vi, vi_end) = boost::vertices(boost_graph);
            vi != vi_end;
            ++vi) {
        boost::tie(e, exists) =
            boost::edge(*vi, mate_map[*vi], boost_graph);
        if (((uint64_t)mate_map[*vi]
                    != boost::graph_traits<G>::null_vertex())
                && (*vi < (uint64_t)mate_map[*vi])) {
            matched_vertices.push_back(get_edge_id(e));
        }
    }
    std::sort(matched_vertices.begin(), matched_vertices.end());
    return matched_vertices;
}

}  // namespace flow
}  // namespace pgrouting
