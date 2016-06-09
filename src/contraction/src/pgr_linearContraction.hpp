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
namespace pgRouting {



	template < class G >
		class Pgr_linearContraction {
			public:
				typedef typename G::V V;
				typedef typename G::E E;
				typedef typename G::V_i V_i;
				typedef typename G::E_i E_i;
				typedef typename G::EO_i EO_i;
				typedef typename G::EI_i EI_i;
				typedef typename G::degree_size_type degree_size_type;
				void setForbiddenVertices(G &graph,
						Identifiers<int64_t> forbidden_vertices,
						std::ostringstream& debug);
				void setForbiddenVertices(G &graph, int64_t *forbidden_vertices,
						size_t size_forbidden_vertices,
						std::ostringstream& debug);
				void calculateVertices(G &graph,
						std::ostringstream& debug);
				void doContraction(G &graph,
						std::ostringstream& debug);
				 
			private:

				Identifiers<V> linearVertices;
				Identifiers<V> forbiddenVertices;
				std::map<V, std::pair<int64_t, int64_t> > edgePairsMap;
				int64_t last_edge_id = 0;
				bool is_linear(G &graph, V v,
						std::ostringstream& debug);
				void add_if_linear(G &graph, V v,
						std::ostringstream& debug);
				void add_edge_pair(V vertex, int64_t &incoming_eid,
					int64_t &outgoing_eid,
					std::ostringstream& debug);
				void add_shortcuts(G &graph, V vertex, int64_t incoming_eid,
					int64_t outgoing_eid,
					std::ostringstream& debug);
				void add_shortcut(G &graph, V vertex,
					contraction::Edge incoming_edge,
					contraction::Edge outgoing_edge,
					int64_t id, bool first, 
					std::ostringstream& debug);

		};

	template < class G >
		void Pgr_linearContraction< G >::setForbiddenVertices(G &graph, 
				Identifiers<int64_t> forbidden_vertices,
				std::ostringstream& debug) {
			debug << "Setting forbidden vertices\n";
			for (auto forbiddenVertex : forbidden_vertices) {
				forbiddenVertices += graph.get_V(forbiddenVertex);
			}

		}

	template < class G >
		void Pgr_linearContraction< G >::setForbiddenVertices(G &graph, 
				int64_t *forbidden_vertices,
				size_t size_forbidden_vertices,
				std::ostringstream& debug) {

			debug << "Setting forbidden vertices\n";
			for (int64_t i = 0; i < size_forbidden_vertices; ++i) {
				forbiddenVertices += graph.get_V(forbidden_vertices[i]);
			}

		}

template < class G >
		void Pgr_linearContraction< G >::add_edge_pair(V vertex, int64_t &incoming_eid,
					int64_t &outgoing_eid,
					std::ostringstream& debug) {

			debug << "Adding edge pair\n";
			debug << "incoming id: "<< incoming_eid 
			<< ", outgoing id: " << outgoing_eid << std::endl;
			std::pair<int64_t, int64_t> epair(incoming_eid, outgoing_eid);
			//edgePairs.push_back(epair);
			edgePairsMap[vertex] = epair;

		}
template < class G >
		bool Pgr_linearContraction<G>::is_linear(G &graph, V v,
				std::ostringstream& debug) {

			debug << "Is linear: " << graph.graph[v].id << "?\n";
			//debug << "in_degree: " << graph.in_degree(vertex_id) << '\n';
			//debug << "out_degree: " << graph.out_degree(vertex_id) << '\n';
			degree_size_type in_degree, out_degree;
			in_degree = graph.in_degree(v);
			out_degree = graph.out_degree(v);
			
			
			if(out_degree <= 2 && in_degree <= 2) {
				int64_t incoming_eids[2] = {-1, -1};
				int64_t outgoing_eids[2] = {-1, -1};

				int incoming_count, outgoing_count;
				incoming_count = outgoing_count = 0;
				EO_i out, out_end;
				EI_i in, in_end;
				// Storing the incoming and outgoing edge descs in arrays
				for (boost::tie(out, out_end) = out_edges(v, graph.graph);
						out != out_end; ++out) {
					outgoing_eids[outgoing_count++] = graph.graph[*out].id;
									}
				for (boost::tie(in, in_end) = in_edges(v, graph.graph);
						in != in_end; ++in) {
					incoming_eids[incoming_count++] = graph.graph[*in].id;
									}
				pgassert(in_degree == incoming_count);
				pgassert(out_degree == outgoing_count);
				debug << "Incoming ids\n";
				debug << "{" << incoming_eids[0] << ", " << incoming_eids[1] << "}\n";
				debug << "Outgoing ids\n";
				debug << "{" << outgoing_eids[0] << ", " << outgoing_eids[1] << "}\n";
				//case 1
				if(in_degree == 1 && out_degree == 1) {
					debug << "Case 1\n"; 
					if(incoming_eids[0] != outgoing_eids[0])
					{
						debug << "Yes\n";
						add_edge_pair(v, incoming_eids[0], outgoing_eids[0],
						debug);
						return true;
					}
				}
				// case 2
				if (out_degree == 2 && in_degree == 2) {
					debug << "Case 2\n";
					if(incoming_eids[0] == outgoing_eids[0] &&
						incoming_eids[1] == outgoing_eids[1]) {
						debug << "Yes\n";
						add_edge_pair(v, incoming_eids[0], outgoing_eids[1],
						debug);
						#if 0
						add_edge_pair(incoming_eids[1], outgoing_eids[0],
						debug);
						#endif
						return true;
				}
					if(incoming_eids[0] == outgoing_eids[1] &&
						incoming_eids[1] == outgoing_eids[0]) {
		
						debug << "Yes\n";
						add_edge_pair(v, incoming_eids[0], outgoing_eids[0],
						debug);
						#if 0
						add_edge_pair(incoming_eids[1], outgoing_eids[1],
						debug);
						#endif
							return true;
						}

				}

				// case 3
				else if (out_degree == 1 && in_degree == 2) {
					debug << "Case 3\n";
					if(outgoing_eids[0] == incoming_eids[0] ||
						outgoing_eids[0] == incoming_eids[1])
					{
						debug << "Yes\n";
						if (outgoing_eids[0] == incoming_eids[0])
						{
							add_edge_pair(v, incoming_eids[1], outgoing_eids[0],
							debug);
						}
						else if (outgoing_eids[0] == incoming_eids[1])
						{
							add_edge_pair(v, incoming_eids[0], outgoing_eids[0],
							debug);
						}
						
						return true;
					}
				}

				// case 4
				else if (out_degree == 2 && in_degree == 1) {
					debug << "Case 4\n";
					if(incoming_eids[0] == outgoing_eids[0] ||
						incoming_eids[0] == outgoing_eids[1])
					{
						debug << "Yes\n";
						if (outgoing_eids[0] == incoming_eids[0])
						{
							add_edge_pair(v, incoming_eids[0], outgoing_eids[1],
							debug);
						}
						else if (outgoing_eids[1] == incoming_eids[0])
						{
							add_edge_pair(v, incoming_eids[0], outgoing_eids[0],
							debug);
						}
						return true;
					}	
				}
				
				debug << "No\n"; 
				return false;
			}

			else{
			debug << "No\n"; 
			return false;
			}
		}

template < class G >
		void Pgr_linearContraction<G>::calculateVertices(G &graph,
				std::ostringstream& debug) {
			debug << "Calculating vertices\n";
			V_i vi;
			for (vi = vertices(graph.graph).first; vi != vertices(graph.graph).second; ++vi) {
				debug << "Checking vertex " << graph.graph[(*vi)].id << '\n';
				if (is_linear(graph, *vi, debug)) {
					debug << "Adding " << graph.graph[(*vi)].id << " to linear" << '\n';
					linearVertices += (*vi);
				}
			}
			linearVertices -= forbiddenVertices;
		}



template < class G >
		void Pgr_linearContraction<G>::doContraction(G &graph,
				std::ostringstream& debug) {
			debug << "Performing contraction\n";
			std::priority_queue<V, std::vector<V>, std::greater<V> > linearPriority;
			for (V linearVertex : linearVertices) {
				linearPriority.push(linearVertex);
			}
			debug << "Linear vertices" << std::endl;
			debug << linearVertices;
			debug << std::endl;
			debug << "V | " << "outgoing | " << "incoming" << std::endl;
			while(!linearPriority.empty()) {

				V current_vertex = linearPriority.top();
				linearPriority.pop();
				if (!is_linear(graph, current_vertex, debug))
				{
					continue;
				}
				int64_t incoming_eid = edgePairsMap[current_vertex].first;
				int64_t outgoing_eid = edgePairsMap[current_vertex].second; 
				debug << graph[current_vertex].id <<" | " << incoming_eid << " | " << outgoing_eid << std::endl;
				add_shortcuts(graph, current_vertex, incoming_eid, outgoing_eid, debug);

			}
		}

template < class G >
		void Pgr_linearContraction<G>::add_shortcuts(G &graph, V vertex, int64_t incoming_eid,
					int64_t outgoing_eid, std::ostringstream& debug) {
			//pgr_edge_t shortcut;
			debug << "vertex: " << graph[vertex].id 
			<< ", in: " << graph.in_degree(vertex)
			 << ", out: " << graph.out_degree(vertex) << std::endl;
			debug << "Adding shortcut between " << incoming_eid 
			<< ", " << outgoing_eid << std::endl;

			#if 1
			if (graph.in_degree(vertex) == 2 && graph.out_degree(vertex) == 2)
			{
				#if 0
				graph.print_incoming_edge(incoming_eid,
					vertex, debug);
				graph.print_outgoing_edge(outgoing_eid,
					vertex, debug);
				#endif
				contraction::Edge incoming1 = graph.get_incoming_edge(incoming_eid,
					vertex, debug);
				contraction::Edge outgoing1 = graph.get_outgoing_edge(outgoing_eid,
					vertex, debug);
				contraction::Edge incoming2 = graph.get_incoming_edge(outgoing_eid,
					vertex, debug);
				contraction::Edge outgoing2 = graph.get_outgoing_edge(incoming_eid,
					vertex, debug);
				add_shortcut(graph, vertex, incoming1, outgoing1, --last_edge_id, true, debug);
				add_shortcut(graph, vertex, incoming2, outgoing2, last_edge_id, false, debug);
				
			}

			else
			{
				#if 0
				graph.print_incoming_edge(incoming_eid,
					vertex, debug);
				graph.print_outgoing_edge(outgoing_eid,
					vertex, debug);
				#endif
				contraction::Edge incoming = graph.get_incoming_edge(incoming_eid,
					vertex, debug);
				contraction::Edge outgoing = graph.get_outgoing_edge(outgoing_eid,
					vertex, debug);
				add_shortcut(graph, vertex, incoming, outgoing, --last_edge_id, true, debug);
			}
			#endif

}
template < class G >
		void Pgr_linearContraction<G>::add_shortcut(G &graph, V vertex,
			contraction::Edge incoming_edge,
			contraction::Edge outgoing_edge,
			int64_t id, bool first,
			std::ostringstream& debug) {

			contraction::Edge shortcut(id, incoming_edge.source,
				outgoing_edge.target, incoming_edge.cost + outgoing_edge.cost, first);
			shortcut.add_contracted_vertex(graph[vertex], vertex);
			graph.graph_add_edge(shortcut);
			//graph.get_outgoing_edge(last_edge_id, incoming_edge.source, debug).add_contracted_vertex(graph[vertex], vertex);
			debug << "Added shortcut\n";
			debug << shortcut;
			graph.disconnect_vertex(debug, vertex);
		}


}