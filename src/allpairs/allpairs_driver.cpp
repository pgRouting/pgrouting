/*PGR-GNU*****************************************************************
File: allpairs_driver.cpp

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

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

#include "drivers/allpairs_driver.hpp"

#include <sstream>
#include <deque>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

#include "c_types/iid_t_rt.h"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/to_postgres.hpp"

#include "allpairs/allpairs.hpp"
#include "metrics/betweennessCentrality.hpp"
#include "planar/planarFaces.hpp"

namespace pgrouting {
namespace drivers {

void
do_allpairs(
        const std::string &edges_sql,
        bool directed,

        Which which,

        IID_t_rt* &return_tuples,
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
        using pgrouting::to_postgres::matrix_to_tuple;
        using pgrouting::to_postgres::vector_to_tuple;
        using pgrouting::to_postgres::get_tuples;


        using pgrouting::DirectedGraph;
        using pgrouting::UndirectedGraph;

        using pgrouting::johnson;
        using pgrouting::floydWarshall;
        using pgrouting::functions::betweennessCentrality;
        using pgrouting::functions::planarFaces;

        hint = edges_sql;
        auto edges = get_edges(edges_sql, true, true);

        if (edges.empty()) {
            notice << "No edges found";
            log << edges_sql;
            return;
        }

        hint = "";

        UndirectedGraph undigraph;
        DirectedGraph digraph;

        if (directed) {
            digraph.insert_edges(edges);
            switch (which) {
                case JOHNSON:
                    matrix_to_tuple(digraph, johnson(digraph), return_count, return_tuples);
                    break;
                case FLOYD:
                    matrix_to_tuple(digraph, floydWarshall(digraph), return_count, return_tuples);
                    break;
                case BETWEENCENTRALITY:
                    vector_to_tuple(digraph, betweennessCentrality(digraph), return_count, return_tuples);
                    break;
                default:
                    err << "coloring_driver.cpp: Unknown function with name '" << get_name(which)
                        << "' for directed graph";
                    return;
            }
        } else {
            if (which == PLANARFACES) {
                undigraph.insert_cost1_edges(edges);
            } else {
                undigraph.insert_edges(edges);
            }

            switch (which) {
                case JOHNSON:
                    matrix_to_tuple(undigraph, johnson(undigraph), return_count, return_tuples);
                    break;
                case FLOYD:
                    matrix_to_tuple(undigraph, floydWarshall(undigraph), return_count, return_tuples);
                    break;
                case BETWEENCENTRALITY:
                    vector_to_tuple(undigraph, betweennessCentrality(undigraph), return_count, return_tuples);
                    break;
                case PLANARFACES:
                    return_count = get_tuples(planarFaces(undigraph), return_tuples);
                    break;
                default:
                    err << "coloring_driver.cpp: Unknown function with name '" << get_name(which)
                        << "' for undirected graph";
                    return;
            }
        }

        if (return_count == 0) {
            notice << "No result found\n";
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
