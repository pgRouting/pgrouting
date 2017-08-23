/*PGR-GNU*****************************************************************

FILE: vehicle_node.cpp

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


#include "vrp/vehicle_node.h"


namespace pgrouting {
namespace vrp {


/*!
 * \param[in] cargoLimit of the vehicle
 */
void
Vehicle_node::evaluate(double cargoLimit) {
    if (is_start()) {
        /* time */
        m_travel_time = 0;
        m_arrival_time = opens();
        m_wait_time = 0;
        m_departure_time = arrival_time() + service_time();

        /* time aggregates */
        m_tot_travel_time = 0;
        m_tot_wait_time = 0;
        m_tot_service_time = service_time();

        /* cargo aggregates */
        m_cargo = demand();

        /* violation aggregates */
        m_twvTot = m_cvTot = 0;
        m_cvTot = has_cv(cargoLimit) ? 1 : 0;
        m_delta_time = 0;
    }
}


/*!
  \param[in] pred The node preceding this node in the path.
  \param[in] cargoLimit of the vehicle.
  \param[in] speed of the vehicle.
  */
void
Vehicle_node::evaluate(
        const Vehicle_node &pred,
        double cargoLimit,
        double speed) {
    /* time */
    m_travel_time    = pred.travel_time_to(*this, speed);
    m_arrival_time   = pred.departure_time() + travel_time();
    m_wait_time      = is_early_arrival(arrival_time()) ?
        opens() - m_arrival_time :
        0;
    m_departure_time = arrival_time() + wait_time() + service_time();

    /* time aggregates */
    m_tot_travel_time = pred.total_travel_time() + travel_time();
    m_tot_wait_time    = pred.total_wait_time()    + wait_time();
    m_tot_service_time = pred.total_service_time() + service_time();

    /* cargo aggregates */
    if (is_dump() &&  pred.cargo() >= 0) {
        demand(-pred.cargo());
    }
    m_cargo = pred.cargo() + demand();

    /* violations aggregates */

    m_twvTot = has_twv() ? pred.twvTot() + 1 : pred.twvTot();
    m_cvTot = has_cv(cargoLimit) ? pred.cvTot() + 1 : pred.cvTot();
    m_delta_time = departure_time() - pred.departure_time();
}



std::ostream&
operator << (std::ostream &log, const Vehicle_node &v) {
    log << static_cast<const Tw_node&>(v)
        << " twv = " << v.has_twv()
        << ", twvTot = " << v.twvTot()
        << ", cvTot = " << v.cvTot()
        << ", cargo = " << v.cargo()
        << ", travel_time = " << v.travel_time()
        << ", arrival_time = " << v.arrival_time()
        << ", wait_time = " << v.wait_time()
        << ", service_time = " << v.service_time()
        << ", departure_time = " << v.departure_time();
    return log;
}


/*! @brief Creates a disconected vehicle node
 *
 * A node that is not served by any vehicle
 *
 * @param[in] node Time window node
 */
Vehicle_node::Vehicle_node(const Tw_node &node)
    : Tw_node(node),
    m_travel_time(0),
    m_arrival_time(0),
    m_wait_time(0),
    m_departure_time(0),
    m_delta_time(0),
    m_cargo(0),
    m_twvTot(0),
    m_cvTot(0),
    m_tot_wait_time(0),
    m_tot_travel_time(0),
    m_tot_service_time(0) {
    }



bool
Vehicle_node::deltaGeneratesTWV(double delta_time) const {
    return is_late_arrival(m_arrival_time + delta_time);
}


/*!
  The actual arrival time at \b This node, given that:
  \b this node is visited directly after \b other node
  and that the actual arrival time at \b other node was arrival(other)
  */
double
Vehicle_node::arrival_i_arrives_j(
        const Vehicle_node &other,
        double speed) const {
    return other.arrival_time()
        + other.service_time()
        + other.travel_time_to(*this, speed);
}

}  //  namespace vrp
}  //  namespace pgrouting
