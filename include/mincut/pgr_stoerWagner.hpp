/*PGR-GNU*****************************************************************
File: pgr_stoerWagner.hpp

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

#ifndef INCLUDE_MINCUT_PGR_STOERWAGNER_HPP_
#define INCLUDE_MINCUT_PGR_STOERWAGNER_HPP_
#pragma once

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/one_bit_color_map.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
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

template < class G > class Pgr_stoerWagner;
// user's functions
// for development

//******************************************

template < class G >
class Pgr_stoerWagner {
 public:
     typedef typename G::V V;
     typedef typename G::E E;
     typedef typename G::E_i E_i;

     std::vector<pgr_stoerWagner_t> stoerWagner(
                 G &graph);

 private:
     std::vector< pgr_stoerWagner_t >
     generatestoerWagner(
        const G &graph ) {
       std::vector< pgr_stoerWagner_t > results;
       auto parities = boost::make_one_bit_color_map(
                                        num_vertices(graph.graph),
                                        get(boost::vertex_index, graph.graph));

       double w = stoer_wagner_min_cut(
                                   graph.graph,
                                   get(&G::G_T_E::cost, graph.graph),
                                   boost::parity_map(parities));

       double totalcost = 0;
       E_i ei, ei_end;
       for (boost::tie(ei, ei_end) = edges(graph.graph); ei != ei_end; ei++) {
          auto s = source(*ei, graph.graph);
          auto t = target(*ei, graph.graph);

          if (get(parities, s) != get(parities, t)) {
               pgr_stoerWagner_t tmp;

               tmp.cost = graph[*ei].cost;

               auto edge_id =
                 graph.get_edge_id(
                         source(*ei, graph.graph),
                         target(*ei, graph.graph),
                         tmp.cost);

               tmp.edge = edge_id;
               totalcost += tmp.cost;
               tmp.mincut = totalcost;
               results.push_back(tmp);
             }
       }

       pgassert(w == totalcost);
       return results;
     }
};

template < class G >
std::vector<pgr_stoerWagner_t>
Pgr_stoerWagner< G >::stoerWagner(
                G &graph) {
      pgassert(num_vertices(graph.graph) > 1);
      return generatestoerWagner(
                             graph);
}


#endif  // INCLUDE_MINCUT_PGR_STOERWAGNER_HPP_
