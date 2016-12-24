/*PGR-GNU*****************************************************************

FILE: solution.h

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

#ifndef SRC_PICKDELIVER_SRC_ORDERS_H
#define SRC_PICKDELIVER_SRC_ORDERS_H
#pragma once

#include <vector>
#include "./../../common/src/pgr_types.h"
#include "./../../common/src/identifiers.hpp"
#include "./pgr_messages.h"
#include "./pd_problem.h"


namespace pgrouting {
namespace vrp {

class Pgr_pickDeliver;
class Order;

class PD_Orders : public Pgr_messages , public PD_problem {
     typedef typename std::vector<Order> Orders;
 public:
     typedef typename Orders::iterator o_iterator;
     typedef typename Orders::const_iterator o_const_iterator;
     typedef size_t OID;
     typedef Identifiers<OID> setof_OID;
 protected:
     Orders m_orders;

 public:
     /* @brief constructor
      *
      * @params [in] p_problem \t pointer to problem
      *
      */
     PD_Orders() = default;

     void build_orders(
             const std::vector<PickDeliveryOrders_t> &pd_orders,
             size_t &node_id 
             );



     bool is_valid(double speed) const;

     //! name vector like functions
     //@{
     Order& operator[](OID o);
     const Order& operator[](OID o) const;
     size_t size() const {return m_orders.size();}
     o_iterator begin() {return m_orders.begin();};
     o_iterator end() {return m_orders.end();};
     o_const_iterator begin() const {return m_orders.begin();};
     o_const_iterator end() const {return m_orders.end();};
     //@}
};


}  //  namespace vrp
}  //  namespace pgrouting

#endif  // SRC_PICKDELIVER_SRC_ORDERS_H
