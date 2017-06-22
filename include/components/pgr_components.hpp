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

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#if 0
#include "./../../common/src/signalhandler.h"
#endif


template < class G > class Pgr_components;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_components {
 public:
     typedef typename G::V V;

     //! Connected Components Vertex Version
     std::vector<pgr_componentsV_rt> connectedComponentsV(
             G &graph);

     //! Strongly Connected Components Vertex Version
     std::vector<pgr_componentsV_rt> strongComponentsV(
             G &graph);

 private:
     //! Generate Results, Vertex Version
     std::vector<pgr_componentsV_rt> generate_resultsV(
             G &graph,
             std::vector< V >);

};


/******************** IMPLEMENTTION ******************/

//! Generate Results, Vertex Version
template < class G >
std::vector<pgr_componentsV_rt>
Pgr_components< G >::generate_resultsV(
        G &graph,
        std::vector< V > components) {

    // generate results
    auto totalNodes = num_vertices(graph.graph);

    std::vector< pgr_componentsV_rt > results;
    results.resize(totalNodes);

    std::vector< int64_t > result_comp;
    result_comp.resize(0);
    size_t temp_size = 0;
    for (V i = 0; i < totalNodes; i++) {
		results[i].node = graph[i].id;
        if (components[i] >= temp_size) {
            result_comp.push_back(results[i].node);
            temp_size++;
        } else {
            result_comp[components[i]] =
                std::min(results[i].node, result_comp[components[i]]);
        }
    }

    // generate component number
    for (V i = 0; i < totalNodes; i++) {
        results[i].component = result_comp[components[i]];
    }

    // sort results and generate n_seq
	std::sort(results.begin(), results.end(),
            [](const pgr_componentsV_rt &left, const pgr_componentsV_rt &right) {
			return left.node < right.node; });

	std::stable_sort(results.begin(), results.end(),
            [](const pgr_componentsV_rt &left, const pgr_componentsV_rt &right) {
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
std::vector<pgr_componentsV_rt>
Pgr_components< G >::connectedComponentsV(
        G &graph) {
    // perform the algorithm
    std::vector< V > components(num_vertices(graph.graph));
    boost::connected_components(graph.graph, &components[0]);

	//get the results
    std::vector<pgr_componentsV_rt> results = generate_resultsV(graph, components);

    return results;
}

//! Strongly Connected Components Vertex Version
template < class G >
std::vector<pgr_componentsV_rt>
Pgr_components< G >::strongComponentsV(
        G &graph) {
    // perform the algorithm
    std::vector< V > components(num_vertices(graph.graph));
    boost::strong_components(graph.graph, &components[0]);

    // get the results
    std::vector<pgr_componentsV_rt> results = generate_resultsV(graph, components);

    return results;
}

#endif  // INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
