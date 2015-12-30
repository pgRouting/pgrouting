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

#include <vector>
#include <algorithm>
#include "pdp.hpp"

class Route  {
 public:
     int twv;
     int cv;
     int dis;
     std::vector < int > path;
     std::vector < int > order;
     int capacity;

     explicit Route(int _capacity) :
         twv(0),
         cv(0),
         dis(0),
         capacity(_capacity)  {
             path.clear();
             order.clear();
         }
     State append(
             const Pickup &p,
             State S);
     void update(const std::vector<Customer> &c, const Depot &d);
     double cost() const;
     int HillClimbing(const std::vector<Customer> &c, const Depot &d);
     int insertOrder(const std::vector<Customer> &c, const Depot &d);
     void remove(const State &S);
     int RemoveOrder(const Pickup &p);
     double distance(double x1, double y1, double x2, double y2) const;
};


/*
 * Returns:
 * 1 - when the order was found
 * 0 - When the order was not found
 */
int
Route::RemoveOrder(const Pickup &pickup) {
    auto Pickup_pos = find(path.begin(), path.end(), pickup.Pid);

    if (Pickup_pos != path.end())  {
        path.erase(Pickup_pos);
        order.erase(Pickup_pos);
    } else  {
        return 0;
    }

    auto Delivery_pos = find(path.begin(), path.end(), pickup.Did);

    if (Delivery_pos !=  path.end())  {
        path.erase(Delivery_pos);
        order.erase(Delivery_pos);
    }
    return 1;
}

State
Route::append(
        /*Customer *c, */
        const Pickup &p,
        /* Depot d, int CustomerLength, int PickupLength,*/
        State S) {
    // Save State;
    S.twv = twv;
    S.cv = cv;
    S.dis = dis;
    S.path_length = path.size();

    for (size_t i = 0; i < path.size(); i++) {
        S.path[i] = path[i];
        S.order[i] = order[i];
    }

    //  Insert Order

    path.push_back(p.Pid);
    path.push_back(p.Did);

    order.push_back(p.id);
    order.push_back(p.id);
    return S;
}


void
Route::update(const std::vector<Customer> &c, const Depot &d)  {
    dis = 0, twv = 0, cv = 0;
    int load = 0;
    for (int i = -1; i < static_cast<int>(path.size()); i++) {
        // Depot to first Customer
        if (i == -1) {
            dis += CalculateDistance(c[path[i+1]], d);
            if (dis < c[path[i+1]].Etime) {
                dis = c[path[i+1]].Etime;
            } else if (dis > c[path[i+1]].Ltime) {
                twv += 1;
            }
            dis += c[path[i+1]].Stime;
            load += c[path[i+1]].demand;
        }
        // Last cusotmer to Depot
        if (i ==  static_cast<int>(path.size() - 1))  {
            dis += CalculateDistance(c[path[i]], d);
            if (dis > d.Ltime) {
                twv += 1;
            }
        }
        // Middle Customers
        if (i >= 0 && i < static_cast<int>(path.size() - 1)) {
            dis += CalculateDistance(c[path[i]], c[path[i + 1]]);
            if (dis < c[path[i+1]].Etime) {
                dis = c[path[i+1]].Etime;
            } else if (dis > c[path[i+1]].Ltime) {
                twv += 1;
            }
            dis += c[path[i+1]].Stime;
            load += c[path[i+1]].demand;
        }

        /*
         * works only with 200 load
         */
        if (load > 200 || load < 0)  {
            cv += 1;
        }
    }
    return;
}

double
Route::cost() const  {
    return (0.3*dis)+(0.5*twv)+(0.2*cv);
}

int
Route::insertOrder(const std::vector<Customer> &c, const Depot &d
        /*, Pickup p*/)  {
    twv = 0, cv = 0, dis = 0;


    update(c, d);
    if (twv == 0 && cv == 0 && dis < d.Ltime)
        return 0;

    for (size_t i = 0; i < path.size(); i++)  {
        for (size_t j = 0; j < path.size(); j++)  {
            if ((c[path[i]].Ltime > c[path[j]].Ltime)) {
                std::swap(path[i], path[j]);
                std::swap(order[i], order[j]);
            }
        }
    }

    std::reverse(path.begin(), path.end());
    std::reverse(order.begin(), order.end());


    twv = 0, cv = 0, dis = 0;
    update(c, d);

    return twv > 0 || cv > 0 || dis > d.Ltime;
}

int Route::HillClimbing(const std::vector<Customer> &c, const Depot &d
        /*, Pickup p*/) {
    double cost1 = 0, cost2 = 0;
    twv = 0, cv = 0, dis = 0;
    update(c, d);
    cost1 = cost();
    if (twv == 0 && cv == 0 && dis < d.Ltime)
        return 0;

    for (size_t i = 0; i < path.size(); i++) {
        for (size_t j = 0; j < path.size(); j++) {
            int swap_flag = 0, count_flag = 0;
            if ((c[path[i]].Ltime > c[path[j]].Ltime) && (count_flag == 0)) {
                swap_flag = 1;
                count_flag = 1;
                std::swap(path[i], path[j]);
                std::swap(order[i], order[j]);
            }
            update(c, d);
            cost2 = cost();
            if (cost2 > cost1 && swap_flag == 1)  {
                std::swap(path[i], path[j]);
                std::swap(order[i], order[j]);
            }
        }
    }

    std::reverse(path.begin(), path.end());
    std::reverse(order.begin(), order.end());

    twv = 0, cv = 0, dis = 0;
    update(c, d);

    return twv > 0 || cv > 0 || dis > d.Ltime;
}

void Route::remove(const State &S)  {
    twv = S.twv;
    cv = S.cv;
    dis = S.dis;

    path.clear();
    order.clear();
    for (int i = 0; i < S.path_length; i++)  {
        path[i] = S.path[i];
        order[i] = S.order[i];
    }
}
