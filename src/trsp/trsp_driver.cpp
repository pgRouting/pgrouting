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

        restrict_t *restricts,
        size_t restrict_count,

        int64_t start_vertex,
        int64_t end_vertex,

        bool directed,
        bool has_reverse_cost,

        path_element_tt **path,
        size_t *path_count,
        char **err_msg) {
    std::ostringstream err;
    try {
        pgassert(*path == NULL);
        pgassert(*path_count == 0);


        std::vector<pgrouting::trsp::PDVI> ruleTable;
        size_t MAX_RULE_LENGTH = 5;

        size_t i, j;
        ruleTable.clear();
        for (i = 0; i < restrict_count; i++) {
            std::vector<int64_t> seq;
            seq.clear();
            seq.push_back(restricts[i].target_id);
            for (j = 0; j < MAX_RULE_LENGTH && restricts[i].via[j] > -1; j++) {
                seq.push_back(restricts[i].via[j]);
            }
            ruleTable.push_back(make_pair(restricts[i].to_cost, seq));
        }

        pgrouting::trsp::Pgr_trspHandler gdef;
        auto res = gdef.initializeAndProcess(
                edges, total_edges,
                ruleTable,
                start_vertex, end_vertex,
                directed, has_reverse_cost,
                err_msg);

        path_element_tt *return_tuples = nullptr;
        return_tuples = pgr_alloc(res.size(), (return_tuples));
        i = 0;
        for (const auto r : res) {
            return_tuples[i] = r;
            ++i;
        }
        *path = return_tuples;
        *path_count = res.size();

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

