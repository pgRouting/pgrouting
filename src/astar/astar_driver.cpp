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

#include "drivers/astar/astar_driver.h"

#include <sstream>
#include <deque>
#include <algorithm>
#include <vector>

#include "astar/pgr_astar.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"


template < class G >
std::deque<Path>
pgr_astar(
        G &graph,
        std::vector<int64_t> sources,
        std::vector<int64_t> targets,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,
        bool normal) {
    std::sort(sources.begin(), sources.end());
    sources.erase(
            std::unique(sources.begin(), sources.end()),
            sources.end());

    std::sort(targets.begin(), targets.end());
    targets.erase(
            std::unique(targets.begin(), targets.end()),
            targets.end());

    pgrouting::algorithms::Pgr_astar< G > fn_astar;
    auto paths = fn_astar.astar(graph, sources, targets,
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
void do_pgr_astarManyToMany(
        Pgr_edge_xy_t *edges, size_t total_edges,
        int64_t  *start_vidsArr, size_t size_start_vidsArr,
        int64_t  *end_vidsArr, size_t size_end_vidsArr,
        bool directed,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,
        bool normal,
        General_path_element_t **return_tuples,
        size_t *return_count,
        char** log_msg,
        char** notice_msg,
        char** err_msg) {
    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);


        log << "Inserting target vertices into a c++ vector structure\n";
        std::vector< int64_t > end_vids(
                end_vidsArr,
                end_vidsArr + size_end_vidsArr);
        std::vector< int64_t > start_vids(
                start_vidsArr,
                start_vidsArr + size_start_vidsArr);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path >paths;
        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::xyDirectedGraph digraph(
                    pgrouting::extract_vertices(edges, total_edges),
                    gType);
            digraph.insert_edges(edges, total_edges);
            paths = pgr_astar(digraph, start_vids, end_vids,
                    heuristic, factor, epsilon, only_cost, normal);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::xyUndirectedGraph undigraph(
                    pgrouting::extract_vertices(edges, total_edges),
                    gType);
            undigraph.insert_edges(edges, total_edges);
            paths = pgr_astar(undigraph, start_vids, end_vids,
                    heuristic, factor, epsilon, only_cost, normal);
        }

        size_t count(0);
        count = count_tuples(paths);


        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No paths found\n";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "Converting a set of paths into the tuples\n";
        (*return_count) = (collapse_paths(return_tuples, paths));

        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}

