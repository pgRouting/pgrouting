/*PGR-GNU*****************************************************************

Copyright (c) 2014 Manikata Kondeti
mani.iiit123@gmail.com

Modified 2016 by Vicky Vergara

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

/*

 *****list of files in this dir*******

 The main problem is in two steps. 1.)Getting the initial solutiion and 2.)Optimizing it.

 1.) "Initial solution":
 A few heuristics are applied to find a feasible initial solution.
 Sequential Construction and Hill climbing.
 More implementation details are found here::
 https://github.com/pgRouting/pgrouting/wiki/VRP-Pickup-Delivery-Problem


 2.) "Optimizing the Solution":
 A reactive tabu search is applied on the initial solution to get a feasible
 optimized solution.
 TabuSearch comes under local search methods. We have three neighborhoods
    i) Single Paired Insertion
    ii) Swapping pairs between routes
    iii)Within Route Insertion.
Tabu attributes plays an important role in giving the best solution
(it includes TabuLength, A vector containing feasible solutions and a counter for number of solutions).
Reactive part discussed in the paper is to modify TabuLength based on the solution cycle.

 */
#pragma once

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif

extern "C" {
#include "./../../common/src/pgr_types.h"
}

#include <sstream>
#include <string.h>


#include "./pdp.hpp"
#include "./vehicle_node.h"
#include "./order.h"
#include "./Solution.h"



class Pgr_pickDeliver {
 public:
    void
        TabuSearch(
                const std::vector<Customer_t> &customers,
                const std::vector<Pickup> &pickups,
                int maxIter,
                std::vector<Solution> &T);
    void
        get_result(
                Solution &solution,
                const std::vector < Customer_t > &customers,
                const Depot &depot,
                int64_t VehicleLength,
                std::vector< General_vehicle_orders_t > &result);


    Pgr_pickDeliver(
            const Customer_t *c1, size_t total_customers,
            int VehicleLength,
            double capacity,
            int max_cycles,
            std::string &error);

    int64_t Solver();
    std::ostream& get_log(std::ostream &p_log) const {
        p_log << log.str() << "\n";
        log.str("");
        log.clear();
        return p_log;
    };
    bool data_consistency() const;

 private:
    double max_capacity;
    int max_cycles;
    int max_vehicles;
    Vehicle_node starting_site, ending_site;
    std::vector<Customer_t> original_data;
    std::vector<Order> orders;
    mutable std::ostringstream log;

};

