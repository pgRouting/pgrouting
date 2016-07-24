/*PGR-GNU*****************************************************************
File: pgr_contractionGraph.hpp

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

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>

#ifdef open
#undef open
#endif

#ifdef unlink
#undef unlink
#endif

#endif


#include <limits> 
#include <algorithm>
#include "../../common/src/pgr_base_graph.hpp"


namespace pgrouting {

	namespace graph {

		template <class G, typename T_V, typename T_E>
			class Pgr_contractionGraph;
	}

	typedef  graph::Pgr_contractionGraph <
		boost::adjacency_list < boost::listS, boost::vecS,
		boost::undirectedS,
		contraction::Vertex, contraction::Edge >,
		contraction::Vertex, contraction::Edge > CHUndirectedGraph;

	typedef  graph::Pgr_contractionGraph <
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
					   
                    /*! @brief Binary function that accepts two elements , and returns a value convertible to bool.
                        Used as a compare function to sort the edges in increasing order of edge id 
                    */
                    static bool compareById(const T_E &edge1, const T_E &edge2)
                    {
                        return edge1.id > edge2.id;
                    }

                    /*!
                    Prepares the _graph_ to be of type *gtype* with vertices as *vertices*
                    */
                    Pgr_contractionGraph< G , T_V, T_E >(const std::vector< T_V > &vertices, graphType gtype)
						: Pgr_base_graph< G , T_V, T_E >(vertices, gtype) {
						}

                    /*!
                    Prepares the _graph_ to be of type *gtype*
                    */
					explicit Pgr_contractionGraph< G , T_V, T_E >(graphType gtype)
						: Pgr_base_graph< G , T_V, T_E >(gtype) {
						}

                    //! @name Insert data
                    //@{
                    /*! \brief Inserts *count* edges of type *T* into the graph
                    *  
                    *  Converts the edges to a std::vector<T> & calls the overloaded
                    *  twin function.
                    *
                    *  @param edges
                    *  @param count
                    */
					template < typename T >
						void graph_insert_data(const T *edges, int64_t count) {
							graph_insert_data(std::vector < T >(edges, edges + count));
						}
                    /*! \brief Inserts vector of edges of type *T* into the graph
                    *  
                    *  @param edges
                    */
					template < typename T >
						void graph_insert_data(const std::vector < T > &edges) {
							for (const auto edge : edges) {
								graph_add_edge(edge);
							}
						}
                    //@}

                    //! @brief True when *v* is in the graph
                    /*!
                    True when 
                        - Indegree of *v* is 0 &
                        - Outdegree of *v* is 0
                    @param [in] v vertex_id
                    */
					bool is_connected(int64_t v) const {
						if (this->in_degree(this->get_V(v)) == 0 && this->out_degree(this->get_V(v)) == 0) {
							return false;
						}
						return true;
					}

                    //! @brief get the vertex descriptor of the vertex adjacent to *v*
                    /*!
                    - Degree of *v* is 1                    
                    @param [in] v vertex_descriptor
                    @return V: The vertex descriptor of the vertex adjacent to *v*
                    */
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

                    /*! @brief get the vertex descriptors of adjacent vertices of *v*
                    @param [in] v vertex_descriptor
                    @return Identifiers<V>: The set of vertex descriptors adjacent to the given vertex *v*
                    */
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

                    /*! @brief get the user ids given the boost graph ids in string format
                    @param [in] log string
                    @param [in] boost_ids The set of boost graph ids of vertices
                    */
					void get_ids(std::ostringstream &log,
							Identifiers<int64_t> boost_ids) {
						log << "{";
						for (auto id : boost_ids) {
							log << this->graph[id].id << ", ";
						}
						log << "}";
					}

					/*! @brief get the user ids given the boost graph ids in array format
                    @param [in] boost_ids The set of boost graph ids of vertices
                    @param [in] contracted_vertices The array of contracted vertices
                    @param [in] contracted_vertices_size The size of the array of contracted vertices
                    */
					void get_ids(int64_t **contracted_vertices,
                    	int &contracted_vertices_size,
							Identifiers<int64_t> boost_ids) {
						contracted_vertices_size = (int)boost_ids.size();
						(*contracted_vertices) = (int64_t*)malloc(sizeof(int64_t)*contracted_vertices_size);
                        int64_t count = 0;
						for (auto id : boost_ids) {
							(*contracted_vertices)[count++] = this->graph[id].id;
							}
						}					

                    /*! @brief get the remaining vertices of the graph after contraction
                    @param [in] remaining_vertices The vector of vertices remaining after contraction
                    */
                    void get_remaining_vertices(std::vector<T_V>& remaining_vertices) {
                        for (auto vi = vertices(this->graph).first; vi != vertices(this->graph).second; ++vi) {
                            if (!removed_vertices.has(*vi))
                            {
                                remaining_vertices.push_back(this->graph[*vi]);
                            }
                        }
                    }

                    /*! @brief get the vertices of the graph with atleast one contracted vertex 
                    @param [in] remaining_vertices The set of vertices with atleast one contracted vertex
                    */
                    void get_changed_vertices(Identifiers<int64_t>& remaining_vertices) {
                        //log << "remaining_vertices\n";
                        for (auto vi = vertices(this->graph).first; vi != vertices(this->graph).second; ++vi) {
                            if (!removed_vertices.has(*vi) && this->graph[*vi].has_contracted_vertices())
                            {
                                remaining_vertices += this->graph[*vi].id;
                            }
                        }
                    }

                    /*! @brief get the edges of the graph that are added during contraction
                    @param [in] shortcut_edges The vector of edges added during contraction
                    */
                    void get_shortcuts(std::vector<T_E>& shortcut_edges)
                    {
                        //log << "Getting shortcuts\n";
                        for (auto shortcut : shortcuts)
                        {
                            //log << shortcut;
                            shortcut_edges.push_back(shortcut);
                        }
                        std::sort(shortcut_edges.begin(), shortcut_edges.end(), compareById);
                    }

                    /*! @brief get the edge with minimum cost between two vertices
                    @param [in] source vertex_descriptor of source vertex
                    @param [in] destination vertex_descriptor of target vertex
                    @return E: The edge descriptor of the edge with minimum cost
                    */
                    E get_min_cost_edge(V source, V destination)
                    {
                        E e;
                        EO_i out_i, out_end;
                        E min_cost_edge;
                        double min_cost = std::numeric_limits<double>::max();
                        for (boost::tie(out_i, out_end) = boost::out_edges(source, this->graph);
                                out_i != out_end; ++out_i) {
                            e = *out_i;
                            if (target(e, this->graph) == destination)
                            {
                                if (this->graph[e].cost < min_cost)
                                {
                                    min_cost = this->graph[e].cost;
                                    min_cost_edge = e;
                                }
                            }
                        }
                        //log << "Min cost edge from " << this->graph[source].id << " to " << this->graph[destination].id << std::endl;
                        //log << this->graph[min_cost_edge];  
                        return min_cost_edge;
                    }

                    /*! @brief get the in-degree of a vertex from its neighbor
                    @param [in] vertex vertex_descriptor of the given vertex
                    @param [in] neighbor vertex_descriptor of neighbor 
                    @return degree_size_type: The in-degree of *vertex* from *neighbor*
                    */
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

                    /*! @brief get the out-degree of a vertex to its neighbor
                    @param [in] vertex vertex_descriptor of the given vertex
                    @param [in] neighbor vertex_descriptor of neighbor 
                    @return degree_size_type: The out-degree of *vertex* to *neighbor*
                    */
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

                    /*! @brief print the edges added during contraction
                    */
					void print_shortcuts(std::ostringstream& log)
					{
						log << "Printing shortcuts\n";
						for (auto shortcut : shortcuts)
						{
							log << shortcut;
						}
					}

                    /*! @brief print the graph with contracted vertices of
                        all vertices and edges
                    */
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

                    /*! @brief get the contracted vertex ids of a given vertex in string format
                    @param [in] vid vertex_id
                    @param [in] log stringstream which stores the vertex ids of contracted vertices of *vid*
                    */
                    void get_contracted_vertices(std::ostringstream &log, int64_t vid) {
                        if (!this->has_vertex(vid)) return;
                        V v = this->get_V(vid);
                        log << "{";
                        for (auto vertex : this->graph[v].contracted_vertices()) {
                            log << this->graph[vertex].id << ", ";
                        }
                        log << "}";

                    }


                    /*! @brief get the contracted vertex ids of a given vertex in string format
                    @param [in] vid vertex_id
                    @param [in] contracted_vertices The array of contracted vertices of *vid*
                    @param [in] contracted_vertices_size The size of the array of contracted vertices of *vid*
                    */
                    void get_contracted_vertices(int64_t **contracted_vertices,
                    	int &contracted_vertices_size, int64_t vid) {
                        if (!this->has_vertex(vid)) return;
                        V v = this->get_V(vid);
                        contracted_vertices_size = (int)this->graph[v].contracted_vertices().size();
                        (*contracted_vertices) = (int64_t*)malloc(sizeof(int64_t)*contracted_vertices_size);
                        int64_t count = 0;
                        for (auto vertex : this->graph[v].contracted_vertices()) {
                            (*contracted_vertices)[count++] = this->graph[vertex].id;
                        }

                    }

                    /*! @brief add the contracted vertices of an edge *e* to the vertex *v*
                    @param [in] v vertex_descriptor
                    @param [in] e Edge of type *T_E*
                    */
					void add_contracted_edge_vertices(V v, T_E &e)
					{
						for (auto vid : e.contracted_vertices())
						{
							this->graph[v].add_vertex_id(vid);
						}
						e.clear_contracted_vertices();
					}

                    
                    /*! \brief Inserts an edge of type *T* into the graph
                     *  @param edge
                     */
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

                    /*! \brief add edges(shortuct) to the graph during contraction
                    @param [in] edge of type *T_E* is to be added
                    @param [in] log string stream used for debugging purposes
                    */  
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

                    /*! \brief Disconnects all incomming and outgoing edges from the vertex
                    boost::graph doesn't recommend th to insert/remove vertices, so a vertex removal is
                    simulated by disconnecting the vertex from the graph
                    - No edge is disconnected if the vertices id's do not exist in the graph
                    - All removed edges are stored for future reinsertion
                    - All parallel edges are disconnected (automatically by boost)
                    @param [in] vertex original vertex id of the starting point of the edge
                    @param [in] log string stream used for debugging purposes
                    */   
                    void disconnect_vertex(std::ostringstream &log, V vertex) {

                        T_E d_edge;
                        EO_i out, out_end;
                        log << "Disconnecting current vertex " << this->graph[vertex].id << "\n";
                        removed_vertices += vertex;
                        // store the edges that are going to be removed
                        for (boost::tie(out, out_end) = out_edges(vertex, this->graph);
                                out != out_end; ++out) {

                            d_edge.id = this->graph[*out].id;
                            d_edge.source = this->graph[source(*out, this->graph)].id;
                            d_edge.target = this->graph[target(*out, this->graph)].id;
                            d_edge.cost = this->graph[*out].cost;
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
                                this->removed_edges.push_back(d_edge);
                            }
                        }
                        try
                        {
                            boost::clear_vertex(vertex, this->graph);
                        }
                        catch ( ... ) {
                            log << "Caught unknown expection!\n";
                        }
                    }
			};
	} // namespace graph

} // namespace pgrouting

