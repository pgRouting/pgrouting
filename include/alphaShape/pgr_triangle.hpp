/*PGR-GNU*****************************************************************
file: pgr_triangle.hpp

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

#ifndef INCLUDE_ALPHASHAPE_PGR_TRIANGLE_HPP_
#define INCLUDE_ALPHASHAPE_PGR_TRIANGLE_HPP_
#pragma once

#include "cpp_common/bpoint.hpp"
#include <iosfwd>
#include "cpp_common/pgr_messages.h"

namespace pgrouting {
namespace alphashape {


class Pgr_triangle : public Pgr_messages {
 public:
     Pgr_triangle() = default;
     Pgr_triangle(Bpoint p1, Bpoint p2, Bpoint p3);

     bool has_point(const Bpoint &p) const;
     bool has_edge(const Bpoint &p1, const Bpoint &p2) const;

     friend std::ostream& operator<<(std::ostream&, const Pgr_triangle&);

 protected:
     Bpoint m_p1;
     Bpoint m_p2;
     Bpoint m_p3;
};


}  // namespace alphashape
}  // namespace pgrouting

#endif  // INCLUDE_ALPHASHAPE_PGR_TRIANGLE_HPP_
