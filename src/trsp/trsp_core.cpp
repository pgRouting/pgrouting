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

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif

#include "trsp/GraphDefinition.h"


int trsp_node_wrapper(
    edge_t *edges,
    size_t edge_count,
    restrict_t *restricts,
    size_t restrict_count,
    int64_t start_vertex,
    int64_t end_vertex,
    bool directed,
    bool has_reverse_cost,
    path_element_tt **path,
    size_t *path_count,
    char **err_msg
    ) {
    try {
        std::vector<PDVI> ruleTable;

        size_t i, j;
        ruleTable.clear();
        for (i = 0; i < restrict_count; i++) {
            std::vector<long> seq;
            seq.clear();
            seq.push_back(restricts[i].target_id);
            for (j = 0; j < MAX_RULE_LENGTH && restricts[i].via[j] > -1; j++) {
                seq.push_back(restricts[i].via[j]);
            }
            ruleTable.push_back(make_pair(restricts[i].to_cost, seq));
        }

        GraphDefinition gdef;
        int res = gdef.my_dijkstra(edges, edge_count, start_vertex, end_vertex,
            directed, has_reverse_cost, path, path_count, err_msg, ruleTable);


        if (res < 0)
            return res;
        else
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

int trsp_edge_wrapper(
    edge_t *edges,
    size_t edge_count,
    restrict_t *restricts,
    size_t restrict_count,
    int64_t start_edge,
    double start_pos,
    int64_t end_edge,
    double end_pos,
    bool directed,
    bool has_reverse_cost,
    path_element_tt **path,
    size_t *path_count,
    char **err_msg
    ) {
    try {
        std::vector<PDVI> ruleTable;

        size_t i, j;
        ruleTable.clear();
        for (i = 0; i < restrict_count; i++) {
            std::vector<long> seq;
            seq.clear();
            seq.push_back(restricts[i].target_id);
            for (j = 0; j < MAX_RULE_LENGTH && restricts[i].via[j] >- 1; j++) {
                seq.push_back(restricts[i].via[j]);
            }
            ruleTable.push_back(make_pair(restricts[i].to_cost, seq));
        }

        GraphDefinition gdef;
        auto res = gdef.my_dijkstra(edges, edge_count, start_edge, start_pos,
            end_edge, end_pos, directed, has_reverse_cost, path, path_count,
            err_msg, ruleTable);


        if (res < 0)
            return res;
        else
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
