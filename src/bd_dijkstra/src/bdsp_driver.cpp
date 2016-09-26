/*PGR-MIT*****************************************************************

* $Id$
*
* Project: pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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

********************************************************************PGR-MIT*/

#if defined(__MINGW32__) ||  defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include "./bdsp_driver.h"

#include <exception>
#include "./BiDirDijkstra.h"



int bidirsp_wrapper(
        edge_t *edges,
        unsigned int edge_count,
        int maxNode,
        int start_vertex,
        int end_vertex,
        bool directed,
        bool has_reverse_cost,
        path_element_t **path,
        int *path_count,
        char **err_msg
        ) {
    int res;

    try {
        // DBG("Calling BiDirDijkstra initializer.\n");
        BiDirDijkstra bddijkstra;
        // DBG("BiDirDijkstra initialized\n");
        res = bddijkstra.bidir_dijkstra(edges, edge_count, maxNode, start_vertex, end_vertex, path, path_count, err_msg);
        // TODO(someone) this are an unused parameters have to be used
        if (has_reverse_cost) {}
        if (directed) {}
    }
    catch(std::exception& e) {
        // DBG("catch(std::exception e.what: %s\n", e.what());
        *err_msg = (char *) e.what();
        return -1;
    }
    catch(...) {
        // DBG("catch(...\n");
        *err_msg = (char *) "Caught unknown exception!";
        return -1;
    }

    // DBG("Back from bddijkstra.bidir_dijkstra()\n");
    if (res < 0)
        return res;
    return 0;
}
