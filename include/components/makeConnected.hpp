/*PGR-GNU*****************************************************************
File: makeConnected.hpp

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

#ifndef INCLUDE_COMPONENTS_MAKECONNECTED_HPP_
#define INCLUDE_COMPONENTS_MAKECONNECTED_HPP_
#pragma once

#include <vector>
#include <set>
#include <map>
#include <cstdint>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/make_connected.hpp>
#include <boost/ref.hpp>

#include "c_types/ii_t_rt.h"
#include "cpp_common/messages.hpp"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
//******************************************

namespace pgrouting {
namespace functions {

template < class G >
class Pgr_makeConnected : public pgrouting::Pgr_messages {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;
     std::vector<II_t_rt> makeConnected(G &graph) {
                   return generatemakeConnected(graph);
     }

 private:
      std::vector< II_t_rt > generatemakeConnected(G &graph ) {
      std::vector<size_t> component(boost::num_vertices(graph.graph));
      auto comp = boost::connected_components(graph.graph, &component[0]);
      comp--;
      auto edgeCount = boost::num_edges(graph.graph);
      size_t newEdge = 0;
      log << "Number of Components before: " << boost::connected_components(graph.graph, &component[0]) << "\n";
      size_t i = 0;

      /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
      CHECK_FOR_INTERRUPTS();
      try {
         boost::make_connected(graph.graph);
      } catch (boost::exception const& ex) {
         (void)ex;
         throw;
      } catch (std::exception &e) {
         (void)e;
         throw;
      } catch (...) {
         throw;
      }

      log << "Number of Components after: " << boost::connected_components(graph.graph, &component[0]) << "\n";
      E_i  ei, ei_end;
      std::vector< II_t_rt > results(comp);
      for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ++ei) {
              int64_t src = graph[graph.source(*ei)].id;
              int64_t tgt = graph[graph.target(*ei)].id;
              log<< "src:" << src<< "tgt:" << tgt <<"\n";
              if (newEdge >= edgeCount) {
                   results[i] = {{src}, {tgt}};
                   i++;
              }
              newEdge++;
          }
      return results;
      }
};
}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_MAKECONNECTED_HPP_
