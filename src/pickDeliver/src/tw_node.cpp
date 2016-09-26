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

#include "./tw_node.h"

#include <limits>
#include <string>


#include "./../../common/src/pgr_assert.h"
#include "./pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {


double
Tw_node::travel_time_to(const Node &other) const {
    pgassert(problem->speed() > 0);
    return distance(other) / problem->speed();
}


/*
 * I -> J = (*this)
 */
double
Tw_node::arrival_j_opens_i(const Tw_node &I) const {
    if (m_type == kStart) return (std::numeric_limits<double>::max)();
    return I.opens() + I.service_time() + I.travel_time_to(*this);
}

double
Tw_node::arrival_j_closes_i(const Tw_node &I) const {
    if (m_type == kStart) return  (std::numeric_limits<double>::max)();
    return I.closes() + I.service_time() + I.travel_time_to(*this);
}




bool
Tw_node::is_compatible_IJ(const Tw_node &I) const {
    /*
     * I /->  kStart
     */
    if (m_type == kStart) return false;
    /*
     * kEnd /-> (*this)
     */
    if (I.m_type == kEnd) return false;

    return !is_late_arrival(arrival_j_opens_i(I));
}

bool
Tw_node::is_partially_compatible_IJ(const Tw_node &I) const {
    return
        is_compatible_IJ(I)
         && !is_early_arrival(arrival_j_opens_i(I))
         && is_late_arrival(arrival_j_closes_i(I));
}

bool
Tw_node::is_tight_compatible_IJ(const Tw_node &I) const {
    return
        is_compatible_IJ(I)
         && !is_early_arrival(arrival_j_opens_i(I))
         && !is_late_arrival(arrival_j_closes_i(I));
}

bool
Tw_node::is_partially_waitTime_compatible_IJ(const Tw_node &I) const {
    return
        is_compatible_IJ(I)
         && is_early_arrival(arrival_j_opens_i(I));
}

bool
Tw_node::is_waitTime_compatible_IJ(const Tw_node &I) const {
    return
        is_compatible_IJ(I)
         && is_early_arrival(arrival_j_opens_i(I));
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
         && (0 <= opens())
         && (opens() < closes())
         && (service_time() >= 0)
         && (demand() == 0);
}

bool
Tw_node::is_pickup() const {
    return m_type == kPickup
         && (0 <= opens())
         && (opens() < closes())
         && (service_time() >= 0)
         && (demand() > 0);
}


bool
Tw_node::is_delivery() const {
    return m_type == kDelivery
         && (0 <= opens())
         && (opens() < closes())
         && (service_time() >= 0)
         && (demand() < 0);
}


bool
Tw_node::is_dump() const {
    return m_type == kDump
         && (0 <= opens())
         && (opens() < closes())
         && (service_time() >= 0)
         && (demand() <= 0);
}


bool
Tw_node::is_load() const {
    return m_type == kLoad
         && (0 <= opens())
         && (opens() < closes())
         && (service_time() >= 0)
         && (demand() >= 0);
}


bool
Tw_node::is_end() const {
    return m_type == kEnd
         && (0 <= opens())
         && (opens() < closes())
         && (service_time() >= 0)
         && (demand() == 0);
}


bool
Tw_node::operator ==(const Tw_node &rhs) const {
    if (&rhs == this) return true;
    return (static_cast<Node>(*this) == static_cast<Node>(rhs));
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
        Customer_t data,
        NodeType type,
        const Pgr_pickDeliver *p_problem) :
    Node(id, data.id, data.x, data.y),
    m_opens(data.Etime),
    m_closes(data.Ltime),
    m_service_time(data.Stime),
    m_demand(data.demand),
    m_type(type),
    problem(p_problem) {
    }


Tw_node::Tw_node(
        size_t id,
        int64_t original_id,
        double x,
        double y,
        double opens,
        double closes,
        double service_time,
        double demand,
        NodeType type,
        const Pgr_pickDeliver *p_problem) :
    Node(id, original_id, x, y),
    m_opens(opens),
    m_closes(closes),
    m_service_time(service_time),
    m_demand(demand),
    m_type(type),
    problem(p_problem) {
    }


/*! * \brief Print the contents of a Twnode object. */
std::ostream& operator << (std::ostream &log, const Tw_node &n) {
    log << static_cast<const Node&>(n)
        << "[opens = " << n.m_opens
        << "\tcloses = " << n.m_closes
        << "\tservice = " << n.m_service_time
        << "\tdemand = " << n.m_demand
        << "\ttype = " << n.type_str()
        << "]";
    if (n.is_pickup() ||  n.is_delivery()) {
        log << "->" << n.m_otherid << "\n";
    } else {
        log << "\n";
    }
    return log;
}

}  //  namespace vrp
}  //  namespace pgrouting

