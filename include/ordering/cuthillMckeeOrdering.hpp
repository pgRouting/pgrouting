/*PGR-GNU*****************************************************************
File: cuthillMckeeOrdering.hpp

Generated with Template by:
Copyright (c) 2022 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2022 Shobhit Chaurasia
Mail: 000shobhitchaurasia at gmail.com

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

#ifndef INCLUDE_ORDERING_CUTHILLMCKEEORDERING_HPP_
#define INCLUDE_ORDERING_CUTHILLMCKEEORDERING_HPP_
#pragma once

#include <algorithm>
#include <vector>
#include <map>
#include <cstdint>

#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/vector_property_map.hpp>
#include <boost/type_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cuthill_mckee_ordering.hpp>

#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include "cpp_common/messages.hpp"

#include "c_types/ii_t_rt.h"

/** @file cuthillMckeeOrdering.hpp
 * @brief The main file which calls the respective boost function.
 *
 * Contains actual implementation of the function and the calling
 * of the respective boost function.
 */


namespace pgrouting {
namespace functions {

template <class G>
class CuthillMckeeOrdering : public Pgr_messages {
 public:
    typedef typename G::V V;
    typedef typename G::E E;
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> Graph;
    typedef boost::graph_traits<Graph>::vertices_size_type size_type;
    typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

    /** @name CuthillMckeeOrdering
      * @{
      *
      */

     /** @brief cuthillMckeeOrdering function
      *
      * It does all the processing and returns the results.
      *
      * @param graph      the graph containing the edges
      *
      * @returns results, when results are found
      *
      * @see [boost::cuthill_mckee_ordering]
      * (https://www.boost.org/libs/graph/doc/cuthill_mckee_ordering.html)
      */

        std::vector<II_t_rt>
        cuthillMckeeOrdering(G &graph) {
        std::vector<II_t_rt>results;

        // map which store the indices with their nodes.
        auto i_map = boost::get(boost::vertex_index, graph.graph);

        // vector which will store the order of the indices.
        std::vector<Vertex> inv_perm(boost::num_vertices(graph.graph));

        // vector which will store the color of all the vertices in the graph
        std::vector <boost::default_color_type> colors(boost::num_vertices(graph.graph));

        // An iterator property map which records the color of each vertex
        auto color_map = boost::make_iterator_property_map(&colors[0], i_map, colors[0]);

        // map which store the degree of each vertex.
        auto out_deg = boost::make_out_degree_map(graph.graph);

         /* abort in case of an interruption occurs (e.g. the query is being cancelled) */
         CHECK_FOR_INTERRUPTS();

         try {
             boost::cuthill_mckee_ordering(graph.graph, inv_perm.rbegin(), color_map, out_deg);
         } catch (boost::exception const& ex) {
             (void)ex;
             throw;
         } catch (std::exception &e) {
             (void)e;
             throw;
         } catch (...) {
             throw;
         }

         results = get_results(inv_perm, graph);

         return results;
     }

      //@}

 private:
     /** @brief to get the results
      *
      * Uses the `inv_perm` vector to get the results i.e. the ordering.
      *
      * @param inv_perm    vector which contains the new ordering of indices.
      * @param graph       the graph containing the edges
      *
      * @returns `results` vector
      */
     std::vector <II_t_rt> get_results(
            std::vector <size_type> & inv_perm,
            const G &graph) {
            std::vector <II_t_rt> results;

        for (std::vector<Vertex>::const_iterator i = inv_perm.begin();
             i != inv_perm.end(); ++i) {
            log << inv_perm[*i] << " ";
            auto seq = graph[*i].id;
            results.push_back({{seq}, {static_cast<int64_t>(graph.graph[*i].id)}});
            seq++;
            }

            return results;
        }
};
}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_ORDERING_CUTHILLMCKEEORDERING_HPP_
