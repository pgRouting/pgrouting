#ifndef _VRP_H
#define _VRP_H

#include "postgres.h"


typedef struct vrp_vehicles 
{
	int id;
	int capacity;
	float8 costperkm;
} vrp_vehicles_t;


typedef struct vrp_orders 
{
	int id;
	int orderunit;
	int opentime;
	int closetime;
	int servicetime;
	
	float8 x;
	float8 y;
	
} vrp_orders_t;

typedef struct vrp_cost_element
{
	int src_id;
	int dest_id;
	int cost;
	int distance;
	int traveltime;
	
	int costtype; // 0 for depot-order, 1 for order-order, 2 for order-depot

}vrp_cost_element_t;


typedef struct vrp_result_element
{
	int order_id;
	int order_pos;
	int vehicle_id;
	int arrival_time;
	int depart_time;
} vrp_result_element_t;


#ifdef __cplusplus
extern "C"
#endif

int vrp_basic_wrapper(vrp_vehicles_t *vehicles, int vehicle_count, 
					  vrp_orders_t *orders, int order_count,
					  vrp_cost_element_t *costmatrix, int cost_count,
					  int depot_id,
					  vrp_result_element_t **result, int *result_count, char **err_msg);

#ifdef __cplusplus
extern "C"
#endif

#endif
