/*PGR-GNU*****************************************************************
File: many_to_many_dijkstra_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

Copyright (c) 2020 The combinations_sql signature is added by Mahmoud SAKR
and Esteban ZIMANYI
mail: m_attia_sakr@yahoo.com, estebanzimanyi@gmail.com

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

#include "drivers/dijkstra/dijkstra_driver.h"
#include <c_types/pgr_combination_t.h>

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <limits>

#include "dijkstra/pgr_dijkstra.hpp"

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

namespace detail {

void
post_process(std::deque<Path> &paths, bool only_cost, bool normal, size_t n_goals) {
    paths.erase(std::remove_if(paths.begin(), paths.end(),
                [](const Path &p){
                    return p.size()==0;}),
            paths.end());

    if (!normal) {
        for (auto &path : paths) path.reverse();
    }

    if (!only_cost) {
        for (auto &p : paths) {
            p.recalculate_agg_cost();
        }
    }

    if (n_goals != (std::numeric_limits<size_t>::max)()) {
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.end_id() < e2.end_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.start_id() < e2.start_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.tot_cost() < e2.tot_cost();
                });
        if (n_goals < paths.size()) {
            paths.erase(paths.begin() + n_goals, paths.end());
        }
    } else {
        std::sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.end_id() < e2.end_id();
                });
        std::stable_sort(paths.begin(), paths.end(),
                [](const Path &e1, const Path &e2)->bool {
                    return e1.start_id() < e2.start_id();
                });
    }
}


template < class G >
std::deque< Path >
pgr_dijkstra(
        G &graph,
        std::vector < int64_t > sources,
        std::vector < int64_t > targets,
        bool only_cost,
        bool normal,
        size_t n_goals) {
    std::sort(sources.begin(), sources.end());
    sources.erase(
            std::unique(sources.begin(), sources.end()),
            sources.end());

    std::sort(targets.begin(), targets.end());
    targets.erase(
            std::unique(targets.begin(), targets.end()),
            targets.end());

    pgrouting::Pgr_dijkstra< G > fn_dijkstra;
    auto paths = fn_dijkstra.dijkstra(
            graph,
            sources, targets,
            only_cost, n_goals);

    post_process(paths, only_cost, normal, n_goals);

    return paths;
}


template < class G >
std::deque< Path >
pgr_dijkstra(
        G &graph,
        std::vector < pgr_combination_t > &combinations,
        bool only_cost,
        bool normal) {
    pgrouting::Pgr_dijkstra< G > fn_dijkstra;
    auto paths = fn_dijkstra.dijkstra(
            graph,
            combinations,
            only_cost);

    post_process(paths, only_cost, normal, (std::numeric_limits<size_t>::max)());

    return paths;
}
}  // namespace detail


// CREATE OR REPLACE FUNCTION pgr_dijkstra(
// sql text,
// start_vids anyarray,
// end_vids anyarray,
// directed boolean default true,
void
do_pgr_many_to_many_dijkstra(
        pgr_edge_t  *data_edges,
        size_t total_edges,
        int64_t  *start_vidsArr,
        size_t size_start_vidsArr,
        int64_t  *end_vidsArr,
        size_t size_end_vidsArr,
        bool directed,
        bool only_cost,
        bool normal,
        int64_t n_goals,

        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;

    try {
        pgassert(total_edges != 0);
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;

        log << "Inserting vertices into a c++ vector structure";
        std::vector<int64_t>
            start_vertices(start_vidsArr, start_vidsArr + size_start_vidsArr);
        std::vector< int64_t >
            end_vertices(end_vidsArr, end_vidsArr + size_end_vidsArr);

        size_t n = n_goals <= 0? (std::numeric_limits<size_t>::max)() : static_cast<size_t>(n_goals);

        std::deque< Path >paths;
        if (directed) {
            log << "\nWorking with directed Graph";
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_edges);
            paths = detail::pgr_dijkstra(
                    digraph,
                    start_vertices, end_vertices,
                    only_cost, normal, n);
        } else {
            log << "\nWorking with Undirected Graph";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_edges);
            paths = detail::pgr_dijkstra(
                    undigraph,
                    start_vertices, end_vertices,
                    only_cost, normal, n);
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


// CREATE OR REPLACE FUNCTION pgr_dijkstra(
// sql text,
// combinations sql text,
// directed boolean default true,
void
do_pgr_combinations_dijkstra(
        pgr_edge_t  *data_edges,
        size_t total_edges,
        pgr_combination_t *combinations,
        size_t total_combinations,
        bool directed,
        bool only_cost,
        bool normal,

        General_path_element_t **return_tuples,
        size_t *return_count,
        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;

    try {
        pgassert(total_edges != 0);
        pgassert(total_combinations != 0);
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        graphType gType = directed? DIRECTED: UNDIRECTED;


        log << "Inserting combinations into a c++ vector structure";
        std::vector<pgr_combination_t>
                combinations_vector(combinations, combinations + total_combinations);

        std::deque< Path >paths;
        if (directed) {
            log << "\nWorking with directed Graph";
            pgrouting::DirectedGraph digraph(gType);
            digraph.insert_edges(data_edges, total_edges);
            paths = detail::pgr_dijkstra(
                    digraph,
                    combinations_vector,
                    only_cost, normal);
        } else {
            log << "\nWorking with Undirected Graph";
            pgrouting::UndirectedGraph undigraph(gType);
            undigraph.insert_edges(data_edges, total_edges);
            paths = detail::pgr_dijkstra(
                    undigraph,
                    combinations_vector,
                    only_cost, normal);
        }
        combinations_vector.clear();
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


