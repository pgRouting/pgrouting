/*PGR-GNU*****************************************************************
File: maximumcardinalitymatching.hpp

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Ignoring directed flag & works only for undirected graph
Copyright (c) 2022 Celia Virginia Vergara Castillo
Mail: vicky at erosion.dev

Function's developer:
Copyright (c) 2016 Andrea Nardelli
Mail: nrd.nardelli at gmail.com

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

#ifndef INCLUDE_MAX_FLOW_MAXIMUMCARDINALITYMATCHING_HPP_
#define INCLUDE_MAX_FLOW_MAXIMUMCARDINALITYMATCHING_HPP_
#pragma once

#include <set>

#include "cpp_common/undirectedNoCostBG.hpp"


namespace pgrouting {
namespace flow {

  std::set<int64_t> maxCardinalityMatch(pgrouting::graph::UndirectedNoCostsBG&);

}  // namespace flow
}  // namespace pgrouting

#endif  // INCLUDE_MAX_FLOW_MAXIMUMCARDINALITYMATCHING_HPP_
