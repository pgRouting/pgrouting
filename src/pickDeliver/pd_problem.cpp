/*PGR-GNU*****************************************************************

FILE: pgr_pickDeliver.h

Copyright (c) 2015 pgRouting developers
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


#include "vrp/pd_problem.h"

namespace pgrouting {
namespace vrp {

Pgr_pickDeliver* PD_problem::problem;
Pgr_messages PD_problem::msg;

PD_problem::PD_problem(Pgr_pickDeliver* p_problem) {
    PD_problem::problem = p_problem;
    msg.clear();
}

PD_problem::PD_problem(const PD_problem &) {
}


}  //  namespace vrp
}  //  namespace pgrouting

