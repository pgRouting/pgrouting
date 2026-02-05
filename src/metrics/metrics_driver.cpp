/*PGR-GNU*****************************************************************
File: metrics_driver.cpp

Generated with Template by:
Copyright (c) 2015-2026 pgRouting developers
Mail: project@pgrouting.org

Developer:
Copyright (c) 2025 Saloni Kumari
Mail: chaudharysaloni2510 at gmail.com

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

#include "drivers/metrics_driver.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

#include "cpp_common/pgdata_getters.hpp"

#include "metrics/bandwidth.hpp"

namespace pgrouting {
namespace drivers {

uint64_t
do_metrics(
        const std::string &edges_sql,
        Which which,

        std::ostringstream &log,
        std::ostringstream &err) {
    std::string hint;

    try {
        if (edges_sql.empty()) {
            err << "Empty edges SQL";
            return 0;
        }

        using pgrouting::pgget::get_edges;
        using pgrouting::UndirectedGraph;

        hint = edges_sql;
        auto edges = get_edges(edges_sql, true, true);

        if (edges.empty()) {
            err << "No edges found";
            log << edges_sql;
            return 0;
        }

        hint = "";

        auto vertices(pgrouting::extract_vertices(edges));
        UndirectedGraph undigraph(vertices);

        undigraph.insert_edges(edges);

        uint64_t result = 0;

        switch (which) {
            case BANDWIDTH:
                {
                    result = pgrouting::metrics::bandwidth(undigraph);
                    break;
                }
            default:
                break;
        }

        return result;
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
    return 0;
}

}  // namespace drivers
}  // namespace pgrouting
