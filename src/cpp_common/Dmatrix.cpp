/*PGR-GNU*****************************************************************
File: Dmatrix.cpp

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

#include "cpp_common/Dmatrix.hpp"

#include <string>
#include <utility>
#include <sstream>
#include <algorithm>
#include <limits>
#include <vector>
#include <map>
#include <cmath>

#include "cpp_common/assert.hpp"
#include "c_types/iid_t_rt.h"


namespace pgrouting {
namespace tsp {

void
Dmatrix::set_ids(const std::vector < IID_t_rt > &data_costs) {
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
    for (const auto &i : ids) {
        if (i == id) return true;
    }
    return false;
}


/*! given a users id returns the internal index
 *
 * in[] id
 * returns index
 */
size_t
Dmatrix::get_index(int64_t id) const {
    for (size_t pos = 0; pos < ids.size(); ++pos) {
        if (ids[pos] == id) return pos;
    }
    throw std::make_pair(std::string("(INTERNAL) Dmatrix: Unable to find node on matrix"), id);
}

int64_t
Dmatrix::get_id(size_t id) const {
    return ids[id];
}

/*
 * Transforms the input data to a matrix
 */
Dmatrix::Dmatrix(const std::vector < IID_t_rt > &data_costs) {
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


double
get_distance(std::pair<double, double> p1 , std::pair<double, double> p2) {
    auto dx = p1.first - p2.first;
    auto dy = p1.second - p2.second;
    return std::sqrt(dx * dx + dy * dy);
}

/*
 * constructor for euclidean
 */
Dmatrix::Dmatrix(const std::map<std::pair<double, double>, int64_t> &euclidean_data) {
    ids.reserve(euclidean_data.size());
    for (const auto &e : euclidean_data) {
        ids.push_back(e.second);
    }
    costs.resize(
            ids.size(),
            std::vector<double>(
                ids.size(),
                (std::numeric_limits<double>::max)()));

    for (const auto &from : euclidean_data) {
        for (const auto &to : euclidean_data) {
            auto from_id = get_index(from.second);
            auto to_id = get_index(to.second);
            costs[from_id][to_id] = get_distance(from.first, to.first);
            costs[to_id][from_id] = costs[from_id][to_id];
        }
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
    for (const auto &row : matrix.costs) {
        size_t j = 0;
        for (const auto cost : row) {
            log << "Internal(" << i << "," << j << ")"
                << "\tUsers(" << matrix.ids[i] << "," << matrix.ids[j] << ")"
                << "\t = " << cost
                << "\n";
            ++j;
        }
        ++i;
    }
    return log;
}


}  // namespace tsp
}  // namespace pgrouting
