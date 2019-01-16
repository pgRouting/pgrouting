/*PGR-GNU*****************************************************************
file: pgr_delauny.cpp

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

#include "alphaShape/pgr_delauny.hpp"

#include <vector>

namespace pgrouting {
namespace alphashape {

Pgr_delauny::Pgr_delauny(
             const std::vector<Bpoint> &p_points,
             const std::vector<Delauny_t> &p_delauny) :
    m_points(p_points),
    m_delauny(p_delauny) {
       for (size_t i = 0; i < m_points.size(); ++i) {
           m_relation.insert(std::pair<size_t, std::vector<Pgr_triangle*> >(i, std::vector<Pgr_triangle*>()));
       }

       for (size_t i = 0; i < m_delauny.size(); i = i + 3) {
           Bpoint p1(m_delauny[i].x, m_delauny[i].y);
           //which is the point id?
       }
}

void
Pgr_delauny::clear() {
    m_relation.clear();
    m_triangles.clear();
    m_points.clear();
}

}  // namespace alphashape
}  // namespace pgrouting
