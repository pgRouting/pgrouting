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

#include "cpp_common/base_graph.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/to_postgres.hpp"
#include "cpp_common/undirectedNoCostBG.hpp"

#include "ordering/sloanOrdering.hpp"
#include "ordering/kingOrdering.hpp"
#include "ordering/cuthillMckeeOrdering.hpp"
#include "ordering/topologicalSort.hpp"
#include "components/components.hpp"
#include "max_flow/maximumcardinalitymatching.hpp"
#include "max_flow/maxWeightedMatching.hpp"

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
    return_tuples = nullptr;
    return_count = 0;

    try {
        if (edges_sql.empty()) {
            err << "Empty edges SQL";
            return;
        }

        using pgrouting::pgget::get_edges;
        using pgrouting::pgget::get_basic_edges;
        using pgrouting::to_postgres::get_vertexId;
        using pgrouting::to_postgres::get_identifiers;


        using pgrouting::DirectedGraph;
        using pgrouting::UndirectedGraph;
        using pgrouting::graph::UndirectedNoCostsBG;
        using pgrouting::graph::UndirectedHasCostBG;

        using pgrouting::functions::sloanOrdering;
        using pgrouting::functions::kingOrdering;
        using pgrouting::functions::cuthillMckeeOrdering;
        using pgrouting::functions::topologicalSort;
        using pgrouting::algorithms::bridges;
        using pgrouting::algorithms::articulationPoints;
        using pgrouting::flow::maxCardinalityMatch;
        using pgrouting::functions::maxWeightedMatch;

        hint = edges_sql;
        auto bedges = (which == MAXCARDINALITYMATCH)? get_basic_edges(edges_sql) : std::vector<Edge_bool_t>();
        auto edges  = (which != MAXCARDINALITYMATCH)? get_edges(edges_sql, true, false) : std::vector<Edge_t>();
        auto no_edges = (which == MAXCARDINALITYMATCH)? bedges.empty() : edges.empty();

        if (no_edges) {
            notice << "No edges found";
            log << edges_sql;
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
        DirectedGraph digraph;
        UndirectedHasCostBG wgraph;
        UndirectedNoCostsBG bgraph = (which == MAXCARDINALITYMATCH)? UndirectedNoCostsBG(bedges) :  UndirectedNoCostsBG(std::vector<Edge_bool_t>());

        std::vector<typename UndirectedGraph::V> undi_results;
        std::vector<typename DirectedGraph::V> di_results;

        if (directed) {
            digraph.insert_edges(edges);
            switch (which) {
                case TOPOSORT:
                    get_vertexId(digraph, topologicalSort(digraph), return_count, return_tuples);
                    break;
                default:
                    err << "ordering_driver.cpp: Unknown function with name '" << get_name(which)
                        << "' for directed graph";
                    return;
            }
        } else {
            if (which == MAXWEIGHTMATCH) {
                wgraph.insert_maxCost_edge_no_parallel_no_loop(edges);
            } else if (which != MAXCARDINALITYMATCH) {
                undigraph.insert_edges(edges);
            }

            switch (which) {
                case SLOAN:
                    get_vertexId(undigraph, sloanOrdering(undigraph), return_count, return_tuples);
                    break;
                case CUTCHILL:
                    get_vertexId(undigraph, cuthillMckeeOrdering(undigraph), return_count, return_tuples);
                    break;
                case KING:
                    get_vertexId(undigraph, kingOrdering(undigraph), return_count, return_tuples);
                    break;
                case ARTICULATIONPOINTS:
                    return_count = get_identifiers(articulationPoints(undigraph), return_tuples);
                    break;
                case BRIDGES:
                    return_count = get_identifiers(bridges(undigraph), return_tuples);
                    break;
                case MAXCARDINALITYMATCH:
                    return_count = get_identifiers(maxCardinalityMatch(bgraph), return_tuples);
                    break;
                case MAXWEIGHTMATCH:
                    return_count = get_identifiers(maxWeightedMatch(wgraph), return_tuples);
                    break;
                default:
                    err << "ordering_driver.cpp: Unknown function with name '" << get_name(which)
                        << "' for undirected graph";
                    return;
            }
        }

        if (return_count == 0) {
            notice << "No results found";
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
