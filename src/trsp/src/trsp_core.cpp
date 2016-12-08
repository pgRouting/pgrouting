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


#include "./GraphDefinition.h"
#include "./utils.h"


int trsp_node_wrapper(
    edge_t *edges,
    uint32_t edge_count,
    restrict_t *restricts,
    uint32_t restrict_count,
    long start_vertex,
    long end_vertex,
    bool directed,
    bool has_reverse_cost,
    path_element_t **path,
    uint32_t *path_count,
    char **err_msg
    )
{
    try {

        std::vector<PDVI> ruleTable;

        int j;
        ruleTable.clear();
        for (size_t i=0; i<restrict_count; i++) {
            std::vector<int> seq;
            seq.clear();
            seq.push_back(restricts[i].target_id);
            for(j = 0; j<MAX_RULE_LENGTH && restricts[i].via[j]>-1; j++)
            {
                seq.push_back(restricts[i].via[j]);
            }
            ruleTable.push_back(make_pair(restricts[i].to_cost, seq));
        }

        size_t count;
        GraphDefinition gdef;
        int res = gdef.my_dijkstra3(edges, edge_count, start_vertex, end_vertex, directed, has_reverse_cost, path, &count, err_msg, ruleTable);
        *path_count = static_cast<uint32_t> (count);


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
    uint32_t edge_count,
    restrict_t *restricts,
    uint32_t restrict_count,
    long start_edge,
    double start_pos,
    long end_edge,
    double end_pos,
    bool directed,
    bool has_reverse_cost,
    path_element_t **path,
    uint32_t *path_count,
    char **err_msg
    )
{
    try {

        std::vector<PDVI> ruleTable;

        int j;
        ruleTable.clear();
        for (size_t i=0; i<restrict_count; i++) {
            std::vector<int> seq;
            seq.clear();
            seq.push_back(restricts[i].target_id);
            for(j = 0; j<MAX_RULE_LENGTH && restricts[i].via[j]>-1; j++)
            {
                seq.push_back(restricts[i].via[j]);
            }
            ruleTable.push_back(make_pair(restricts[i].to_cost, seq));
        }

        GraphDefinition gdef;
        size_t count;
        int res = gdef.my_dijkstra4(edges, edge_count, start_edge, start_pos, end_edge, end_pos, directed, has_reverse_cost, path, &count, err_msg, ruleTable);
        *path_count = static_cast<uint32_t>(count);


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
