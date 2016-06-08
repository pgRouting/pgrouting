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
				bool is_linear(G &graph, V v,
						std::ostringstream& debug);
				void add_if_linear(G &graph, V v,
						std::ostringstream& debug);
				void doContraction(G &graph,
						std::ostringstream& debug);
			private:
				Identifiers<V> linearVertices;
				Identifiers<V> forbiddenVertices;

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
		bool Pgr_linearContraction<G>::is_linear(G &graph, V v,
				std::ostringstream& debug) {

			debug << "Is linear: " << graph.graph[v].id << "?\n";
			//debug << "in_degree: " << graph.in_degree(vertex_id) << '\n';
			//debug << "out_degree: " << graph.out_degree(vertex_id) << '\n';
			degree_size_type in_degree, out_degree;
			in_degree = graph.in_degree(v);
			out_degree = graph.out_degree(v);
			//case 1
			if(in_degree == 1 && out_degree == 1) {

				debug << "Case 1\n"; 
				return true;
			}
			
			if(out_degree == 2 || in_degree == 2) {
				int64_t incoming_eids[2] = {-1, -1};
				int64_t outgoing_eids[2] = {-1, -1};
				int incoming_count, outgoing_count;
				incoming_count = outgoing_count = 0;
				EO_i out, out_end;
				EI_i in, in_end;
				// Storing the incoming and outgoing edge ids in arrays
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
				// case 2
				if (out_degree == 2 && in_degree == 2) {
					debug << "Case 2\n";
					if(incoming_eids[0] == outgoing_eids[0] &&
						incoming_eids[0] == outgoing_eids[1])
						return true;
					if(incoming_eids[0] == outgoing_eids[1] &&
						incoming_eids[1] == outgoing_eids[0])
						return true;

				}

				// case 3
				if (out_degree == 1 && in_degree == 2) {
					debug << "Case 3\n";
					if(outgoing_eids[0] == incoming_eids[0] ||
						outgoing_eids[0] == incoming_eids[1])
						return true;
				}

				// case 4
				if (out_degree == 2 && in_degree == 1) {
					debug << "Case 4\n";
					if(incoming_eids[0] == outgoing_eids[0] ||
						incoming_eids[0] == outgoing_eids[1])
						return true;	
				}
				
				debug << "No\n"; 
				return false;
			}


			debug << "No\n"; 
			return false;
		}

