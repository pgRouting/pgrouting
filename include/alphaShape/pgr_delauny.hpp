/*PGR-GNU*****************************************************************
file: pgr_delauny.hpp

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

#ifndef INCLUDE_ALPHASHAPE_PGR_DELAUNY_HPP_
#define INCLUDE_ALPHASHAPE_PGR_DELAUNY_HPP_
#pragma once

#include <vector>
#include <map>

#include "c_types/delauny_t.h"
#include "cpp_common/bpoint.hpp"
#include "alphaShape/pgr_triangle.hpp"

namespace pgrouting {
namespace alphashape {


class Pgr_delauny {
 public:
     Pgr_delauny() = default;
     Pgr_delauny(
             const std::vector<Bpoint> &points,
             const std::vector<Delauny_t> &triangles);

     void clear();

 private:
     std::vector<Bpoint> m_points;
     std::vector<Delauny_t> m_delauny;
     std::vector<Pgr_triangle> m_triangles;
     std::map<size_t, std::vector<Pgr_triangle*> > m_relation;
};


}  // namespace alphashape
}  // namespace pgrouting

#endif  // INCLUDE_ALPHASHAPE_PGR_DELAUNY_HPP_
