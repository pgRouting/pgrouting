/*PGR-GNU*****************************************************************
File: bline.hpp

Copyright (c) 2019 Celia Virginia Vergara Castillo
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

/*! @file */

#ifndef INCLUDE_CPP_COMMON_BLINE_HPP_
#define INCLUDE_CPP_COMMON_BLINE_HPP_
#pragma once

#include <boost/geometry.hpp>

#if BOOST_Geometry_VERSION_OK
#include <boost/geometry/geometries/geometries.hpp>
#else
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <boost/geometry/multi/geometries/multi_linestring.hpp>
#include <boost/geometry/multi/geometries/multi_polygon.hpp>
#endif

namespace bg = boost::geometry;
namespace pgrouting {

using Bpoint = bg::model::d2::point_xy<double>;
using Bpoints = bg::model::multi_point<Bpoint>;

using Bline = bg::model::linestring<Bpoint>;
using Blines = bg::model::multi_linestring<Bline>;

using Bpoly = bg::model::polygon<Bpoint>;
using Bpolys = bg::model::multi_polygon<Bpoly>;


}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_BLINE_HPP_
