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

#include "./node.h"

namespace pgRouting {
namespace vrp {

bool Node::isSamePos(const Node &other) const {
    return comparable_distance(other) == 0;
}

double Node::travel_time_to(const Node &other) const {
    return distance(other);
}

std::ostream& operator << (std::ostream &log, const Node &node) {
    log << node.m_original_id
        << "(" << node.m_id << ")"
       << "(" << node.m_point.x() << ", " << node.m_point.y() << ")";
    return log;
}

double
Node::distance(const Node &other) const {
    auto dx = m_point.x() - other.m_point.x();
    auto dy = m_point.y() - other.m_point.y();
    return sqrt(dx * dx + dy * dy);
}

double
Node::comparable_distance(const Node &other) const {
    auto dx = m_point.x() - other.m_point.x();
    auto dy = m_point.y() - other.m_point.y();
    return dx * dx + dy * dy;
}


Node::Node(size_t id, int64_t original_id, double _x, double _y)
    : m_point(_x, _y),
    m_id(id),
    m_original_id(original_id) {
    }

bool
Node::operator ==(const Node &rhs) const {
    if (&rhs == this) return true;
    return
        (id() == rhs.id())
         && (original_id() == rhs.original_id())
         && (m_point == rhs.m_point);
}

}  //  namespace vrp
}  //  namespace pgRouting


