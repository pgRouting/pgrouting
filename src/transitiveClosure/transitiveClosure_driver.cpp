/*PGR-GNU*****************************************************************
File: transitiveClosure_driver.cpp
Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org
Function's developer:
Copyright (c) 2019 Hang Wu
mail: nike0good@gmail.com
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

#include "drivers/transitiveClosure/transitiveClosure_driver.h"

#include <string>
#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>

#include "transitiveClosure/transitiveClosure.hpp"

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/identifiers.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/basePath_SSEC.hpp"
#include "cpp_common/base_graph.hpp"

#include "c_types/transitiveClosure_rt.h"

namespace {

template < class G >
boost::adjacency_list <>
pgr_transitiveClosure(
        G &graph) {
    Pgr_transitiveClosure< G > fn_transitiveClosure;
    return fn_transitiveClosure.transitiveClosure(graph);
}


template <typename G>
void get_postgres_result(
        G &graph,
        TransitiveClosure_rt **return_tuples,
        size_t *count) {
    using pgrouting::pgr_alloc;
    boost::adjacency_list <> TC;
    TC = pgr_transitiveClosure(graph);
    (*count) = boost::num_vertices(TC);
    (*return_tuples) = pgr_alloc((*count), (*return_tuples));
    size_t sequence = 0;
    boost::graph_traits < boost::adjacency_list <> >::vertex_iterator i, end;
    boost::graph_traits < boost::adjacency_list <> >::adjacency_iterator ai, a_end;
    int ii = 1;
    for (boost::tie(i, end) = boost::vertices(TC); i != end; ++i) {
            auto u = *i;
            int64_t* target_array = nullptr;
            auto uid = graph.graph[u].id;
            boost::tie(ai, a_end) = adjacent_vertices(u, TC);
            size_t adj_siz = static_cast<size_t>(a_end - ai);
            target_array = pgr_alloc(adj_siz , target_array);
            int number = 0;
            for (; ai != a_end; ++ai) {
                auto v = *ai;
                int64_t vid = graph.graph[v].id;
                target_array[number++] = vid;
            }

        (*return_tuples)[sequence] = {
            ii++,
            uid,
            target_array,
            number};

         ++sequence;
    }
}

}  // namespace


/************************************************************
  edges_sql TEXT
 ***********************************************************/
void
pgr_do_transitiveClosure(
        char *edges_sql,

        TransitiveClosure_rt **return_tuples,
        size_t *return_count,
        char **log_msg,
        char **notice_msg,
        char **err_msg) {
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;

    std::ostringstream log;
    std::ostringstream notice;
    std::ostringstream err;
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

        pgrouting::DirectedGraph digraph;
        digraph.insert_edges(edges);

        get_postgres_result(
                digraph,
                return_tuples,
                return_count);

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
