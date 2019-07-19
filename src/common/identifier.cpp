/*PGR-GNU*****************************************************************

Copyright (c) 2017 Celia Virginia Vergara Castillo
vicky_vergara@hotmail.com

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

#include "cpp_common/identifier.h"


namespace pgrouting {


Identifier::Identifier(
        const size_t _idx,
        const int64_t _id) :
    m_idx(_idx),
    m_id(_id) {}

size_t
Identifier::idx() const {
    return m_idx;
}

int64_t
Identifier::id() const {
    return m_id;
}

void
Identifier::reset_id(int64_t _id) {
    m_id = _id;
}

std::ostream& operator<<(std::ostream& log, const Identifier &ident) {
    log << "id(idx) = " << ident.id() << "(" << ident.idx() << ")";
    return log;
}

}  // namespace pgrouting
