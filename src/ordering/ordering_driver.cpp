/*PGR-GNU*****************************************************************
File: ordering_driver.cpp
Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
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

#include <algorithm>
#include <sstream>
#include <deque>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/to_postgres.hpp"

#include "ordering/sloanOrdering.hpp"
#include "ordering/kingOrdering.hpp"
#include "ordering/cuthillMckeeOrdering.hpp"
#include "ordering/topologicalSort.hpp"

template <typename G, typename Func>
void
process(const std::vector<Edge_t> &edges, G &graph, Func ordering,
        size_t &return_count,
        int64_t* &return_tuples) {
    using pgrouting::to_postgres::get_vertexId;
    graph.insert_edges(edges);
    auto results = ordering(graph);
    get_vertexId(graph, results, return_count, return_tuples);
}

void
do_ordering(
        const std::string &edges_sql,
        Which which,

        int64_t *&return_tuples,
        size_t &return_count,

        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    std::string hint = "";

    try {
        pgassert(!edges_sql.empty());

        using pgrouting::pgget::get_edges;
        using pgrouting::UndirectedGraph;
        using pgrouting::DirectedGraph;

        using pgrouting::functions::sloanOrdering;
        using pgrouting::functions::kingOrdering;
        using pgrouting::functions::cuthillMckeeOrdering;
        using pgrouting::functions::topologicalSort;


        hint = edges_sql;
        auto edges = get_edges(edges_sql, true, false);
        if (edges.empty()) {
            notice << "No edges found";
            log << hint;
            return;
        }
        hint = "";


        auto vertices = which == 0 || which == 2?
            pgrouting::extract_vertices(edges)
            : std::vector<pgrouting::Basic_vertex>();

        UndirectedGraph undigraph = which < 11? UndirectedGraph(vertices) : UndirectedGraph();
        DirectedGraph digraph = which >= 11? DirectedGraph(vertices) : DirectedGraph();;


        switch (which) {
            case SLOAN: {
                            process(edges, undigraph, &sloanOrdering, return_count, return_tuples);
                            break;
                        }
            case CUTCHILL: {
                               process(edges, undigraph, &cuthillMckeeOrdering, return_count, return_tuples);
                               break;
                           }
            case KING: {
                           process(edges, undigraph, &kingOrdering, return_count, return_tuples);
                           break;
                       }
            case TOPOSORT: {
                               process(edges, digraph, &topologicalSort, return_count, return_tuples);
                               break;
                           }
            default:
                           break;
        }

        if (return_count == 0) {
            notice << "No results found";
            return;
        }
    } catch (AssertFailedException &except) {
        err << except.what();
    } catch (const std::pair<std::string, std::string>& ex) {
        err << ex.first;
        log << ex.second;
    } catch (const std::string &ex) {
        err << ex;
        log << hint;
    } catch (std::exception &except) {
        err << except.what();
    } catch (...) {
        err << "Caught unknown exception!";
    }
}
