/*PGR-GNU*****************************************************************

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
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include <string.h>
#include <sstream>
#include "GraphDefinition.h"


int trsp_node_wrapper(
    edge_t *edges,         size_t edge_count,
    restrict_t *restricts, size_t restrict_count,
    int start_vertex,
    int end_vertex,
    bool directed,
    bool has_reverse_cost,
    path_element_t **path, size_t *path_count,
    char **err_msg) {

    std::ostringstream log;
    try {

        std::vector<PDVI> ruleTable;

        int j;
        ruleTable.clear();
        for (size_t i=0; i<restrict_count; i++) {
            std::vector<int64_t> seq;
            seq.clear();
            seq.push_back(restricts[i].target_id);
            for(j = 0; j<MAX_RULE_LENGTH && restricts[i].via[j]>-1; j++)
            {
                seq.push_back(restricts[i].via[j]);
            }
            ruleTable.push_back(make_pair(restricts[i].to_cost, seq));
        }

        GraphDefinition gdef(edges, static_cast<unsigned int>(edge_count), directed, has_reverse_cost);
        gdef.set_restrictions(start_vertex, end_vertex, ruleTable);
        int res = gdef.my_dijkstra(start_vertex, end_vertex, path, path_count, log);


        if (res < 0)
            return res;
        else
            return EXIT_SUCCESS;
    }
    catch(std::exception& e) {
        log << e.what();
        *err_msg = strdup(log.str().c_str());
        return -1;
    }
    catch(...) {
        log << "Caught unknown exception!";
        *err_msg = strdup(log.str().c_str());
        return -1;
    }
}

int trsp_edge_wrapper(
    edge_t *edges,          size_t edge_count,
    restrict_t *restricts,  size_t restrict_count,
    int start_edge,
    double start_pos,
    int end_edge,
    double end_pos,
    bool directed,
    bool has_reverse_cost,
    path_element_t **path, size_t *path_count,
    char **err_msg) {
    std::ostringstream log;
    try {

        std::vector<PDVI> ruleTable;

        int j;
        ruleTable.clear();
        for (size_t i=0; i<restrict_count; i++) {
            std::vector<int64_t> seq;
            seq.clear();
            seq.push_back(restricts[i].target_id);
            for(j = 0; j<MAX_RULE_LENGTH && restricts[i].via[j]>-1; j++)
            {
                seq.push_back(restricts[i].via[j]);
            }
            ruleTable.push_back(make_pair(restricts[i].to_cost, seq));
        }

        GraphDefinition gdef(edges, static_cast<unsigned int>(edge_count), directed, has_reverse_cost);

        int64_t start_vertex = 0;
        int64_t end_vertex = 0;
        gdef.add_virtual_vertices(
                start_edge, start_pos,
                end_edge, end_pos,
                start_vertex, end_vertex);

        gdef.set_restrictions(start_vertex, end_vertex, ruleTable);
        int res = gdef.my_dijkstra(start_vertex, end_vertex, path, path_count, log);


        if (res < 0)
            return res;
        else
            return EXIT_SUCCESS;
    }
    catch(std::exception& e) {
        log << e.what();
        *err_msg = strdup(log.str().c_str());
        return -1;
    }
    catch(...) {
        log << "Caught unknown exception!";
        *err_msg = strdup(log.str().c_str());
        return -1;
    }
}
