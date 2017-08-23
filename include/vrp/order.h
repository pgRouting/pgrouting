/*PGR-GNU*****************************************************************

FILE: order.h

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

#ifndef INCLUDE_VRP_ORDER_H_
#define INCLUDE_VRP_ORDER_H_
#pragma once


#include <iosfwd>
#include "cpp_common/identifier.h"
#include "cpp_common/identifiers.hpp"
#include "vrp/vehicle_node.h"

namespace pgrouting {
namespace vrp {


class Order : public Identifier {
 public:
     /*! @name Constructors
      * @{
      */
     Order(size_t p_id, int64_t p_idx,
             const Vehicle_node &p_pickup,
             const Vehicle_node &p_deliver);

     Order(const Order &) = default;
     /*!@}*/


     /*! @name Accessors
      * @{
      */


     /*! The delivery node identifier
      *
      * It hold's the idx of the node
      */
     const Vehicle_node& delivery() const;

     /*! The delivery node identifier
      *
      * It hold's the idx of the node
      */
     const Vehicle_node& pickup() const;

     /*! Get a subset of the orders that can be placed after @b this order
      
      @dot
      digraph G {
      graph [rankdir=LR];
      this [color=green];
      this -> "this.J * J";
      }
      @enddot

      * @param[in] J set of orders
      * @result ithe set intersection of the @b J orders with the @b compatible_J orders of @b this order
      */
     Identifiers<size_t> subsetJ(const Identifiers<size_t> &J) const;
     Identifiers<size_t> subsetI(const Identifiers<size_t> &I) const;
     /*!@}*/


     /*! @name Modifiers
      * @{
      */

     void set_compatibles(const Order order, double speed);
     /*!@}*/

     /*! @name To be or not to be
      * @{
      */

     /*! @brief validate a pickup/delivery order 
      *
      * An order is valid when:
      *   - The pickup is well formed
      *   - The delivery is well formed
      *   - isCompatibleIJ to go to delivery after inmediatly visiting pickup
      */
     bool is_valid(double speed) const;


     /*! @brief Can order @b I be placed before @b this order?
      *

      @dot
      digraph G {
      graph [rankdir=LR];
      this [color=green];
      "I" -> this;
      }
      @enddot

      @param[in] I order
      @param[in] speed to be used for evaluating the order

      @returns true when order @b I can be placed before @b this order
      */
     bool isCompatibleIJ(const Order &I, double speed) const;
     /*!@}*/



     /*! @name Friends
      * @{
      */

     friend std::ostream& operator << (std::ostream&, const Order &);
     /*!@}*/

 private:
     /*! The pick up node identifier
      *
      * It hold's the idx of the node
      */

     Vehicle_node m_pickup;

     /*! The delivery node identifier
      *
      * It hold's the idx of the node
      */
     Vehicle_node m_delivery;

     /*! Stores all the orders that can be placed after this order
      *

      @dot
      digraph G {
      graph [rankdir=LR];
      this [color=green];
      this -> "{J}";
      }
      @enddot

      @todo TODO
      - compatibility changes based on the speed this is not taking that into account (here) 
      - check where is it talking that into account
      */
     Identifiers<size_t> m_compatibleJ;

     /*! Stores all the orders that can be placed before this order
      *

      @dot
      digraph G {
      graph [rankdir=LR];
      this [color=green];
      "{I}" -> this;
      }
      @enddot

      */
     Identifiers<size_t> m_compatibleI;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_ORDER_H_
