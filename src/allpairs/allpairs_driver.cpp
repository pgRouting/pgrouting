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
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/assert.hpp"
#include "cpp_common/to_postgres.hpp"
#include "allpairs/allpairs.hpp"

void
do_allpairs(
        const std::string &edges_sql,
        bool directed,
        Which which,

        IID_t_rt* &return_tuples,
        size_t &return_count,
        std::ostringstream &log,
        std::ostringstream &err) {
    std::string hint = "";

    try {
        pgassert(!edges_sql.empty());

        using pgrouting::pgget::get_edges;
        using pgrouting::to_postgres::matrix_to_tuple;
        using pgrouting::UndirectedGraph;
        using pgrouting::DirectedGraph;

        using pgrouting::johnson;
        using pgrouting::floydWarshall;

        hint = edges_sql;
        auto edges = get_edges(edges_sql, true, true);

        if (edges.empty()) {
            err << "No edges found";
            log << edges_sql;
            return;
        }

        hint = "";

        if (directed) {
            DirectedGraph graph;
            graph.insert_edges(edges);

            if (which == JOHNSON) {
                auto matrix = johnson(graph);
                matrix_to_tuple(graph, matrix, return_count, return_tuples);
            } else {
                auto matrix = floydWarshall(graph);
                matrix_to_tuple(graph, matrix, return_count, return_tuples);
            }
        } else {
            UndirectedGraph graph;
            graph.insert_edges(edges);

            if (which == JOHNSON) {
                auto matrix = johnson(graph);
                matrix_to_tuple(graph, matrix, return_count, return_tuples);
            } else {
                auto matrix = floydWarshall(graph);
                matrix_to_tuple(graph, matrix, return_count, return_tuples);
            }
        }

        if (return_count == 0) {
            err << "No result generated, report this error\n";
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
