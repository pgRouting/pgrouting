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

/*! @file */

#ifndef INCLUDE_CPP_COMMON_DMATRIX_H_
#define INCLUDE_CPP_COMMON_DMATRIX_H_
#pragma once

#include <iostream>
#include <vector>

#include "c_types/matrix_cell_t.h"

namespace pgrouting {
namespace tsp {

class Tour;  // for tourCost

class Dmatrix {
 public:
    Dmatrix() = default;
    explicit Dmatrix(const std::vector < Matrix_cell_t > &data_costs);

    bool has_no_infinity() const;
    bool obeys_triangle_inequality() const;
    bool is_symmetric() const;

    /*! @brief sets a special value for the distance(i,j)
     *
     * @param [in] i - index in matrix
     * @param [in] j - index in matrix
     * @param [in] dist - distance from i to j & from j to i
     *
     */
    void set(size_t i, size_t j, double dist) {
        costs[i][j] = costs[j][i] = dist;}

    /*! @brief original id -> true
     *
     * @param [in] id - original id
     * @returns true if id is in the distance table
     */
    bool has_id(int64_t id) const;

    /*! @brief original id -> idx
     *
     * @param [in] id - original id
     * @returns idx index of the id in the distance table
     */
    size_t get_index(int64_t id) const;

    /*! @brief idx -> original id
     *
     * @param [in] idx - index (i-th coordinate)
     * @returns the original id corresponding to idx
     */
    int64_t get_id(size_t idx) const;

    /*! @brief |idx|
     *
     * @returns the total number of coordinates
     */
    size_t size() const {return ids.size();}

    /*! @brief tour evaluation
     *
     * @param [in] tour
     * @returns total cost of traversing the tour
     */
    double tourCost(const Tour &tour) const;

    /*! @brief returns a row of distances
     *
     * @param [in] idx - row index
     * @returns distances from idx to all other coordinates
     */
    const std::vector<double>& get_row(size_t idx) const {
        return costs[idx];}

    double comparable_distance(size_t i, size_t j) const {
        return distance(i, j);}

    double distance(int64_t i, int64_t j) const {
        return distance(get_index(i), get_index(j));}

    double distance(size_t i, size_t j) const {
        return costs[i][j];}

    friend std::ostream& operator<<(
            std::ostream &log,
            const Dmatrix &matrix);

    double empty() const {
        return ids.empty();
    }

 protected:
    void set_ids(const std::vector<matrix_cell> &data_costs);
    std::vector<int64_t> ids;

 private:
    typedef std::vector < std::vector < double > > Costs;
    Costs costs;
    std::vector< double >& operator[] (size_t i) {return costs[i];}
    const std::vector< double >& operator[] (size_t i) const {return costs[i];}
};

}  // namespace tsp
}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_DMATRIX_H_
