/*PGR-GNU*****************************************************************

FILE: point.h

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

#pragma once

#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>

namespace pgRouting {
namespace vrp {

class Point {
  public:
      Point(double x, double y);
      Point(const Point &) = default;

      double x() const;
      double y() const;

      bool operator ==(const Point &rhs) const;

      friend
          std::ostream& operator << (std::ostream& log, const Point &p);

      template <typename T>
          double distance(const T&p2) const {
                  return boost::geometry::distance(point, p2);
          }

      template <typename T>
          double comparable_distance(const T&p2) const {
                  return boost::geometry::comparable_distance(point, p2);
          }



  protected:
      boost::geometry::model::d2::point_xy<double> point;
};

}  //  namespace vrp
}  //  namespace pgRouting
