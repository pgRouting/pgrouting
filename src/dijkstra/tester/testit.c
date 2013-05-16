#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

typedef double float8;
typedef int bool;

typedef struct edge
{
    int id;
    int source;
    int target;
    float8 cost;
    float8 reverse_cost;
} edge_t;

typedef struct path_element
{
    int vertex_id;
    int edge_id;
    float8 cost;
} path_element_t;

#ifdef __cplusplus
extern "C"
#endif
int boost_dijkstra(edge_t *edges, unsigned int count, int start_vertex,
        int end_vertex, bool directed, bool has_reverse_cost,
        path_element_t **path, int *path_count, char **err_msg);

static edge_t edges[23] = {
    {134034453,4401484,4401467,1491.35562366006,0.0},
    {134034463,4401489,4401485,12.1046145920234,0.0},
    {134034458,4401487,4401477,177.374061187772,0.0},
    {134095663,4401465,4401476,2014.71852797927,0.0},
    {134034459,4401485,4401487,201.968135582698,0.0},
    {134095834,4401478,4401465,293.089423713387,0.0},
    {134034461,4401488,4401489,418.687894948968,0.0},
    {629678698,4415069,134860887,3776.78929640359,0.0},
    {134034456,4401477,4401481,491.242305990214,0.0},
    {134095832,4401482,4401487,76.1157555542275,0.0},
    {134034465,4401490,4401489,1956.98967514724,0.0},
    {134034454,4401483,4401486,1356.25190452873,0.0},
    {134034462,4401487,4401478,17.2205341642897,0.0},
    {134095833,4401477,4401478,2014.66722340654,0.0},
    {134034455,4401485,4401483,53.5613132396201,0.0},
    {134034467,4401488,4417647,2597.20106449741,0.0},
    {134034452,4401483,4401467,350.071683838508,0.0},
    {134034446,4401481,4401476,568.270689073724,0.0},
    {134072383,4416226,4401482,322.141177736713,0.0},
    {134034447,4401482,4401481,1522.8331095897,0.0},
    {134034466,4401486,4401490,610.880612548267,0.0},
    {134034468,4417647,4401486,507.803184036552,0.0},
    {134034464,4401490,4401485,149.914370088613,0.0}
};

int main(void) {
    int total_tuples = 23;
    int i;

    int ret;
    char *err_msg = NULL;

    int start_vertex = 4401489;
    int end_vertex   = 4401483;

    int v_min_id = LONG_MAX;
    int v_max_id = 0;

    int s_count = 0;
    int t_count = 0;

    path_element_t *path = NULL;
    int path_count = 0;

    for (i=0; i<total_tuples; i++) {
        if(edges[i].source < v_min_id) v_min_id = edges[i].source;
        if(edges[i].target < v_min_id) v_min_id = edges[i].target;
        if(edges[i].source > v_max_id) v_max_id = edges[i].source;
        if(edges[i].target > v_max_id) v_max_id = edges[i].target;
    }
    printf("%i <-> %i\n", v_min_id, v_max_id);

    for (i=0; i<total_tuples; i++) {
        if (edges[i].source == start_vertex || edges[i].target == start_vertex)
            s_count++;
        if (edges[i].source == end_vertex || edges[i].target == end_vertex)
            t_count++;
        edges[i].source -= v_min_id;
        edges[i].target -= v_min_id;
    }

    assert(s_count);
    assert(t_count);

    start_vertex -= v_min_id;
    end_vertex   -= v_min_id;

    ret = boost_dijkstra(edges, total_tuples, start_vertex, end_vertex,
                         0, 0, &path, &path_count, &err_msg);

    if (ret < 0) {
        printf("Error computing path: %s\n", err_msg);
        exit(1);
    }

    printf("   seq:  vertex_id,    edge_id,       cost\n");
    for (i=0; i<path_count; i++) {
        path[i].vertex_id += v_min_id;
        printf("%6d: %10d, %10d, %10.4f\n", i, path[i].vertex_id, path[i].edge_id, path[i].cost);
    }

    if (path) free(path);

    exit(0);
}
