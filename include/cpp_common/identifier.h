/*PGR-GNU*****************************************************************
 *

Copyright (c) 2017 Celia Virginia Vergara Castillo
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

#ifndef INCLUDE_CPP_COMMON_IDENTIFIER_H_
#define INCLUDE_CPP_COMMON_IDENTIFIER_H_
#pragma once

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <stdint.h>
#endif

#include <ostream>

namespace pgrouting {

class Identifier {
 public:
     Identifier() = default;
     Identifier(const Identifier &v) = default;
     Identifier(const size_t _idx, const int64_t _id);

     int64_t id() const;
     size_t idx() const;
     void reset_id(int64_t);

     friend std::ostream& operator<<(std::ostream& log, const Identifier&);

 private:
     size_t  m_idx;
     int64_t m_id;
};


}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_IDENTIFIER_H_
