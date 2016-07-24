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

#include "../../common/src/identifiers.hpp"
#include <queue>  
namespace pgrouting {



	template < class G >
		class Pgr_deadEndContraction {
			public:
				typedef typename G::V V;
				typedef typename G::E E;
				typedef typename G::V_i V_i;
				typedef typename G::E_i E_i;
				typedef typename G::EO_i EO_i;
				typedef typename G::EI_i EI_i;
				void setForbiddenVertices(G &graph,
						Identifiers<int64_t> forbidden_vertices,
						std::ostringstream& debug);
				void setForbiddenVertices(G &graph, int64_t *forbidden_vertices,
						size_t size_forbidden_vertices,
						std::ostringstream& debug);
				void calculateVertices(G &graph,
						std::ostringstream& debug);
				bool is_dead_end(G &graph, V v,
						std::ostringstream& debug);
				void add_if_dead_end(G &graph, V v,
						std::ostringstream& debug);
				void doContraction(G &graph,
						std::ostringstream& debug);
			private:
				Identifiers<V> deadendVertices;
				Identifiers<V> forbiddenVertices;

		};

	template < class G >
		void Pgr_deadEndContraction< G >::setForbiddenVertices(G &graph, 
				Identifiers<int64_t> forbidden_vertices,
				std::ostringstream& debug) {

			debug << "Setting forbidden vertices\n";
			for (auto forbiddenVertex : forbidden_vertices) {
				forbiddenVertices += graph.get_V(forbiddenVertex);
			}

		}

	template < class G >
		void Pgr_deadEndContraction< G >::setForbiddenVertices(G &graph, 
				int64_t *forbidden_vertices,
				size_t size_forbidden_vertices,
				std::ostringstream& debug) {

			debug << "Setting forbidden vertices\n";
			for (int64_t i = 0; i < size_forbidden_vertices; ++i) {
				forbiddenVertices += graph.get_V(forbidden_vertices[i]);
			}

		}

	template < class G >
		void Pgr_deadEndContraction<G>::calculateVertices(G &graph,
				std::ostringstream& debug) {
			debug << "Calculating vertices\n";
			V_i vi;
			for (vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
				debug << "Checking vertex " << graph.graph[(*vi)].id << '\n';
				if (is_dead_end(graph, *vi, debug)) {
					debug << "Adding " << graph.graph[(*vi)].id << " to dead end" << '\n';
					deadendVertices += (*vi);
				}
			}
			deadendVertices -= forbiddenVertices;
		}

	template < class G >
		bool Pgr_deadEndContraction<G>::is_dead_end(G &graph, V v,
				std::ostringstream& debug) {

			debug << "Is dead end: " << graph.graph[v].id << "?\n";
			//debug << "in_degree: " << graph.in_degree(vertex_id) << '\n';
			//debug << "out_degree: " << graph.out_degree(vertex_id) << '\n';
			//if(graph.out_degree(v) == 1 && graph.in_degree(v) == 0) return true;
			//if(graph.out_degree(v) == 0 && graph.in_degree(v) == 1) return true;
			if (graph.m_gType == UNDIRECTED)
			{
				/* the condition in case of undirected graph
				   is all incoming edges come from a single vertex
				*/
				debug << "undirected\nAdjacent Vertices\n";

				Identifiers<V> adjacent_vertices = graph.find_adjacent_vertices(v);
				debug << adjacent_vertices;
				// only one adjacent vertex
				if (adjacent_vertices.size() == 1)
					return true;
				else 
					return false;
			}
			else
			{
			if(graph.out_degree(v) == 1 && graph.in_degree(v) == 1) {
				int64_t incoming_edge_id = -1;
				int64_t outgoing_edge_id = -2;
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
				if(incoming_edge_id == outgoing_edge_id) {
					debug << "Yes\n"; 
					return true;
				}
				debug << "No\n"; 
				return false;
			}
			// additional cases
			if (graph.out_degree(v) == 0 && graph.in_degree(v) > 0)
			 {
			 	return true;
			 }
			 #if 0
			 // dead start
			 if (graph.in_degree(v) == 0 && graph.out_degree(v) > 0)
			 {
			 	return true;
			 }
			 #endif
			debug << "No\n"; 
			return false;
		}
		return false;
		}

	template < class G >
		void Pgr_deadEndContraction<G>::add_if_dead_end(G &graph, V v,
				std::ostringstream& debug) {
			debug << "Adding if dead end\n"; 
			if (is_dead_end(graph, v, debug)) {
				deadendVertices += v;
			}
			else {
				debug << "Not dead end\n";
			}
		}

	template < class G >
		void Pgr_deadEndContraction<G>::doContraction(G &graph,
				std::ostringstream& debug){
			debug << "Performing contraction\n";
			std::priority_queue<V, std::vector<V>, std::greater<V> > deadendPriority;
			for (V deadendVertex : deadendVertices) {
				deadendPriority.push(deadendVertex);
			}
			//debug << "Dead end vertices" << std::endl;
			//debug << deadendVertices;
			while(!deadendPriority.empty()) {
			//for (auto deadendVertex : deadendVertices) {

				V current_vertex = deadendPriority.top();
				deadendPriority.pop();
				if (!is_dead_end(graph, current_vertex, debug))
				{
					continue;
				}
				Identifiers<V> adjacent_vertices = graph.find_adjacent_vertices(current_vertex);
				for(auto adjacent_vertex : adjacent_vertices) {
					//debug << "Current Vertex: "<< graph[current_vertex].id << std::endl;
				//debug << "Adjacent Vertex: "<< graph[adjacent_vertex].id << std::endl;

				

				debug << "Contracting current vertex "<< graph[current_vertex].id << std::endl;

				graph[adjacent_vertex].add_contracted_vertex(graph[current_vertex], current_vertex); 

				// Adding contracted vertices of the edge
				EO_i out, out_end;
				EI_i in, in_end;
				debug << "Adding contracted vertices of the edge\n";
				for (boost::tie(out, out_end) = out_edges(current_vertex, graph.graph);
						out != out_end; ++out) {
						debug << graph.graph[*out];
						graph.add_contracted_edge_vertices(adjacent_vertex, graph.graph[*out]);
					}
				for (boost::tie(in, in_end) = in_edges(current_vertex, graph.graph);
						in != in_end; ++in) {
						debug << graph.graph[*in];
						graph.add_contracted_edge_vertices(adjacent_vertex, graph.graph[*in]);
					}

				debug << "Current Vertex:\n";
				debug << graph[current_vertex];
				//debug << graph.graph[current_vertex].print_vertex(debug, graph.graph);
				debug << "Adjacent Vertex:\n";
				//debug << graph.graph[adjacent_vertex].print_vertex(debug, graph.graph);
				debug << graph[adjacent_vertex];


				
				graph.disconnect_vertex(debug, current_vertex) ;
				deadendVertices-=current_vertex;
				debug << "Adjacent vertex dead_end?: " << is_dead_end(graph, adjacent_vertex, debug) << std::endl;
				if(is_dead_end(graph, adjacent_vertex, debug)
				 && !forbiddenVertices.has(adjacent_vertex)) {
					deadendVertices += adjacent_vertex;
					deadendPriority.push(adjacent_vertex);
				}
				}

			}
		}

}
