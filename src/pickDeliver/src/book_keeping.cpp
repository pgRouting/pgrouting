
#include "./book_keeping.h"

#include <queue>
#include <iostream>

namespace pgrouting {
namespace vrp {

Swap_info::Swap_info(const Vehicle_pickDeliver &from,
        const Vehicle_pickDeliver &to,
        size_t from_o,
        size_t to_o,
        double delta) :
    from_truck(from),
    to_truck(to),
    from_order(from_o),
    to_order(to_o),
    estimated_delta(delta)
    { }

std::ostream& operator << (std::ostream& log, const Swap_info &d) {
    log   
        << "\n" << d.from_truck.tau() << " --> "
        << d.from_order << "(" << d.from_truck.orders()[d.from_order].pickup().original_id() << ")"
        << "\n" << d.to_truck.tau() << " --> "
        << d.to_order << "(" << d.to_truck.orders()[d.to_order].pickup().original_id() << ")"
        << "\n" << "delta = " << d.estimated_delta;
    return log;
}


std::ostream& operator<< (std::ostream &log, const Swap_bk &data) {
    auto q = data.m_swaps;
    while(!q.empty()) {
        log << q.top() << " ";
        q.pop();
    }
    log << '\n';
    return log;
}


}  // namespace vrp
}  // namespace pgrouting

