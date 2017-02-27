/*PGR-GNU*****************************************************************
File: patrix_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Patrick Morales
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


#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#ifdef open
#undef open
#endif
#endif


#include <sstream>
#include <deque>
#include <algorithm>
#include <vector>
#include "./pgr_dijkstra.hpp"
#include "./patrix_driver.h"

#include "./../../common/src/pgr_types.h"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_alloc.hpp"


// -------------- patrix : function TDQPPP

#include <string>
#include <iostream>
//#include <fstream>
#include <set>
#include <math.h>
#include <cfloat>


#include <bits/stdc++.h>
#define PI 3.14159265

// ---------------------------------------



// Patrix : Ici on va ajouter le code TDQPPP de Hamza
// --------------------------------------------------






/************************************************************
  edges_sql TEXT,
    start_pid BIGINT,
    end_pids ANYARRAY,
    directed BOOLEAN DEFAULT true,
 ***********************************************************/
void
do_pgr_patrix(
		pgr_edge_t_patrix *data_edges, size_t total_edges,
        int64_t start_vid,
        int64_t  *end_vidsArr, size_t size_end_vidsArr,
        bool directed,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** err_msg)

{
    std::ostringstream err;
    std::ostringstream log;
    try {
        
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);


        // elog(DEBUG1,"Test al interior...!");
        // total_edges

        log << "Test al interior...!\n";
        *log_msg = strdup(log.str().c_str());


        /* depending on the functionality some tests can be done here
         * For example */

        if (total_edges <= 1) {
            err << "Required: more than one edges\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(err.str().c_str());
            return;
        }


        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path >paths;


        // samll logs
        log << "Inserting vertices into a c++ vector structure\n";

        std::vector< int64_t > end_vertices(end_vidsArr, end_vidsArr + size_end_vidsArr);
        std::sort(end_vertices.begin(),end_vertices.end());

#ifndef NDEBUG
        // big logs with cycles wrap them so on release they wont consume time
        log << "end vids: ";

        for (const auto &vid : end_vertices)
        	log << vid << ",";
            log << "\nstart vid:" << start_vid << "\n";

#endif

			if (directed) {
				// very detailed logging
				log << "Working with directed Graph\n";
				pgrouting::DirectedGraph digraph(gType);
				log << "Working with directed Graph 1 \n";

				// - - - - - - - - - - - - - - - - - - - - - - - - - -
				log << data_edges->id << " ";
				log << data_edges->source << " ";
				log << data_edges->target << " ";
				log << data_edges->cost << "\n";
				log << "# de edges: " << total_edges << "\n";


				// Patrix : Carga del vector que contiene los datos de los arcos, obtenidos desde el SQL.
				// Dans ce cas, j'ai ajouté une nouvelle structure : pgr_edge_t_patrix

				const std::vector < pgr_edge_t_patrix > patrix_edges(data_edges, data_edges + total_edges);  // Patrix Idea : std::vector < pgr_edge_t >(edges, edges + count);

				for (unsigned i=0; i< 10 ; i++)   // prueba para los 10 primeros elementos y verificar si se carga correctamente.
					log << 	" id: " 		<< patrix_edges.at(i).id <<
							" source: " 	<< patrix_edges.at(i).source <<
							" target: " 	<< patrix_edges.at(i).target <<
							" cost: " 		<< patrix_edges.at(i).cost <<
							" reverse cost: "<< patrix_edges.at(i).reverse_cost <<
							" distance: " 	<< patrix_edges.at(i).distance <<
							" speed limit: "<< patrix_edges.at(i).speed_limit <<
							" sens: " 		<< patrix_edges.at(i).sens <<
							" travel speed: "<< patrix_edges.at(i).travel_speeds << "\n";

					//log << " source: " << patrix_edges.at(i).source << " target: " << patrix_edges.at(i).target << patrix_edges.at(i).travel_speeds <<  "\n";


				// TODO: VERIFICAR SI LOS DATOS ESTAN BIEN CARGADOS EN  " patrix_edges " !!

				log << " Patrix no lo lograrás >:D  " << "\n";


				// --------------------------------------------------
				// Patrix : Ici on va ajouter le code TDQPPP de Hamza
				// --------------------------------------------------



			    // --------------------------------------------------
			    // Patrix : FIN Code Hamza
			    // --------------------------------------------------



				// Patrix : pasos a seguir
			    // 1) Carga del grafo de Hamza
			    // 2) Aplicación del algoritmo
				// 3) Resultado enviado a PostgreSQL
				// 4) FIN
			    // - - - - - - - - - - - - - - - - - - - - - - - - - -

				// digraph.graph_insert_data(data_edges, total_edges);

#ifndef NDEBUG
            // a graph log is a big log
            // log << digraph.m_num_vertices;  /// prueba comno funciona esta estrutura. ....
#endif
            
            log << "Working with directed Graph 2\n";
            // log << digraph;
            log << " DESPUES \n";


            // Patrix :- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            // std::cout << "Dijkstra (fixed arc travel times) between 327734 and 173675: "
            //           << dijkstra(tdn.adj, tdn.atp_p, tdn.preds, 327734, 173675, starting_time)
            //           << std::endl;
            // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


            // funciona con otro tipo de entrada: pgr_dijkstra(digraph, paths, start_vid, end_vertices, false);
            // Patrix : primero saber como es la entrada del grafo , y luego la salida
            // --->>>  digraph.graph.m_vertices.

            log << "Working with directed Graph 3\n";
        } else {
            // maybe the code is working so cleaner logging
            log << "Working with Undirected Graph\n";
            // pgrouting::UndirectedGraph undigraph(gType);
            // undigraph.graph_insert_data(data_edges, total_edges);

            std::cout << "Hola! "<< std::endl;
            // undigraph.graph.m_vertices.

            // funciona con otro tipo de entrada: pgr_dijkstra(undigraph, paths, start_vid, end_vertices, false);
        }

        // use auto when possible
        auto count(count_tuples(paths));

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log << 
                "No paths found between Starting and any of the Ending vertices\n";
            *log_msg = strdup(log.str().c_str());
            return;
        }

        // get the space required to store all the paths
        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));

        *err_msg = NULL;
        *log_msg = strdup(log.str().c_str());

    } catch (AssertFailedException &exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch (std::exception& exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what() << "\n";
        *err_msg = strdup(log.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch(...) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!\n";
        *err_msg = strdup(log.str().c_str());
        *log_msg = strdup(log.str().c_str());
    }
}
