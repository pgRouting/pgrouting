/*PGR-GNU*****************************************************************
File: pgr_lineGraph.hpp
Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org
Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com
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

#ifndef INCLUDE_COMPONENTS_PGR_LINEGRAPH_HPP_
#define INCLUDE_COMPONENTS_PGR_LINEGRAPH_HPP_
#pragma once


#include <vector>
#include "cpp_common/pgr_base_graph.hpp"


namespace pgrouting {

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_lineGraph;

}  // namespace graph

typedef graph::Pgr_lineGraph <
boost::adjacency_list < boost::vecS, boost::vecS,
    boost::bidirectionalS,
    Basic_vertex, Basic_edge >,
    Basic_vertex, Basic_edge > LinearDirectedGraph;

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_lineGraph : public Pgr_base_graph<G, T_V, T_E> {
 private:
    std::vector< Line_graph_rt > m_lineGraph_edges;
    int64_t m_num_edges;

 public:
     Pgr_lineGraph< G, T_V, T_E >()
         : Pgr_base_graph< G, T_V, T_E >(DIRECTED),
         m_num_edges(0) {
         }

    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    typedef typename boost::graph_traits < G >::edge_descriptor E;
    typedef typename boost::graph_traits < G >::vertex_iterator V_i;
    typedef typename boost::graph_traits < G >::edge_iterator E_i;
    typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
    typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;

    template < typename T >
         void insert_edges(const std::vector < T > &edges) {
             if (this.num_vertices() == 0) {
                 auto vertices = pgrouting::extract_vertices(edges);
                 pgassert(pgrouting::check_vertices(vertices) == 0);
                 this.add_vertices(vertices);
             }
             for (const auto edge : edges) {
                 this.graph_add_edge(edge);
             }
         }

    void transform() {
        V_i vertexIt, vertexEnd;

        for(boost::tie(vertexIt, vertexEnd) = boost::vertices(this->graph);
            vertexIt != vertexEnd; vertexIt++) {
            create_vertex(vertexIt);
        }
    }

    std::vector< Line_graph_rt > Linegraph() const { return m_lineGraph_edges;}
    int64_t num_edges() const { return m_num_edges; }

 private:
   template < typename V >
   void create_vertex(V vertex) {
       EO_i outIt, outEnd;
       EI_i inIt, inEnd;
       for (boost::tie(outIt, outEnd) = out_edges(*vertex, this->graph);
            outIt != outEnd; outIt++) {
            for (boost::tie(inIt, inEnd) = out_edges(*vertex, this->graph);
                inIt != inEnd; inIt++) {
                ++m_num_edges;
                Line_graph_rt lr = {
                    m_num_edges,
                    (*inIt).id,
                    (*outIt).id,
                    -1,
                    -1
                };
                m_lineGraph_edges.push_back(lr);
            }
       }
   }
};

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_PGR_LINEGRAPH_HPP_
