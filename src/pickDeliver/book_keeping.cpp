/*PGR-GNU*****************************************************************

FILE: book_keeping.cpp

Copyright (c) 2017 pgRouting developers
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


#include "vrp/book_keeping.h"

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
    log << "\n" << d.from_truck.tau() << " --> "
        << d.from_order
        << "("
        << d.from_truck.orders()[d.from_order].pickup().id()
        << ")"
        << "\n" << d.to_truck.tau() << " --> "
        << d.to_order
        << "("
        << d.to_truck.orders()[d.to_order].pickup().id()
        << ")"
        << "\n" << "delta = " << d.estimated_delta;
    return log;
}


std::ostream& operator<< (std::ostream &log, const Swap_bk &data) {
    auto q = data.m_swaps;
    while (!q.empty()) {
        log << q.top() << " ";
        q.pop();
    }
    log << '\n';
    return log;
}


}  // namespace vrp
}  // namespace pgrouting
