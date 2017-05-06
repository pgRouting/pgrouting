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


#include "pickDeliver/order.h"

#include <set>
#include "cpp_common/pgr_assert.h"
#include "pickDeliver/pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {
namespace pickdeliver {


Identifiers<size_t>
Order::subsetI(const Identifiers<size_t> &I) const {
    return m_compatibleI * I;
}

Identifiers<size_t>
Order::subsetJ(const Identifiers<size_t> &J) const {
    return m_compatibleJ * J;
}


Order::Order(
        int64_t p_id,
        size_t p_idx,
        const Vehicle_node &p_pickup,
        const Vehicle_node &p_delivery) :
    Identifier(p_idx, p_id),
    pickup_id(p_pickup.id()),
    delivery_id(p_delivery.id()) {
    }

double
Order::distance() const {
    return pickup().distance(delivery());
}

std::ostream&
operator << (std::ostream &log, const Order &order) {
    log << "\n\nOrder " << order.id() << ":\n"
#if 0
        << "\tOrder idx: " << order.m_idx << "\n"
#endif
        << "\tPickup: " << order.pickup() << "\n"
        << "\tDelivery: " << order.delivery() << "\n\n";
#if 0
    if (order.delivery().is_partially_compatible_IJ(order.pickup(), speed)) {
        log << "\tis_partially_compatible_IJ: ";
    } else if (order.delivery().is_tight_compatible_IJ(order.pickup(), speed)) {
        log << "\tis_tight_compatible_IJ: ";
    } else if (order.delivery().is_waitTime_compatible_IJ(
                order.pickup(), speed)) {
        log << "\tis_waitTime_compatible_IJ: ";
    } else {
        pgassert(false);
    }
#endif
    log << "There are | {I}| = "
        << order.m_compatibleI.size()
        << " -> order(" << order.id()
        << ") -> | {J}| = " << order.m_compatibleJ.size()
        << "\n {";
    for (const auto o : order.m_compatibleI) {
        log << o << ", ";
    }
    log << "} -> " << order.id() << " -> {";
    for (const auto o : order.m_compatibleJ) {
        log << o << ", ";
    }
    log << "}\n";

    log << "Cost (distinace/time): " << order.distance();

    return log;
}



const Vehicle_node&
Order::delivery() const {
    return problem->node(delivery_id);
}


const Vehicle_node&
Order::pickup() const {
    return problem->node(pickup_id);
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

}  //  namespace pickdeliver
}  //  namespace vrp
}  //  namespace pgrouting

