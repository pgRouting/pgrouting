/*PGR-GNU*****************************************************************
File: orders_input.h

Copyright (c) 2016 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#ifndef SRC_COMMON_SRC_ORDERS_INPUT_H_
#define SRC_COMMON_SRC_ORDERS_INPUT_H_
#pragma once

#include "./pgr_types.h"

/** @brief Reads the pick-Deliver orders
 *
 * @param[in] pd_orders_sql
 * @param[in] default_pick_window_t
 * @param[in] default_deliver_window_t
 * @param[in] default_pick_service_t
 * @param[in] default_deliver_service_t
 * @param[out] pd_orders
 * @param[out] total_pd_orders
 */
void
pgr_get_pd_orders(
        char *pd_orders_sql,
        double default_pick_window_t,
        double default_deliver_window_t,
        double default_pick_service_t,
        double default_deliver_service_t,
        PickDeliveryOrders_t **pd_orders,
        size_t *total_pd_orders);

#endif  // SRC_COMMON_SRC_ORDERS_INPUT_H_
