/*PGR-GNU*****************************************************************
File: pgr_lengauerTarjanDominatorTree_driver.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2020 Prakash Tiwari
Mail: 85prakash2017@gmail.com

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


#ifndef INCLUDE_DOMINATOR_PGR_LENGAUERTARJANDOMINATORTREE_DRIVER_HPP_
#define INCLUDE_DOMINATOR_PGR_LENGAUERTARJANDOMINATORTREE_DRIVER_HPP_
#pragma once

#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dominator_tree.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>

#include "cpp_common/interruption.hpp"
#include "cpp_common/pgr_messages.hpp"
#include "cpp_common/pgr_base_graph.hpp"
#include "c_types/ii_t_rt.h"


namespace pgrouting {
namespace functions {

template<class G>
class Pgr_LTDTree : public pgrouting::Pgr_messages {
 public:
            typedef typename G::V Vertex;
            typedef typename G::E_i E_i;
            typedef typename G::V_i V_i;
            std::vector <II_t_rt> pgr_ltdtree(
                    G &graph,
                    int64_t root
                    ){
                std::vector<II_t_rt> results;
                if (!graph.has_vertex(root)) return results;
                std::vector<int64_t> idoms = std::vector<int64_t>(boost::num_vertices(graph.graph), -1);
                auto dominatorTree =
                make_iterator_property_map
                (idoms.begin(), boost::get(boost::vertex_index, graph.graph));
               /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
                CHECK_FOR_INTERRUPTS();
                try {
             // calling the boost function
                boost::lengauer_tarjan_dominator_tree(graph.graph, graph.get_V(root), dominatorTree);
                     } catch (boost::exception const& ex) {
                          (void)ex;
                             throw;
                     } catch (std::exception &e) {
                         (void)e;
                         throw;
                      } catch (...) {
                          throw;
                      }

                V_i v, vend;
                for (boost::tie(v, vend) = vertices(graph.graph); v != vend; ++v) {
                    int64_t vid = graph[*v].id;
                    results.push_back({{vid}, {(idoms[*v] != -1 ? (idoms[*v]+1) : 0)}});
                }

                 return results;
            }
};
}  // namespace functions
}  // namespace pgrouting
#endif  // INCLUDE_DOMINATOR_PGR_LENGAUERTARJANDOMINATORTREE_DRIVER_HPP_

