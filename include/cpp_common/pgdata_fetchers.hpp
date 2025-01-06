/*PGR-GNU*****************************************************************
File: pgdata_fetchers.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2024 Celia Virginia Vergara Castillo
- Return (C++) structure
Copyright (c) 2023 Celia Virginia Vergara Castillo
- cat into one file
Copyright (c) 2020 Mahmoud SAKR and Esteban ZIMANYI
mail: m_attia_sakrcw at yahoo.com, estebanzimanyicw at gmail.com
Copyright (c) 2016 Rohith Reddy
Copyright (c) 2016 Andrea Nardelli
mail: nrd.nardelli at gmail.com
Copyright (c) 2015 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev

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

#ifndef INCLUDE_CPP_COMMON_PGDATA_FETCHERS_HPP_
#define INCLUDE_CPP_COMMON_PGDATA_FETCHERS_HPP_
#pragma once

/* for HeapTuple, TupleDesc */
extern "C" {
#include <postgres.h>
#include <access/htup_details.h>
}

#include <vector>
#include <cstdint>

#include "cpp_common/undefPostgresDefine.hpp"

#include "cpp_common/info_t.hpp"
#include "c_types/ii_t_rt.h"
#include "cpp_common/coordinate_t.hpp"
#include "cpp_common/delauny_t.hpp"
#include "c_types/edge_bool_t_rt.h"
#include "cpp_common/costFlow_t.hpp"
#include "cpp_common/edge_xy_t.hpp"
#include "cpp_common/edge_t.hpp"
#include "c_types/iid_t_rt.h"
#include "cpp_common/orders_t.hpp"
#include "cpp_common/restriction_t.hpp"
#include "cpp_common/point_on_edge_t.hpp"
#include "cpp_common/vehicle_t.hpp"

namespace pgrouting {
namespace pgget {

II_t_rt fetch_combination(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

Coordinate_t fetch_coordinate(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

Delauny_t fetch_delauny(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

Edge_bool_t fetch_basic_edge(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

Edge_t fetch_edge(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

Edge_xy_t fetch_edge_xy(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

CostFlow_t fetch_costFlow_edge(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

IID_t_rt pgr_fetch_row(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

Orders_t fetch_orders(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

Restriction_t fetch_restriction(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

Point_on_edge_t fetch_point(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

Vehicle_t fetch_vehicle(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, size_t*, bool);

}  // namespace pgget
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_PGDATA_FETCHERS_HPP_
