/*PGR-GNU*****************************************************************
File: withPoints_driver.cpp

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

#include "drivers/yen/withPoints_ksp_driver.h"


#include <sstream>
#include <deque>
#include <vector>

#include "yen/pgr_ksp.hpp"

#include "withPoints/pgr_withPoints.hpp"
#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

using pgrouting::yen::Pgr_ksp;

// CREATE OR REPLACE FUNCTION pgr_withPointsKSP(
// edges_sql TEXT,
// points_sql TEXT,
// start_pid BIGINT,
// end_pid BIGINT,
// directed BOOLEAN DEFAULT true


int
do_pgr_withPointsKsp(
        pgr_edge_t  *edges,           size_t total_edges,
        Point_on_edge_t  *points_p,   size_t total_points,
        pgr_edge_t  *edges_of_points, size_t total_edges_of_points,
        int64_t start_pid,
        int64_t end_pid,
        size_t k,
        bool directed,
        bool heap_paths,
        char driving_side,
        bool details,
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

        pgrouting::Pg_points_graph pg_graph(
                std::vector<Point_on_edge_t>(
                    points_p,
                    points_p + total_points),
                std::vector< pgr_edge_t >(
                    edges_of_points,
                    edges_of_points + total_edges_of_points),
                true,
                driving_side,
                directed);

        if (pg_graph.has_error()) {
            log << pg_graph.get_log();
            err << pg_graph.get_error();
            *log_msg = pgr_msg(log.str().c_str());
            *err_msg = pgr_msg(err.str().c_str());
            return -1;
        }


        int64_t start_vid(start_pid);
        int64_t end_vid(end_pid);

        log << "start_pid" << start_pid << "\n";
        log << "end_pid" << end_pid << "\n";
        log << "driving_side" << driving_side << "\n";
        log << "start_vid" << start_vid << "\n";
        log << "end_vid" << end_vid << "\n";
        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::deque< Path > paths;

        auto vertices(pgrouting::extract_vertices(edges, total_edges));
        vertices = pgrouting::extract_vertices(vertices, pg_graph.new_edges());

        log << "extracted vertices: ";
        for (const auto v : vertices) {
            log << v.id << ", ";
        }
        log << "\n";

        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(vertices, gType);
            digraph.insert_edges(edges, total_edges);
            log << "graph after inserting edges\n";
            log << digraph << "\n";

            digraph.insert_edges(pg_graph.new_edges());
            log << "graph after inserting new edges\n";
            log << digraph << "\n";

            Pgr_ksp< pgrouting::DirectedGraph  > fn_yen;
            paths = fn_yen.Yen(digraph, start_vid, end_vid, k, heap_paths);
            // pgassert(true==false);
        } else {
            log << "Working with undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(edges, total_edges);
            undigraph.insert_edges(pg_graph.new_edges());

            Pgr_ksp< pgrouting::UndirectedGraph > fn_yen;
            paths = fn_yen.Yen(undigraph, start_vid, end_vid, k, heap_paths);
        }


        if (!details) {
            for (auto &path : paths) {
                path = pg_graph.eliminate_details(path);
            }
        }

        auto count(count_tuples(paths));

        if (count == 0) {
            return 0;
        }


        *return_tuples = NULL;
        *return_tuples = pgr_alloc(count, (*return_tuples));

        size_t sequence = 0;
        int route_id = 0;
        for (const auto &path : paths) {
            if (path.size() > 0)
                path.get_pg_ksp_path(return_tuples, sequence, route_id);
            ++route_id;
        }

        if (count != sequence) {
            return 2;
        }
        (*return_count) = sequence;

        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
        return 0;
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
    return 1000;
}
