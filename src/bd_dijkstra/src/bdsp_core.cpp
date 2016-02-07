/*PGR-MIT*****************************************************************

* $Id$
*
* Project:  pgRouting bdsp and bdastar algorithms
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

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif

#include <string.h>
#include <sstream>
#include <exception>
#include "BiDirDijkstra.h"
#include "bdsp.h"



int bidirsp_wrapper(
    edge_t *edges, size_t edge_count,
    int64_t maxNode,
    int64_t start_vertex,
    int64_t end_vertex,
    bool directed,
    bool has_reverse_cost,
    path_element_t **path, size_t *path_count,
    char **err_msg) {

    try {
        std::ostringstream log;
        log << "Calling BiDirDijkstra initializer.\n";
        BiDirDijkstra bddijkstra;
        log << "BiDirDijkstra initialized\n";
        auto res = bddijkstra.bidir_dijkstra(edges, edge_count, static_cast<int>(maxNode), static_cast<int>(start_vertex), static_cast<int>(end_vertex), path, path_count, log);
        *err_msg = strdup(log.str().c_str());
        return 0;
        // TODO  this are an unused parameters have to be used
        if (has_reverse_cost) {};
        if (directed) {};
        *err_msg = strdup(log.str().c_str());
        return res;
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
    *err_msg = (char *) "Should be here";
    return -1;

#if 0
    // DBG("Back from bddijkstra.bidir_dijkstra()\n");
    if (res < 0)
        return res;
    else
        return 0;
#endif
}

