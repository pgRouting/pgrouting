/*PGR-GNU*****************************************************************

FILE: order.cpp

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


#include "vrp/order.h"


namespace pgrouting {
namespace vrp {


Identifiers<size_t>
Order::subsetI(const Identifiers<size_t> &I) const {
    return m_compatibleI * I;
}

Identifiers<size_t>
Order::subsetJ(const Identifiers<size_t> &J) const {
    return m_compatibleJ * J;
}


Order::Order(
        size_t p_idx, int64_t p_id,
        const Vehicle_node &p_pickup,
        const Vehicle_node &p_delivery) :
    Identifier(p_idx, p_id),
    m_pickup(p_pickup),
    m_delivery(p_delivery) {
    }

std::ostream&
operator<< (std::ostream &log, const Order &order) {
    log << "\n\nOrder "
        << static_cast<Identifier>(order) << ": \n"
        << "\tPickup: " << order.pickup() << "\n"
        << "\tDelivery: " << order.delivery() << "\n\n"
        << "\tTravel time: "
        << order.pickup().travel_time_to(order.delivery(), 1);
    log << "\nThere are | {I}| = "
        << order.m_compatibleI.size()
        << " -> order(" << order.idx()
        << ") -> | {J}| = " << order.m_compatibleJ.size()
        << "\n\n {";
    for (const auto o : order.m_compatibleI) {
        log << o << ", ";
    }
    log << "} -> " << order.idx() << " -> {";
    for (const auto o : order.m_compatibleJ) {
        log << o << ", ";
    }
    log << "}";

    return log;
}



const Vehicle_node&
Order::delivery() const {
    return m_delivery;
}


const Vehicle_node&
Order::pickup() const {
    return m_pickup;
}


bool
Order::is_valid(double speed) const {
    return
        pickup().is_pickup()
        && delivery().is_delivery()
        /* IS P -> D */
        && delivery().is_compatible_IJ(pickup(), speed);
}


/*
 * Initializing the set of nodes that can be placed
 * inmediately after \bthis node
 *
 * (*this) -> J
 *
 */
void
Order::set_compatibles(const Order J, double speed) {
    if (J.idx() == idx()) return;
    if (J.isCompatibleIJ(*this, speed)) {
        /*
         * this -> {J}
         */
        m_compatibleJ += J.idx();
    }
    if (this->isCompatibleIJ(J, speed)) {
        /*
         * {J} -> this
         */
        m_compatibleI += J.idx();
    }
}


/*
 * True when
 *
 * I -> (*this)
 *
 */
bool
Order::isCompatibleIJ(const Order &I, double speed) const {
    /* this is true in all cases */
    auto all_cases(
            pickup().is_compatible_IJ(I.pickup(), speed)
            && delivery().is_compatible_IJ(I.pickup(), speed));

    /* case other(P) other(D) this(P) this(D) */
    auto case1(pickup().is_compatible_IJ(I.delivery(), speed)
            && delivery().is_compatible_IJ(I.delivery(), speed));

    /* case other(P) this(P) other(D) this(D) */
    auto case2(I.delivery().is_compatible_IJ(pickup(), speed)
            && delivery().is_compatible_IJ(I.delivery(), speed));

    /* case other(P) this(P) this(D) other(D) */
    auto case3(I.delivery().is_compatible_IJ(pickup(), speed)
            && I.delivery().is_compatible_IJ(delivery(), speed));

    return all_cases &&  (case1 ||  case2 ||  case3);
}

}  //  namespace vrp
}  //  namespace pgrouting

