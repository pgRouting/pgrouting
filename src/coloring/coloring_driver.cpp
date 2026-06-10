/*PGR-GNU*****************************************************************
File: coloring_driver.cpp

Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Design of one process & driver file by
Copyright (c) 2025 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#include "drivers/coloring_driver.hpp"

#include <sstream>
#include <deque>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

#include "c_types/ii_t_rt.h"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/to_postgres.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

#include "coloring/bipartite.hpp"
#include "coloring/edgeColoring.hpp"
#include "coloring/sequentialVertexColoring.hpp"
#include "components/components.hpp"
#include "components/makeConnected.hpp"

namespace pgrouting {
namespace drivers {

void do_coloring(
        const std::string &edges_sql,
        bool directed,

        Which which,

        II_t_rt* &return_tuples,
        size_t &return_count,
        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    std::string hint = "";
    return_tuples = nullptr;
    return_count = 0;

    try {
        if (edges_sql.empty()) {
            err << "Empty edges SQL";
            return;
        }

        using pgrouting::pgget::get_edges;
        using pgrouting::to_postgres::get_tuples;

        using pgrouting::DirectedGraph;
        using pgrouting::UndirectedGraph;

        using pgrouting::functions::edgeColoring;
        using pgrouting::functions::sequentialVertexColoring;
        using pgrouting::functions::pgr_bipartite;
        using pgrouting::algorithms::biconnectedComponents;
        using pgrouting::algorithms::connectedComponents;
        using pgrouting::algorithms::strongComponents;
        using pgrouting::functions::makeConnected;

        hint = edges_sql;
        auto edges = get_edges(edges_sql, true, false);

        if (edges.empty()) {
            notice << "No edges found";
            log << edges_sql;
            return;
        }

        hint = "";

        UndirectedGraph undigraph;
        DirectedGraph digraph;

        std::vector<II_t_rt> results;
        std::vector<std::vector<int64_t>> component_results;

        if (directed) {
            digraph.insert_edges(edges);
            switch (which) {
                case STRONGCOMPONENTS:
                    component_results = strongComponents(digraph);
                    break;
                default:
                    err << __FILE_NAME__ << ": Unknown function with name '" << get_name(which)
                        << "' for directed graph";
                    return;
            }
        } else {
            undigraph.insert_edges(edges);

            switch (which) {
                case EDGECOLORING:
                    results = edgeColoring(undigraph);
                    break;
                case BIPARTITE:
                    results = pgr_bipartite(undigraph);
                    break;
                case SEQUENTIAL:
                    results = sequentialVertexColoring(undigraph);
                    break;
                case BICONNECTEDCOMPONENTS:
                    component_results = biconnectedComponents(undigraph);
                    break;
                case MAKECONNECTED:
                    {
                        results = makeConnected(undigraph);
                        break;
                    }
                case CONNECTEDCOMPONENTS:
                    component_results = connectedComponents(undigraph);
                    break;
                default:
                    err << __FILE_NAME__ << ": Unknown function with name '" << get_name(which)
                        << "' for undirected graph";
                    return;
            }
        }

        if (!results.empty()) {
            return_count = get_tuples(results, return_tuples);
        } else if (!component_results.empty()) {
            return_count = get_tuples(component_results, return_tuples);
        }

        if (return_count == 0) {
            log << "No results found";
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

}  // namespace drivers
}  // namespace pgrouting
