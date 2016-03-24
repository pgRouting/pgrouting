


#include "./../../common/src/pgr_assert.h"
#include "pgr_pickDeliver.h"
#include "order.h"


Order::Order(ID p_id,
        const Vehicle_node &p_pickup,
        const Vehicle_node &p_delivery,
        const Pgr_pickDeliver &p_problem) :
    m_id(p_id),
    pickup_id(p_pickup.id()),
    delivery_id(p_delivery.id()),
    problem(p_problem) { 
        pgassert(pickup().is_pickup());
        pgassert(delivery().is_delivery());
    }

std::ostream&
operator<<(std::ostream &log, const Order &order) {
    log << "Order " << order.m_id << ":\n"
        << "\tPickup: " << order.pickup() << "\n"
        << "\tDelivery: " << order.delivery() << "\n\n";
    return log;
}



const Vehicle_node&
Order::delivery() const {return problem.nodes[delivery_id];}


const Vehicle_node&
Order::pickup() const {return problem.nodes[pickup_id];}


bool
Order::is_valid() const {
    return 
        pickup().is_pickup()
        && delivery().is_delivery()
        /* P -> D  */ 
        && delivery().is_ok_after_visiting(pickup());
}

bool
Order::isOrderCompatibleIJ(const Order &other) const {

    /* this is true in all cases */
    auto all_cases(pickup().is_ok_after_visiting(other.pickup()) 
            && delivery().is_ok_after_visiting(other.pickup())
            && other.delivery().is_ok_after_visiting(other.pickup()) 
            &&delivery().is_ok_after_visiting(pickup()));


    /* case other(P) other(D) this(P) this(D) */
    auto case1( pickup().is_ok_after_visiting(other.delivery()) 
            && delivery().is_ok_after_visiting(other.delivery()));

    /* case other(P) this(P) other(D) this(D) */
    auto case2(other.delivery().is_ok_after_visiting(pickup()) 
            && delivery().is_ok_after_visiting(other.delivery()));

    /* case other(P) this(P) this(D) other(D) */
    auto case3(other.delivery().is_ok_after_visiting(pickup()) 
            && other.delivery().is_ok_after_visiting(delivery()));
    return all_cases && (case1 || case2 || case3);
}


bool
Order::isOrderCompatibleEnd(const Vehicle_node &node) const {
    return false;
}

bool
Order::isOrderCompatibleStart(const Vehicle_node &node) const {
    return false;
}
