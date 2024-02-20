/*PGR-GNU*****************************************************************
File: orders_t.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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
/*! @file */

#ifndef INCLUDE_CPP_COMMON_PICKDELIVER_ORDERS_T_HPP_
#define INCLUDE_CPP_COMMON_PICKDELIVER_ORDERS_T_HPP_
#pragma once

#include <cstdint>

struct Orders_t {
    int64_t id;
    double demand;

    double pick_x;
    double pick_y;
    int64_t pick_node_id;

    double pick_open_t;
    double pick_close_t;
    double pick_service_t;

    double deliver_x;
    double deliver_y;
    int64_t deliver_node_id;

    double deliver_open_t;
    double deliver_close_t;
    double deliver_service_t;
};

#endif  // INCLUDE_CPP_COMMON_PICKDELIVER_ORDERS_T_HPP_
