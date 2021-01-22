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
#include "vrp/dnode.h"
#include "vrp/fleet.h"
#include "vrp/solution.h"
#include "vrp/tw_node.h"
#include "vrp/pd_orders.h"
#include "vrp/vehicle.h"

namespace pgrouting {
namespace vrp {

/** When the problem gets initialized
 *  - The static pointers to the problem are initialized
 */
PD_problem::PD_problem(Pgr_pickDeliver* p) {
    Dnode::problem = p;
    Fleet::problem = p;
    Solution::problem = p;
    Tw_node::problem = p;
    PD_Orders::problem = p;
    Vehicle::problem = p;
}

}  //  namespace vrp
}  //  namespace pgrouting

