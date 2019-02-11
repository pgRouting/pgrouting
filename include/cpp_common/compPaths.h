/*PGR-GNU*****************************************************************
File: compPaths.h

Class's developer:
Copyright (c) 2018 Celia Virginia Vergara Castillo
Mail: vicky@georepublic.de

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

#ifndef INCLUDE_CPP_COMMON_COMPPATHS_H_
#define INCLUDE_CPP_COMMON_COMPPATHS_H_
#pragma once

#include "cpp_common/basePath_SSEC.hpp"

namespace pgrouting {

/*! \class compPathsLess
 *
 * Compares 2 paths for \b <
 *
 * @returns true when p1 < p2
 *
 * Testing takes place in the following order:
 *
 * - The aggregate cost is compared
 * - The length is compared
 * - The Node id is compared
 *
 */
class compPathsLess {
 public:
     bool operator()(const Path &p1, const Path &p2) const;
};

}  // namespace pgrouting

#endif  // INCLUDE_CPP_COMMON_COMPPATHS_H_
