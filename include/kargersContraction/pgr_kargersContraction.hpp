/*PGR-GNU*****************************************************************
File: pgr_kargersContraction.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2020 Himanshu Raj
raj.himanshu2@gmail.com

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

#ifndef INCLUDE_KARGERSCONTRACTION_PGR_KARGERSCONTRACTION_HPP_
#define INCLUDE_KARGERSCONTRACTION_PGR_KARGERSCONTRACTION_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/one_bit_color_map.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/typeof/typeof.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "c_types/pgr_kargersContraction_t.h"

template < class G > class Pgr_kargersContraction;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_kargersContraction {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;

     std::vector<pgr_kargersContraction_t> kargersContraction(
                 G &graph);

 private:
     std::vector< pgr_kargersContraction_t >
     generateskargersContraction(
        const G &graph ) {
       std::vector< pgr_kargersContraction_t > results;
       return results;
     }
};

template < class G >
std::vector<pgr_kargersContraction_t>
Pgr_kargersContraction< G >::kargersContraction(
                G &graph) {
      pgassert(num_vertices(graph.graph) > 1);
      return generateskargersContraction(
                             graph);
}


#endif  // INCLUDE_KARGERSCONTRACTION_PGR_KARGERSCONTRACTION_HPP_
