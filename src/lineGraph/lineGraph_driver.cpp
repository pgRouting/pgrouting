/*PGR-GNU*****************************************************************
File: lineGraph_driver.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2017 Vidhan Jain
Mail: vidhanj1307 at gmail.com

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

#include "drivers/lineGraph/lineGraph_driver.h"

#include <sstream>
#include <deque>
#include <vector>
#include <utility>
#include <string>

#include "bgraph/line_graph.hpp"
#include "bgraph/graph_to_edges.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/base_graph.hpp"
#include "c_types/edge_rt.h"

namespace {

template<typename G>
std::vector<Edge_t> line_graph(const G& original, bool add_self_loop) {
    auto lg_result = pgrouting::b_g::line_graph(original.graph);

    return pgrouting::b_g::graph_to_existing_edges(lg_result, add_self_loop);
}

}  // namespace

void
pgr_do_lineGraph(
        char *edges_sql,
        bool directed,

        Edge_rt **return_tuples,
        size_t *return_count,

        char ** log_msg,
        char ** notice_msg,
        char ** err_msg) {
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    char *hint = nullptr;

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);


        hint = edges_sql;
        auto edges = pgrouting::pgget::get_edges(std::string(edges_sql), true, false);
        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
            return;
        }
        hint = nullptr;

        std::vector<Edge_t> line_graph_edges;
        if (directed) {
            pgrouting::DirectedGraph ograph;
            ograph.insert_edges(edges);
            line_graph_edges = line_graph(ograph, directed);
        } else {
            pgrouting::UndirectedGraph ograph;
            ograph.insert_edges(edges);
            line_graph_edges = line_graph(ograph, directed);
        }

        auto count = line_graph_edges.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            *log_msg = to_pg_msg(log);
            return;
        }

        size_t sequence = 0;
        using pgrouting::pgr_alloc;
        (*return_tuples) = pgr_alloc(line_graph_edges.size(), (*return_tuples));

        for (const auto &e : line_graph_edges) {
            auto rev_c = directed? e.reverse_cost : -1;
            (*return_tuples)[sequence] = {e.id, e.source, e.target, e.cost, rev_c};
            sequence++;
        }
        (*return_count) = sequence;

        pgassert(*err_msg == NULL);
        *log_msg = to_pg_msg(log);
        *notice_msg = to_pg_msg(notice);
    } catch (AssertFailedException &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (const std::string &ex) {
        *err_msg = to_pg_msg(ex);
        *log_msg = hint? to_pg_msg(hint) : to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch(...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << "Caught unknown exception!";
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    }
}
