/*PGR-GNU*****************************************************************

FILE: euclideanDmatrix.cpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

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


#include "tsp/euclideanDmatrix.h"

#include <algorithm>
#include <vector>
#include <cmath>

#include "tsp/tour.h"
#include "cpp_common/pgr_assert.h"


namespace pgrouting {
namespace tsp {


const std::vector<double>
EuclideanDmatrix::get_row(size_t i) const {
    std::vector<double> result;

    for (size_t j = 0; j < ids.size(); ++j) {
        result.push_back(distance(i, j));
    }

    pgassert(result.size() == ids.size());
    return result;
}


double
EuclideanDmatrix::comparable_distance(size_t i, size_t j) const {
    if (special_distance >= 0 &&
            ((row == i && column == j)
             ||(row == j && column == i))) {
        return special_distance * special_distance;
    }
    auto dx = coordinates[i].x - coordinates[j].x;
    auto dy = coordinates[i].y - coordinates[j].y;
    return dx * dx + dy * dy;
}

double
EuclideanDmatrix::distance(size_t i, size_t j) const {
    if (special_distance >= 0 &&
            ((row == i && column == j)
             ||(row == j && column == i))) {
        return special_distance;
    }
    if (i == j) return 0;
    return std::sqrt(comparable_distance(i, j));
}

double
EuclideanDmatrix::tourCost(const Tour &tour) const {
    double total_cost(0);
    if (tour.cities.empty()) return total_cost;

    auto prev_id = tour.cities.front();
    for (const auto &id : tour.cities) {
        if (id == tour.cities.front()) continue;

        total_cost += distance(prev_id, id);
        prev_id = id;
    }
    total_cost += distance(prev_id, tour.cities.front());
    return total_cost;
}



bool
EuclideanDmatrix::has_id(int64_t id) const {
    auto pos = std::lower_bound(ids.begin(), ids.end(), id);
    return *pos == id;
}

size_t
EuclideanDmatrix::get_index(int64_t id) const {
    auto pos = std::lower_bound(ids.begin(), ids.end(), id);
    return pos - ids.begin();
}

int64_t
EuclideanDmatrix::get_id(size_t id) const {
    return ids[id];
}

/*
 * constructor
 */
EuclideanDmatrix::EuclideanDmatrix(
        const std::vector < Coordinate_t > &data_coordinates)
    : coordinates(data_coordinates) {
        set_ids();
        std::sort(coordinates.begin(), coordinates.end(),
                [](const Coordinate_t &lhs, const Coordinate_t &rhs)
                {return lhs.id < rhs.id;});
    }

void
EuclideanDmatrix::set_ids() {
    ids.reserve(coordinates.size());
    for (const auto &data : coordinates) {
        ids.push_back(data.id);
    }
    std::sort(ids.begin(), ids.end());
#ifndef NDEBUG
    auto total = ids.size();
#endif
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
    pgassertwm(total == ids.size(), "Duplicated id found");
}

bool
EuclideanDmatrix::has_no_infinity() const {
    return true;
}

bool
EuclideanDmatrix::obeys_triangle_inequality() const {
    return true;
}

bool
EuclideanDmatrix::is_symmetric() const {
    return true;
}

std::ostream& operator<<(std::ostream &log, const EuclideanDmatrix &matrix) {
    for (const auto id : matrix.ids) {
        log << "\t" << id;
    }
    log << "\n";
    for (const auto row : matrix.coordinates) {
        log << row.id << "(" << row.x << "," << row.y << ")\n";
    }
    return log;
}


}  // namespace tsp
}  // namespace pgrouting
