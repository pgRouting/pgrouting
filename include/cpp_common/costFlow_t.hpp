/*PGR-GNU*****************************************************************
File: costFlow_t.hpp

Copyright (c) 2018 Maoguang Wang
Mail: xjtumg1007@gmail.com

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

#ifndef INCLUDE_CPP_COMMON_COSTFLOW_T_HPP_
#define INCLUDE_CPP_COMMON_COSTFLOW_T_HPP_
#pragma once

#include <cstdint>

struct CostFlow_t {
  int64_t edge_id;
  int64_t source;
  int64_t target;
  int64_t capacity;
  int64_t reverse_capacity;
  double cost;
  double reverse_cost;
};

#endif  // INCLUDE_CPP_COMMON_COSTFLOW_T_HPP_
