/*PGR-GNU*****************************************************************
File: pgdata_fetchers.hpp

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

#ifndef INCLUDE_CPP_COMMON_PGDATA_FETCHERS_HPP_
#define INCLUDE_CPP_COMMON_PGDATA_FETCHERS_HPP_

extern "C" {
#include <postgres.h>
#include <access/htup_details.h>
}

#include <vector>
using Edge_t = struct Edge_t;

namespace pgrouting {
using Column_info_t = struct Column_info_t;

void fetch_edge(
        const HeapTuple, const TupleDesc &,
        const std::vector<Column_info_t> &,
        int64_t*, Edge_t*, size_t*, bool);

}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_PGDATA_FETCHERS_HPP_
