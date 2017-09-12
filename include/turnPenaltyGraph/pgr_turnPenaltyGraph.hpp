/*PGR-GNU*****************************************************************
File: pgr_turnPenaltyGraph.hpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Anthony Nicola Tasca
Mail: atasca10@gmail.com

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

#ifndef INCLUDE_COMPONENTS_PGR_TURNPENALTYGRAPH_HPP_
#define INCLUDE_COMPONENTS_PGR_TURNPENALTYGRAPH_HPP_
#pragma once


#include <vector>
#include <set>
#include <utility>
#include <map>

#include "cpp_common/pgr_base_graph.hpp"
#include "cpp_common/line_vertex.h"
#include "dijkstraTRSP/restriction.h"

namespace pgrouting {

namespace graph {
template <class G, typename T_V, typename T_E>
class Pgr_turnPenaltyGraph;
}  // namespace graph

typedef graph::Pgr_turnPenaltyGraph <
boost::adjacency_list < boost::vecS, boost::vecS,
    boost::bidirectionalS,
    Line_vertex, Basic_edge >,
    Line_vertex, Basic_edge > LinearDirectedGraph;

namespace graph {

template <class G, typename T_V, typename T_E>
class Pgr_turnPenaltyGraph : public Pgr_base_graph<G, T_V, T_E> {
 private:
    int64_t m_num_edges;
    std::map < int64_t, pgr_edge_t > m_edges;
    std::map < std::pair< int64_t, int64_t >, int64_t > m_vertex_map;

    void add_vertices(std::vector< T_V > vertices);

    void create_edges(const pgrouting::DirectedGraph& digraph);

    template < typename T >
        void graph_add_edge(int64_t, const T &source, const T&target, int64_t, int64_t);

 public:
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
    typedef typename boost::graph_traits < G >::edge_descriptor E;
    typedef typename boost::graph_traits < G >::vertex_iterator V_i;
    typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
    typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;

    std::ostringstream log;

    explicit Pgr_turnPenaltyGraph< G, T_V, T_E >(graphType gtype)
         : Pgr_base_graph< G, T_V, T_E >(gtype),
           m_num_edges(0) {
         }

    template < typename T >
        void insert_vertices(const T* edges, int64_t count) {
            insert_vertices(std::vector < T >(edges, edges + count));
        }

    template < typename T >
        void insert_vertices(const std::vector < T > &edges) {

            for (auto &it: edges)
                m_edges[it.id] = it;
            std::vector < Line_vertex > vertices = extract_vertices();

            log << "\nVertices of transformed graph: \n";
            for (auto vertex: vertices) {
                log << vertex.id << "(" << vertex.source << " - > ";
                log << vertex.target << ")" << vertex.cost << "\n";
            }

            add_vertices(vertices);
        }

    std::vector < Line_vertex > extract_vertices();

    void transform(pgrouting::DirectedGraph& digraph) {
        create_edges(digraph);
    }

    int64_t num_edges() const { return m_num_edges; }
    std::vector< Line_graph_rt > get_postgres_results_directed();

    friend std::ostream& operator<<(
            std::ostream &log, const Pgr_turnPenaltyGraph< G, T_V, T_E > &g) {
        typename Pgr_base_graph< G, T_V, T_E >::EO_i out, out_end;

        for (auto vi = vertices(g.graph).first;
                vi != vertices(g.graph).second; ++vi) {
            if ((*vi) >= g.m_num_vertices) break;
            log << (*vi) << ": " << " out_edges_of(" << g.graph[(*vi)] << "):";
            for (boost::tie(out, out_end) = out_edges(*vi, g.graph);
                    out != out_end; ++out) {
                log << ' '
                    << g.graph[*out].id << "=("
                    << g[g.source(*out)].id << ", "
                    << g[g.target(*out)].id << ")\t";
            }
            log << std::endl;
        }
        return log;
    }
};

template < class G, typename T_V, typename T_E >
std::vector< Line_graph_rt >
Pgr_turnPenaltyGraph< G, T_V, T_E >::get_postgres_results_directed() {
    std::vector< Line_graph_rt > results;

    typename boost::graph_traits < G >::edge_iterator edgeIt, edgeEnd;
    std::map < std::pair<int64_t,int64_t >, Line_graph_rt > unique;
    int64_t count = 0;

    log << "\nPostgres results\n";
    for (boost::tie(edgeIt, edgeEnd) = boost::edges(this->graph);
            edgeIt != edgeEnd; edgeIt++) {
        E e = *edgeIt;
        auto e_source = this->graph[this->source(e)].vertex_id;
        auto e_target = this->graph[this->target(e)].vertex_id;

        // This section extracts the original vertex and edge ids from the vertex id in the original graph
        int64_t t_hldr = e_target;
        int digits = 0;
        while(t_hldr >= 10){
          t_hldr /= 10;
          digits++;
        }

        int mod = 1;
        digits = digits - (t_hldr);
        while(digits > 0){
          mod *= 10;
          digits--;
        }
 
        int64_t e_id = e_target % mod;

        int digits2 = 0;
        int64_t s_hldr = e_source;
        while(s_hldr >= 10){
          s_hldr /= 10;
          digits2++;
        }

        mod = 1;
        digits2 = digits2 - (s_hldr);
        while(digits2 > 0){
          mod *= 10;
          digits2--;
        }
 
        int64_t e_id2 = e_source % mod;
        float e_cost = 0; 
        if(e_id == e_id2){
          for (const auto &it : m_edges) {
            auto edge2 = it.second;
            if(e_id == edge2.id){
              e_cost = edge2.cost;
              break; 
            }
          }
        }

        log << "e_source = " << e_source << " | e_target = " << e_target << "\n";

        Line_graph_rt edge = {
            ++count,
            e_source,
            e_target,
            e_cost,
            -1.0
        };

        unique[ std::pair<int64_t,int64_t>(e_source, e_target)] = edge;
    }
    for (const auto &edge: unique) {
        results.push_back(edge.second);
    }
    return results;
}

template < class G, typename T_V, typename T_E >
std::vector < Line_vertex >
Pgr_turnPenaltyGraph< G, T_V, T_E >::extract_vertices() {
    if (m_edges.empty()) return std::vector< Line_vertex >();

    std::vector< Line_vertex > vertices;

#if 1
    log << "\nEdges of original graph\n";
#endif

    for (const auto &it : m_edges) {
        auto edge = it.second;
        Line_vertex vertex(edge);

#if 1
        log << "ID: " << edge.id;
        log << "| source: " << edge.source;
        log << "| target: " << edge.target;
        log << "| cost: " << edge.cost;
        log << "| reverse_cost: " << edge.reverse_cost << "\n\n";
#endif

        if (edge.cost > 0) {
            vertex.id = (++(this->m_num_vertices));
            vertices.push_back(vertex);
            m_vertex_map[ std::pair<int64_t,int64_t>(edge.id, edge.source) ] = this->m_num_vertices;
        }

        if (edge.reverse_cost > 0) {
            vertex.id = (++(this->m_num_vertices));
            vertex.cost = edge.reverse_cost;
            vertex.vertex_id *= -1;
            std::swap(vertex.source, vertex.target);
            vertices.push_back(vertex);
            m_vertex_map[ std::pair<int64_t,int64_t>(edge.id, edge.target) ] = this->m_num_vertices;
        }
    }
#if 1
    for (auto it: m_vertex_map) {
        log << it.first.first << " | " << it.first.second << " | " << it.second << "\n";
    }
#endif
    return vertices;
}

template < class G, typename T_V, typename T_E >
template < typename T>
void
Pgr_turnPenaltyGraph< G, T_V, T_E >::graph_add_edge(
        int64_t _id,
        const T &source,
        const T &target,
        int64_t source_in_edge,
        int64_t source_out_edge) {

    bool inserted;
    typename Pgr_base_graph< G, T_V, T_E >::E e;

    pgassert(m_vertex_map.find( {source, source_in_edge} ) !=
        m_vertex_map.end());
    pgassert(m_vertex_map.find( {target, source_out_edge} ) !=
            m_vertex_map.end());

    auto index_source_edge = m_vertex_map[ std::pair<int64_t,int64_t>(source, source_in_edge) ];
    auto index_target_edge = m_vertex_map[ std::pair<int64_t,int64_t>(target, source_out_edge) ];

#if 1
    log << "\nsource_in_edge = " << source_in_edge << " | "
        << "source_out_edge = " << source_out_edge << " | "
        << "index_source_edge = " << index_source_edge << " | "
        << "index_target_edge = " << index_target_edge << " | "
        << "edge.source = " << source << " | "
        << "edge.target = " << target << "\n";
#endif

    auto vm_s = this->get_V(index_source_edge);
    auto vm_t = this->get_V(index_target_edge);

    pgassert(this->vertices_map.find(index_source_edge) != this->vertices_map.end());
    pgassert(this->vertices_map.find(index_target_edge) != this->vertices_map.end());

    boost::tie(e, inserted) =
        boost::add_edge(vm_s, vm_t, this->graph);

    this->graph[e].id = _id;
}

template < class G, typename T_V, typename T_E >
void
Pgr_turnPenaltyGraph< G, T_V, T_E >::create_edges(
        const pgrouting::DirectedGraph& digraph) {

    V_i vertexIt, vertexEnd;
    EO_i e_outIt, e_outEnd;
    EI_i e_inIt, e_inEnd;

    for(boost::tie(vertexIt, vertexEnd) = boost::vertices(digraph.graph);
            vertexIt != vertexEnd; vertexIt++) {
        V vertex = *vertexIt;

        // Generate the first half of the vertex id of each vertex in the new graph created
        // by spliting up this vertex in the original graph apart.
        int64_t vid = digraph[vertex].id;
        int64_t digits = 0;
        while (vid) {
            vid /= 10;
            digits++;
        }
        
        vid = digraph[vertex].id;
        int64_t times = 1;
        while (times <= vid)
           times *= 10;
        vid = (digits*times) + vid;

        for (boost::tie(e_outIt, e_outEnd) = boost::out_edges(vertex, digraph.graph);
                e_outIt != e_outEnd; e_outIt++) {

            // Generate the vertex id of the vertex created in the new graph that represents this
            // outgoing edge of the corresponding vertex in the original graph
            int64_t curid1 = vid;
            int64_t times1 = 1;
            while (times1 <= digraph.graph[*e_outIt].id)
                times1 *= 10;
            curid1 = curid1*times1 + digraph.graph[*e_outIt].id;

            // Add a vertex to the new graph for this incoming edge of this vertex in the original graph
            ++(this->m_num_vertices);
            m_vertex_map[ std::pair<int64_t,int64_t>(curid1, curid1) ] = this->m_num_vertices;
            auto v =  add_vertex(this->graph);
            this->graph[v].cp_members(this->m_num_vertices, curid1);
            this->graph[v].vertex_id = curid1;
            this->vertices_map[this->m_num_vertices] = v;
        }

        for (boost::tie(e_inIt, e_inEnd) = boost::in_edges(vertex, digraph.graph);
                e_inIt != e_inEnd; e_inIt++) {
 
            // Generate the vertex id of the vertex created in the new graph that represents this
            // incoming edge of the corresponding vertex in the original graph
            int64_t curid2 = vid;
            int64_t times3 = 1;
            while (times3 <= digraph.graph[*e_inIt].id)
                times3 *= 10;
            curid2 = curid2*times3 + digraph.graph[*e_inIt].id;

            // Add a vertex to the new graph for this outgoing edge of this vertex in the original graph
            ++(this->m_num_vertices);
            m_vertex_map[ std::pair<int64_t,int64_t>(curid2, curid2) ] = this->m_num_vertices;
            auto v2 =  add_vertex(this->graph);
            this->graph[v2].cp_members(this->m_num_vertices, curid2);
            this->graph[v2].vertex_id = curid2;
            this->vertices_map[this->m_num_vertices] = v2;

            for (boost::tie(e_outIt, e_outEnd) = boost::out_edges(vertex, digraph.graph);
                    e_outIt != e_outEnd; e_outIt++) {
               
                // This part creates an edge between this incoming edge's new vertex and all outgoing edge's new
                // vertices 
                auto source_out_edge = digraph.graph[*e_outIt].id;
                int64_t curid = vid;
                int64_t times2 = 1;

                while (times2 <= source_out_edge)
                    times2 *= 10;
                curid = curid*times2 + source_out_edge;

                ++m_num_edges;

                graph_add_edge(
                    m_num_edges,
                    curid2,
                    curid,
                    curid2,
                    curid
                );
            }
        }
    }
    // This part links all of these new clusters of vertices that represent individual
    // vertices in the original graph together with the edges from the original graph
    for(boost::tie(vertexIt, vertexEnd) = boost::vertices(digraph.graph);
        vertexIt != vertexEnd; vertexIt++) {

        V vertex = *vertexIt;
        int64_t vid = digraph[vertex].id;
        int64_t digits = 0;
        while (vid) {
            vid /= 10;
            digits++;
        }
        
        vid = digraph[vertex].id;
        int64_t times = 1;
        while (times <= vid)
           times *= 10;
        vid = (digits*times) + vid;

        for (boost::tie(e_inIt, e_inEnd) = boost::in_edges(vertex, digraph.graph);
                e_inIt != e_inEnd; e_inIt++) {
            int64_t curid2 = vid;
            int64_t times3 = 1;
            while (times3 <= digraph.graph[*e_inIt].id)
                times3 *= 10;
            curid2 = curid2*times3 + digraph.graph[*e_inIt].id;

            int64_t sourceid = digraph[digraph.source(*e_inIt)].id;
            int64_t curid5 = sourceid;
            int64_t digits2 = 0;
            while (curid5) {
                curid5 /= 10;
                digits2++;
            }
            curid5 = sourceid;

            int64_t times = 1;
            while (times <= curid5)
                times *= 10;
            curid5 = (digits2*times) + curid5;

            times3 = 1;
            while (times3 <= digraph.graph[*e_inIt].id)
                times3 *= 10;
            curid5 = curid5*times3 + digraph.graph[*e_inIt].id;

            ++m_num_edges;

            graph_add_edge(
               m_num_edges,
               curid5,
               curid2,
               curid5,
               curid2
            );
        }
    } 
}

template < class G, typename T_V, typename T_E >
 void
 Pgr_turnPenaltyGraph< G, T_V, T_E >::add_vertices(
         std::vector< T_V > vertices) {

     for (const auto vertex : vertices) {
         pgassert(this->vertices_map.find(vertex.id) == this->vertices_map.end());

         auto v =  add_vertex(this->graph);
         this->vertices_map[vertex.id] = v;
         this->graph[v].cp_members(vertex);

         pgassert(boost::num_vertices(this->graph) == this->num_vertices());
     }
     return;
 }

}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_PGR_TURNPENALTYGRAPH_HPP_
