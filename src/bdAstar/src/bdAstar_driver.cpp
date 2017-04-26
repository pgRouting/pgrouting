/*PGR-GNU*****************************************************************
File: bdAstar_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer: 
Copyright (c) 2016 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#include "drivers/bdAstar/bdAstar_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>

#include "bdAstar/pgr_bdAstar.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"





/************************************************************
  edges_sql TEXT,
    start_vid BIGINT,
    end_vid BIGINT,
    directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
 ***********************************************************/

template < class G >
static
std::deque<Path>
pgr_bdAstar(
        G &graph,
        std::vector < int64_t > sources,
        std::vector < int64_t > targets,

        int heuristic,
        double factor,
        double epsilon,
        std::ostream &log,
        bool only_cost) {
    log << "entering static function\n";
    std::sort(sources.begin(), sources.end());
    sources.erase(
            std::unique(sources.begin(), sources.end()),
            sources.end());

    std::sort(targets.begin(), targets.end());
    targets.erase(
            std::unique(targets.begin(), targets.end()),
            targets.end());


    pgrouting::bidirectional::Pgr_bdAstar<G> fn_bdAstar(graph);
    std::deque<Path> paths;
    for (const auto source : sources) {
        for (const auto target : targets) {
            fn_bdAstar.clear();

            paths.push_back(fn_bdAstar.pgr_bdAstar(
            graph.get_V(source), graph.get_V(target),
            heuristic, factor, epsilon, only_cost));
        }
    }
    log << fn_bdAstar.log();

    return paths;
}


void
do_pgr_bdAstar(
        Pgr_edge_xy_t *edges,
        size_t total_edges,
        int64_t  *start_vidsArr,
        size_t size_start_vidsArr,
        int64_t  *end_vidsArr,
        size_t size_end_vidsArr,


        bool directed,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,

        General_path_element_t **return_tuples,
        size_t *return_count,

        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);
        pgassert(total_edges != 0);


        log << "Inserting vertices into a c++ vector structure";
        std::vector<int64_t>
            start_vertices(start_vidsArr, start_vidsArr + size_start_vidsArr);
        std::vector< int64_t >
            end_vertices(end_vidsArr, end_vidsArr + size_end_vidsArr);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque<Path> paths;
        log << "starting process\n";
        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::xyDirectedGraph digraph(
                    pgrouting::extract_vertices(edges, total_edges),
                    gType);
            digraph.insert_edges(edges, total_edges);

            paths = pgr_bdAstar(digraph,
                    start_vertices,
                    end_vertices,
                    heuristic,
                    factor,
                    epsilon,
                    log,
                    only_cost);
        } else {
            log << "Working with Undirected Graph\n";
            pgrouting::xyUndirectedGraph undigraph(
                    pgrouting::extract_vertices(edges, total_edges),
                    gType);
            undigraph.insert_edges(edges, total_edges);

            paths = pgr_bdAstar(
                    undigraph,
                    start_vertices,
                    end_vertices,
                    heuristic,
                    factor,
                    epsilon,
                    log,
                    only_cost);
        }

        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No paths found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        log << "\nConverting a set of paths into the tuples";
        (*return_count) = (collapse_paths(return_tuples, paths));


#if 0
        auto count = path.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice <<
                "No paths found between start_vid and end_vid vertices";
        } else {
            (*return_tuples) = pgr_alloc(count, (*return_tuples));
            size_t sequence = 0;
            path.generate_postgres_data(return_tuples, sequence);
            (*return_count) = sequence;
        }
#endif

        pgassert(*err_msg == NULL);
        *log_msg = log.str().empty()?
            nullptr :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            nullptr :
            pgr_msg(notice.str().c_str());
    } catch (AssertFailedException &except) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch (std::exception& except) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    } catch(...) {
        if (*return_tuples) free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = pgr_msg(err.str().c_str());
        *log_msg = pgr_msg(log.str().c_str());
    }
}
