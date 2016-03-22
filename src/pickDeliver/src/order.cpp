


#include "./../../common/src/pgr_assert.h"
#include "order.h"


Order::Order(ID p_id, const Tw_node &p_pickup, const Tw_node &p_delivery)
         : m_id(p_id),
         m_pickup(p_pickup),
         m_delivery(p_delivery) { 
             pgassert(m_pickup.is_pickup());
             pgassert(m_delivery.is_delivery());
         }

std::ostream& operator<<(std::ostream &log, const Order &order) {
    log << "Order " << order.m_id << ":\n"
        << "\tPickup: " << order.m_pickup << "\n"
        << "\tDelivery: " << order.m_delivery << "\n\n";
    return log;
}



Vehicle_node  Order::delivery() const {return m_delivery;}


Vehicle_node  Order::pickup() const {return m_pickup;}


bool
Order::is_valid() const {
    return 
        m_pickup.is_pickup()
        && m_delivery.is_delivery()
        && m_pickup.is_valid()
        && m_delivery.is_valid()
        /* P -> D  */ 
        && m_delivery.is_ok_after_visiting(m_pickup);
}

bool Order::isOrderCompatibleIJ(const Order &other) const {

    /* this is true in all cases */
    auto all_cases(m_pickup.is_ok_after_visiting(other.m_pickup) 
            && m_delivery.is_ok_after_visiting(other.m_pickup)
            && other.m_delivery.is_ok_after_visiting(other.m_pickup) 
            &&m_delivery.is_ok_after_visiting(m_pickup));


    /* case other(P) other(D) this(P) this(D) */
    auto case1( m_pickup.is_ok_after_visiting(other.m_delivery) 
            && m_delivery.is_ok_after_visiting(other.m_delivery));

    /* case other(P) this(P) other(D) this(D) */
    auto case2(other.m_delivery.is_ok_after_visiting(m_pickup) 
            && m_delivery.is_ok_after_visiting(other.m_delivery));

    /* case other(P) this(P) this(D) other(D) */
    auto case3(other.m_delivery.is_ok_after_visiting(m_pickup) 
            && other.m_delivery.is_ok_after_visiting(m_delivery));
    return all_cases && (case1 || case2 || case3);
}


bool Order::isOrderCompatibleEnd(const Vehicle_node &node) const {
    return false;
}

bool Order::isOrderCompatibleStart(const Vehicle_node &node) const {
    return false;
}
