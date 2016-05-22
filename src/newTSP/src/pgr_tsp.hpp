/*PGR-GNU*****************************************************************
 * File: tsp_driver.cpp
 *
 * Generated with Template by:
 * Copyright (c) 2015 pgRouting developers
 * Mail: project@pgrouting.org
 *
 * Function's developer:
 * Copyright (c) 2015 Celia Virginia Vergara Castillo
 * Mail: vicky_vergara@hotmail.com
 *
 * ------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  ********************************************************************PGR-GNU*/

#include <sstream>
#include <vector>
#include <set>

#include "../../common/src/pgr_types.h"
#include "../../common/src/pgr_assert.h"
#include "./Dmatrix.h"
#include "./eucledianDmatrix.h"
#include "./tour.h"


namespace pgRouting {
namespace tsp {

template < typename MATRIX >
class TSP: public MATRIX {
 public:
     using MATRIX::distance;
     using MATRIX::tourCost;
     using MATRIX::get_row;

     /*
      * function members
      */
     explicit TSP(const MATRIX &_costs)
         : MATRIX(_costs),
         current_tour(_costs.size()),
         best_tour(_costs.size()),
         epsilon(0.000001),
         n(_costs.size())
          {
             pgassert(n == MATRIX::size());
             bestCost = MATRIX::tourCost(best_tour);
             current_cost = MATRIX::tourCost(current_tour);
             pgassert(bestCost == current_cost);
         }


     Tour get_tour() const {return best_tour;};
     std::string get_log() const {return log.str();};
     void greedyInitial();
     void annealing(
             double initial_temperature,
             double final_temperature,
             double cooling_factor,
             int64_t tries_per_temperature,
             int64_t change_per_temperature,
             bool fix_random
             );


 private:
     Tour current_tour;
     Tour best_tour;
     double bestCost;
     double current_cost;
     double epsilon;
     size_t n;

     int updatecalls;
     std::ostringstream log;

 private:
     void invariant() const;

     size_t find_closest_city(
             size_t current_city,
             const std::set<size_t> inserted) const;

     double getDeltaSlide(
             size_t posP,
             size_t posF,
             size_t posL) const;

     void swapClimb();

     double getDeltaSwap(
             size_t posA,
             size_t posC) const;

     double getDeltaReverse(
             size_t posA,
             size_t posC) const;

     void update_if_best();

};

}  // namespace tsp
}  // namespace pgRouting

#include "pgr_tsp.cpp"
