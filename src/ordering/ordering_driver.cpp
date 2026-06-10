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
#include "cpp_common/to_postgres.hpp"
#include "cpp_common/utilities.hpp"

#include "ordering/sloanOrdering.hpp"
#include "ordering/kingOrdering.hpp"
#include "ordering/cuthillMckeeOrdering.hpp"
#include "ordering/topologicalSort.hpp"
#include "components/components.hpp"


namespace pgrouting {
namespace drivers {

void
do_ordering(
        const std::string &edges_sql,
        bool directed,
        Which which,

        int64_t *&return_tuples,
        size_t &return_count,

        std::ostringstream &log,
        std::ostringstream &notice,
        std::ostringstream &err) {
    std::string hint = "";

    try {
        if (edges_sql.empty()) {
            err << "Empty edges SQL";
            return;
        }

        using pgrouting::pgget::get_edges;
        using pgrouting::to_postgres::get_vertexId;
        using pgrouting::to_postgres::get_identifiers;

        using pgrouting::UndirectedGraph;
        using pgrouting::DirectedGraph;

        using pgrouting::functions::sloanOrdering;
        using pgrouting::functions::kingOrdering;
        using pgrouting::functions::cuthillMckeeOrdering;
        using pgrouting::functions::topologicalSort;
        using pgrouting::algorithms::bridges;
        using pgrouting::algorithms::articulationPoints;


        hint = edges_sql;
        auto edges = get_edges(edges_sql, true, false);
        if (edges.empty()) {
            notice << "No edges found";
            log << hint;
            return;
        }
        hint = "";


        /*
         * Decide which algorithms needs extract vertices
         */
        auto vertices = which == SLOAN || which == KING?
            pgrouting::extract_vertices(edges)
            : std::vector<pgrouting::Basic_vertex>();

        /*
         * Initialize the graphs
         * No need to use extract_vertices on directed graph, because
         * SLOAN & KING are for undirected graph
         */
        UndirectedGraph undigraph = vertices.empty()? UndirectedGraph() : UndirectedGraph(vertices);
        DirectedGraph digraph = DirectedGraph();

        std::vector<typename UndirectedGraph::V> undi_results;
        std::vector<typename DirectedGraph::V> di_results;
        Identifiers<int64_t> id_results;

        if (directed) {
            digraph.insert_edges(edges);
            switch (which) {
                case TOPOSORT:
                    {
                        di_results = topologicalSort(digraph);
                        break;
                    }
                default:
                    err << __FILE_NAME__ << ": Unknown function with name '" << get_name(which)
                        << "' for undirected graph";
                    return;
            }

        } else {
            undigraph.insert_edges(edges);
            switch (which) {
                case SLOAN:
                    {
                        undi_results = sloanOrdering(undigraph);
                        break;
                    }
                case CUTCHILL:
                    {
                        undi_results = cuthillMckeeOrdering(undigraph);
                        break;
                    }
                case KING:
                    {
                        undi_results = kingOrdering(undigraph);
                        break;
                    }
                case ARTICULATIONPOINTS:
                    {
                        id_results = articulationPoints(undigraph);
                        break;
                    }
                case BRIDGES:
                    {
                        id_results = bridges(undigraph);
                        break;
                    }
                default:
                    err << __FILE_NAME__ << ": Unknown function with name '" << get_name(which)
                        << "' for undirected graph";
                    return;
            }
        }

        if (!undi_results.empty()) {
            get_vertexId(undigraph, undi_results, return_count, return_tuples);
        } else if (!di_results.empty()) {
            get_vertexId(digraph, di_results, return_count, return_tuples);
        } else if (!id_results.empty()) {
            return_count = get_identifiers(id_results, return_tuples);
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

}  // namespace drivers
}  // namespace pgrouting
