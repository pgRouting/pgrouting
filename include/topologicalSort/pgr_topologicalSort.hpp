/*PGR-GNU*****************************************************************
File: pgr_topologicalSort.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2019 Hang Wu
mail: nike0good@gmail.com

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

#ifndef INCLUDE_TOPOLOGICALSORT_PGR_TOPOLOGICALSORT_HPP_
#define INCLUDE_TOPOLOGICALSORT_PGR_TOPOLOGICALSORT_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/graph/topological_sort.hpp>


#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/interruption.h"

template < class G > class Pgr_topologicalSort;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_topologicalSort {
 public:
     typedef typename G::V V;

     std::vector<pgr_topologicalSort_t> topologicalSort(
                 G &graph);

 private:
     std::vector< pgr_topologicalSort_t >
     generatetopologicalSort(
        const G &graph ) {
        std::vector< pgr_topologicalSort_t > results;

        typedef typename std::vector< V > container;
        container c;

        /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
        CHECK_FOR_INTERRUPTS();

        boost::topological_sort(graph.graph, std::back_inserter(c));

        typename std::vector< V >::reverse_iterator ii;
        for (ii = c.rbegin(); ii != c.rend(); ++ii) {
            auto t = *ii;
            pgr_topologicalSort_t tmp;
            tmp.sorted_v = graph.graph[t].id;
            results.push_back(tmp);
        }

        return results;
     }
};

template < class G >
std::vector<pgr_topologicalSort_t>
Pgr_topologicalSort< G >::topologicalSort(
                G &graph) {
      return generatetopologicalSort(
                             graph);
}


#endif  // INCLUDE_TOPOLOGICALSORT_PGR_TOPOLOGICALSORT_HPP_
