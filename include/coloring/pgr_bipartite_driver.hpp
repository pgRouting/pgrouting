/*PGR-GNU*****************************************************************
File: pgr_bipartite_driver.hpp

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


#ifndef INCLUDE_COLORING_PGR_BIPARTITE_DRIVER_HPP_
#define INCLUDE_COLORING_PGR_BIPARTITE_DRIVER_HPP_
#pragma once

#include <iostream>
#include <algorithm>
#include <vector>

#include <boost/property_map/property_map.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>
#include <boost/graph/bipartite.hpp>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/pgr_messages.hpp"
#include "cpp_common/interruption.hpp"
typedef struct II_t_rt II_t_rt;
#include "c_types/ii_t_rt.h"

namespace pgrouting {
namespace functions {

template<class G>
class Pgr_Bipartite : public pgrouting::Pgr_messages {
 public:
            typedef typename G::V_i V_i;
            std::vector<II_t_rt> print_Bipartite(
                    G &graph) {
                std::vector<II_t_rt> results;
                std::vector <boost::default_color_type> partition(graph.num_vertices());
                auto partition_map =
                    make_iterator_property_map(partition.begin(), boost::get(boost::vertex_index, graph.graph));

               /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
                CHECK_FOR_INTERRUPTS();
                try {
                boost::is_bipartite(graph.graph, boost::get(boost::vertex_index, graph.graph), partition_map);
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
                    boost::get(partition_map, *v) ==
                        boost::color_traits <boost::default_color_type>::white() ?
                        results.push_back({{vid}, {0}}) : results.push_back({{vid}, {1}});
                }
                return results;
            }
            std::vector<II_t_rt> pgr_bipartite(
                    G &graph ){
                std::vector<II_t_rt> results;
                bool bipartite = boost::is_bipartite(graph.graph);
                if (bipartite) results = print_Bipartite(graph);
                return results;
            }
};
}  // namespace functions
}  // namespace pgrouting
#endif  // INCLUDE_COLORING_PGR_BIPARTITE_DRIVER_HPP_
