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
