/*PGR-GNU*****************************************************************

FILE: tw_node.cpp

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

#include "vrp/tw_node.h"

#include <limits>
#include <string>

#include "cpp_common/pgr_assert.h"
#include "vrp/pgr_pickDeliver.h"


namespace pgrouting {
namespace vrp {


double
Tw_node::travel_time_to(const Tw_node &to, double speed) const {
    /** TODO(vicky)
     * shall call Node or Dnode
     * static cast won't work I think
     *
     */
    auto from =  problem->m_base_nodes[idx()].get();
    auto destination = problem->m_base_nodes[to.idx()].get();
    pgassert(speed != 0);
     /*! @todo TODO evaluate with matrix also*/
    return from->distance(destination) / speed;
}


/*
 * I -> J = (*this)
 */
double
Tw_node::arrival_j_opens_i(const Tw_node &I, double speed) const {
    if (m_type == kStart) return (std::numeric_limits<double>::max)();
    return I.opens() + I.service_time() + I.travel_time_to(*this, speed);
}

double
Tw_node::arrival_j_closes_i(const Tw_node &I, double speed) const {
    if (m_type == kStart) return  (std::numeric_limits<double>::max)();
    return I.closes() + I.service_time() + I.travel_time_to(*this, speed);
}




bool
Tw_node::is_compatible_IJ(const Tw_node &I, double speed) const {
    /*
     * I /->  kStart
     */
    if (m_type == kStart) return false;
    /*
     * kEnd /-> (*this)
     */
    if (I.m_type == kEnd) return false;

    return !is_late_arrival(arrival_j_opens_i(I, speed));
}

bool
Tw_node::is_partially_compatible_IJ(const Tw_node &I, double speed) const {
    return
        is_compatible_IJ(I, speed)
        && !is_early_arrival(arrival_j_opens_i(I, speed))
        && is_late_arrival(arrival_j_closes_i(I, speed));
}

bool
Tw_node::is_tight_compatible_IJ(const Tw_node &I, double speed) const {
    return
        is_compatible_IJ(I, speed)
        && !is_early_arrival(arrival_j_opens_i(I, speed))
        && !is_late_arrival(arrival_j_closes_i(I, speed));
}

bool
Tw_node::is_partially_waitTime_compatible_IJ(
        const Tw_node &I,
        double speed) const {
    return
        is_compatible_IJ(I, speed)
        && is_early_arrival(arrival_j_opens_i(I, speed));
}

bool
Tw_node::is_waitTime_compatible_IJ(const Tw_node &I, double speed) const {
    return
        is_compatible_IJ(I, speed)
        && is_early_arrival(arrival_j_opens_i(I, speed));
}


std::string Tw_node::type_str() const {
    switch (type()) {
        case kStart: return "START"; break;
        case kEnd: return "END"; break;
        case kDump: return "DUMP"; break;
        case kLoad: return "LOAD"; break;
        case kPickup: return "PICKUP"; break;
        case kDelivery: return "DELIVERY"; break;
        default: return "UNKNOWN";
    }
}

bool
Tw_node::is_start() const {
    return
        m_type == kStart
        && (opens() < closes())
        && (service_time() >= 0)
        && (demand() == 0);
}

bool
Tw_node::is_pickup() const {
    return m_type == kPickup
        && (opens() < closes())
        && (service_time() >= 0)
        && (demand() > 0);
}


bool
Tw_node::is_delivery() const {
    return m_type == kDelivery
        && (opens() < closes())
        && (service_time() >= 0)
        && (demand() < 0);
}


bool
Tw_node::is_dump() const {
    return m_type == kDump
        && (opens() < closes())
        && (service_time() >= 0)
        && (demand() <= 0);
}


bool
Tw_node::is_load() const {
    return m_type == kLoad
        && (opens() < closes())
        && (service_time() >= 0)
        && (demand() >= 0);
}


bool
Tw_node::is_end() const {
    return m_type == kEnd
        && (opens() < closes())
        && (service_time() >= 0)
        && (demand() == 0);
}


bool
Tw_node::operator ==(const Tw_node &other) const {
    if (&other == this) return true;
    auto lhs = static_cast<const Node&>(
            *problem->m_base_nodes[idx()].get());
    auto rhs = static_cast<const Node&>(
            *problem->m_base_nodes[other.idx()].get());
    return lhs == rhs;
}



bool Tw_node::is_valid() const {
    switch (type()) {
        case kStart:
            return is_start();
            break;

        case kEnd:
            return is_end();
            break;

        case kDump:
            return is_dump();
            break;

        case kDelivery:
            return is_delivery();
            break;

        case kPickup:
            return is_pickup();
            break;

        case kLoad:
            return is_load();
            break;

        default:
            return false;
            break;
    }

    return false;
}


Tw_node::Tw_node(
        size_t id,
        PickDeliveryOrders_t data,
        NodeType type) :
    Identifier(id, data.pick_node_id),
    m_order(data.id),
    m_opens(data.pick_open_t),
    m_closes(data.pick_close_t),
    m_service_time(data.pick_service_t),
    m_demand(data.demand),
    m_type(type)  {
        if (m_type == kDelivery) {
            reset_id(data.deliver_node_id);
            m_opens = data.deliver_open_t;
            m_closes = data.deliver_close_t;
            m_service_time = data.deliver_service_t;
            m_demand *= -1;
        }
    }

Tw_node::Tw_node(
        size_t id,
        Vehicle_t data,
        NodeType type) :
    Identifier(id, data.start_node_id),
    m_opens(data.start_open_t),
    m_closes(data.start_close_t),
    m_service_time(data.start_service_t),
    m_demand(0),
    m_type(type) {
        if (m_type == kEnd) {
            reset_id(data.end_node_id);
            m_opens = data.end_open_t;
            m_closes = data.end_close_t;
            m_service_time = data.end_service_t;
        }
    }


/*! * \brief Print the contents of a Twnode object. */
std::ostream& operator << (std::ostream &log, const Tw_node &n) {
    log << static_cast<const Node&>(
            *n.problem->m_base_nodes[n.idx()].get())
        << "[opens = " << n.m_opens
        << "\tcloses = " << n.m_closes
        << "\tservice = " << n.m_service_time
        << "\tdemand = " << n.m_demand
        << "\ttype = " << n.type_str()
        << "]"
        << "\n";
    return log;
}

}  //  namespace vrp
}  //  namespace pgrouting

