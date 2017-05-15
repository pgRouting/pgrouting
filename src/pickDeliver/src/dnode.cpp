/*PGR-GNU*****************************************************************

FILE: dnode.cpp

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

#include "pickDeliver/dnode.h"
#include "vrp/pgr_pickDeliver.h"
#include "cpp_common/pgr_messages.h"

namespace pgrouting {
namespace vrp {

#if 0
bool Dnode::isSamePos(const Dnode &other) const {
    return comparable_distance(other) == 0;
}
#endif

std::ostream& operator << (std::ostream &log, const Dnode &node) {
    log << node.id()
        << "(" << node.idx() << ")";
    return log;
}

#if 0
double
Dnode::distance(const Dnode &other) const {
    ENTERING();
    msg.log << "idx" << idx()
     << "\t id " << id()
     << "\t matrix.get_id " << problem->m_cost_matrix.get_id(id())
     << "\t matrix.get_index " << problem->m_cost_matrix.get_index(id())
     << "\t matrix.get_index " << problem->m_cost_matrix.get_id(problem->m_cost_matrix.get_index(id()))
     << "\n";

    msg.log << "other idx" << other.idx()
     << "\t id " << other.id()
     << "\t matrix.get_id " << problem->m_cost_matrix.get_id(other.id())
     << "\t matrix.get_index " << problem->m_cost_matrix.get_index(other.id())
     << "\t matrix.get_index " << problem->m_cost_matrix.get_id(problem->m_cost_matrix.get_index(other.id()))
     << "\n";

    msg.log << "Distance " << problem->m_cost_matrix.distance(
            problem->m_cost_matrix.get_index(id()),
            problem->m_cost_matrix.get_index(other.id()))
        << "\n\n";
    EXITING();
    return problem->m_cost_matrix.distance(
            problem->m_cost_matrix.get_index(id()),
            problem->m_cost_matrix.get_index(other.id()));
}
#endif

double
Dnode::distance(const Base_node *other) const {
    ENTERING();
    EXITING();
    return problem->m_cost_matrix.distance(
            problem->m_cost_matrix.get_index(id()),
            problem->m_cost_matrix.get_index(other->id()));
}

#if 0
double
Dnode::comparable_distance(const Dnode &other) const {
    distance(other);
    return problem->m_cost_matrix.comparable_distance(0,0);
}
#endif

Dnode::Dnode(size_t id, int64_t original_id, double, double)
    : Base_node(id, original_id) {
    }

#if 0
bool
Dnode::operator ==(const Dnode &rhs) const {
    if (&rhs == this) return true;
    return
        (id() == rhs.id())
         && (id() == rhs.id())
         && (m_point == rhs.m_point);
}
#endif

}  //  namespace vrp
}  //  namespace pgrouting


