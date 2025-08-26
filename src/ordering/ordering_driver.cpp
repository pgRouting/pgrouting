/*PGR-GNU*****************************************************************
File: ordering_driver.cpp
Generated with Template by:
Copyright (c) 2025 pgRouting developers
Mail: project@pgrouting.org

Developers:

Copyright (c) 2025 Bipasha Gayary
Mail: bipashagayary at gmail.com

Copyright (c) 2025 Fan Wu
Mail: wifiblack0131 at gmail.com

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

#include "drivers/ordering_driver.hpp"

#include <sstream>
#include <deque>
#include <vector>
#include <algorithm>
#include <string>


#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

#include "ordering/sloanOrdering.hpp"
#include "ordering/kingOrdering.hpp"

namespace {
template <class G>
std::vector <int64_t>
sloanOrdering(G &graph) {
        pgrouting::functions::SloanOrdering <G> fn_sloanOrdering;
        auto results = fn_sloanOrdering.sloanOrdering(graph);
        return results;
}
}

void
do_ordering(
    std::string edges_sql,
    int which,

    int64_t **return_tuples,
    size_t *return_count,

    char **log_msg,
    char **notice_msg,
    char **err_msg) {

    using pgrouting::pgr_alloc;
    using pgrouting::to_pg_msg;
    using pgrouting::pgr_free;
    using pgrouting::kingOrdering;
    using pgrouting::pgget::get_edges;
    using pgrouting::UndirectedGraph;

    std::ostringstream log;
    std::ostringstream err;
    std::ostringstream notice;
    std::string hint = "";

    try {
        pgassert(!(*log_msg));
        pgassert(!(*notice_msg));
        pgassert(!(*err_msg));
        pgassert(!(*return_tuples));
        pgassert(*return_count == 0);

        hint = edges_sql;
        auto edges = get_edges(std::string(edges_sql), true, false);
        if (edges.empty()) {
            *notice_msg = to_pg_msg("No edges found");
            *return_tuples = nullptr;
            *return_count = 0;
            return;
        }
        hint = "";

        std::vector<int64_t> results;

        auto vertices(pgrouting::extract_vertices(edges));
        UndirectedGraph undigraph(vertices);
        undigraph.insert_edges(edges);

        if (which == 0) {
            results = sloanOrdering(undigraph);
        } else if (which == 2) {
            results = kingOrdering(undigraph);
        } else if (which == 3) {
            results = minDegreeOrdering(undigraph);
        }

        auto count = results.size();

        if (count == 0) {
            *notice_msg = to_pg_msg("No results found \n");
            *err_msg = to_pg_msg(err);
            *return_tuples = nullptr;
            *return_count = 0;
            return;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));

        for (size_t i = 0; i < count; ++i) {
              (*return_tuples)[i] = results[i];
        }

        (*return_count) = count;

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
        *log_msg = hint.empty()? to_pg_msg(hint) : to_pg_msg(log);
    } catch (std::exception &except) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        err << except.what();
        *err_msg = to_pg_msg(err);
        *log_msg = to_pg_msg(log);
    } catch (...) {
        (*return_tuples) = pgr_free(*return_tuples);
        (*return_count) = 0;
        *err_msg = to_pg_msg("Caught unknown exception");
        *log_msg = to_pg_msg(log);
    }
}
