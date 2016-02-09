/*PGR-GNU*****************************************************************

Copyright (c) 2014 Manikata Kondeti
mani.iiit123@gmail.com

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
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <vector>
#include <algorithm>
#include "../../common/src/pgr_types.h"
#include "pdp_types.hpp"

class Route  {
 public:
     int64_t twv;
     int64_t cv;
     double dis;
     std::vector < int64_t > path;
     double capacity;
     Depot depot;

     Route(double _capacity, const Depot &_depot) :
         twv(0),
         cv(0),
         dis(0),
         capacity(_capacity),
         depot(_depot) {
             path.clear();
         }
     void update(const Customers &c);
     double cost() const;
     bool insertOrder(const Customers &c, const Pickup &order);
     void append(const Customers &c, const Pickup &order);
     void remove(const State &S);
     int RemoveOrder(
             const Customers &customers,
             const Pickup &p);
     bool has_violation(const Customers &customers);
     bool has_twv(const Customers &customers);
};


/*
 * Returns:
 * 1 - when the order was found and removed
 * 0 - When the order was not found
 */
int
Route::RemoveOrder(
        const Customers &customers,
        const Pickup &pickup) {
    Route oldRoute = *this;
    oldRoute.update(customers);


    auto pickup_pos = find(path.begin(), path.end(), pickup.Pid);

    if (pickup_pos != path.end())  {
        path.erase(pickup_pos);
        auto delivery_pos = find(path.begin(), path.end(), pickup.Did);
        path.erase(delivery_pos);

        update(customers);
        return ((dis < depot.Ltime)
                && (twv < oldRoute.twv || cv < oldRoute.cv || dis < oldRoute.dis))? 1: 2;
    } else {
        return 0;
    }
}

void
Route::append(const Customers &customers,
        const Pickup &pickup) {
    path.push_back(pickup.Pid);
    path.push_back(pickup.Did);
    update(customers);
}


void
Route::update(const Customers &customers)  {
    dis = 0;
    twv = 0;
    cv = 0;
    double load = 0;
    double agg_cost = 0;
    int64_t prev_node = 0;
    for (const auto &node : path) {
        /*
         * Between nodes
         */
        agg_cost += CalculateDistance(customers[prev_node], customers[node]);

        if (agg_cost < customers[node].Etime) {
            /*
             * Arrving before the opening time, adjust time, moving it to the opening time
             */
            agg_cost = customers[node].Etime;
        }
        if (customers[node].Ltime < agg_cost) {
            /*
             * arrived after closing time
             */
            ++twv;
        }
        agg_cost += customers[node].Stime;
        load += customers[node].demand;
        if (load > capacity || load < 0) {
            ++cv;
        }
        prev_node = node;
    }
    /*
     * Going back to the depot
     */
    agg_cost += CalculateDistance(customers[prev_node], depot);
    if (depot.Ltime < agg_cost) {
        ++twv;
    }

    if (load != 0)  {
        ++cv;
    }
    dis = agg_cost;
    return;
}


double
Route::cost() const  {
    return (0.3 * dis) + (0.5 * static_cast<double>(twv)) + (0.2 * static_cast<double>(cv));
}

bool
Route::has_violation(const Customers &customers) {
    update(customers);
    return (twv > 0 || cv > 0);
}

bool
Route::has_twv(const Customers &customers) {
    update(customers);
    return (twv > 0);
}


bool
Route::insertOrder(const Customers &customers, const Pickup &order) {
    /*
     *  inserting only on a route that does not have twv or cv
     */
    if (has_violation(customers)) return false;


    path.insert(path.begin(), order.Pid);
    /*
     * The pickup is in:
     */
    size_t Ppos = 0;
    size_t i;
    for (i = 1; i < path.size(); i++)  {
        /*
         * Inserting without creating violation
         */
        if (!has_twv(customers)) break;
        std::swap(path[Ppos], path[i]);
        Ppos = i;
    }

    path.push_back(order.Did);
    /*
     * The delivery is in:
     */
    auto Dpos = path.size() - 1;

    auto j = Dpos;
    for (j = Dpos; Ppos < j; --j)  {
        if (!has_violation(customers)) break;
        std::swap(path[j], path[Dpos]);
        Dpos = j;
    }

    /*
     * Here 2 thngs can happen
     * We inserted succesfully without violation
     * Inserting created a violation
     */


    if (has_violation(customers)) {
        RemoveOrder(customers, order);
        return false;
    }

    /* 
     * ....  Ppos a b c d Dpos ....
     *
     * The current route
     */
    update(customers);
    Route bestRoute = *this;
    Route currRoute = *this;

    for (i = Ppos; i < Dpos; i++) {
        *this = currRoute;
        /* 
         * ....  Ppos a b c d Dpos ....
         */
        std::swap(path[Ppos], path[i]);
        Ppos = i;
        currRoute = *this;

        /*
         * for the same position of Ppos
         * - move the D
         * - keep track of the best
         */
        for (j = Dpos; Ppos < j; j--) {
            std::swap(path[j], path[Dpos]);
            Dpos = j;
            if (!has_violation(customers)
                    && dis < bestRoute.dis) {
                bestRoute = *this;
                bestRoute.update(customers);
            }
        }
    }

    *this = bestRoute;

    return true;
}

void Route::remove(const State &S)  {
    twv = S.twv;
    cv = S.cv;
    dis = S.dis;
    path  = S.path;
}
