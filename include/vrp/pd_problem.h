/*PGR-GNU*****************************************************************

FILE: pd_problem.h

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

#ifndef INCLUDE_VRP_PD_PROBLEM_H_
#define INCLUDE_VRP_PD_PROBLEM_H_
#pragma once

#include "cpp_common/pgr_messages.h"
#include "vrp/initials_code.h"



namespace pgrouting {
namespace vrp {

class Pgr_pickDeliver;

class PD_problem {
 public:
    explicit PD_problem(Pgr_pickDeliver* p_problem);
    PD_problem(const PD_problem &problem);
    PD_problem() { }

 public:
     static Pgr_messages msg;

 protected:
     static Pgr_pickDeliver* problem;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_PD_PROBLEM_H_
