/*PGR-GNU*****************************************************************
file: bpoint.cpp

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

#include "cpp_common/bpoint.hpp"

namespace pgrouting {

#if 0
bool
operator<(const Bpoint &p1, const Bpoint &p2){
    auto x1 = boost::geometry::get<0>(p1);
    auto y1 = boost::geometry::get<1>(p1);
    auto x2 = boost::geometry::get<0>(p2);
    auto y2 = boost::geometry::get<1>(p2);
    if (x1 < x2) return true;
    if (x1 > x2) return false;
    if (y1 < y2) return true;
    return false;
}
#endif

}  // namespace pgrouting

