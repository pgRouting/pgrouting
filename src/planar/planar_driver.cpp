/*PGR-GNU*****************************************************************
File: planar_driver.cpp

Copyright (c) 2026-2026 pgRouting developers
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

#include "drivers/planar_driver.hpp"

#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <cstdint>

#include "c_types/iid_t_rt.h"
#include "cpp_common/base_graph.hpp"
#include "cpp_common/pgdata_getters.hpp"
#include "cpp_common/utilities.hpp"
#include "cpp_common/alloc.hpp"
#include "cpp_common/assert.hpp"

#include "planar/planarFaces.hpp"

namespace pgrouting {
namespace drivers {

void do_planar(
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
        using pgrouting::UndirectedGraph;

        hint = edges_sql;
        auto edges = get_edges(edges_sql, true, true);

        if (edges.empty()) {
            notice << "No edges found";
            log << edges_sql;
            return;
        }

        hint = "";

        /* the planar family works on the undirected structure only */
        (void)directed;

        UndirectedGraph undigraph;
        undigraph.insert_edges(edges);

        std::vector<IID_t_rt> results;

        switch (which) {
            case PLANARFACES:
                {
                    pgrouting::functions::Pgr_planarFaces<UndirectedGraph> fn;
                    results = fn.planarFaces(undigraph);
                    log << fn.get_log();
                }
                break;
            default:
                err << "planar_driver.cpp: Unknown function with name '" << get_name(which)
                    << "' for undirected graph";
                return;
        }

        auto count = results.size();

        if (count == 0) {
            notice << "No results found";
            return;
        }

        return_tuples = pgr_alloc(count, return_tuples);
        for (size_t i = 0; i < count; ++i) {
            return_tuples[i] = results[i];
        }
        return_count = count;
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
