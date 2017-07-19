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
#include "cpp_common/line_vertex.h"

namespace pgrouting {

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_lineGraph;

}  // namespace graph

typedef graph::Pgr_lineGraph <
boost::adjacency_list < boost::vecS, boost::vecS,
    boost::bidirectionalS,
    Line_vertex, Basic_edge >,
    Line_vertex, Basic_edge > LinearDirectedGraph;

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_lineGraph : public Pgr_base_graph<G, T_V, T_E> {
 private:
    std::vector< Line_graph_rt > m_lineGraph_edges;
    int64_t m_num_edges;
    pgrouting::DirectedGraph m_original_graph;

    void add_vertices(std::vector< T_V > vertices);

 public:
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    typedef typename boost::graph_traits < G >::edge_descriptor E;
    typedef typename boost::graph_traits < G >::vertex_iterator V_i;
    typedef typename boost::graph_traits < G >::edge_iterator E_i;
    typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
    typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;

    explicit Pgr_lineGraph< G, T_V, T_E >(graphType gtype) :
        Pgr_base_graph< G, T_V, T_E >(gtype),
        m_num_edges(0) {}

    template < typename T >
        void insert_vertices(const T* edges, int64_t count) {
            insert_vertices(std::vector < T >(edges, edges + count));
        }

    template < typename T >
        void insert_edges(const std::vector < T > &edges) {
            auto vertices = pgrouting::extract_vertices_from_edges(edges);
            add_vertices(vertices);
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

    void add_graph(const pgrouting::DirectedGraph& _graph) { m_original_graph = _graph; }

 private:

   template < typename V >
   void create_vertex(V vertex) {
       EO_i outIt, outEnd;
       EI_i inIt, inEnd;
       for (boost::tie(outIt, outEnd) = out_edges(*vertex, this->graph);
            outIt != outEnd; outIt++) {
            for (boost::tie(inIt, inEnd) = in_edges(*vertex, this->graph);
                inIt != inEnd; inIt++) {
                ++m_num_edges;
                Line_graph_rt lr;
                lr.seq = (int)m_num_edges;
                lr.source = (this->graph[*inIt]).id;
                lr.target = (this->graph[*outIt]).id;
                lr.cost = -1;
                lr.reverse_cost = -1;
                m_lineGraph_edges.push_back(lr);
            }
       }
   }
};

template < class G, typename T_V, typename T_E >
 void
 Pgr_lineGraph< G, T_V, T_E >::add_vertices(
         std::vector< T_V > vertices) {
     pgassert(this->m_num_vertices == 0);
     for (const auto vertex : vertices) {
         pgassert(this->vertices_map.find(vertex.id) == this->vertices_map.end());

         auto v =  add_vertex(this->graph);
         this->vertices_map[vertex.id] =  this->m_num_vertices++;
         this->graph[v].cp_members(vertex);

         pgassert(boost::num_vertices(this->graph) == this->num_vertices());
     }
     return;
 }

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_PGR_LINEGRAPH_HPP_
