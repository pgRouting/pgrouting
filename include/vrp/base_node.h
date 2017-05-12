/*PGR-GNU*****************************************************************

FILE: base_node.h

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

#ifndef INCLUDE_PICKDELIVER_BASE_NODE_H_
#define INCLUDE_PICKDELIVER_BASE_NODE_H_
#pragma once

#include <string>
#include <ostream>
#include <memory>
#include "cpp_common/identifier.h"
#include "vrp/pd_problem.h"

#if defined(__MINGW32__) || defined(_MSC_VER)
#include <stdint.h>
#endif

namespace pgrouting {
namespace vrp {
namespace pickdeliver {

/*! \class Base_node
 * \brief The Base_node class defines the operations that can be performed in a 2D node.
 *
 * A Base_node is an identifier of a Node or Dnode.
 */
class Base_node : public Identifier {
 public:
     /*! @name accessors
     @{
     */

#if 0
     /*! @brief Returns the idx */
     size_t  idx() const;

     /*! @brief Returns the original id */
     int64_t id() const;
#endif
     ///@}

     Base_node() = default;
     Base_node(size_t, int64_t);


     /** @name to be or not to be */
     ///@ {

     virtual bool isSamePos(const Base_node &other) const;
     virtual bool operator ==(const Base_node &rhs) const;

     /*!@}*/

     friend std::ostream& operator<<(
             std::ostream &log,
             const Base_node &node);

     /*! @name distance
     @{
     */
#if 1
     virtual double distance(const Base_node &node) const = 0;
#endif
     virtual double comparable_distance(const Base_node &other) const;
     /*!@}*/


     virtual void print(std::ostream& os) const;

 protected:
#if 0
     size_t m_idx;                ///< internal node number
     int64_t m_id;      ///< user supplied node number
#endif
};

}  //  namespace pickdeliver
}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_PICKDELIVER_BASE_NODE_H_
