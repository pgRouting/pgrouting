/*PGR-GNU*****************************************************************
File: trsp_pgfetch.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2023 Celia Virginia Vergara Castillo
mail: vicky at erosion.dev
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
/** @file
This is a copy of pgdata_fetchers.hpp

The old version of TRSP
- Is the only function that has its own edge reader within its code,
  hence the name of the file.
- The old version of TRSP will exist during 3.x series


@note
- The only function that should remain after all functions read the data on the
  C++ code is `fetch_edge`
- The code to be removed is marked with "if 0 ... endif" directives
- The old version of TRSP will be removed on V4

 @todo: Remove marked code
 @todo: V4 Delete this file
*/

#ifndef INCLUDE_CPP_COMMON_TRSP_PGFETCH_HPP_
#define INCLUDE_CPP_COMMON_TRSP_PGFETCH_HPP_

/* for HeapTuple, TupleDesc */
extern "C" {
#include <postgres.h>
#include <access/htup_details.h>
}

#include <vector>
#include "cpp_common/undefPostgresDefine.hpp"

#if 0
using II_t_rt = struct II_t_rt;
using IID_t_rt = struct IID_t_rt;
using Coordinate_t = struct Coordinate_t;
using Delauny_t = struct Delauny_t;
using Edge_bool_t = struct Edge_bool_t;
#endif
using Edge_t = struct Edge_t;
#if 0
using CostFlow_t = struct CostFlow_t;
using Edge_xy_t = struct Edge_xy_t;
using Orders_t = struct Orders_t;
using Restriction_t = struct Restriction_t;
using Point_on_edge_t = struct Point_on_edge_t;
using Vehicle_t = struct Vehicle_t;
#endif

namespace pgrouting {
using Column_info_t = struct Column_info_t;

#if 0
void fetch_combination(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, II_t_rt*, size_t*, bool);

void fetch_coordinate(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Coordinate_t*, size_t*, bool);

void fetch_delauny(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Delauny_t*, size_t*, bool);

void fetch_basic_edge(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Edge_bool_t*, size_t*, bool);
#endif

void fetch_edge(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Edge_t*, size_t*, bool);

#if 0
void fetch_costFlow_edge(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, CostFlow_t*, size_t*, bool);

void fetch_edge_with_xy(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Edge_xy_t*, size_t*, bool);

void pgr_fetch_row(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, IID_t_rt*, size_t*, bool);

void fetch_orders(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Orders_t*, size_t*, bool);

void fetch_restriction(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Restriction_t*, size_t*, bool);

void fetch_point(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Point_on_edge_t*, size_t*, bool);

void fetch_vehicle(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Vehicle_t*, size_t*, bool);
#endif

}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_TRSP_PGFETCH_HPP_
