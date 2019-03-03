/*PGR-GNU*****************************************************************
File: turnRestrictedPath_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307@gmail.com

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

#include "drivers/yen/turnRestrictedPath_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <string>

#include "cpp_common/pgr_alloc.hpp"
#include "cpp_common/pgr_assert.h"

#include "cpp_common/rule.h"

#include "cpp_common/basePath_SSEC.hpp"
#include "yen/pgr_turnRestrictedPath.hpp"

using pgrouting::yen::Pgr_turnRestrictedPath;
using pgrouting::trsp::Rule;

template < class G >
static
std::deque<Path>
pgr_dijkstraTR(
        G &graph,
        const std::vector<pgrouting::trsp::Rule> &restrictions,
        int64_t source,
        int64_t target,
        std::string& log,
        size_t k,
        bool heap_paths,
        bool stop_on_first,
        bool strict) {
    Pgr_turnRestrictedPath< G > fn_TRSP;

    auto paths = fn_TRSP.turnRestrictedPath(graph,
                    restrictions,
                    source,
                    target,
                    k,
                    heap_paths,
                    stop_on_first,
                    strict);

    log += fn_TRSP.get_log();
    return paths;
}

void
do_pgr_turnRestrictedPath(
        pgr_edge_t *data_edges,
        size_t total_edges,

        Restriction_t *restrictions,
        size_t total_restrictions,

        int64_t start_vid,
        int64_t end_vid,

        size_t k,
        bool directed,
        bool heap_paths,
        bool stop_on_first,
        bool strict,

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

        std::vector<pgrouting::trsp::Rule> ruleList;
        for (size_t i = 0; i < total_restrictions; ++i) {
            ruleList.push_back(Rule(*(restrictions + i)));
        }

        log << "\n---------------------------------------\nRestrictions data\n";
        for (const auto &r : ruleList) {
            log << r << "\n";
        }
        log <<"------------------------------------------------------------\n";

        graphType gType = directed? DIRECTED: UNDIRECTED;

        std::vector < pgr_edge_t > edges(data_edges, data_edges + total_edges);

        std::deque<Path> paths;

        std::string logstr;
        if (directed) {
            log << "Working with directed Graph\n";
            pgrouting::DirectedGraph digraph(gType);
            Pgr_turnRestrictedPath < pgrouting::DirectedGraph > fn_TRSP;
            digraph.insert_edges(edges);
            log << digraph;
            paths = pgr_dijkstraTR(digraph,
                    ruleList,

                    start_vid,
                    end_vid,

                    logstr,
                    k,
                    heap_paths,
                    stop_on_first,
                    strict);
        } else {
            log << "TODO Working with Undirected Graph\n";
            pgrouting::UndirectedGraph undigraph(gType);
            Pgr_turnRestrictedPath < pgrouting::UndirectedGraph > fn_TRSP;
            undigraph.insert_edges(data_edges, total_edges);
            paths = pgr_dijkstraTR(undigraph,
                    ruleList,

                    start_vid,
                    end_vid,

                    logstr,

                    k,
                    heap_paths,
                    stop_on_first,
                    strict);
        }

        log << logstr;

        auto count(count_tuples(paths));
        log << "\nCount = " << count;

        if (!(count == 0)) {
            *return_tuples = NULL;
            *return_tuples = pgr_alloc(count, (*return_tuples));

            size_t sequence = 0;
            int route_id = 0;
            for (const auto &path : paths) {
                if (path.size() > 0) {
                    path.get_pg_turn_restricted_path(
                            return_tuples,
                            sequence,
                            route_id);
                }
                log << "the agg cost" << (*return_tuples)[0].agg_cost;
                ++route_id;
            }
        }
        *return_count = count;


        pgassert(*err_msg == NULL);
        *log_msg = log.str().empty()?
            *log_msg :
            pgr_msg(log.str().c_str());
        *notice_msg = notice.str().empty()?
            *notice_msg :
            pgr_msg(notice.str().c_str());
        pgassert(!log.str().empty());
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
