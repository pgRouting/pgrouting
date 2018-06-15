/*PGR-GNU*****************************************************************
File: pgr_kruskal.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Aditya Pratap Singh
adityapratap.singh28@gmail.com

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

#ifndef INCLUDE_PRIM_PGR_KRUSKAL_HPP_
#define INCLUDE_PRIM_PGR_KRUSKAL_HPP_
#pragma once

#include <boost/config.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>

#include <deque>
#include <set>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <limits>

#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/pgr_base_graph.hpp"

template < class G > class Pgr_kruskal;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_kruskal {
 public:

     typedef typename G::V V;
     typedef typename G::B_G B_G;

     std::vector<pgr_prim_t> kruskal(
                 G &graph);
};

template < class G >
std::vector<pgr_prim_t>
Pgr_kruskal< G >::kruskal(
                G &graph) {
 
             std::vector< pgr_prim_t > results;
             return results;        
} 


#endif  // INCLUDE_PRIM_PGR_KRUSKAL_HPP_
