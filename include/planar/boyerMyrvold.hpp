/*PGR-GNU*****************************************************************
File: boyerMyrvold.hpp

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

#ifndef INCLUDE_PLANAR_BOYERMYRVOLD_HPP_
#define INCLUDE_PLANAR_BOYERMYRVOLD_HPP_
#pragma once

#include <vector>
#include <cstdint>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/is_kuratowski_subgraph.hpp>
#include <boost/ref.hpp>

#include "cpp_common/messages.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include "c_types/iid_t_rt.h"
//******************************************
namespace pgrouting {
namespace functions {

template < class G >
class Pgr_boyerMyrvold : public pgrouting::Pgr_messages {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;
     std::vector<IID_t_rt> boyerMyrvold(G &graph) {
          return generateboyerMyrvold(graph);
     }

     bool isPlanar(G &graph) {
          /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
          CHECK_FOR_INTERRUPTS();
          try {
             return (boost::boyer_myrvold_planarity_test(graph.graph));
          } catch (boost::exception const& ex) {
             (void)ex;
             throw;
          } catch (std::exception &e) {
             (void)e;
             throw;
          } catch (...) {
             throw;
          }
          return false;
     }

 private:
     std::vector< IID_t_rt >generateboyerMyrvold(const G &graph ) {
     std::vector< IID_t_rt > results;
     auto check = boost::boyer_myrvold_planarity_test(graph.graph);
     if (check) {
         E_i ei, ei_end;
     for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ++ei) {
         int64_t src = graph[graph.source(*ei)].id;
         int64_t tgt = graph[graph.target(*ei)].id;
         double cost = graph[*ei].cost;
         results.push_back({src, tgt, cost});
      }
    }
     return results;
    }
};
}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_PLANAR_BOYERMYRVOLD_HPP_
