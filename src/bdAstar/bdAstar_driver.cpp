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

#include "cpp_common/combinations.h"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "c_types/ii_t_rt.h"




/************************************************************
  edges_sql TEXT,
    start_vid BIGINT,
    end_vid BIGINT,
    directed BOOLEAN DEFAULT true,
    only_cost BOOLEAN DEFAULT false,
 ***********************************************************/
namespace {
template < class G >
std::deque<pgrouting::Path>
pgr_bdAstar(
        G &graph,
        const std::map<int64_t, std::set<int64_t>> &combinations,
        int heuristic,
        double factor,
        double epsilon,
        std::ostream &log,
        bool only_cost) {
    using pgrouting::Path;

    pgrouting::bidirectional::Pgr_bdAstar<G> fn_bdAstar(graph);
    std::deque<Path> paths;

    /* for each departure process each destination */
    for (const auto &c : combinations) {
        if (!graph.has_vertex(c.first)) continue;

        for (const auto &destination : c.second) {
            if (!graph.has_vertex(destination)) continue;

            fn_bdAstar.clear();

            paths.push_back(fn_bdAstar.pgr_bdAstar(
                        graph.get_V(c.first), graph.get_V(destination),
                        heuristic, factor, epsilon, only_cost));

        }
    }

    log << fn_bdAstar.log();

    return paths;
}

}  // namespace

void
do_pgr_bdAstar(
        Edge_xy_t *edges, size_t total_edges,

        II_t_rt *combinationsArr, size_t total_combinations,

        int64_t  *start_vidsArr, size_t size_start_vidsArr,
        int64_t  *end_vidsArr, size_t size_end_vidsArr,
        bool directed,
        int heuristic,
        double factor,
        double epsilon,
        bool only_cost,

        Path_rt **return_tuples,
        size_t *return_count,

        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    using pgrouting::Path;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_alloc;

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


        auto combinations = total_combinations?
            pgrouting::utilities::get_combinations(combinationsArr, total_combinations)
            : pgrouting::utilities::get_combinations(start_vidsArr, size_start_vidsArr, end_vidsArr, size_end_vidsArr);
#if 0
        log << "Inserting vertices into a c++ vector structure";
        std::vector<int64_t>
            start_vertices(start_vidsArr, start_vidsArr + size_start_vidsArr);
        std::vector< int64_t >
            end_vertices(end_vidsArr, end_vidsArr + size_end_vidsArr);
        std::vector< II_t_rt >
            combinations_vector(combinations, combinations + total_combinations);
#endif

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
                    combinations,
                    heuristic, factor, epsilon,
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
                    combinations,
                    heuristic, factor, epsilon,
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
