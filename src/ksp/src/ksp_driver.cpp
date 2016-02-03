/*PGR-GNU*****************************************************************
File: ksp_driver.cpp

Copyright (c) 2015 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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
#ifdef unlink
#undef unlink
#endif
#endif


#include <deque>
#include <sstream>


#include "./ksp_driver.h"
#include "../../common/src/memory_func.hpp"
#include "./pgr_ksp.hpp"



int  do_pgr_ksp(pgr_edge_t  *data_edges, int64_t total_tuples,
                       int64_t  start_vertex, int64_t  end_vertex,
                       int k, bool directedFlag, bool heap_paths,
                       General_path_element_t **ksp_path, size_t *path_count,
                       char ** err_msg) {
    try {
        std::ostringstream log;

        graphType gType = directedFlag? DIRECTED: UNDIRECTED;
        const int initial_size = total_tuples;

        std::deque< Path > paths;

        if (directedFlag) {
            Pgr_base_graph< DirectedGraph > digraph(gType, initial_size);
            Pgr_ksp< Pgr_base_graph< DirectedGraph > > fn_yen;
            digraph.graph_insert_data(data_edges, initial_size);
            paths = fn_yen.Yen(digraph, start_vertex, end_vertex, k, heap_paths);
        } else {
            Pgr_base_graph< UndirectedGraph > undigraph(gType, initial_size);
            Pgr_ksp< Pgr_base_graph< UndirectedGraph > > fn_yen;
            undigraph.graph_insert_data(data_edges, initial_size);
            paths = fn_yen.Yen(undigraph, start_vertex, end_vertex, k, heap_paths);
        }


        int count(count_tuples(paths));

        if (count == 0) {
            *err_msg = strdup(
                    "NOTICE: No paths found between Starting and Ending vertices");
            *ksp_path = noResult(path_count, (*ksp_path));
            return 0;
        }

        // get the space required to store all the paths
        *ksp_path = NULL;
        *ksp_path = get_memory(count, (*ksp_path));

        int sequence = 0;
        int route_id = 0;
        for (const auto &path : paths) {
            if (path.size() > 0)
                path.get_pg_ksp_path(ksp_path, sequence, route_id);
            ++route_id;
        }

        if (count != sequence) {                                
            *err_msg = NULL;
            return 2;
        }                                                                                                       
        *path_count = count;

#if 1
        *err_msg = strdup("OK");
#else
        *err_msg = strdup(log.str().c_str());
#endif
        return EXIT_SUCCESS;
    } catch ( ... ) {
        *err_msg = strdup("Caught unknown expection!");
        return -1;
    }
}

