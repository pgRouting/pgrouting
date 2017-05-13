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
#include "cpp_common/pgr_assert.h"

namespace pgrouting {
namespace vrp {
namespace pickdeliver {

bool
Base_node::isSamePos(const Base_node &rhs) const {
    return id() == rhs.id();
}

#if 0
int64_t
Base_node::id() const {
    return m_id;
}

size_t
Base_node::idx() const {
    return m_idx;
}
#endif

std::ostream& operator << (std::ostream &log, const Base_node &node) {
    node.print(log);
    return log;
}

void
Base_node::print(std::ostream& os) const {
    os << "USING BASE_NODE\n";
    os << id()
        << "(" << idx() << ")";
}

#if 0
double
Base_node::distance(const Base_node &node) const {
    return 0;
}
#endif
double
Base_node::comparable_distance(const Base_node &) const {
    return 0;
}


Base_node::Base_node(size_t _idx, int64_t _id)
    : Identifier(_idx, _id) {
    }

bool
Base_node::operator ==(const Base_node &rhs) const {
    if (&rhs == this) return true;
    return
        (idx() == rhs.idx())
         && (id() == rhs.id());
}

}  //  namespace pickdeliver
}  //  namespace vrp
}  //  namespace pgrouting


