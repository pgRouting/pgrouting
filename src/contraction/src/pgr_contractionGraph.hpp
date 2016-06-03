/*PGR-GNU*****************************************************************
File: pgr_contractionGraph.c

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Rohith Reddy
Mail: 

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
#pragma once
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif

#include "../../common/src/pgr_base_graph.hpp"
namespace pgRouting {

namespace graph {

template <class G, typename T_V, typename T_E>
        class Pgr_contractionGraph;
    }

typedef typename graph::Pgr_contractionGraph <
boost::adjacency_list < boost::listS, boost::vecS,
boost::undirectedS,
contraction::Vertex, Basic_edge >,
contraction::Vertex, Basic_edge > CHUndirectedGraph;

typedef typename graph::Pgr_contractionGraph <
boost::adjacency_list < boost::listS, boost::vecS,
boost::bidirectionalS,
contraction::Vertex, Basic_edge >,
contraction::Vertex, Basic_edge > CHDirectedGraph;

namespace graph{

template <class G, typename T_V, typename T_E>
class Pgr_contractionGraph : public Pgr_base_graph<G, T_V, T_E> {
 public:
    typedef typename boost::graph_traits < G >::vertex_descriptor V;
     typedef typename boost::graph_traits < G >::edge_descriptor E;
     typedef typename boost::graph_traits < G >::vertex_iterator V_i;
     typedef typename boost::graph_traits < G >::edge_iterator E_i;
     typedef typename boost::graph_traits < G >::out_edge_iterator EO_i;
     typedef typename boost::graph_traits < G >::in_edge_iterator EI_i;
     Pgr_contractionGraph< G , T_V, T_E >(const std::vector< T_V > &vertices, graphType gtype)
     : Pgr_base_graph< G , T_V, T_E >(vertices, gtype) {
     }

     explicit Pgr_contractionGraph< G , T_V, T_E >(graphType gtype)
         : Pgr_base_graph< G , T_V, T_E >(gtype) {
     }

     bool is_connected(int64_t v) const {
        if (this->in_degree(this->get_V(v)) == 0 && this->out_degree(this->get_V(v)) == 0) {
            return false;
        }
        return true;
     }

     V find_adjacent_vertex(V v) const {
        EO_i out, out_end;
        EI_i in, in_end;
        V out_vertex, in_vertex;
        out_vertex = in_vertex = -1;
        #if 0
        if (!graph.get_gVertex(vertex_id, v)) {
                return adjacent_vertices_set;
        }
        #endif
        for (boost::tie(out, out_end) = out_edges(v, this->graph);
                out != out_end; ++out) {
                out_vertex = target(*out, this->graph);
        }
        for (boost::tie(in, in_end) = in_edges(v, this->graph);
                in != in_end; ++in) {
                in_vertex = source(*in, this->graph);
        }
        if (in_vertex == -1)
            return out_vertex;
        else if (out_vertex == -1)
            return in_vertex;
        else if (out_vertex == in_vertex)
            return in_vertex;
        return out_vertex;
     }

     T_V& operator[](V v) {
         return this->graph[v];
     }

     std::ostream& print_graph(std::ostream &log) {

         EO_i out, out_end;

         for (auto vi = vertices(this->graph).first; vi != vertices(this->graph).second; ++vi) {
             if ((*vi) >= this->m_num_vertices) break;
             //log << this->graph[(*vi)].print_vertex(log, this->graph);
             log << this->graph[(*vi)];
             log << " out_edges_of(" << this->graph[(*vi)].id << "):";
             for (boost::tie(out, out_end) = out_edges(*vi, this->graph);
                     out != out_end; ++out) {
                 log << ' ' << this->graph[*out].id << "=(" << this->graph[source(*out, this->graph)].id
                     << ", " << this->graph[target(*out, this->graph)].id << ") = "
                     <<  this->graph[*out].cost <<"\t";
             }
             log << std::endl;
         }
         return log;
     }

     std::ostream& disconnect_vertex(std::ostream &log, V vertex) {
        
        T_E d_edge;
        EO_i out, out_end;
        log << "Disconnecting current vertex " << this->graph[vertex].id << "\n";
        
        
        // store the edges that are going to be removed
        //#if 0
        for (boost::tie(out, out_end) = out_edges(vertex, this->graph);
            out != out_end; ++out) {

            d_edge.id = this->graph[*out].id;
            d_edge.source = this->graph[source(*out, this->graph)].id;
            d_edge.target = this->graph[target(*out, this->graph)].id;
            d_edge.cost = this->graph[*out].cost;
            log << "Out edge: { source : "<< d_edge.source << ", target: " << d_edge.target  << "}\n";
            this->removed_edges.push_back(d_edge);
        }

        // special case
        if (this->m_gType == DIRECTED) {
            EI_i in, in_end;
            for (boost::tie(in, in_end) = in_edges(vertex, this->graph);
                in != in_end; ++in) {
                d_edge.id = this->graph[*in].id;
                d_edge.source = this->graph[source(*in, this->graph)].id;
                d_edge.target = this->graph[target(*in, this->graph)].id;
                d_edge.cost = this->graph[*in].cost;
                log << "In edge: { source : "<< d_edge.source << ", target: " << d_edge.target  << "}\n";
                this->removed_edges.push_back(d_edge);
            }
        }
        
            //#endif
        #if 0
        // delete incomming and outgoing edges from the vertex
        try
        {

            boost::clear_vertex(vertex, this->graph);
        }
        catch ( ... ) {
            log << "Caught unknown expection!\n";
        }
        //log << "Disconnected current vertex " << this->graph[vertex].id << "\n";
        #endif
        return log;
    }
};

} // namespace graph

} // namespace pgrouting
 