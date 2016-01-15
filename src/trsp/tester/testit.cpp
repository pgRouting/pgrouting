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
#include <stdio.h>
#include <vector>

#include "GraphDefinition.h"
#include "utils.h"

#include "issue191-bad-m1.h"
//#include "issue191-bad.h"
//#include "issue191-good.h"

int main(void) {
    path_element_t *path;
    int path_count;
    char *err_msg;

    std::vector<PDVI> ruleTable;
    ruleTable.clear();

    GraphDefinition gdef;
    int res = gdef.my_dijkstra(edges, NEDGE, 2963487, 0.5, 3345813, 0.5, &path, &path_count, &err_msg, ruleTable);

    if (res < 0) {
        printf("error: res: %d\n", res);
        printf("%s\n", err_msg);
        return 1;
    }

    printf("Path count: %d\n", path_count);

    float8 tcost = 0.0;
    for (int i=0; i<path_count; i++) {
        printf("%4d: %7d, %7d, %.4f\n", i+1, path[i].vertex_id, path[i].edge_id, path[i].cost);
        tcost += path[i].cost;
    }
    printf("\nTotal cost = %.4f\n", tcost);

    return 0;
}
