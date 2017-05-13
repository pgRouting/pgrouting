/*PGR-GNU*****************************************************************

FILE: node.h

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

/*! @file */

#ifndef INCLUDE_PICKDELIVER_NODE_H_
#define INCLUDE_PICKDELIVER_NODE_H_
#pragma once

#include <string>
#include <ostream>
#include "vrp/base_node.h"
#include "cpp_common/xy_vertex.h"
#include "vrp/pd_problem.h"

namespace pgrouting {
namespace vrp {
namespace pickdeliver {

/*! @class Node
 * @brief The Node class defines a point in 2D space with an id.
 *
 * A Node is a point that defines a location in 2D space. It maintains
 * a user @c id and an internal @c nid along with its @c x, @c y location.
 * This is the base object that things like depots, customer locations, etc.
 * are built upon.
 *
 */
class Node : public Base_node , public PD_problem {
 public:
     Node() = default;
     Node(size_t id, int64_t original_id, double _x, double _y);


     /** @name state */
     ///@ {

     bool isSamePos(const Node &other) const;

     bool operator ==(const Node &rhs) const;

     double distance(const Node &) const;
     double distance(const Base_node *) const;
     double distance(const Base_node &) const;
     double comparable_distance(const Node &other) const;

     friend std::ostream& operator << (std::ostream &log, const Node &node);
     void print(std::ostream& os) const;

 protected:
     pgrouting::Point m_point;
#if 0
     size_t m_id;                ///< internal node number
     int64_t m_original_id;      ///< user supplied node number
#endif
};

}  // namespace pickdeliver
}  // namespace vrp
}  // namespace pgrouting

#endif  // INCLUDE_PICKDELIVER_NODE_H_
