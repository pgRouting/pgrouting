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
