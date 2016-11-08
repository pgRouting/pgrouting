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


#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include <sstream>
#include <deque>
#include <algorithm>
#include <vector>
#include "./astarOneToMany_driver.h"

#include "./../../common/src/pgr_types.h"
#include "./../../common/src/pgr_assert.h"
#include "./../../common/src/pgr_alloc.hpp"
#include "./pgr_astar.hpp"

template < class G >
std::deque< Path >
pgr_astar(
        G &graph,
        int64_t source,
        std::vector < int64_t > targets,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,
        bool normal) {
    std::sort(targets.begin(), targets.end());
    targets.erase(
            std::unique(targets.begin(), targets.end()),
            targets.end());

    pgrouting::algorithms::Pgr_astar< G > fn_astar;
    auto paths = fn_astar.astar(graph, source, targets,
            heuristic, factor, epsilon, only_cost);

    if (!normal) {
        for (auto &path : paths) {
                path.reverse();
        }
    }
    return paths;
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
        bool normal) {
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
        std::vector< int64_t > end_vids(
                end_vidsArr,
                end_vidsArr + size_end_vidsArr);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        if (directed) {
            pgrouting::xyDirectedGraph digraph(
                    pgrouting::extract_vertices(edges, total_edges),
                    gType);
            digraph.insert_edges(edges, total_edges);
            paths = pgr_astar(digraph, start_vid, end_vids,
                    heuristic, factor, epsilon, only_cost, normal);
        } else {
            pgrouting::xyUndirectedGraph undigraph(
                    pgrouting::extract_vertices(edges, total_edges),
                    gType);
            undigraph.insert_edges(edges, total_edges);
            paths = pgr_astar(undigraph, start_vid, end_vids,
                    heuristic, factor, epsilon, only_cost, normal);
        }

        size_t count(0);
        count = count_tuples(paths);


        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            log << "No paths found\n";
            *log_msg = strdup(log.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = (collapse_paths(return_tuples, paths));

        pgassert(!(*err_msg));
        *log_msg = strdup(log.str().c_str());

    } catch (AssertFailedException &except) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << except.what() << "\n";
        *err_msg = strdup(err.str().c_str());
        *log_msg = strdup(log.str().c_str());
    } catch (std::exception& except) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << except.what() << "\n";
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
