/*PGR-GNU*****************************************************************

FILE: dnode.h

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

/*! @file dnode.h */

#ifndef SRC_PICKDELIVER_SRC_DNODE_H_
#define SRC_PICKDELIVER_SRC_DNODE_H_
#pragma once

#include <string>
#include <ostream>
#include "../include/base_node.h"
#include "./pd_problem.h"

namespace pgrouting {
namespace vrp {

/*! @class Dnode
 * @brief The Dnode class defines a the operations when its a matrix.
 *
 */

class Dnode : public Base_node, public PD_problem {
 public:
     Dnode() = default;
     Dnode(size_t id, int64_t original_id);


     using Base_node::isSamePos;
     using Base_node::operator==;
     /*! @name to be or not to be
     @{
     bool isSamePos(const Dnode &other) const;
     bool operator==(const Dnode &rhs) const;
     @}*/

     double distance(const Dnode &other) const;
     double comparable_distance(const Dnode &other) const;

     friend std::ostream& operator << (std::ostream &log, const Dnode &node);
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // SRC_PICKDELIVER_SRC_NODE_H_
