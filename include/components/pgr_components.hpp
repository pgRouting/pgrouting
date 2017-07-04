/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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

#ifndef INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#define INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/biconnected_components.hpp>

#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include "pgr_componentsGraph.hpp"

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
             std::vector< std::vector< int64_t > >);
};


/******************** IMPLEMENTTION ******************/

//! Generate Results, Vertex Version
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::generate_results(
        std::vector< std::vector< int64_t > > components) {
    // sort identifier
    size_t num_comps = components.size();
    for (size_t i = 0; i < num_comps; i++) {
        std::sort(components[i].begin(), components[i].end());
    }
    sort(components.begin(), components.end());

    // generate results
    std::vector< pgr_components_rt > results;
    for (size_t i = 0; i < num_comps; i++) {
        int64_t tempComp = components[i][0];
        size_t sizeCompi = components[i].size();
        for (size_t j = 0; j < sizeCompi; j++) {
            pgr_components_rt tmp;
            tmp.identifier = components[i][j];
            tmp.n_seq = static_cast< int > (j + 1);
            tmp.component = tempComp;
            results.push_back(tmp);
        }
    }
    return results;
}

//! Connected Components Vertex Version
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::connectedComponentsV(
        G &graph) {
    size_t totalNodes = num_vertices(graph.graph);

    // perform the algorithm
    std::vector< int > components(totalNodes);
    int num_comps = boost::connected_components(graph.graph, &components[0]);

    // get the results
    std::vector< std::vector< int64_t > > results;
    results.resize(num_comps);
    for (size_t i = 0; i < totalNodes; i++)
        results[components[i]].push_back(graph[i].id);

    return generate_results(results);
}

//! Strongly Connected Components Vertex Version
template < class G >
std::vector<pgr_components_rt>
Pgr_components< G >::strongComponentsV(
        G &graph) {
    size_t totalNodes = num_vertices(graph.graph);

    // perform the algorithm
    std::vector< int > components(totalNodes);
    int num_comps = boost::strong_components(graph.graph,
            boost::make_iterator_property_map(components.begin(),
                                              get(boost::vertex_index,
                                                  graph.graph)));

    // get the results
    std::vector< std::vector< int64_t > > results;
    results.resize(num_comps);
    for (size_t i = 0; i < totalNodes; i++)
        results[components[i]].push_back(graph[i].id);

    return generate_results(results);
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
    size_t num_comps = biconnected_components(graph.graph, bimap);

    // get the results
    E_i ei, ei_end;
    std::vector< std::vector< int64_t > > components(num_comps);
    for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ei++)
        components[bimap[*ei]].push_back(graph[*ei].id);

    return generate_results(components);
}

#endif  // INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
