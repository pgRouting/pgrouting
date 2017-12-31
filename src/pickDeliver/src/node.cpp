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

std::ostream& operator << (std::ostream &log, const Node &node) {
    node.print(log);
    return log;
}

void
Node::print(std::ostream& os) const {
    os << id()
#ifndef NDEBUG
        << "(" << idx() << ")"
#endif
       << " (x,y) = (" << m_point.x() << ", " << m_point.y() << ")";
}

double
Node::distance(const Node &other) const {
    auto dx = m_point.x() - other.m_point.x();
    auto dy = m_point.y() - other.m_point.y();
    return sqrt(dx * dx + dy * dy);
}

/**
 * para[in] n bas node to be casted as Node
 */
double
Node::distance(const Base_node *n) const {
    return distance(*dynamic_cast<const Node*>(n));
}

/**
 * @param[in] _idx index to a container
 * @param[in] _id  original identifier
 * @param[in] _x   coordinate value
 * @param[in] _y   coordinate value
 */
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

}  //  namespace vrp
}  //  namespace pgrouting
