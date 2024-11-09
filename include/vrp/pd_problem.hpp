/*PGR-GNU*****************************************************************
File: pd_problem.hpp

Copyright (c) 2017 pgRouting developers
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

/*! @file */

#ifndef INCLUDE_VRP_PD_PROBLEM_HPP_
#define INCLUDE_VRP_PD_PROBLEM_HPP_
#pragma once

#include "cpp_common/messages.hpp"
#include "vrp/initials_code.hpp"



namespace pgrouting {
namespace vrp {

class Pgr_pickDeliver;

class PD_problem {
 public:
    /** @brief Initializing the problem pointer */
    explicit PD_problem(Pgr_pickDeliver* p_problem);

    /** @brief Not wllowing to copy the problem */
    PD_problem(const PD_problem &problem) = delete;

    /** @brief Not allwing initialization without information */
    PD_problem() = delete;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_PD_PROBLEM_HPP_
