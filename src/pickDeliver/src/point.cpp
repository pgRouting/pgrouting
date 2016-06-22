/*PGR-GNU*****************************************************************

FILE: point.cpp

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

#include "./point.h"

namespace pgRouting {
namespace vrp {

Point::Point(double x, double y)
    : point(x, y) {
    }

double Point::x() const {return point.x();}

double Point::y() const {return point.y();}



std::ostream& operator << (std::ostream& log, const Point &p) {
    log << "(" << p.point.x() << ", " << p.point.y() << ")";
    return log;
}

bool
Point::operator ==(const Point &rhs) const {
    if (&rhs == this) return true;
    return boost::geometry::equals(point, rhs.point);
}

}  //  namespace vrp
}  //  namespace pgRouting
