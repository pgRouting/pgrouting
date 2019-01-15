/*PGR-GNU*****************************************************************
file: pgr_triangle.cpp

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

#include "alphaShape/pgr_triangle.hpp"

namespace pgrouting {
namespace alphashape {

Pgr_triangle::Pgr_triangle(
             Bpoint p1, Bpoint p2, Bpoint p3) :
    m_p1(p1),
    m_p2(p2),
    m_p3(p3)
{
}

bool
Pgr_triangle::has_point(const Bpoint &p) const {
    return true;
}

bool
Pgr_triangle::has_edge(const Bpoint &p1, const Bpoint &p2) const {
    return true;
}

}  // namespace alphashape
}  // namespace pgrouting
