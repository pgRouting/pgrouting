/*PGR-GNU*****************************************************************

FILE: eucledianDmatrix.h

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

class eucledianDmatrix {
 public:
    eucledianDmatrix() = default;
    explicit eucledianDmatrix(
            const std::vector< Coordinate_t > &data_coordinates);

    bool has_no_infinity() const;
    bool obeys_triangle_inequality() const;
    bool is_symetric() const;

    /*! @brief sets a special value for the distance(i,j)
     *
     * @params [IN] i - index in matrix
     * @params [IN] j - index in matrix
     * @params [IN] dist - distance from i to j & from j to i
     *
     */
    void set(size_t i, size_t j, double dist) {
        row = i; column = j; special_distance = dist;}

    /*! @brief original id -> true
     *
     * @params [IN] id - original id
     * @returns true if id is in the distance table
     */
    bool has_id(int64_t id) const;

    /*! @brief original id -> idx
     *
     * @params [IN] id - original id
     * @returns idx index of the id in the distance table
     */
    size_t get_index(int64_t id) const;

    /*! @brief idx -> original id
     *
     * @params [IN] idx - index (i-th coordinate)
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
     * @params [IN] tour
     * @returns total cost of traversing the tour
     */
    double tourCost(const Tour &tour) const;

    /*! @brief returns a row of distances
     *
     * @params [IN] idx - row index
     * @returns distances from idx to all other coordinates
     */
    const std::vector<double> get_row(size_t idx) const;

    double comparable_distance(size_t i, size_t j) const;
    double distance(size_t i, size_t j) const;

    friend std::ostream& operator<<(
            std::ostream &log,
            const eucledianDmatrix &matrix);

 protected:
    void set_ids();
    std::vector<size_t> ids;

 private:
    std::vector < Coordinate_t >  coordinates;
    size_t row;
    size_t column;
    double special_distance;
};

}  // namespace tsp
}  // namespace pgRouting
