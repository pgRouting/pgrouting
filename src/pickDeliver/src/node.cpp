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
// TODO (vicky) license

#include "./node.h"

namespace pgRouting {
namespace vrp {

bool Node::isSamePos(const Node &other) const {
    return comparable_distance(other.point) == 0;
}

double Node::travel_time_to(const Node &other) const {
    return distance(other.point);
}

std::ostream& operator << (std::ostream &log, const Node &node) {
    log << node.m_original_id << "(" << node.m_id << ")" << static_cast<const Point&>(node);
    return log;
}

Node::Node(size_t id, int64_t original_id, double x, double y)
    : Point(x, y),
    m_id(id),
    m_original_id(original_id) {
    }

bool
Node::operator ==(const Node &rhs) const {
    if (&rhs == this) return true;
    return
        (id() == rhs.id())
         && (original_id() == rhs.original_id())
         && (static_cast<Point>(*this) == static_cast<Point>(rhs));
}

}  //  namespace pgRouting
}  //  namespace vrp


