/*PGR-GNU*****************************************************************
File: info_t.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2015 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

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

#ifndef INCLUDE_C_TYPES_INFO_T_HPP_
#define INCLUDE_C_TYPES_INFO_T_HPP_
#pragma once

#include <cstdint>
#include <string>

namespace pgrouting {

enum expectType {
    ANY_INTEGER,
    ANY_NUMERICAL,
    TEXT,
    CHAR1,
    ANY_INTEGER_ARRAY
};


struct Column_info_t {
    int colNumber;
    uint64_t type;
    bool strict;
    std::string name;
    expectType eType;
};

}  // namespace pgrouting

#endif  // INCLUDE_C_TYPES_INFO_T_HPP_
