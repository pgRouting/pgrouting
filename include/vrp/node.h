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

#ifndef INCLUDE_VRP_NODE_H_
#define INCLUDE_VRP_NODE_H_
#pragma once

#include <iosfwd>
#include "cpp_common/bpoint.hpp"
#include "vrp/base_node.h"

namespace pgrouting {
namespace vrp {

/*! @class Node
 * @brief The Node class defines a point in 2D space with an id.
 *
 * A Node is a point that defines a location in 2D space. It maintains
 * a user @c id and an internal @c nid along with its @c x, @c y location.
 * This is the base object that things like depots, customer locations, etc.
 * are built upon.
 *
 */
class Node : public Base_node {
 public:
     Node() = default;
     Node(size_t id, int64_t original_id, double _x, double _y);


     /** @name to be or not to be */
     /*!@{*/
     bool operator ==(const Node &rhs) const;
     /*!@}*/

     /** @brief Calculates distance between two Node */
     double distance(const Base_node *) const;

     friend std::ostream& operator << (std::ostream &log, const Node &node);

 private:
     double distance(const Node &) const;
     void print(std::ostream& os) const;

 private:
     Bpoint m_point;
};

}  // namespace vrp
}  // namespace pgrouting

#endif  // INCLUDE_VRP_NODE_H_
