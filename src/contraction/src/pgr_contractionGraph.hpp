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

#include <limits> 
#include <algorithm>
#include "../../common/src/pgr_base_graph.hpp"


namespace pgRouting {

namespace graph {

template <class G, typename T_V, typename T_E>
        class Pgr_contractionGraph;
    }

typedef typename graph::Pgr_contractionGraph <
boost::adjacency_list < boost::listS, boost::vecS,
boost::undirectedS,
contraction::Vertex, contraction::Edge >,
contraction::Vertex, contraction::Edge > CHUndirectedGraph;

typedef typename graph::Pgr_contractionGraph <
boost::adjacency_list < boost::listS, boost::vecS,
boost::bidirectionalS,
contraction::Vertex, contraction::Edge >,
contraction::Vertex, contraction::Edge > CHDirectedGraph;

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
     typedef typename std::map< int64_t, V > id_to_V;
     typedef typename id_to_V::const_iterator LI;
     Identifiers<V> removed_vertices;
     std::vector< T_E > shortcuts;
     typedef typename boost::graph_traits < G >::degree_size_type       degree_size_type;
     Pgr_contractionGraph< G , T_V, T_E >(const std::vector< T_V > &vertices, graphType gtype)
     : Pgr_base_graph< G , T_V, T_E >(vertices, gtype) {
     }

     explicit Pgr_contractionGraph< G , T_V, T_E >(graphType gtype)
         : Pgr_base_graph< G , T_V, T_E >(gtype) {
     }


      template < typename T >
         void graph_insert_data(const T *edges, int64_t count) {
             graph_insert_data(std::vector < T >(edges, edges + count));
         }
        template < typename T >
         void graph_insert_data(const std::vector < T > &edges) {
             for (const auto edge : edges) {
                 graph_add_edge(edge);
             }
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

     Identifiers<V> find_adjacent_vertices(V v) const {
        EO_i out, out_end;
        EI_i in, in_end;
        Identifiers<V> adjacent_vertices;
        V out_vertex, in_vertex;
        out_vertex = in_vertex = -1;
        for (boost::tie(out, out_end) = out_edges(v, this->graph);
                out != out_end; ++out) {
                out_vertex = target(*out, this->graph);
                adjacent_vertices += out_vertex;
        }
        for (boost::tie(in, in_end) = in_edges(v, this->graph);
                in != in_end; ++in) {
                in_vertex = source(*in, this->graph);
                adjacent_vertices += in_vertex;
        }
        return adjacent_vertices;
     }

     T_V& operator[](V v) {
         return this->graph[v];
     }

     T_E& operator[](E e) {
         return this->graph[e];
     }

     void print_graph(std::ostringstream &log) {

         EO_i out, out_end;
         for (auto vi = vertices(this->graph).first; vi != vertices(this->graph).second; ++vi) {
             if ((*vi) >= this->m_num_vertices) break;
             log << this->graph[(*vi)].id << "(" << (*vi) << ")" 
             << this->graph[(*vi)].contracted_vertices() << std::endl;
             log << " out_edges_of(" << this->graph[(*vi)].id << "):";
             for (boost::tie(out, out_end) = out_edges(*vi, this->graph);
                     out != out_end; ++out) {
                 log << ' ' << this->graph[*out].id << "=(" << this->graph[source(*out, this->graph)].id
                     << ", " << this->graph[target(*out, this->graph)].id << ") = "
                     <<  this->graph[*out].cost <<"\t";
             }
             log << std::endl;
         }
     }

     void print_contracted_vertices(std::ostringstream &log, int64_t vertex_id) {
         if (!this->has_vertex(vertex_id)) return;
         V v = this->get_V(vertex_id);
        log << "{";
        for (auto vertex : this->graph[v].contracted_vertices()) {
            log << this->graph[vertex].id << ", ";
        }
        log << "}";
         
     }

     void get_ids(std::ostringstream &log,
        Identifiers<int64_t> boost_ids) {
        log << "{";
         for (auto id : boost_ids) {
            log << this->graph[id].id << ", ";
        }
        log << "}";
     }

     degree_size_type in_degree_from_vertex(V vertex, V neighbor)
     {
        degree_size_type degree = 0;
        EI_i in_i, in_end;
        E e;
        for (boost::tie(in_i, in_end) = boost::in_edges(vertex, this->graph);
                in_i != in_end; ++in_i) {
            e = *in_i;

            if (source(e, this->graph) == neighbor)
            {
                degree++;
            }
        }
        return degree;
     }

     degree_size_type out_degree_to_vertex(V vertex, V neighbor)
     {
        degree_size_type degree = 0;
        EO_i out_i, out_end;
        E e;
        for (boost::tie(out_i, out_end) = boost::out_edges(vertex, this->graph);
                out_i != out_end; ++out_i) {
            e = *out_i;

            if (target(e, this->graph) == neighbor)
            {
                degree++;
            }
        }
        return degree;
     }

     void disconnect_vertex(std::ostringstream &log, V vertex) {
        
        T_E d_edge;
        EO_i out, out_end;
        log << "Disconnecting current vertex " << this->graph[vertex].id << "\n";
        removed_vertices += vertex;
        // store the edges that are going to be removed
        //#if 0
        for (boost::tie(out, out_end) = out_edges(vertex, this->graph);
            out != out_end; ++out) {

            d_edge.id = this->graph[*out].id;
            d_edge.source = this->graph[source(*out, this->graph)].id;
            d_edge.target = this->graph[target(*out, this->graph)].id;
            d_edge.cost = this->graph[*out].cost;
            //log << "Out edge: { source : "<< d_edge.source << ", target: " << d_edge.target  << "}\n";
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
                //log << "In edge: { source : "<< d_edge.source << ", target: " << d_edge.target  << "}\n";
                this->removed_edges.push_back(d_edge);
            }
        }
        
            //#endif
        #if 1
        // delete incomming and outgoing edges from the vertex
        try
        {
            boost::clear_vertex(vertex, this->graph);
            //print_graph(log );
        }
        catch ( ... ) {
            log << "Caught unknown expection!\n";
        }
        //log << "Disconnected current vertex " << this->graph[vertex].id << "\n";
        #endif
        //log << "return disconnect_vertex\n";
    }

    void get_remaining_vertices(std::ostringstream& log, 
        std::vector<T_V>& remaining_vertices) {
        //log << "remaining_vertices\n";
        for (auto vi = vertices(this->graph).first; vi != vertices(this->graph).second; ++vi) {
            if (!removed_vertices.has(*vi))
            {
                //log << this->graph[*vi].id << "\n";
                remaining_vertices.push_back(this->graph[*vi]);
            }
        }
        //return remaining_vertices;
    }

    void get_remaining_vertices(std::ostringstream& log, 
        Identifiers<int64_t>& remaining_vertices) {
        log << "remaining_vertices\n";
        for (auto vi = vertices(this->graph).first; vi != vertices(this->graph).second; ++vi) {
            if (!removed_vertices.has(*vi) && this->graph[*vi].has_contracted_vertices())
            {
                //log << this->graph[*vi].id << "\n";
                remaining_vertices += this->graph[*vi].id;
            }
        }
        //return remaining_vertices;
    }
    void get_boost_ids(Identifiers<int64_t> user_ids, std::ostringstream& log)
    {
        Identifiers<V> boost_ids;
        for (auto id: user_ids)
        {
            boost_ids += this->get_V(id);
        }
        log << boost_ids;
    }
    

    E get_min_cost_edge(V source, V destination, std::ostringstream& log)
    {
        E e;
        EO_i out_i, out_end;
        E min_cost_edge;
        double min_cost = std::numeric_limits<double>::max();
        //log << "Max min cost " << min_cost << std::endl;
        for (boost::tie(out_i, out_end) = boost::out_edges(source, this->graph);
                out_i != out_end; ++out_i) {
            e = *out_i;
            //log << this->graph[e];
            if (target(e, this->graph) == destination)
            {
                if (this->graph[e].cost < min_cost)
                {
                    min_cost = this->graph[e].cost;
                    min_cost_edge = e;
                }
            }
        }
        //return this->graph[e];
        log << "Min cost edge from " << this->graph[source].id << " to " << this->graph[destination].id << std::endl;
        log << this->graph[min_cost_edge];  
        return min_cost_edge;
    }

    static bool compareById(const T_E &edge1, const T_E &edge2)
    {
        return edge1.id > edge2.id;
    }

    void get_shortcuts(std::vector<T_E>& shortcut_edges, std::ostringstream& log)
    {
        log << "Getting shortcuts\n";
        for (auto shortcut : shortcuts)
        {
            log << shortcut;
            shortcut_edges.push_back(shortcut);
        }
        std::sort(shortcut_edges.begin(), shortcut_edges.end(), compareById);
    }

    void print_shortcuts(std::ostringstream& log)
    {
        log << "Printing shortcuts\n";
        for (auto shortcut : shortcuts)
        {
            log << shortcut;
        }
    }
    void add_contracted_edge_vertices(V v, T_E &e)
    {
        for (auto vid : e.contracted_vertices())
        {
            this->graph[v].add_vertex_id(vid);
        }
        e.clear_contracted_vertices();
    }

template < typename T>
void graph_add_edge(const T &edge) {
    bool inserted;
    E e;
    if ((edge.cost < 0) && (edge.reverse_cost < 0))
        return;

    /*
     * true: for source
     * false: for target
     */
    auto vm_s = this->get_V(T_V(edge, true));
    auto vm_t = this->get_V(T_V(edge, false));

    pgassert(this->vertices_map.find(edge.source) != this->vertices_map.end());
    pgassert(this->vertices_map.find(edge.target) != this->vertices_map.end());
    if (edge.cost >= 0) {
        boost::tie(e, inserted) =
            boost::add_edge(vm_s, vm_t, this->graph);
        this->graph[e].cost = edge.cost;
        this->graph[e].id = edge.id;
        this->graph[e].first = true;
        this->graph[e].source = edge.source;
        this->graph[e].target = edge.target;
    }

    if (edge.reverse_cost >= 0) {
        boost::tie(e, inserted) =
            boost::add_edge(vm_t, vm_s, this->graph);

        this->graph[e].cost = edge.reverse_cost;
        this->graph[e].id = edge.id;
        this->graph[e].first = false;
        this->graph[e].target = edge.source;
        this->graph[e].source = edge.target;
    }
}
    
void graph_add_shortcut(const T_E &edge, std::ostringstream& log) {
    bool inserted;
    E e;
    if (edge.cost < 0)
        return;

    /*
     * true: for source
     * false: for target
     */

    log << "Graph before adding shortcut\n";
    print_graph(log);

    pgassert(this->vertices_map.find(edge.source) != this->vertices_map.end());
    pgassert(this->vertices_map.find(edge.target) != this->vertices_map.end());
    auto vm_s = this->get_V(edge.source);
    auto vm_t = this->get_V(edge.target);

    log << "Adding edge between " << this->graph[vm_s] << ", "
    << this->graph[vm_t] << std::endl;

    if (edge.cost >= 0) {
        boost::tie(e, inserted) =
            boost::add_edge(vm_s, vm_t, this->graph);
        log << "inserted: " << inserted << std::endl;
        this->graph[e].cp_members(edge, log);
        log << this->graph[e];
        //this->graph[e].id = this->graph[e].eid;
        log << "Graph after adding shortcut\n";
        print_graph(log);
        T_E shortcut;
        shortcut.cp_members(edge, log);
        shortcuts.push_back(shortcut);
        
    }
}



bool graph_add_shortcut(int64_t source, int64_t target, double cost, E &e, std::ostringstream& log) {
    bool inserted = false;
    if (cost < 0)
        return inserted;

    log << "Graph before adding shortcut\n";
    print_graph(log);

    pgassert(this->vertices_map.find(source) != this->vertices_map.end());
    pgassert(this->vertices_map.find(target) != this->vertices_map.end());
    auto vm_s = this->get_V(source);
    auto vm_t = this->get_V(target);

    log << "Adding edge between " << this->graph[vm_s] << ", "
    << this->graph[vm_t] << std::endl;

    if (cost >= 0) {
        boost::tie(e, inserted) =
            boost::add_edge(vm_s, vm_t, this->graph);
        log << "inserted: " << inserted << std::endl;
        //this->graph[e].cp_members(edge, log);
        //this->graph[e].id = this->graph[e].eid;
        log << "Graph after adding shortcut\n";
        print_graph(log);
    }
    return inserted;
}




};

} // namespace graph

} // namespace pgrouting
 