/*PGR-GNU*****************************************************************

Copyright (c) 2013 Khondoker Md. Razequl Islam
ziboncsedu@gmail.com

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

#ifndef SRC_VRP_BASIC_SRC_VRP_H_
#define SRC_VRP_BASIC_SRC_VRP_H_

#include <stddef.h>

typedef struct vrp_vehicles {
    int id;
    int capacity;
} vrp_vehicles_t;


typedef struct vrp_orders {
    int id;
    int order_unit;
    int open_time;
    int close_time;
    int service_time;

    double x;
    double y;
} vrp_orders_t;

typedef struct vrp_cost_element {
    int src_id;
    int dest_id;
    double cost;
    double distance;
    double traveltime;
} vrp_cost_element_t;


typedef struct vrp_result_element {
    int order_id;
    int order_pos;
    int vehicle_id;
    int arrival_time;
    int depart_time;
} vrp_result_element_t;


#ifdef __cplusplus
extern "C"
#endif

int find_vrp_solution(vrp_vehicles_t *vehicles, size_t vehicle_count,
        vrp_orders_t *orders, size_t order_count,
        vrp_cost_element_t *costmatrix, size_t cost_count,
        int depot_id,
        vrp_result_element_t **result, size_t *result_count, char **err_msg);

#ifdef __cplusplus
extern "C"
#endif

#endif  // SRC_VRP_BASIC_SRC_VRP_H_
