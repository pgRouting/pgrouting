/*PGR-GNU*****************************************************************
File: planarFaces.hpp

Copyright (c) 2026-2026 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2026 Sakir Ahmed
Mail: sakirahmed75531 at gmail.com

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

#ifndef INCLUDE_PLANAR_PLANARFACES_HPP_
#define INCLUDE_PLANAR_PLANARFACES_HPP_
#pragma once

#include <vector>
#include <map>
#include <string>
#include <cstdint>

#include <boost/graph/graph_traits.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>
#include <boost/graph/planar_face_traversal.hpp>

#include "c_types/iid_t_rt.h"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/interruption.hpp"
#include "cpp_common/messages.hpp"

namespace pgrouting {
namespace functions {

template <class G>
class Pgr_planarFaces : public pgrouting::Pgr_messages {
 public:
     typedef typename G::B_G B_G;
     typedef typename boost::graph_traits<B_G>::edge_descriptor E;

     /*
      * results are returned on the shared IID_t_rt of the planar family:
      * from_vid holds the face id, to_vid the edge id and cost the side,
      * which is 1 (left) or 2 (right)
      */
     struct FaceVisitor : public boost::planar_face_traversal_visitor {
         const G &m_graph;
         std::vector<IID_t_rt> &m_results;
         std::map<E, int> &m_visit_count;
         int64_t face_id;
         int64_t seq;

         FaceVisitor(const G &graph,
                     std::vector<IID_t_rt> &results,
                     std::map<E, int> &visit_count)
             : m_graph(graph), m_results(results),
               m_visit_count(visit_count), face_id(0), seq(0) {}

         void begin_face() { ++face_id; }

         void next_edge(E e) {
             int n = ++m_visit_count[e];
             IID_t_rt row;
             row.from_vid = face_id;
             row.to_vid   = m_graph.graph[e].id;
             row.cost     = n;
             m_results.push_back(row);
         }
     };

     std::vector<IID_t_rt> planarFaces(G &graph) {
         CHECK_FOR_INTERRUPTS();

         std::vector<IID_t_rt> results;

         std::map<E, std::size_t> e_index;
         boost::associative_property_map<std::map<E, std::size_t>>
                 e_index_map(e_index);
         std::size_t edge_count = 0;
         typename boost::graph_traits<B_G>::edge_iterator ei, ei_end;
         for (boost::tie(ei, ei_end) = boost::edges(graph.graph);
                 ei != ei_end; ++ei) {
             boost::put(e_index_map, *ei, edge_count++);
         }

         typedef std::vector<E> vec_t;
         std::vector<vec_t> embedding_storage(boost::num_vertices(graph.graph));
         auto embedding = boost::make_iterator_property_map(
                 embedding_storage.begin(),
                 boost::get(boost::vertex_index, graph.graph));

         bool is_planar = false;
         try {
             is_planar = boost::boyer_myrvold_planarity_test(
                     boost::boyer_myrvold_params::graph = graph.graph,
                     boost::boyer_myrvold_params::embedding = embedding,
                     boost::boyer_myrvold_params::edge_index_map = e_index_map);
         } catch (boost::exception const& ex) {
             (void)ex;
             throw;
         } catch (std::exception &e) {
             (void)e;
             throw;
         } catch (...) {
             throw;
         }

         if (!is_planar) {
             throw std::string("Graph is not planar");
         }

         std::map<E, int> visit_count;
         FaceVisitor vis(graph, results, visit_count);

         try {
             boost::planar_face_traversal(
                     graph.graph, embedding, vis, e_index_map);
         } catch (boost::exception const& ex) {
             (void)ex;
             throw;
         } catch (std::exception &e) {
             (void)e;
             throw;
         } catch (...) {
             throw;
         }

         return results;
     }
};

}  // namespace functions
}  // namespace pgrouting

#endif  // INCLUDE_PLANAR_PLANARFACES_HPP_
