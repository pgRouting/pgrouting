/*
 * Dial-a-Ride Problem solution algorithm for PostgreSQL
 *
 * Copyright (c) 2010 Anton A. Patrushev, Georepublic
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
 
#define _DARP_H
#define MAX_ORDERS 200
#define MAX_VEHICLES 100

#include "postgres.h"
#include "dijkstra.h"
#include "utils/date.h" //for TimeTzADT
#include "utils/nabstime.h" //for TimeInterval

typedef struct vehicle
{
  int id;
  int vehicle_id;
  float8 capacity1;
  float8 capacity2;
  float8 capacity3;
} vehicle_t;

typedef struct order
{
  int id;
  int order_id;
  int puTime;
  Interval* puLT;
  Interval* puUT;
  int doTime;
  Interval* doLT;
  Interval* doUT;
  float8 size1;
  float8 size2;
  float8 size3;
  //float8 fromX;
  //float8 fromY;
  //float8 toX;
  //float8 toY;
  int from;
  int to;
} order_t;

typedef struct itinerary
{
  int id;
  int vehicle;
  int order;
  //bool isPickUp;
  int point;
  double time;
} itinerary_t;

#ifdef __cplusplus
extern "C"
{
#endif
  int find_darp_solution(int order_num, int vehicle_num,
			vehicle_t *vehicles,
			order_t *orders,
			// Depot is included twice.
			itinerary_t *itinerary,
			// Here goes a kind of hack.
			// Each order has two points - pick-up and drop-off,
			// so we need to store distances between them and
			// reference them back to an order somehow using
			// arrays only.
			// Depot is also included.
			// Ugly but simple solution.
			double *dist,
			int depot,
			int *penalties,
			float* fit, char** err_msg);
#ifdef __cplusplus
}
#endif
