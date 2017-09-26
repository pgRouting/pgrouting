/*PGR-GNU*****************************************************************

File: trsp_core.cpp

Generated with Template by:
Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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


#include "drivers/trsp/trsp_driver.h"
#include <utility>
#include <vector>
#include <cstdint>
#include <sstream>
#include "trsp/pgr_trspHandler.h"
#include "cpp_common/pgr_assert.h"
#include "cpp_common/pgr_alloc.hpp"



int do_trsp(
        pgr_edge_t *edges,
        size_t total_edges,

        Restriction_t *restrictions,
        size_t restrictions_size,

        int64_t start_vertex,
        int64_t end_vertex,

        bool directed,

        General_path_element_t **return_tuples,
        size_t *return_count,
        char **err_msg) {
    std::ostringstream err;
    try {
        pgassert(*return_tuples == NULL);
        pgassert(*return_count == 0);

        typedef std::pair<double, std::vector<int64_t> > PDVI;


        std::vector<pgrouting::trsp::PDVI> ruleTable;
        for (size_t i = 0; i < restrictions_size; ++ i) {
            std::vector<int64_t> vias(restrictions[i].via, restrictions[i].via + restrictions[i].via_size);
            std::rotate(vias.begin(), vias.begin() + 1, vias.end()); 
            PDVI rule = std::make_pair(restrictions[i].cost, vias);
            ruleTable.push_back(rule);
        }


        pgrouting::trsp::Pgr_trspHandler gdef;
        std::deque<Path> paths;

        paths.push_back(gdef.initializeAndProcess(
                edges,
                total_edges,
                ruleTable,
                start_vertex,
                end_vertex,
                directed,
                err_msg));


        size_t count(0);
        count = count_tuples(paths);

        if (count == 0) {
            (*return_tuples) = NULL;
            (*return_count) = 0;
            err << "No paths found";
            *err_msg = pgr_msg(err.str().c_str());
            return -1;
        }

        (*return_tuples) = pgr_alloc(count, (*return_tuples));
        (*return_count) = collapse_paths(return_tuples, paths);


        return EXIT_SUCCESS;
    }
    catch(std::exception& e) {
        *err_msg = (char *) e.what();
        return -1;
    }
    catch(...) {
        *err_msg = (char *) "Caught unknown exception!";
        return -1;
    }
}

