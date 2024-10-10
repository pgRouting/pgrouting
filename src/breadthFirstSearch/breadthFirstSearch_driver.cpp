/*PGR-GNU*****************************************************************
File: breadthFirstSearch_driver.cpp

Generated with Template by:
Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Function's developer:
Copyright (c) 2019 Gudesa Venkata Sai Akhil
Mail: gvs.akhil1997@gmail.com

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

#include "drivers/breadthFirstSearch/breadthFirstSearch_driver.h"

#include <vector>
#include <algorithm>
#include <string>
#include <set>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

#include "breadthFirstSearch/breadthFirstSearch.hpp"

namespace {

#if 0
template < class G >
std::vector<MST_rt>
pgr_breadthFirstSearch(
        G &graph,
        std::vector < int64_t > sources,
        int64_t max_depth) {
    std::sort(sources.begin(), sources.end());
    sources.erase(
            std::unique(sources.begin(), sources.end()),
            sources.end());


    pgrouting::functions::Pgr_breadthFirstSearch< G > fn_breadthFirstSearch;
    auto results = fn_breadthFirstSearch.breadthFirstSearch(
            graph, sources, max_depth);
    return results;
}
#endif

template <class G> std::vector<MST_rt> breadthFirstSearch(
        G &graph,
        std::set<int64_t> sources,
        int64_t max_depth) {
    pgrouting::functions::Pgr_breadthFirstSearch<G> fn_breadthFirstSearch;
    auto results = fn_breadthFirstSearch.breadthFirstSearch(graph, sources, max_depth);
    return results;
}

}  // namespace

void
pgr_do_breadthFirstSearch(
                char *edges_sql,
                ArrayType* starts,
                int64_t  max_depth,
                bool directed,

                MST_rt **return_tuples,
                size_t *return_count,
                char ** log_msg,
                char ** notice_msg,
                char ** err_msg) {
    using pgrouting::pgr_alloc;
    using pgrouting::pgr_msg;
    using pgrouting::pgr_free;
    using pgrouting::pgget::get_intSet;

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
            *notice_msg = pgr_msg("No edges found");
            *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
            return;
        }
        hint = nullptr;

        auto roots = get_intSet(starts);

        std::vector<MST_rt> results;
        if (directed) {
            pgrouting::DirectedGraph digraph;
            digraph.insert_edges(edges);
            results = breadthFirstSearch(digraph, roots, max_depth);

        } else {
            pgrouting::UndirectedGraph undigraph;
            undigraph.insert_edges(edges);
            results = breadthFirstSearch(undigraph, roots, max_depth);
        }

        auto count = results.size();

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            notice << "No traversal found";
            *log_msg = pgr_msg(notice.str().c_str());
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = count;
        for (size_t i = 0; i < count; i++) {
            *((*return_tuples) + i) = results[i];
        }

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
    } catch (const std::string &ex) {
        *err_msg = pgr_msg(ex.c_str());
        *log_msg = hint? pgr_msg(hint) : pgr_msg(log.str().c_str());
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
