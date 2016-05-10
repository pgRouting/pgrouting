/*PGR-GNU*****************************************************************
File: astarOneToOne_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2015 Celia Virginia Vergara Castillo
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
#endif


#include <sstream>
#include <deque>
#include <algorithm>
#include <vector>
#include "./astarOneToMany_driver.h"

// #define DEBUG

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include "./pgr_astar.hpp"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_alloc.hpp"

template < class G >
void
pgr_astar(
        G &graph,
        std::deque< Path >  &paths,
        int64_t source,
        std::vector < int64_t > targets,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost = false) {
    Pgr_astar< G > fn_astar;
    fn_astar.astar(graph, paths, source, targets, heuristic, factor, epsilon, only_cost);
}


/************************************************************
  edges_sql TEXT,
  vertex_table TEXT,
  start_vid BIGINT,
  end_vid BIGINT  directed BOOLEAN DEFAULT true,
 ***********************************************************/
void do_pgr_astarOneToMany(
        Pgr_edge_xy_t  *edges,
        size_t total_edges,
        int64_t start_vid,
        int64_t  *end_vidsArr, size_t size_end_vidsArr,
        bool directed,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** err_msg,
        bool normal){
    std::ostringstream err;
    std::ostringstream log;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);


        if (total_edges <= 1) {
            err << "Required: more than one edge\n";
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *err_msg = strdup(log.str().c_str());
            return;
        }

        std::deque< Path >paths;
        log << "Inserting target vertices into a c++ vector structure\n";
        std::vector< int64_t > end_vids(end_vidsArr, end_vidsArr + size_end_vidsArr);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        if (directed) {
            log << "Working with directed Graph\n";
            pgRouting::xyDirectedGraph digraph(
                    pgRouting::extract_vertices(edges, total_edges),
                    gType);
            digraph.graph_insert_data(edges, total_edges);
            pgr_astar(digraph, paths, start_vid, end_vids, heuristic, factor, epsilon, only_cost);
        } else {
            log << "Working with Undirected Graph\n";
            pgRouting::xyUndirectedGraph undigraph(
                    pgRouting::extract_vertices(edges, total_edges),
                    gType);
            undigraph.graph_insert_data(edges, total_edges);
            pgr_astar(undigraph, paths, start_vid, end_vids, heuristic, factor, epsilon, only_cost);
        }
        if (!normal) {
            for (auto &path : paths) {
                log << "reversing path\n";
                log << path;
                path.reverse();
                log << "reversed path\n";
                log << path;
            }
        }
        log << "checking reversed paths\n";
        for (auto path : paths) {
            log << "reversed path\n";
            log << path;
        }

        size_t count(0);
        count = count_tuples(paths);


        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log <<
                "No paths found between Starting and any of the Ending vertices\n";
            *err_msg = strdup(log.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));

        *err_msg = NULL;
        *log_msg = strdup(log.str().c_str());

    } catch (AssertFailedException &exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what() << "\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch (std::exception& exept) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << exept.what() << "\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch(...) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    }
}
