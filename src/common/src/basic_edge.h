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

#ifndef SRC_COMMON_SRC_BASIC_EDGE_H_
#define SRC_COMMON_SRC_BASIC_EDGE_H_

#pragma once
#if defined(__MINGW32__) || defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif


#include "./pgr_types.h"


namespace pgrouting {

class Basic_edge{
 public:
     void cp_members(const Basic_edge &other);

 public:
     int64_t source;
     int64_t target;

     int64_t id;
     double cost;
     bool first;  // originally was true (source, target) false (target, source)
};

}  // namespace pgrouting

#endif  // SRC_COMMON_SRC_BASIC_EDGE_H_
