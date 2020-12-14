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

#include "vrp/dnode.h"

#include "vrp/pgr_pickDeliver.h"

namespace pgrouting {
namespace vrp {


std::ostream& operator << (std::ostream &log, const Dnode &node) {
    log << node.id()
        << "(" << node.idx() << ")";
    return log;
}


double
Dnode::distance(const Dnode &other) const {
    return problem->m_cost_matrix.distance(
            problem->m_cost_matrix.get_index(id()),
            problem->m_cost_matrix.get_index(other.id()));
}


Dnode::Dnode(size_t idx, int64_t id) :
    PD_problem(),
    Identifier(idx, id) {
    }

bool
Dnode::operator ==(const Dnode &rhs) const {
    if (&rhs == this) return true;
    return
        (idx() == rhs.idx())
         && (id() == rhs.id());
}

Pgr_messages&
Dnode::msg() {
    return problem->msg;
}

}  //  namespace vrp
}  //  namespace pgrouting


