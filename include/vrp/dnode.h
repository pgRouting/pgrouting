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

#ifndef INCLUDE_VRP_DNODE_H_
#define INCLUDE_VRP_DNODE_H_
#pragma once

#include <string>
#include <iosfwd>
#include "cpp_common/identifier.h"
#include "cpp_common/pgr_messages.h"

namespace pgrouting {
namespace vrp {

class Pgr_pickDeliver;
class PD_problem;

/*! @class Dnode
 * @brief The Dnode class defines a the basic operations when data is a matrix.
 *
 * currently needs the PD_problem
 */
class Dnode : public Identifier {
    friend PD_problem;

 public:
     /*! @name constructors
      * @{
      */
     Dnode() = default;
     /*! @brief data constructor */
     Dnode(size_t id, int64_t original_id);
     /*!@}*/



     double distance(const Dnode &other) const;

     friend std::ostream& operator << (std::ostream &log, const Dnode &node);

 protected:
     static Pgr_messages& msg() ;

     /** The problem */
     static Pgr_pickDeliver* problem;

};

}  // namespace vrp
}  // namespace pgrouting

#endif  // INCLUDE_VRP_DNODE_H_
