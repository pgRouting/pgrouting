#pragma once

#ifdef __cplusplus
extern "C" {
#endif
#include "transit_data.h"

int compute_scheduled_route(char *gtfs_schema, int source_stop_id,
    int destination_stop_id, time_t query_time, gtfs_path_element_t **path,
    int *path_count);
#ifdef __cplusplus
}
#endif
