/*PGR-GNU*****************************************************************
File: pgr_types.h

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky_vergara@hotmail.com

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

#ifndef SRC_COMMON_SRC_PGR_TYPES_H_
#define SRC_COMMON_SRC_PGR_TYPES_H_

#pragma once


#ifndef __cplusplus
#include "postgres.h"
#endif
#include <stdint.h>

typedef struct  {
    int64_t id;
    double x;
    double y;
} Coordinate_t;

typedef struct edge_astar {
    int id;
    int source;
    int target;
    double cost;
    double reverse_cost;
    double s_x;
    double s_y;
    double t_x;
    double t_y;
} edge_astar_t;

typedef struct {
    int64_t id;
    int64_t source;
    int64_t target;
    double cost;
    double reverse_cost;
    double x1;
    double y1;
    double x2;
    double y2;
} Pgr_edge_xy_t;


typedef struct {
    int64_t vertex_id;
    int64_t edge_id;
    double cost;
} path_element_t;

/*
 * This one is for processing
 */
typedef struct {
    int64_t node;
    int64_t edge;
    double cost;
    double agg_cost;
} Path_t;

/*
 * This ones are for returning the info to postgres
 */

typedef struct {
    int seq;
    int64_t start_id;
    int64_t end_id;
    int64_t node;
    int64_t edge;
    double cost;
    double agg_cost;
} General_path_element_t;


typedef struct {
    int route_id;
    int path_id;
    int path_seq;
    int64_t start_vid;
    int64_t end_vid;
    int64_t node;
    int64_t edge;
    double cost;
    double agg_cost;
    double route_agg_cost;
} Routes_t;


typedef struct {
  int64_t id;
  int64_t source;
  int64_t target;
  bool going;
  bool coming;
  int64_t edge_id;
} pgr_basic_edge_t;

typedef struct {
    int64_t id;
    int64_t source;
    int64_t target;
    double cost;
    double reverse_cost;
} pgr_edge_t;

typedef struct {
  int64_t edge;
  int64_t source;
  int64_t target;
  int64_t flow;
  int64_t residual_capacity;
} pgr_flow_t;

typedef struct {
    int seq;
    int64_t source;
    int64_t edge;
    double cost;
} pgr_path_t;

typedef struct matrix_cell {
    int64_t from_vid;
    int64_t to_vid;
    double cost;
} Matrix_cell_t;

// Restrictions used in pgr_turnRestrictions

#define  MAX_RULE_LENGTH 5
typedef struct {
    int64_t target_id;
    double to_cost;
    int64_t via[MAX_RULE_LENGTH];
}
Restrict_t;

typedef struct {
    int64_t pid;
    int64_t edge_id;
    char side;  // 'r', 'l', 'b' (default is both)
    double fraction;
    int64_t vertex_id;  // number is negative and is used for processing
} Point_on_edge_t;

// used for getting the data
typedef
enum {
    ANY_INTEGER,
    ANY_NUMERICAL,
    TEXT,
    CHAR1
} expectType;

typedef
struct {
    int colNumber;
    uint64_t type;
    bool strict;
    char *name;
    expectType eType;
} Column_info_t;

enum graphType {UNDIRECTED = 0, DIRECTED};

/**************************************************************************
 * pickDelivery types
 * ***********************************************************************/
typedef struct {
    int64_t id;
    double x;
    double y;
    double demand;
    double Etime;
    double Ltime;
    double Stime;
    int64_t Pindex;
    int64_t Dindex;
    double Ddist;
} Customer_t;

/*
    OUT seq INTEGER,        done in the .c code
    OUT vehicle_seq INTEGER,
    OUT vehicle_id INTEGER,
    OUT order_id BIGINT,
    OUT travelTime FLOAT,
    OUT arrivalTime FLOAT,
    OUT waitTime FLOAT,
    OUT serviceTime FLOAT,
    OUT departureTime FLOAT,
*/

typedef struct  {
    int vehicle_id;
    int vehicle_seq;
    int64_t order_id;
    double travelTime;
    double arrivalTime;
    double waitTime;
    double serviceTime;
    double departureTime;
} General_vehicle_orders_t;

/*************************************************************************/

#endif  // SRC_COMMON_SRC_PGR_TYPES_H_
