/*PGR-GNU*****************************************************************

FILE: solution.cpp

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

#include "vrp/solution.h"

#include <vector>
#include <string>
#include <algorithm>

#include "vrp/pgr_pickDeliver.h"
#include "c_types/pickDeliver/general_vehicle_orders_t.h"

namespace pgrouting {
namespace vrp {

std::vector<General_vehicle_orders_t>
Solution::get_postgres_result() const {
    std::vector<General_vehicle_orders_t> result;
    /* postgres numbering starts with 1 */
    int i(1);
    for (const auto truck : fleet) {
        std::vector<General_vehicle_orders_t> data =
            truck.get_postgres_result(i);
        result.insert(result.end(), data.begin(), data.end());

        ++i;
    }
    return result;
}



bool
Solution::is_feasable() const {
    for (const auto v : fleet) {
        if (v.is_feasable()) continue;
        return false;
    }
    return true;
}

double
Solution::duration() const {
    double total(0);
    for (const auto v : fleet) {
        total += v.duration();
    }
    return total;
}

int
Solution::twvTot() const {
    int total(0);
    for (const auto v : fleet) {
        total += v.twvTot();
    }
    return total;
}

double
Solution::wait_time() const {
    double total(0);
    for (const auto v : fleet) {
        total += v.total_wait_time();
    }
    return total;
}

double
Solution::total_travel_time() const {
    double total(0);
    for (const auto v : fleet) {
        total += v.total_travel_time();
    }
    return total;
}

double
Solution::total_service_time() const {
    double total(0);
    for (const auto v : fleet) {
        total += v.total_service_time();
    }
    return total;
}

int
Solution::cvTot() const {
    int total(0);
    for (const auto v : fleet) {
        total += v.cvTot();
    }
    return total;
}

Vehicle::Cost
Solution::cost() const {
    double total_duration(0);
    double total_wait_time(0);
    int total_twv(0);
    int total_cv(0);
    for (const auto v : fleet) {
        total_duration += v.duration();
        total_wait_time += v.total_wait_time();
        total_twv += v.twvTot();
        total_cv += v.cvTot();
    }
    return std::make_tuple(
            total_twv, total_cv, fleet.size(),
            total_wait_time, total_duration);
}



std::string
Solution::cost_str() const {
    Vehicle::Cost s_cost(cost());
    std::ostringstream log;

    log << "(twv, cv, fleet, wait, duration) = ("
        << std::get<0>(s_cost) << ", "
        << std::get<1>(s_cost) << ", "
        << std::get<2>(s_cost) << ", "
        << std::get<3>(s_cost) << ", "
        << std::get<4>(s_cost) << ")";

    return log.str();
}

std::string
Solution::tau(const std::string &title) const {
    Vehicle::Cost s_cost(cost());
    std::ostringstream log;

    log << "\n" << title << ": " << std::endl;
    for (const auto v : fleet) {
        log << "\n" << v.tau();
    }
    log << "\n" << cost_str() << "\n";
    return log.str();
}

void
Solution::sort_by_id() {
    std::sort(fleet.begin(), fleet.end(), []
            (const Vehicle_pickDeliver &lhs, const Vehicle_pickDeliver &rhs)
            -> bool {
            return lhs.idx() < rhs.idx();
            });
}

std::ostream&
operator << (std::ostream &log, const Solution &solution) {
    for (const auto vehicle : solution.fleet) {
        log << vehicle;
    }

    log << "\n SOLUTION:\n\n "
        << solution.tau();

    return log;
}

bool
Solution::operator<(const Solution &s_rhs) const {
    Vehicle::Cost lhs(cost());
    Vehicle::Cost rhs(s_rhs.cost());

    /*
     * capacity violations
     */
    if (std::get<0>(lhs) < std::get<0>(rhs))
        return true;
    if (std::get<0>(lhs) > std::get<0>(rhs))
        return false;

    /*
     * time window violations
     */
    if (std::get<1>(lhs) < std::get<1>(rhs))
        return true;
    if (std::get<1>(lhs) > std::get<1>(rhs))
        return false;

    /*
     * fleet size
     */
    if (std::get<2>(lhs) < std::get<2>(rhs))
        return true;
    if (std::get<2>(lhs) > std::get<2>(rhs))
        return false;

    /*
     * waiting time
     */
    if (std::get<3>(lhs) < std::get<3>(rhs))
        return true;
    if (std::get<3>(lhs) > std::get<3>(rhs))
        return false;

    /*
     * duration
     */
    if (std::get<4>(lhs) < std::get<4>(rhs))
        return true;
    if (std::get<4>(lhs) > std::get<4>(rhs))
        return false;

    return false;
}

Solution::Solution() :
    EPSILON(0.0001),
    trucks(problem->trucks()) {
    ENTERING();
    for (const auto &t : trucks) {
        msg.log << t.tau() << "\n";
    }
    EXITING();
}

}  //  namespace vrp
}  //  namespace pgrouting
