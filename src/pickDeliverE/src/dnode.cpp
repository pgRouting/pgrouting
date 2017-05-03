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

#include "pickDeliverE/dnode.h"
#include "pickDeliverE/pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {

#if 0
bool Dnode::isSamePos(const Dnode &other) const {
    return comparable_distance(other) == 0;
}
#endif

std::ostream& operator << (std::ostream &log, const Dnode &node) {
    log << node.m_original_id
        << "(" << node.m_id << ")";
    return log;
}

double
Dnode::distance(const Dnode &other) const {
    msg.log << "id" << id()
     << "\t original_id" << original_id()
     << "\t matrix.get_id" << problem->m_cost_matrix.get_id(original_id())
     << "\t matrix.get_index" << problem->m_cost_matrix.get_index(original_id())
     << "\t matrix.get_index" << problem->m_cost_matrix.get_id(problem->m_cost_matrix.get_index(original_id()));

    msg.log << "other id" << id()
     << "\t original_id" << original_id()
     << "\t matrix.get_id" << problem->m_cost_matrix.get_id(original_id())
     << "\t matrix.get_index" << problem->m_cost_matrix.get_index(original_id())
     << "\t matrix.get_index" << problem->m_cost_matrix.get_id(problem->m_cost_matrix.get_index(original_id()));

    return problem->m_cost_matrix.distance(0,0);
}

double
Dnode::comparable_distance(const Dnode &other) const {
    return problem->m_cost_matrix.comparable_distance(0,0);
}


Dnode::Dnode(size_t id, int64_t original_id)
    : Base_node(id, original_id) {
    }

#if 0
bool
Dnode::operator ==(const Dnode &rhs) const {
    if (&rhs == this) return true;
    return
        (id() == rhs.id())
         && (original_id() == rhs.original_id())
         && (m_point == rhs.m_point);
}
#endif

}  //  namespace vrp
}  //  namespace pgrouting


