/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Maoguang Wang 
xjtumg1007@gmail.com

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

#ifndef INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/biconnected_components.hpp>

#include <vector>
#include <utility>
#include <algorithm>

#if 0
#include "cpp_common/basePath_SSEC.hpp"
#endif
#include "cpp_common/pgr_base_graph.hpp"

template < class G > class Pgr_components;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_components {
 public:
     typedef typename G::V V;
	 typedef typename G::E E;
	 typedef typename G::E_i E_i;

     //! Connected Components Vertex Version
     std::vector<pgr_components_rt> connectedComponentsV(
             G &graph);

     //! Strongly Connected Components Vertex Version
     std::vector<pgr_components_rt> strongComponentsV(
             G &graph);

     //! Biconnected Components
     std::vector<pgr_components_rt> biconnectedComponents(
             G &graph);

 private:
     //! Generate Results, Vertex Version
     std::vector<pgr_components_rt> generate_results(
             G &graph,
             std::vector< V >);

};


/******************** IMPLEMENTTION ******************/

//! Generate Results, Vertex Version
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::generate_results(
        G &graph,
        std::vector< V > components) {

    // generate results
    auto totalNodes = num_vertices(graph.graph);

    std::vector< pgr_components_rt > results;
    results.resize(totalNodes);

    std::vector< int64_t > result_comp;
    result_comp.resize(0);
    size_t temp_size = 0;
    for (V i = 0; i < totalNodes; i++) {
		results[i].identifier = graph[i].id;
        if (components[i] >= temp_size) {
            result_comp.push_back(results[i].identifier);
            temp_size++;
        } else {
            result_comp[components[i]] =
                std::min(results[i].identifier, result_comp[components[i]]);
        }
    }

    // generate component number
    for (V i = 0; i < totalNodes; i++) {
        results[i].component = result_comp[components[i]];
    }

    // sort results and generate n_seq
	std::sort(results.begin(), results.end(),
            [](const pgr_components_rt &left, const pgr_components_rt &right) {
			return left.identifier < right.identifier; });

	std::stable_sort(results.begin(), results.end(),
            [](const pgr_components_rt &left, const pgr_components_rt &right) {
			return left.component < right.component; });

	auto current = results[0].component;
	int seq(0);
	for (auto &result: results) {
		result.n_seq = result.component == current ? ++seq : seq = 1;
		current = result.component;
	}

    return results;
}

//! Connected Components Vertex Version
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::connectedComponentsV(
        G &graph) {
    // perform the algorithm
    std::vector< V > components(num_vertices(graph.graph));
    boost::connected_components(graph.graph, &components[0]);

	//get the results
    return generate_results(graph, components);
}

//! Strongly Connected Components Vertex Version
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::strongComponentsV(
        G &graph) {
    // perform the algorithm
    std::vector< V > components(num_vertices(graph.graph));
    boost::strong_components(graph.graph, 
			boost::make_iterator_property_map(components.begin(), get(boost::vertex_index, graph.graph)));

    // get the results
    return generate_results(graph, components);
}

//! Biconnected Components
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::biconnectedComponents(
        G &graph) {
    // perform the algorithm
	struct order_edges {
		bool operator() (const E &left, const E &right) const {
			return left.get_property() < right.get_property();
		}
	};
	typedef std::map< E, int > edge_map;
	edge_map bicmp_map;
	
	boost::associative_property_map< edge_map > bimap(bicmp_map);
	auto num_comps = biconnected_components(graph.graph, bimap);

	// convert associative_property_map to vector
	//TODO(mg) change to vector< vector< V or E > >
#if 0
	E_i ei, ei_end;
	std::vector< E > components(num_edges(graph.graph));
	for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ei++)
		components[graph[*ei].id] = bimap[*ei];
#endif 

    // get the results
//    return generate_results(graph, components);
}

#endif  // INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
