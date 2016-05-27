/*PGR-GNU*****************************************************************

FILE: Dmatrix.h

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

#pragma once

#include <iostream>
#include <vector>

#include "../../common/src/pgr_types.h"

namespace pgRouting {
namespace tsp {

class Tour;  // for tourCost

class Dmatrix {
 public:
    typedef std::vector < std::vector < double > > Costs;

    Dmatrix() = default;
    explicit Dmatrix(const std::vector < Matrix_cell_t > &data_costs);

    bool has_no_infinity() const;
    bool obeys_triangle_inequality() const;
    bool is_symetric() const;

    void set(size_t i, size_t j, double dist) {
        costs[i][j] = costs[j][i] = dist;}

    size_t get_index(int64_t id) const;
    int64_t get_id(size_t id) const;
    size_t size() const {return ids.size();}

    double tourCost(const Tour &tour) const;

    const std::vector<double>& get_row(size_t i) const {
        return costs[i];}

    double comparable_distance(size_t i, size_t j) const {
        return distance(i, j);}

    double distance(size_t i, size_t j) const {
        return costs[i][j];}

    friend std::ostream& operator<<(
            std::ostream &log,
            const Dmatrix &matrix);

 protected:
    void set_ids(const std::vector<matrix_cell> &data_costs);
    std::vector<size_t> ids;

 private:
    Costs costs;
    std::vector< double >& operator[] (size_t i) {return costs[i];}
    const std::vector< double >& operator[] (size_t i) const {return costs[i];}
};

}  // namespace tsp
}  // namespace pgRouting
