/*PGR-GNU*****************************************************************
 *

 Copyright (c) 2015 Celia Virginia Vergara Castillo
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

#pragma once

#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif

#undef Min
#undef Max
#undef sgn
#undef K
#include <CGAL/Simple_cartesian.h>
// #include <boost/geometry.hpp>
// #include <boost/geometry/geometries/point_xy.hpp>
#include <vector>

#include "./pgr_types.h"  


namespace pgRouting {

// typedef typename boost::geometry::model::d2::point_xy<double> Point;
typedef typename CGAL::Simple_cartesian<double> Simple_cartasian;
typedef typename Simple_cartasian::Point_2  Point;


class XY_vertex {
 public:
  XY_vertex() = default;
  XY_vertex(const XY_vertex &) = default;
  XY_vertex(const int64_t _id, double _x, double _y) :
      id(_id), point(_x, _y) {
      }

  XY_vertex(const Pgr_edge_xy_t &other, bool is_source) :
      id(is_source? other.source : other.target),
      point(is_source? Point(other.x1,other.y1) : Point(other.x2,other.y2))
      {}


  double x() const {return point.x();}
  double y() const {return point.y();}

  inline void cp_members(const XY_vertex &other){
      this->id = other.id;
      this->point = other.point;
  };

  friend std::ostream& operator<<(std::ostream& log, const XY_vertex &v);
  bool operator==(const XY_vertex &rhs) const;

 public:
  int64_t id;
  Point point;
};

size_t
check_vertices(std::vector < XY_vertex > vertices);

std::vector < XY_vertex >
extract_vertices(
    const Pgr_edge_xy_t *data_edges, int64_t count);

std::vector < XY_vertex >
extract_vertices(
    const std::vector < Pgr_edge_xy_t > &data_edges);

#if 0
std::vector < XY_vertex > extract_vertices(
    std::vector < XY_vertex > vertices,
    const Pgr_edge_xy_t *data_edges, int64_t count);

std::vector < XY_vertex > extract_vertices(
    std::vector < XY_vertex > vertices,
    const std::vector < Pgr_edge_xy_t > data_edges);
#endif

} // namespace pgRouting
