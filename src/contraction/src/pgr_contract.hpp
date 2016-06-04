/*PGR-GNU*****************************************************************
File: pgr_contract.hpp

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
#include <stack>
#include <iostream>
#include <sstream>
#include <deque>
#include <queue>
#include <string>
#include <functional>
extern "C" {
#include "postgres.h"
}

#include "./pgr_contractionGraph.hpp"
#include "./pgr_deadEndContraction.hpp"
#include "../../common/src/pgr_assert.h"


template < class G > class Pgr_contract;

template < class G >
void perform_deadEnd(G &graph, Identifiers<int64_t> forbidden_vertices,
    std::ostringstream& debug)
{
    pgRouting::Pgr_deadEndContraction<G> deadendContractor;
    debug << "Setting forbidden_vertices";
    deadendContractor.setForbiddenVertices(graph, forbidden_vertices
        , debug);

    deadendContractor.calculateVertices(graph, debug);
    try
    {
        #if 1
        deadendContractor.doContraction(graph, debug);
        #endif
    }
    catch ( ... ) {
        debug << "Caught unknown expection!\n";
    }
}



template < class G >
void pgr_contractGraph(
    G &graph, Identifiers<int64_t> forbidden_vertices,
    int64_t *contraction_order,
    size_t size_contraction_order,
    int64_t max_cycles,
    std::ostringstream& contracted_graph_name,
    std::ostringstream& contracted_graph_blob,
    std::ostringstream& removedEdges,
    std::ostringstream& removedVertices,
    std::ostringstream& psuedoEdges,
    std::ostringstream& debug) {

    //typedef typename G::V V;
    //Pgr_contract< G > fn_contract;

   /* debug << "Forbidden vertices\n" <<   " { \n";
        for (int64_t i = 0; i < size_forbidden_vertices; ++i) {
            debug << forbidden_vertices[i] << ", ";
            debug << "\n";
        }
        debug << " }\n";  */

    /*
        The forbidden vertices and all vertices are
        computed before contraction
    */
    #if 0
    debug << "Forbidden vertices" << "\n";
    fn_contract.setForbiddenVertices(forbidden_vertices,size_forbidden_vertices);
    fn_contract.print_forbidden_vertices(debug);
    debug << "All vertices" << "\n";
    fn_contract.getAllVertices(graph);
    fn_contract.print_all_vertices(debug);
    #endif

    std::deque<int64_t> contract_order;
    // push -1 to indicate the start of the queue
    contract_order.push_back(-1);
    for (size_t i = 0; i < size_contraction_order; ++i) {
        contract_order.push_back(contraction_order[i]);
    }
    for (int64_t i = 0; i < max_cycles; ++i) {
        int64_t front = contract_order.front();
        debug << "Starting cycle " << i+1 << "\n";
        contract_order.pop_front();
        contract_order.push_back(front);
        front = contract_order.front();
        while (front != -1) {
            switch (front) {
                case -1:
                debug << "Finished cycle " << i+1 << std::endl;
                break;
                default:
                debug << "contraction "<< front << " asked" << std::endl;
                if (front == 0)
                {

                    debug << "Graph before dead end contraction" << std::endl;
                    graph.print_graph(debug);
                    debug << "Performing dead end contraction" << std::endl;
                    perform_deadEnd(graph, forbidden_vertices, debug);
                    debug << "Graph after dead end contraction" << std::endl;
                    graph.print_graph(debug);
                }
                contract_order.pop_front();
                contract_order.push_back(front);
                front = contract_order.front();
            }
        }
    }
    #if 0
    debug << "Dead end set" << "\n";
    fn_contract.getDeadEndSet(graph);
    fn_contract.print_dead_end_vertices(debug);
    debug << "Non contractible set" << "\n";
    fn_contract.getNonContractedVertices();
    fn_contract.print_non_contracted_vertices(debug);
    debug << "Adjacent vertices of vertex 6" << "\n";
    fn_contract.print_identifiers(debug, fn_contract.getAdjacentVertices(graph, 6));
    #endif
    #if 0
    fn_contract.calculateDegrees(graph);
    //fn_contract.degreeMap(graph, debug);
    
    fn_contract.contract_to_level(graph, level);
    fn_contract.getGraphName(contracted_graph_name, level);
    fn_contract.getGraph_string(graph, contracted_graph_blob);
    fn_contract.getRemovedE_string(graph, removedEdges);
    fn_contract.getRemovedV_string(removedVertices);
    fn_contract.getPsuedoE_string(psuedoEdges);
    #endif
}

bool is_valid_contraction_number(int number) {
    switch (number) {
        case -2:
        return false;
        break;
        case -1:
        return false;
        break;
        case 0:
        return true;
        break;
        default:
        return false;
        break;
    }
}

template < class G >
class Pgr_contract {


public:
    /** @name Graph related types
       Type      |     boost meaning     |   pgRouting meaning
       :---------: | :-------------------- | :----------------------
       G        | boost::adjacency_list |   Graph
       V        | vertex_descriptor     |   Think of it as local ID of a vertex
       E        | edge_descriptor       |   Think of it as local ID of an edge
       V_i      | vertex_iterator       |   To cycle the vertices of the Graph
       E_i      | edge_iterator         |   To cycle the edges of the Graph
       EO_i     | out_edge_iterator     |   To cycle the out going edges of a vertex
       EI_i     | in_edge_iterator      |   To cycle the in coming edges of a vertex (only in bidirectional graphs)
    */
     //@{
    typedef typename G::V V;
    typedef typename G::E E;
    typedef typename G::V_i V_i;
    typedef typename G::E_i E_i;
    typedef typename G::EO_i EO_i;
    #if 0
    typedef typename G::degree_to_V_i degree_to_V_i;
    #endif
    typedef typename G::EI_i EI_i;
    //@}

    //! @name Framework related functions
    //@{
    /*! \brief Disconnects all incoming and outgoing edges from the vertex
     *  Calls the disconnect_vertex function of the graph with assertions
     *  @param graph
     *  @param vertex_id
     */
    void disconnectVertex(G &graph, int64_t vertex_id);
    
    /*! \brief Stores the ids of those vertices forbidden from contraction in a set
     *  Stores them in the set *forbidden_vertices*
     *  @param forbidden_vertices
     *  @param size_forbidden_vertices
     */
    void setForbiddenVertices(int64_t *forbidden_vertices,
    size_t size_forbidden_vertices );
    
    /*! \brief Stores ids of all the vertices of the graph in a set
     *  Stores them in the set *all_vertices*
     *  @param graph
     */
    void getAllVertices(G &graph);

    /*! \brief Stores the set of ids of those vertices which are not contracted
     *  Stores them in the set *non_contracted*
     */
    void getNonContractedVertices();

    /*! \brief Returns a set of ids of all those vertices adjacent to vertex 
     *  with id *vertex_id* 
     *  Calls the disconnect_vertex function of the graph with assertions
     *  @param graph
     *  @param vertex_id
     */
    Identifiers<int64_t> getAdjacentVertices(G &graph, int64_t vertex_id);
    
    /*! \brief Writes the string form of identifier class to the *stream*
     *  @param stream
     */
    void print_identifiers(std::ostringstream& stream, Identifiers<int64_t> identifiers);

    /*! \brief Writes the string form of forbidden vertices to the *stream*
     *  @param stream
     */
    void print_forbidden_vertices(std::ostringstream& stream);
    
    /*! \brief Writes the string form of all vertices to the *stream*
     *  @param stream
     */
    void print_all_vertices(std::ostringstream& stream);
    
    /*! \brief Writes the string form of non contracted vertices to the *stream*
     *  @param stream
     */
    void print_non_contracted_vertices(std::ostringstream& stream);

    //@}



    //! @name Dead end contraction related functions
    //@{

    /*! \brief Checks whether a vertex is a dead end vertex
     *  @param graph
     *  @param vertex_id
     */
    bool is_dead_end(G &graph, int64_t vertex_id) const;
    
    /*! \brief Stores ids of dead end vertices of the graph in a set
     *  Stores them in the set *dead_end_vertices*
     *  @param graph
     */
    void getDeadEndSet(G &graph);

    /*! \brief Writes the string form of dead end vertices to the *stream*
     *  @param stream
     */
    void print_dead_end_vertices(std::ostringstream& stream);

    //@}




    //bool is_connected(G &graph, V v) const;
    #if 0
    void contract_to_level(
        G &graph,
        int64_t level);
    #endif

    #if 0
    void dead_end_contraction(G &graph);
    void remove_2_degree_vertices(G &graph);
    

    void calculateDegrees(G &graph);

    void degreeMap(G &graph, std::ostringstream& dmap);

    void getGraphName(std::ostringstream& name, Contraction_type ctype);

    int64_t getGraph_string(G &graph, std::ostringstream& estring);

    void getRemovedE_string(G &graph, std::ostringstream& estring);

    void getRemovedV_string(std::ostringstream& vstring);

    void getPsuedoE_string(std::ostringstream& pstring);
    
    typedef typename std::map<V, std::deque<Edge> > removed_V;
    typedef typename std::map<V, std::deque<Edge> >::iterator removed_V_i;
    typedef typename std::map<int64_t, std::pair<int64_t, int64_t> > psuedo_E;
    typedef typename std::map<int64_t, std::pair<int64_t, int64_t> >::iterator psuedo_E_i;
    typedef std::map< int, std::priority_queue<int64_t, std::vector<int64_t>, std::greater<int64_t> > > degree_to_V;
    //typedef std::map< int, std::vector<int64_t> > degree_to_V;
    typedef typename std::vector<V>::iterator Q_i;
    #endif
private:


    int64_t last_edge_id;
    //! Used for storing the ids of all vertices of the graph
    Identifiers<int64_t> all;
    //! Used for storing the ids of dead end vertices of the graph
    Identifiers<int64_t> dead_end;
    //! Used for storing the ids of vertices of the graph which are not contracted
    Identifiers<int64_t> non_contracted;
    //! Used for storing the ids of vertices forbidden from contraction
    Identifiers<int64_t> forbidden;
    #if 0
    removed_V removedVertices;
    psuedo_E psuedoEdges;
    degree_to_V degree_to_V_map;
    #endif
    //set of dead_end_vertices;
    //Identifiers<V> dead_end_vertices;

};


 /******************** IMPLEMENTATION ******************/

template < class G >
void Pgr_contract< G >::disconnectVertex(G &graph, int64_t vertex_id) {


    pgassert(graph.is_connected(vertex_id));
    pgassert(is_dead_end(vertex_id));
    graph.disconnect_vertex(vertex_id);
    pgassert(!graph.is_connected(vertex_id));

}

template < class G >
void Pgr_contract< G >::setForbiddenVertices(int64_t *forbidden_vertices,
    size_t size_forbidden_vertices ) {


    for (int64_t i = 0; i < size_forbidden_vertices; ++i) {
            forbidden += forbidden_vertices[i];
        }

}

template <class G>
void Pgr_contract< G >::getAllVertices(G &graph) {
    //Identifiers<int64_t> dead_end_vertices;
    V_i vi;
    for (vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
        // debug << "Checking vertex " << graph.graph[(*vi)].id << '\n';
            all += graph.graph[(*vi)].id;
    }
}

template <class G>
void Pgr_contract< G >::getNonContractedVertices() {
    non_contracted = all - dead_end;
}

template < class G >
Identifiers<int64_t> Pgr_contract< G >::getAdjacentVertices(G &graph, int64_t vertex_id) {
    EO_i out, out_end;
    EI_i in, in_end;
    V v;
    Identifiers<int64_t> adjacent_vertices_set;
    if (!graph.has_vertex(vertex_id)) {
            return adjacent_vertices_set;
    }
    v = graph.get_V(vertex_id);
    #if 0
    if (!graph.get_gVertex(vertex_id, v)) {
            return adjacent_vertices_set;
    }
    #endif
    for (boost::tie(out, out_end) = out_edges(v, graph.graph);
            out != out_end; ++out) {
            adjacent_vertices_set += graph.graph[target(*out, graph.graph)].id;
        }
    for (boost::tie(in, in_end) = in_edges(v, graph.graph);
            in != in_end; ++in) {
            adjacent_vertices_set += graph.graph[source(*in, graph.graph)].id;
        }
    return adjacent_vertices_set;
}

template <class G>
void Pgr_contract< G >::print_identifiers(std::ostringstream& stream, Identifiers<int64_t> identifiers) {
    //std::ostringstream out;
    stream << identifiers << '\n';
    //return out;
}

template <class G>
void Pgr_contract< G >::print_forbidden_vertices(std::ostringstream& stream) {
    //std::ostringstream out;
    stream << forbidden << '\n';
    //return out;
}

template <class G>
void Pgr_contract< G >::print_all_vertices(std::ostringstream& stream) {
    //std::ostringstream out;
    stream << all << '\n';
    //return out;
}


template <class G>
void Pgr_contract< G >::print_non_contracted_vertices(std::ostringstream& stream) {
    //std::ostringstream out;
    stream << non_contracted << '\n';
    //return out;
}
#if 0
template < class G >
bool  Pgr_contract< G >::is_connected(G &graph, V v) const {
    if (graph.in_degree(v) == 0 && graph.out_degree(v) == 0) {
        return false;
    }
    return true;
}
#endif

template < class G >
bool Pgr_contract< G >::is_dead_end(G &graph, int64_t vertex_id) const {

    //debug << "in_degree: " << graph.in_degree(vertex_id) << '\n';
    //debug << "out_degree: " << graph.out_degree(vertex_id) << '\n';
    V v;
    if (!graph.has_vertex(vertex_id)) {
            return false;
    }
    v = graph.get_V(vertex_id);
    if(graph.out_degree(v) == 1 && graph.in_degree(v) == 0) return true;
    if(graph.out_degree(v) == 0 && graph.in_degree(v) == 1) return true;
    if(graph.out_degree(v) == 1 && graph.in_degree(v) == 1) {
        int64_t incoming_edge_id, outgoing_edge_id;
        EO_i out, out_end;
        EI_i in, in_end;
        for (boost::tie(out, out_end) = out_edges(v, graph.graph);
                        out != out_end; ++out) {
            outgoing_edge_id = graph.graph[*out].id;
        }
        for (boost::tie(in, in_end) = in_edges(v, graph.graph);
                        in != in_end; ++in) {
            incoming_edge_id = graph.graph[*in].id;
        }
        if(incoming_edge_id == outgoing_edge_id)
            return true;
        return false;
    } 
    return false;
}

template <class G>
void Pgr_contract< G >::getDeadEndSet(G &graph) {
    //Identifiers<int64_t> dead_end_vertices;
    V_i vi;
    for (vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
        // debug << "Checking vertex " << graph.graph[(*vi)].id << '\n';
        if (is_dead_end(graph, graph.graph[(*vi)].id)) {
            // debug << "Adding " << graph.graph[(*vi)].id << "to dead end" << '\n';
            dead_end += graph.graph[(*vi)].id;
        }
    }
    //return dead_end_vertices;
}



template <class G>
void Pgr_contract< G >::print_dead_end_vertices(std::ostringstream& stream) {
    //std::ostringstream out;
    stream << dead_end << '\n';
    //return out;
}


#if 0
 //! \brief Calculates the degree of every vertex in the graph
     /*!
       - A map is generated which maps the degree to a vector of vertices of the particlaur degree 
       - This map changes during the process of contraction 
       */
template < class G >
       void
       Pgr_contract< G >::calculateDegrees(G &graph) {
           EO_i out, out_end;
    // V_i vi;
           for (auto vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
        // graph.graph[(*vi)].degree=boost::out_degree(*vi,graph.graph);
              int degree = boost::in_degree(*vi,graph.graph);
              degree_to_V_map[degree].push(graph.graph[(*vi)].id);
          }
      }

#endif

#if 0
 //! \brief Generates the name of the contracted graph based upon the type of contraction
template < class G >
      void
      Pgr_contract< G >::getGraphName(std::ostringstream& name, Contraction_type ctype) {
       name << "contracted_graph_" << static_cast<int>(ctype);
   }


#endif

#if 0
template < class G >
void
Pgr_contract< G >::contract_to_level(G &graph,int64_t level)
{
    switch(level)
    {
        case 0:
        remove_1_degree_vertices(graph);
        break;
        case 1:
        remove_2_degree_vertices(graph);
        default:
        // do nothing
        ;
    }
}
#endif


#if 0
 //! \brief Returns the *degree_to_V_map* in string format
template < class G >
void
Pgr_contract< G >::degreeMap(G &graph,std::ostringstream& dmap)
{
    // cout << "Printing degree_V map" << endl;
    degree_to_V_i it1;
    dmap << "Degree --> Vertices" ;
    Q_i it2;
    for ( it1=degree_to_V_map.begin(); it1!=degree_to_V_map.end(); ++it1)
    {
        dmap << it1->first << "-->" ;
        for (it2=it1->second.begin(); it2 !=it1->second.end(); ++it2)
        {
            dmap << graph.graph[(*it2)].id << ", ";
        }
        dmap << "\n";
    }
}
#endif


#if 0
//! \brief Performs dead-end contraction on the graph
     /*!
      
       - Picks up vertices in the order of the vertex id
       - Removes all those vertices with one incoming edge(directed)
       - In case of undirected the vertices having one incoming and one outgoing edge(both of same id)
         will be chosen.

       */
template < class G >
         void
         Pgr_contract< G >::dead_end_contraction(G &graph) {
           E_i in, in_end;
           V front;
    // errors << "first vertex: " << front ;
    // std::vector<V> one_degree_vertices = degree_to_V_map[1];
           degree_to_V_i it;
    // std::cout << "1 degree vertices " << one_degree_vertices.size();
    // std::cout << "Front id" << frontid;
    // graph.m_num_vertices--;   
           while (degree_to_V_map[1].size() > 0) {
        // std::cout << "Front " << graph.graph[front].id;
              V front = degree_to_V_map[1].top();
              degree_to_V_map[1].pop();
              int64_t frontid = graph.graph[front].id;
              boost::tie(in, in_end) = in_edges(front, graph.graph);
              V s = source(*in, graph.graph);
              V t = target(*in, graph.graph);
              int64_t source_id = graph.graph[s].id;
              int64_t target_id = graph.graph[t].id;
              graph.graph[t].type = 1;
        // degree_to_V_map[final_target_degree].push_back(t);
        // Edge removed_edge = graph.graph[*in];
        // graph.graph[s].removed_vertices.
        // removedVertices[frontid].push_front(removed_edge);
              graph.disconnect_vertex_c(target_id);
              graph.m_num_vertices--;
              int source_degree = boost::in_degree(s, graph.graph);
              degree_to_V_map[source_degree].push(s);

          }
      }

#endif

#if 0
template < class G >
void
Pgr_contract< G >::remove_2_degree_vertices(G &graph) {
    EO_i out,out_end;
    std::vector<V> neighbors_desc;
    std::vector<V> two_degree_vertices = degree_to_V_map[2];
    std::vector<V> two_degree_vertices_0;
    for (Q_i it = two_degree_vertices.begin() ; it != two_degree_vertices.end(); ++it) {
            //cout << "id" << reduced_graph->graph[*it].id << endl;
            //cout << "contractions" << reduced_graph->graph[*it].contractions << endl;
        if (graph.graph[*it].contractions == 0) {
            two_degree_vertices_0.push_back(*it);
        }
    }
    degree_to_V_i it;
    //std::cout << "2 degree vertices " << two_degree_vertices_0.size()  << endl;
    if (two_degree_vertices_0.size() == 0) {
        return;
    }
    V front = two_degree_vertices_0.front();
    int64_t front_id = -1;
    while (two_degree_vertices_0.size()>0) {
            //cout << "2 degree vertices " << two_degree_vertices_0.size()  << endl;
            //cout << "Edge count" << num_edges(reduced_graph->graph) << endl;
        std::cout << "Front " << graph.graph[front].id ;
        neighbors_desc.clear();
        front = two_degree_vertices_0.front();
        front_id = graph.graph[front].id;
        if (graph.graph[front].contractions==0) {
            int count = 0;
            int64_t tid1=-1, tid2 = -1;
            V  tdesc1=-1, tdesc2 = -1;
            float min_distance = 0.00000;
            for (boost::tie(out, out_end) = out_edges(front, graph.graph);
                out != out_end; ++out) {
                V t = target(*out, graph.graph);
                min_distance += graph.graph[*out].cost;
                graph.graph[t].contractions++;
                neighbors_desc.push_back(t);
                count++;
            }
            if (count == 2) {
                Edge shortcut;
                tdesc1 = neighbors_desc[0];
                tdesc2 = neighbors_desc[1];
                tid1 = graph.graph[tdesc1].id;
                tid2 = graph.graph[tdesc2].id;
                E edesc1 = edge(tdesc1, front,graph.graph).first;
                E edesc2 = edge(front, tdesc2,graph.graph).first;
                int64_t eid1 = graph.graph[edesc1].id;
                int64_t eid2 = graph.graph[edesc2].id;
                int64_t eid;
                bool b1 = edge(tdesc1, tdesc2,graph.graph).second;
                bool b2 = edge(tdesc2, tdesc1,graph.graph).second;
                if (b1 || b2) {
                    E edesc = edge(tdesc1, tdesc2,graph.graph).first;
                    eid = graph.graph[edesc].id;
                    float dist = graph.graph[edesc].cost;
                    if (min_distance-dist >= 0.000001)
                    {
                        min_distance = dist;
                    } else {
                        //std::cout << "Updating shortcut " << "(" << eid1 << ", " << eid2 << ")" << " with " << min_distance << endl;
                        graph.graph[edesc].cost = min_distance;
                        graph.graph[edesc].type = 1;
                    }
                } else {
                    shortcut.id = ++graph.last_edge_id;
                    eid = shortcut.id;
                    shortcut.source = tid1;
                    shortcut.target = tid2; 
                    shortcut.cost = min_distance;
                    shortcut.type = 2;
                    graph.graph_add_shortcut(shortcut);
                }
                psuedoEdges[eid] = std::make_pair(eid1,eid2);
                Edge removed_edge;
                for (int i = 0; i < 2; ++i)
                {
                    V t = neighbors_desc[i];
                    removed_edge = graph.graph[edge(front, t,graph.graph).first];
                    removedVertices[front_id].push_front(removed_edge);
                }
                graph.disconnect_vertex(front_id);
                graph.m_num_vertices--;
            }
        }
        two_degree_vertices_0.erase(two_degree_vertices_0.begin(),
            two_degree_vertices_0.begin()+1);

    }
}
#endif

#if 0
//! \brief Returns the information about the contracted graph in string format
     /*!
       
       - Information about the outgoig edges of a particular vertex gets stored
       - The information includes id,source,target,cost
       - Edge information is delimited by "$" symbol  

       */
template < class G >
       int64_t
       Pgr_contract< G >::getGraph_string(G &graph,std::ostringstream& estring) {
           V_i vi;
           EO_i out,out_end;
           int64_t count=0;
           for (vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
              for (boost::tie(out, out_end) = out_edges(*vi, graph.graph);
                 out != out_end; ++out) {
                 V source_desc = source(*out, graph.graph);
             V target_desc = target(*out, graph.graph);
             int64_t source_id = graph.graph[source_desc].id;
             int64_t target_id = graph.graph[target_desc].id;

             if (target_desc > source_desc)
             {
                estring << graph.graph[*out].id << ","
                << source_id << "," << target_id << ","
                << graph.graph[*out].cost << ","
                << graph.graph[*out].reverse_cost << "$";
                count++;
            }

        }
    }
    return count;
}

#endif

#if 0
//! \brief Returns the information about the removed edges after contraction in string format
     /*!
      
       - The information of the removed edge includes id,source,target,cost
       - Edge information is delimited by "$" symbol  

       */
template < class G >
       void
       Pgr_contract< G >::getRemovedE_string(G &graph,std::ostringstream& estring) {

    #if 0
    for (removed_E_i iter = graph.removed_edges.begin(); iter != graph.removed_edges.end(); iter++)
    {
        // Edge temp=*iter ;
        estring << iter->id << "," << iter->source << ","
        << iter->target << "," << iter->cost << "$"; 
        // << "," << iter->reverse_cost << "$";
    }
    #endif

    for (const auto re : graph.removed_edges_c) 
        estring << re.first << "," << re.second.source << ","
    << re.second.target << "," << re.second.cost << ","
    <<re.second.reverse_cost<< "$";
}
#endif
//! \brief Returns the information about the removed vertices after contraction in string format
     /*!
      
       - The information of the removed vertex includes id
       - Edge information is delimited by "$" symbol  

       */

#if 0

template < class G >
       void
       Pgr_contract< G >::getRemovedV_string(std::ostringstream& vstring) {
           for (removed_V_i iter = removedVertices.begin(); iter != removedVertices.end(); iter++) {
              int64_t vid=iter->first;
              std::deque<Edge>::iterator edge_iter;
              for (edge_iter = iter->second.begin(); edge_iter != iter->second.end(); edge_iter++) {
                 Edge temp = *edge_iter ;
                 vstring << vid <<","<< temp.id << ","
                 << temp.source << "," << temp.target << ","
                 << temp.cost << "," << temp.reverse_cost << "$";
             }
        // std::cout << endl;
         }      

     }

#endif
#if 0
//! \brief Returns the information about the new edges(shortcuts) after contraction in string format
     /*!
      
       - The information of the shortcut includes id,source,target,cost
       - Edge information is delimited by "$" symbol  

       */
template < class G >
       void
       Pgr_contract< G >::getPsuedoE_string(std::ostringstream& pstring) {
           for (const auto pe : psuedoEdges) {
              pstring << pe.first << "," << pe.second.first << ","
              << pe.second.second << "$";
          }

      }
#endif