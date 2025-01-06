/*PGR-GNU*****************************************************************
File: componentsResult.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2017 Maoguang Wang
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

#ifndef INCLUDE_COMPONENTS_COMPONENTSRESULT_HPP_
#define INCLUDE_COMPONENTS_COMPONENTSRESULT_HPP_
#pragma once

#include <vector>
#include <cstdint>

#include "c_types/ii_t_rt.h"


namespace pgrouting {
namespace algorithms {

namespace detail {

std::vector<II_t_rt>
componentsResult(
        std::vector<std::vector<int64_t>> &components);

}  // namespace detail

}  // namespace algorithms
}  // namespace pgrouting

#endif  // INCLUDE_COMPONENTS_COMPONENTSRESULT_HPP_
