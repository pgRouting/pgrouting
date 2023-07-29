/*PGR-GNU*****************************************************************
File: pgdata_fetchers.cpp

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

#include "cpp_common/pgdata_fetchers.hpp"

#include <vector>
#include <cfloat>
#include <cmath>

#include "cpp_common/get_check_data.hpp"

#include "c_types/info_t.hpp"
#include "c_types/edge_t.h"


namespace pgrouting {

void fetch_edge(
        const HeapTuple tuple,
        const TupleDesc &tupdesc,
        const std::vector<Column_info_t> &info,
        int64_t *default_id,
        Edge_t *edge,
        size_t *valid_edges,
        bool normal) {
    if (pgrouting::column_found(info[0].colNumber)) {
        edge->id = pgrouting::getBigInt(tuple, tupdesc, info[0]);
    } else {
        edge->id = *default_id;
        ++(*default_id);
    }

    if (normal) {
        edge->source = pgrouting::getBigInt(tuple, tupdesc,  info[1]);
        edge->target = pgrouting::getBigInt(tuple, tupdesc, info[2]);
    } else {
        edge->target = pgrouting::getBigInt(tuple, tupdesc,  info[1]);
        edge->source = pgrouting::getBigInt(tuple, tupdesc, info[2]);
    }

    edge->cost = pgrouting::getFloat8(tuple, tupdesc, info[3]);

    if (pgrouting::column_found(info[4].colNumber)) {
        edge->reverse_cost = pgrouting::getFloat8(tuple, tupdesc, info[4]);
    } else {
        edge->reverse_cost = -1;
    }

    edge->cost = std::isinf(edge->cost)?
        DBL_MAX : edge->cost;

    edge->reverse_cost = std::isinf(edge->reverse_cost)?
        DBL_MAX : edge->reverse_cost;

    *valid_edges = edge->cost < 0? *valid_edges: *valid_edges + 1;
    *valid_edges = edge->reverse_cost < 0? *valid_edges: *valid_edges + 1;
}

}  // namespace pgrouting
