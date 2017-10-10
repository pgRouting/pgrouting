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
    std::map < int64_t, std::pair< int64_t, int64_t > > m_transformation_map;
    std::map < std::pair< int64_t, int64_t >, int64_t > m_vertex_map;

    void apply_transformation(const pgrouting::DirectedGraph& digraph);

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
        void insert_edges(const T* edges, int64_t count) {
            insert_edges(std::vector < T >(edges, edges + count));
        }

    void transform(pgrouting::DirectedGraph& digraph) {
        apply_transformation(digraph);
    }

    template < typename T >
        void insert_edges(const std::vector < T > &edges){

        for (auto &it: edges)
            m_edges[it.id] = it;

    }

    int64_t num_edges() const { return m_num_edges; }
    std::vector< Turn_penalty_graph_rt > get_postgres_results_directed();

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
std::vector< Turn_penalty_graph_rt >
Pgr_turnPenaltyGraph< G, T_V, T_E >::get_postgres_results_directed() {
    std::vector< Turn_penalty_graph_rt > results;

    typename boost::graph_traits < G >::edge_iterator edgeIt, edgeEnd;
    std::map < std::pair<int64_t,int64_t >, Turn_penalty_graph_rt > unique;
    int64_t count = 0;

    log << "\nPostgres results\n";
    for (boost::tie(edgeIt, edgeEnd) = boost::edges(this->graph);
            edgeIt != edgeEnd; edgeIt++) {

        E e = *edgeIt;
        auto e_source = this->graph[this->source(e)].vertex_id;
        auto e_target = this->graph[this->target(e)].vertex_id;
        
        auto target_vertex_edge_pair = m_transformation_map[e_target];
        auto target_vertex_id = target_vertex_edge_pair.first;
        auto target_edge_id = target_vertex_edge_pair.second;
        auto source_vertex_edge_pair = m_transformation_map[e_source];
        auto source_vertex_id = source_vertex_edge_pair.first;
        auto source_edge_id = source_vertex_edge_pair.second;

        float e_cost = 0; 
        if(source_edge_id == target_edge_id){
            e_cost = m_edges[source_edge_id].cost;
        }

        log << "e_source = " << e_source << " | e_target = " << e_target << "\n";

        Turn_penalty_graph_rt edge = {
            ++count,
            e_source,
            e_target,
            e_cost,
            source_vertex_id,
            source_edge_id,
            target_vertex_id,
            target_edge_id
        };

        unique[ std::pair<int64_t,int64_t>(e_source, e_target)] = edge;
    }
    for (const auto &edge: unique) {
        results.push_back(edge.second);
    }
    return results;
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
Pgr_turnPenaltyGraph< G, T_V, T_E >::apply_transformation(
        const pgrouting::DirectedGraph& digraph) {

    V_i vertexIt, vertexEnd;
    EO_i e_outIt, e_outEnd;
    EI_i e_inIt, e_inEnd;

    // For every incoming and outgoing edge of every vertex in the original graph,
    // create a vertex in the new graph and link incoming edge vertices to the outgoing
    // edge vertices
    for(boost::tie(vertexIt, vertexEnd) = boost::vertices(digraph.graph);
            vertexIt != vertexEnd; vertexIt++) {

        V vertex = *vertexIt;
        int64_t vertex_id = digraph[vertex].id;
        for (boost::tie(e_outIt, e_outEnd) = boost::out_edges(vertex, digraph.graph);
                    e_outIt != e_outEnd; e_outIt++) {
               
                ++(this->m_num_vertices);
                int64_t out_edge_id = digraph.graph[*e_outIt].id;
                int64_t out_edge_vertex_id = this->m_num_vertices;

                m_transformation_map[this->m_num_vertices] = std::pair<int64_t,int64_t>(vertex_id, out_edge_id);
                m_vertex_map[ std::pair<int64_t,int64_t>(vertex_id, out_edge_id) ] = out_edge_vertex_id;
                auto v =  add_vertex(this->graph);
                this->graph[v].cp_members(vertex_id, out_edge_id);
                this->graph[v].vertex_id = this->m_num_vertices;
                this->vertices_map[this->m_num_vertices] = v;
        }

        for (boost::tie(e_inIt, e_inEnd) = boost::in_edges(vertex, digraph.graph);
                e_inIt != e_inEnd; e_inIt++) {
 
            ++(this->m_num_vertices);
            int64_t in_edge_id = digraph.graph[*e_inIt].id;
            int64_t in_edge_vertex_id = this->m_num_vertices;

            m_transformation_map[this->m_num_vertices] = std::pair<int64_t,int64_t>(vertex_id, in_edge_id);
            m_vertex_map[ std::pair<int64_t,int64_t>(vertex_id, in_edge_id) ] = in_edge_vertex_id;
            auto v =  add_vertex(this->graph);
            this->graph[v].cp_members(vertex_id, in_edge_id);
            this->graph[v].vertex_id = this->m_num_vertices;
            this->vertices_map[this->m_num_vertices] = v;

            for (boost::tie(e_outIt, e_outEnd) = boost::out_edges(vertex, digraph.graph);
                    e_outIt != e_outEnd; e_outIt++) {

                int64_t out_edge_id = digraph.graph[*e_outIt].id;

                ++m_num_edges;

                graph_add_edge(
                    m_num_edges,
                    vertex_id,
                    vertex_id,
                    in_edge_id,
                    out_edge_id
                );
            }
        }
    }

    // Add the edges from the original graph to link every cluster 
    // of vertices in the new graph that were created from single vertices
    // in the original graph
    for(boost::tie(vertexIt, vertexEnd) = boost::vertices(digraph.graph);
        vertexIt != vertexEnd; vertexIt++) {

        V vertex = *vertexIt;
        int64_t vertex_id = digraph[vertex].id;

        for (boost::tie(e_inIt, e_inEnd) = boost::in_edges(vertex, digraph.graph);
                e_inIt != e_inEnd; e_inIt++) {

            int64_t source_vertex_id = digraph[digraph.source(*e_inIt)].id;
            int64_t in_edge_id = digraph.graph[*e_inIt].id;

            ++m_num_edges;

            graph_add_edge(
               m_num_edges,
               source_vertex_id,
               vertex_id,
               in_edge_id,
               in_edge_id
            );
        }
    }
}
}  // namespace graph
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_PGR_TURNPENALTYGRAPH_HPP_
