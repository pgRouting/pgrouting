/******************************************************************************
* $Id$
*
* Project:  pgRouting bdsp and bdastar algorithms
* Purpose:
* Author:   Razequl Islam <ziboncsedu@gmail.com>
*

******************************************************************************
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies of this Software or works derived from this Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.

*****************************************************************************/

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif

#include <exception>
#include "BiDirDijkstra.h"
#include "bdsp.h"
#include "utils.h"

#undef DEBUG
//#define DEBUG

#ifdef DEBUG
#include <stdio.h>
static FILE *dbg;
#define DBG(format, arg...) \
    dbg = fopen("/tmp/sew-debug", "a"); \
    if (dbg) { \
        fprintf(dbg, format,  ## arg); \
        fclose(dbg); \
    }
#else
#define DBG(format, arg...) do { ; } while (0)
#endif

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
    )
{
    int res;

    try {
        DBG("Calling BiDirDijkstra initializer.\n");
        BiDirDijkstra bddijkstra;
        DBG("BiDirDijkstra initialized\n");
        res = bddijkstra.bidir_dijkstra(edges, edge_count, maxNode, start_vertex, end_vertex, path, path_count, err_msg);
    }
    catch(std::exception& e) {
        DBG("catch(std::exception e.what: %s\n", e.what());
        *err_msg = (char *) e.what();
        return -1;
    }
    catch(...) {
        DBG("catch(...\n");
        *err_msg = (char *) "Caught unknown exception!";
        return -1;
    }

    DBG("Back from bddijkstra.bidir_dijkstra()\n");
    if (res < 0)
        return res;
    else
	    return EXIT_SUCCESS;
}

