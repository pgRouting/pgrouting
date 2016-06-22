


#include <cassert>
#include "./vehicle_node.h"


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
  \param[in] pred The node preceeding this node (in the path).
  \param[in] cargoLimit The cargo limit of the vehicle.
  */
void
Vehicle_node::evaluate(const Vehicle_node &pred, double cargoLimit) {
    /* time */
    m_travel_time    = pred.travel_time_to(*this);
    m_arrival_time   = pred.departure_time() + travel_time();
    m_wait_time      = is_early_arrival(arrival_time()) ?
        opens() - m_arrival_time :
        0;
    m_departure_time  = arrival_time() + wait_time() + service_time();

    /* time aggregates */
    m_tot_travel_time  = pred.total_travel_time()  + travel_time();
    m_tot_wait_time    = pred.total_wait_time()    + wait_time();
    m_tot_service_time = pred.total_service_time() + service_time();

    /* cargo aggregates */
    if (is_dump() && pred.cargo() >= 0) {
        m_demand = -pred.cargo();
    }
    m_cargo = pred.cargo() + demand();

    /* cargo aggregates */

    m_twvTot = has_twv() ? pred.twvTot() + 1 : pred.twvTot();
    m_cvTot =  has_cv(cargoLimit) ? pred.cvTot() + 1 : pred.cvTot();
    m_delta_time = departure_time() - pred.departure_time();
}



std::ostream&
operator<<(std::ostream &log, const Vehicle_node &v) {
    log << static_cast<Tw_node>(v)
        << "  twv=" << v.has_twv()
        << ", twvTot=" << v.twvTot()
        << ", cvTot=" << v.cvTot()
        << ", cargo=" << v.cargo()
        << ", travel _time=" << v.travel_time()
        << ", arrival _time=" << v.arrival_time()
        << ", wait _time=" << v.wait_time()
        << ", service _time=" << v.service_time()
        << ", departure _time=" << v.departure_time();
    return log;
}


/*!
 * \param[in] id The User node id
 * \param[in] x The X or longitude coordinate for its location
 * \param[in] y The Y or latitude coordinate for its location
 * \param[in] opens The earliest arrival time (TW open)
 * \param[in] closes The latest arrival time (TW close)
 * \param[in] service_time The service time
 * \param[in] demand The demand in units of vehicle capacity
 * \param[in] streetId The street id this node is located
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
    return  is_late_arrival(m_arrival_time + delta_time) ;
}


/*! 
  The actual arrival time at \b This node,  given that:
  \b this node is visited directly after \b other node
  and that the actual arrival time at \b other node was arrival(other)
  */
double
Vehicle_node::arrival_i_arrives_j(const Vehicle_node &other) const {
    return  other.arrival_time() + other.service_time() + other.travel_time_to(*this);
}



