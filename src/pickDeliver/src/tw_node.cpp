
#include "./tw_node.h"

double Tw_node::arrival_i_opens_j(const Tw_node &other) const{
    if (m_type == kStart) return d_max();
    return  other.opens() + other.service_time() + other.travel_time_to(*this);
}



double Tw_node::arrival_i_closes_j(const Tw_node &other) const {
    if (m_type == kStart) return d_max();
    return  other.closes() + other.service_time() + other.travel_time_to(*this);
}

bool Tw_node::is_ok_after_visiting(const Tw_node &other) const {
    return isCompatibleIJ(other);
}


bool Tw_node::isCompatibleIJ(const Tw_node &other) const {
    if (m_type == kStart) return false;
    return  !is_late_arrival(arrival_i_opens_j(other));
}

bool Tw_node::isFullyCompatibleIJ(const Tw_node &other) const {
    return  !is_late_arrival(arrival_i_closes_j(other));
}

bool Tw_node::isTightCompatibleIJ(const Tw_node &other) const {
    return  isFullyCompatibleIJ(other)
        && !is_early_arrival(arrival_i_opens_j(other));
}


std::string Tw_node::type_str() const {

    switch  (type()) {
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
        && (demand() == 0);
}

bool 
Tw_node::is_pickup() const {
    return m_type == kPickup
        && (0 <= opens()) 
        && (opens() < closes())
        && (demand() > 0);
}


bool 
Tw_node::is_delivery() const {
    return m_type == kDelivery
        && (0 <= opens()) 
        && (opens() < closes())
        && (demand() < 0);
}


bool 
Tw_node::is_dump() const {
    return m_type == kDump
        && (0 <= opens()) 
        && (opens() < closes())
        && (demand() <= 0);
}


bool 
Tw_node::is_load() const {
    return m_type == kLoad
        && (0 <= opens()) 
        && (opens() < closes())
        && (demand() >= 0);
}


bool 
Tw_node::is_end() const {
    return m_type == kEnd
        && (0 <= opens()) 
        && (opens() < closes())
        && (demand() == 0);
}




bool Tw_node::is_valid() const {

    if (closes() < opens()) return false;
    if (opens() < 0) return false;
    if (service_time() < 0) return false;

    switch  (type()) {
        case kStart:
            if (demand() == 0) return true;
            break;

        case kEnd:
            if (demand() == 0) return true;
            break;

        case kDump:
            if (demand() <= 0) return true;
            break;

        case kDelivery:
            if (demand() < 0) return true;
            break;

        case kPickup:
            if (demand() > 0) return true;
            break;

        case kLoad:
            if (demand() >= 0) return true;
            break;

        default:
            return false;
            break;
    }

    return false;
}


Tw_node::Tw_node(
        ID id,
        Customer_t data,
        NodeType type
        ) :
    Node(id, data.id, data.x, data.y),
    m_opens(data.Etime),
    m_closes(data.Ltime),
    m_service_time(data.Stime),
    m_demand(data.demand),
    m_type(type) {
    }


Tw_node::Tw_node(
        ID id,
        int64_t original_id,
        double x,
        double y,
        double opens,
        double closes,
        double service_time,
        double demand,
        NodeType type
        ) :
    Node(id, original_id, x, y),
    m_opens(opens),
    m_closes(closes),
    m_service_time(service_time),
    m_demand(demand),
    m_type(type) {
    }



/*!  * \brief Print the contents of a Twnode object.  */
std::ostream& operator<<(std::ostream &log, const Tw_node &n) {
    log <<  static_cast<const Node&>(n)
        << "[opens=" << n.m_opens
        << "\tcloses=" << n.m_closes
        << "\tservice=" << n.m_service_time
        << "\tdemand=" << n.m_demand
        << "\ttype=" << n.type_str()
        << "]\n";
    return log;
}

