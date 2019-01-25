/*PGR-GNU*****************************************************************
file: pgr_alphaShape.hpp

Copyright (c) 2018 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2018 Celia Virginia Vergara Castillo
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

#include "alphaShape/pgr_alphaShape.hpp"

namespace pgrouting {
namespace alphashape {

void
Pgr_alphaShape::clear() {
    m_info.clear();
}

Pgr_alphaShape::Pgr_alphaShape(std::vector<Delauny_t> p_delauny) :
    m_info(p_delauny) {
        log << m_info.get_log();
}

std::ostream&
operator<<(std::ostream& os, const Pgr_alphaShape &a) {
    os << a.m_info;
    return os;
}


}  // namespace alphashape
}  // namespace pgrouting
