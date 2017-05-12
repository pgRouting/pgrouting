/*PGR-GNU*****************************************************************

FILE: node.cpp

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

#include "vrp/node.h"

#include "cpp_common/pgr_assert.h"

namespace pgrouting {
namespace vrp {
namespace pickdeliver {

bool Node::isSamePos(const Node &other) const {
    return comparable_distance(other) == 0;
}

std::ostream& operator << (std::ostream &log, const Node &node) {
    node.print(log);
    return log;
}

void
Node::print(std::ostream& os) const {
    os << "USING NODE\n";
    os << id()
        << "(" << idx() << ")"
       << " (x,y) = (" << m_point.x() << ", " << m_point.y() << ")";
}

double
Node::distance(const Node &other) const {
    ENTERING();
    auto dx = m_point.x() - other.m_point.x();
    auto dy = m_point.y() - other.m_point.y();
    msg.log << *this << "\n";
    msg.log << other << "\n";
    msg.log << "dx: " << dx << "\n";
    msg.log << "dy: " << dx << "\n";
    msg.log << "d: " << sqrt(dx * dx + dy * dy) << "\n";
    EXITING();
    return sqrt(dx * dx + dy * dy);
}

double
Node::distance(const Base_node &node) const {
    if (auto other = dynamic_cast<const Node*>(&node)) {
        ENTERING();
        auto dx = m_point.x() - other->m_point.x();
        auto dy = m_point.y() - other->m_point.y();
        msg.log << *this << "\n";
        msg.log << other << "\n";
        msg.log << "dx: " << dx << "\n";
        msg.log << "dy: " << dx << "\n";
        msg.log << "d: " << sqrt(dx * dx + dy * dy) << "\n";
        EXITING();
        return sqrt(dx * dx + dy * dy);
    }
    return 0;
}

double
Node::distance(const Base_node *n) const {
    ENTERING();
    EXITING();
    return distance(*dynamic_cast<const Node*>(n)); 
}

double
Node::comparable_distance(const Node &other) const {
    auto dx = m_point.x() - other.m_point.x();
    auto dy = m_point.y() - other.m_point.y();
    return dx * dx + dy * dy;
}


Node::Node(size_t _idx, int64_t _id, double _x, double _y)
    : Base_node(_idx, _id),
        m_point(_x, _y) {
    }

bool
Node::operator ==(const Node &rhs) const {
    if (&rhs == this) return true;
    return
        (idx() == rhs.idx())
         && (id() == rhs.id())
         && (m_point == rhs.m_point);
}

}  //  namespace pickdeliver
}  //  namespace vrp
}  //  namespace pgrouting
