/*PGR-GNU*****************************************************************

FILE: base_node.cpp

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

#include "vrp/base_node.h"

namespace pgrouting {
namespace vrp {

bool
Base_node::isSamePos(const Base_node &rhs) const {
    return original_id() == rhs.original_id();
}


int64_t
Base_node::original_id() const {
    return m_original_id;
}

size_t
Base_node::id() const {
    return m_id;
}

std::ostream& operator << (std::ostream &log, const Base_node &node) {
    log << node.m_original_id
        << "(" << node.m_id << ")";
    return log;
}

double
Base_node::distance(const Base_node &) const {
    return 0;
}

double
Base_node::comparable_distance(const Base_node &) const {
    return 0;
}


Base_node::Base_node(size_t id, int64_t original_id)
    : m_id(id),
    m_original_id(original_id) {
    }

bool
Base_node::operator ==(const Base_node &rhs) const {
    if (&rhs == this) return true;
    return
        (id() == rhs.id())
         && (original_id() == rhs.original_id());
}

}  //  namespace vrp
}  //  namespace pgrouting


