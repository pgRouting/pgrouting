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
     std::vector<pgr_components_rt> generate_resultsV(
             G &graph,
             int num_comps,
             std::vector< int >);

};


/******************** IMPLEMENTTION ******************/

//! Generate Results, Vertex Version
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::generate_resultsV(
        G &graph,
        int num_comps,
        std::vector< int > components) {
    int totalNodes = num_vertices(graph.graph);

    // generate identifier
    std::vector< std::vector< int64_t > > tempId;
    tempId.resize(num_comps);
    for (int i = 0; i < totalNodes; i++) {
        tempId[components[i]].push_back(graph[i].id);
    }

    // sort identifier
    for (int i = 0; i < num_comps; i++) {
        std::sort(tempId[i].begin(), tempId[i].end());
    }
    sort(tempId.begin(), tempId.end());

    // generate results
    std::vector< pgr_components_rt > results;
    results.resize(num_vertices(graph.graph));
    auto sizeId = tempId.size();
    int nextRes = 0;
    for (auto i = 0; i < sizeId; i++) {
        int64_t tempComp = tempId[i][0];
        auto sizeIdI = tempId[i].size();
        for (auto j = 0; j < sizeIdI; j++) {
            results[nextRes].identifier = tempId[i][j];
            results[nextRes].n_seq = j + 1;
            results[nextRes].component = tempComp;
            nextRes++;
        }
    }
    
    return results;
}

//! Connected Components Vertex Version
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::connectedComponentsV(
        G &graph) {
    int totalNodes = num_vertices(graph.graph);

    // perform the algorithm
    std::vector< int > components(totalNodes);
    int num_comps = boost::connected_components(graph.graph, &components[0]);

    return generate_resultsV(graph, num_comps, components);
}

//! Strongly Connected Components Vertex Version
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::strongComponentsV(
        G &graph) {
    // perform the algorithm
    std::vector< int > components(num_vertices(graph.graph));
    int num_comps = boost::strong_components(graph.graph, 
			boost::make_iterator_property_map(components.begin(), get(boost::vertex_index, graph.graph)));

    // get the results
    return generate_resultsV(graph, num_comps, components);
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
