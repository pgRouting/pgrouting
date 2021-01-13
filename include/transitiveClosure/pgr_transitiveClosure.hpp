/*PGR-GNU*****************************************************************
File: pgr_contract.hpp

Generated with Template by:
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

#ifndef INCLUDE_TRANSITIVECLOSURE_PGR_TRANSITIVECLOSURE_HPP_
#define INCLUDE_TRANSITIVECLOSURE_PGR_TRANSITIVECLOSURE_HPP_
#pragma once

#include <boost/graph/transitive_closure.hpp>

#include <deque>
#include <vector>
#include <utility>

#include "cpp_common/pgr_assert.h"


template < class G > class Pgr_transitiveClosure;



template < class G >
class Pgr_transitiveClosure {
 public:
    typedef typename G::V V;
    boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::no_property,
     boost::no_property, boost::listS> transitiveClosure(G &graph);

 private:
    boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::no_property,
    boost::no_property, boost::listS> generatetransitiveClosure(G &graph) {
        boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::no_property,
      boost::no_property, boost::listS> TC;
        transitive_closure(graph.graph, TC);
        return TC;
    }
};

template < class G >
boost::adjacency_list <boost::vecS, boost::vecS, boost::directedS, boost::no_property, boost::no_property,
boost::no_property, boost::listS>
Pgr_transitiveClosure< G >::transitiveClosure(
                G &graph) {
      return generatetransitiveClosure(
                             graph);
}



#endif  // INCLUDE_TRANSITIVECLOSURE_PGR_TRANSITIVECLOSURE_HPP_
