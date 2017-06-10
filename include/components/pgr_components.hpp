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
     std::vector<pgr_componentV_t> connectedComponentsV(
             G &graph);

 private:
     //! Call to Connected Components Vertex Version
     std::vector<pgr_componentV_t> do_connectedComponentsV(
             G &graph);

     //! Generate Map V_to_id
     std::map< V, int64_t > V_to_id;
     void generate_map(
             std::map< int64_t, V > id_to_V);
};


/******************** IMPLEMENTTION ******************/

//! Compare two pgr_componentV_t structs
bool
sort_cmp(
        pgr_componentV_t a,
        pgr_componentV_t b) {
    if (a.component == b.component)
        return a.node < b.node;
    return a.component < b.component;
}

//! Generate map V_to_id
template < class G >
void
Pgr_components< G >::generate_map(
        std::map< int64_t, V > id_to_V) {
    V_to_id.clear();
    for (auto iter : id_to_V) {
        V_to_id.insert(std::make_pair(iter.second, iter.first));
    }
}

//! Connected Components Vertex Version
template < class G >
std::vector<pgr_componentV_t>
Pgr_components< G >::connectedComponentsV(
        G &graph) {
    // perform the algorithm
    std::vector<pgr_componentV_t> results = do_connectedComponentsV(graph);

    // get the results
    return results;
}

//! Call Componnets Vertex Version and Generate Results
template < class G >
std::vector<pgr_componentV_t>
Pgr_components< G >::do_connectedComponentsV(
        G &graph) {
    // call to boost
    std::vector< V > components(num_vertices(graph.graph));
    boost::connected_components(graph.graph, &components[0]);

    // generate V_to_id
    generate_map(graph.vertices_map);

    // generate results
    int totalNodes = num_vertices(graph.graph);

    std::vector< pgr_componentV_t > results;
    results.resize(totalNodes);

    std::vector< int64_t > result_comp;
    result_comp.resize(0);
    size_t temp_size = 0;
    for (int i = 0; i < totalNodes; i++) {
        results[i].node = V_to_id.find(i)->second;
        if (components[i] >= temp_size) {
            result_comp.push_back(results[i].node);
            temp_size++;
        } else {
            result_comp[components[i]] =
                std::min(results[i].node, result_comp[components[i]]);
        }
        results[i].n_seq = -100;
    }

    // generate component number
    for (int i = 0; i < totalNodes; i++) {
        results[i].component = result_comp[components[i]];
    }

    // sort results and generate n_seq
    std::sort(results.begin(), results.end(), sort_cmp);
    for (int i = 0; i < totalNodes; i++) {
        if (i == 0 || results[i].component != results[i - 1].component) {
            results[i].n_seq = 1;
        } else {
            results[i].n_seq = results[i - 1].n_seq + 1;
        }
    }
    return results;
}

#endif  // INCLUDE_COMPONENTS_PGR_COMPONENTS_HPP_
