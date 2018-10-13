/*PGR-GNU*****************************************************************

FILE: Dmatrix.cpp

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

#include "cpp_common/Dmatrix.h"

#include <string.h>
#include <sstream>
#include <algorithm>
#include <limits>
#include <vector>
#include <cmath>

#include "tsp/tour.h"
#include "cpp_common/pgr_assert.h"


namespace pgrouting {
namespace tsp {

double
Dmatrix::tourCost(const Tour &tour) const {
    double total_cost(0);
    if (tour.cities.empty()) return total_cost;

    auto prev_id = tour.cities.front();
    for (const auto &id : tour.cities) {
        if (id == tour.cities.front()) continue;

        pgassert(distance(prev_id, id) != (std::numeric_limits<double>::max)());

        total_cost += costs[prev_id][id];
        prev_id = id;
    }
    total_cost += costs[prev_id][tour.cities.front()];
    return total_cost;
}



void
Dmatrix::set_ids(const std::vector < Matrix_cell_t > &data_costs) {
    ids.reserve(data_costs.size() * 2);
    for (const auto &cost : data_costs) {
        ids.push_back(cost.from_vid);
        ids.push_back(cost.to_vid);
    }
    std::sort(ids.begin(), ids.end());
    ids.erase(std::unique(ids.begin(), ids.end()), ids.end());
    /*
     * freeing up unused space
     */
    ids.shrink_to_fit();
}

bool
Dmatrix::has_id(int64_t id) const {
    auto pos = std::lower_bound(ids.begin(), ids.end(), id);
    return *pos == id;
}


/*! given a users id returns the internal index
 *
 * in[] id
 * returns index
 */
size_t
Dmatrix::get_index(int64_t id) const {
    auto pos = std::lower_bound(ids.begin(), ids.end(), id);
    return pos - ids.begin();
}

int64_t
Dmatrix::get_id(size_t id) const {
    return ids[id];
}

/*
 * Transforms the input data to a matrix
 */
Dmatrix::Dmatrix(const std::vector < Matrix_cell_t > &data_costs) {
    set_ids(data_costs);
    costs.resize(
            ids.size(),
            std::vector<double>(
                ids.size(),
                (std::numeric_limits<double>::max)()));

    for (const auto &data : data_costs) {
        costs[get_index(data.from_vid)][get_index(data.to_vid)] = data.cost;
    }

    for (size_t i = 0; i < costs.size(); ++i) {
        costs[i][i] = 0;
    }
}

bool
Dmatrix::has_no_infinity() const {
    for (const auto &row : costs) {
        for (const auto &val : row) {
            if (val == (std::numeric_limits<double>::infinity)()) return false;
            if (val == (std::numeric_limits<double>::max)()) return false;
        }
    }
    return true;
}


/*!
 * Triangle Inequality Theorem.
 *  The sum of the lengths of any two sides of a triangle is greater than the length of the third side.
 *  NOTE: can also be equal for streets
 * costs[i][k] != inf
 * costs[i][k] <= costs[i][j] + costs[j][k]
 */
bool
Dmatrix::obeys_triangle_inequality() const {
    for (size_t i = 0; i < costs.size(); ++i) {
        for (size_t j = 0; j < costs.size(); ++j) {
            for (size_t k = 0; k < costs.size(); ++k) {
                if (!(costs[i][k] <= (costs[i][j] + costs[j][k]))) return false;
            }
        }
    }
    return true;
}

bool
Dmatrix::is_symmetric() const {
    for (size_t i = 0; i < costs.size(); ++i) {
        for (size_t j = 0; j < costs.size(); ++j) {
            if (0.000001 < std::fabs(costs[i][j] - costs[j][i])) {
                std::ostringstream log;
                log << "i \t" << i
                    << "j \t" << j
                    << "costs[i][j] \t" << costs[i][j]
                    << "costs[j][i] \t" << costs[j][i]
                    << "\n";
                log << (*this);
                pgassertwm(false, log.str());
                return false;
            }
        }
    }
    return true;
}


/**
 *
 */
std::ostream& operator<<(std::ostream &log, const Dmatrix &matrix) {
    for (const auto id : matrix.ids) {
        log << "\t" << id;
    }
    log << "\n";
    size_t i = 0;
    for (const auto row : matrix.costs) {
        size_t j = 0;
        for (const auto cost : row) {
            log << "Internal(" << i << "," << j << ")"
                << "\tUsers(" << matrix.ids[i] << "," << matrix.ids[j] << ")"
                << "\t = " << cost
#if 0
                << "\t(" << matrix.get_index(matrix.ids[i])
                << "," << matrix.get_index(matrix.ids[j]) << ")"
                << "\t = " << matrix.costs[i][j]
                << "\t = " << matrix.costs[j][i]
                << "=inf:"
                <<  (matrix.costs[i][j] ==
                        (std::numeric_limits<double>::infinity)())
                << "=inf:"
                <<  (matrix.costs[j][i] ==
                        (std::numeric_limits<double>::infinity)())
#endif
                << "\n";
            ++j;
        }
        ++i;
    }
#if 0
    for (size_t i = 0; i < matrix.costs.size(); ++i) {
        for (size_t j = 0; j < matrix.costs.size(); ++j) {
            for (size_t k = 0; k < matrix.costs.size(); ++k) {
                log << matrix.costs[i][k] << " <= ("
                    << matrix.costs[i][j] << " + "  << matrix.costs[j][k] << ")"
                    << (matrix.costs[i][k]
                            <= (matrix.costs[i][j] + matrix.costs[j][k]))
                    << "\n";
            }
        }
    }
#endif
    return log;
}


}  // namespace tsp
}  // namespace pgrouting
