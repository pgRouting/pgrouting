
/*PGR-GNU*****************************************************************
File: pgr_isPlanar.hpp
Copyright (c) 2020 pgRouting developers
Mail: project@pgrouting.org
Copyright (c) 2020 Himanshu Raj
Mail: raj.himanshu2@gmail.com
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

#ifndef INCLUDE_PLANAR_PGR_ISPLANAR_HPP_
#define INCLUDE_PLANAR_PGR_ISPLANAR_HPP_
#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/ref.hpp>
#include <vector>

#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/is_kuratowski_subgraph.hpp>

#include "cpp_common/pgr_messages.h"
#include "cpp_common/pgr_base_graph.hpp"
#include "c_types/pgr_boyer_t.h"
//******************************************
using namespace boost;
namespace pgrouting {
namespace functions {

template < class G >
class Pgr_isPlanar : public pgrouting::Pgr_messages  {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;
     bool isPlanar(
                 G &graph){
                   return generateisPlanar(
                                          graph);
                 }

 private:
     bool generateisPlanar(
        const G &graph ) {
       auto check = boyer_myrvold_planarity_test(graph.graph);
       if(check){
         return true;
       } else {
         return false;
        }

    }
};
}
}


#endif //INCLUDE_PLANAR_PGR_ISPLANAR_HPP_
