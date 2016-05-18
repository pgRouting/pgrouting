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

#include "../../common/src/pgr_types.h"
#include "../../common/src/pgr_assert.h"
#include "./Dmatrix.h"
#include "./tour.h"


namespace pgRouting {
namespace tsp {

class TSP {
 public:
     Dmatrix dist;
     Tour current_tour;
     Tour best_tour;
     double bestCost;

     size_t n;
     double maxd;
#if 0
     size_t bestlen;
     std::vector<size_t> iorder;
     /*
      * std::vector< bool > visited;
      */
     std::vector<size_t> jorder;
     std::vector<size_t> border;
     double b[4];
#endif

     /*
      * function members
      */
     explicit TSP(Dmatrix  _costs)
         : dist(_costs),
         current_tour(_costs.size()),
         best_tour(_costs.size()),
         n(_costs.size()),
         maxd(dist.max()) {
             pgassert(n == dist.size());
#if 0
             iorder.resize(n);
             jorder.resize(n);
             /*
              * identity_permutations
              */
             std::iota(std::begin(iorder), std::end(iorder), 0);

             /*
              * best order
              */
             border = iorder;
#endif
             bestCost = dist.tourCost(current_tour);
         }
#if 1
     void update(std::vector<size_t> new_order);
#endif
     bool findEulerianPath();
     void annealing(
             std::ostringstream &log,
             double temperature = 100,
             double final_temperature = 0.1,
             double cooling_factor = 0.9,
             size_t tries_per_temperature = 500,
             size_t improve_path_per_temperature = 60);

     double getDeltaSlide(const Tour &tour, size_t posP, size_t posF,
             size_t posL);

     double getDeltaSwap(
             const Tour &tour,
             size_t posA,
             size_t posC) const;

     double getDeltaReverse(
             const Tour &tour,
             size_t posA,
             size_t posC) const;

     void update_if_best(
             const Tour &current_tour,
             double current_cost);

};

}  // namespace tsp
}  // namespace pgRouting
