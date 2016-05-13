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

#include <vector>

#include "../../common/src/pgr_types.h"
#include "./Dmatrix.h"

class TSP {
 public:
/*
 * Defs
 */
typedef size_t Path[3];      /* specify how to change path */

typedef std::vector< std::vector < double > > Costs;
typedef std::vector< int64_t > Ids;

     Dmatrix dist;
     size_t n;
     double maxd;
     size_t bestlen;
     double bestCost;
     Ids iorder;
     /*
      * std::vector< bool > visited;
      */
     Ids jorder;
     Ids border;
     double b[4];


     /*
      * function members
      */
     TSP(Dmatrix  _costs)
         : dist(_costs),
         n(_costs.size()) {
             iorder.resize(n);
             jorder.resize(n);
             maxd = dist.max();

             /*
              * identity_permutations
              */
             std::iota(std::begin(iorder), std::end(iorder), 0);
#if 0
             for (auto &e : iorder) {
                  e = i;
             }
#endif
             /*
              * best order
              */
             border = iorder;
             bestCost = dist.pathCost(border);
         }
     void update(Ids new_order);
     bool findEulerianPath();
     void annealing();
     void doThreeWay(Path p);
     double getThreeWayCost (Path p);
     double getReverseCost(Path p);
     void doReverse(Path p);
     double D(size_t, size_t);
};

