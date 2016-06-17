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
				Pgr_linearContraction():last_edge_id(0){}
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
				int64_t last_edge_id;
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
				void add_shortcut(G &graph, V vertex,
					E incoming_edge,
					E outgoing_edge,
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
			Identifiers<V> adjacent_vertices = graph.find_adjacent_vertices(v);
			if (adjacent_vertices.size() == 2)
			{
				if (in_degree > 0 && out_degree > 0)
				{
					return true;
				}
			}
			return false;

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
			for(auto v : linearVertices)
			{
				debug << graph[v].id << ", ";
			}
			debug << std::endl;
			
			//debug << "V | " << "outgoing | " << "incoming" << std::endl;
			while(!linearPriority.empty()) {

				V current_vertex = linearPriority.top();
				linearPriority.pop();
				if (!is_linear(graph, current_vertex, debug))
				{
					linearVertices -= current_vertex;
					continue;
				}
				Identifiers<V> adjacent_vertices = graph.find_adjacent_vertices(current_vertex);
				pgassert(adjacent_vertices.size() == 2);
				V vertex_1 = adjacent_vertices[0];
				V vertex_2 = adjacent_vertices[1];
				debug << "Adjacent vertices\n";
				debug << graph[vertex_1].id << ", " << graph[vertex_2].id << std::endl;
				
				debug << "Out degree of " << graph[vertex_1].id 
				<< " to " << graph[current_vertex].id 
				<< " : " << graph.out_degree_to_vertex(vertex_1, current_vertex) << std::endl;
				debug << "Out degree of " << graph[vertex_2].id
				<< " to " << graph[current_vertex].id 
				<< " : " << graph.out_degree_to_vertex(vertex_2, current_vertex) << std::endl;
				debug << "In degree of " << graph[vertex_1].id 
				<< " from " << graph[current_vertex].id 
				<< " : " << graph.in_degree_from_vertex(vertex_1, current_vertex) << std::endl;
				debug << "In degree of " << graph[vertex_2].id 
				<< " from " << graph[current_vertex].id 
				<< " : " << graph.in_degree_from_vertex(vertex_2, current_vertex) << std::endl;



				
				if (graph.out_degree_to_vertex(vertex_1, current_vertex) > 0 
					&& graph.in_degree_from_vertex(vertex_2, current_vertex) > 0)
				{
					E e1 = graph.get_min_cost_edge(vertex_1, 
						current_vertex, debug);
					E e2 = graph.get_min_cost_edge(current_vertex, 
						vertex_2, debug);
					add_shortcut(graph, current_vertex, e1, e2, debug);

				}

				if (graph.out_degree_to_vertex(vertex_2, current_vertex) > 0 
					&& graph.in_degree_from_vertex(vertex_1, current_vertex) > 0)
				{
					E e1 = graph.get_min_cost_edge(vertex_2, 
						current_vertex, debug);
					E e2 = graph.get_min_cost_edge(current_vertex, 
						vertex_1, debug);
					add_shortcut(graph, current_vertex, e1, e2, debug);
				}
				graph.disconnect_vertex(debug, current_vertex);
				linearVertices -= current_vertex;
				
				if (is_linear(graph, vertex_1, debug))
				{
					linearPriority.push(vertex_1);
					linearVertices += vertex_1;
				}
				if (is_linear(graph, vertex_2, debug))
				{
					linearPriority.push(vertex_2);
					linearVertices += vertex_2;
				}

				#if 0
				int64_t incoming_eid = edgePairsMap[current_vertex].first;
				int64_t outgoing_eid = edgePairsMap[current_vertex].second; 
				debug << graph[current_vertex].id <<" | " << incoming_eid << " | " << outgoing_eid << std::endl;
				add_shortcuts(graph, current_vertex, incoming_eid, outgoing_eid, debug);
				
				#endif


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
				// different id to every shortcut
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
			shortcut.add_contracted_edge_vertices(incoming_edge);
			shortcut.add_contracted_edge_vertices(outgoing_edge);
			graph.graph_add_edge(shortcut);
			//graph.get_outgoing_edge(last_edge_id, incoming_edge.source, debug).add_contracted_vertex(graph[vertex], vertex);
			debug << "Added shortcut\n";
			debug << shortcut;
			graph.disconnect_vertex(debug, vertex);


}

template < class G >
		void Pgr_linearContraction<G>::add_shortcut(G &graph, V vertex,
			E incoming_edge,
			E outgoing_edge,
			std::ostringstream& debug) {

			contraction::Edge shortcut(--last_edge_id, graph[incoming_edge].source,
				graph[outgoing_edge].target,
				graph[incoming_edge].cost + graph[outgoing_edge].cost);
			shortcut.add_contracted_vertex(graph[vertex], vertex);
			shortcut.add_contracted_edge_vertices(graph[incoming_edge]);
			shortcut.add_contracted_edge_vertices(graph[outgoing_edge]);
			graph.graph_add_edge(shortcut);
			//graph.get_outgoing_edge(last_edge_id, incoming_edge.source, debug).add_contracted_vertex(graph[vertex], vertex);
			debug << "Added shortcut\n";
			debug << shortcut;
		}


}