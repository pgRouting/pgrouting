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

#ifndef SRC_PICKDELIVER_SRC_ORDER_H_
#define SRC_PICKDELIVER_SRC_ORDER_H_
#pragma once


#include <set>
#include <iostream>
#include "./vehicle_node.h"
#include "./../../common/src/identifiers.hpp"

namespace pgrouting {
namespace vrp {


class Pgr_pickDeliver;
class Initial_solution;


class Order {
 public:
     friend class Initial_solution;
     friend class Pgr_pickDeliver;

     Order(size_t p_id,
             const Vehicle_node &p_pickup,
             const Vehicle_node &p_deliver,
             const Pgr_pickDeliver *p_problem);

     Order(const Order &) = default;



     /*************accesosrs*/

     inline size_t id() const {return m_id;}
     const Vehicle_node& delivery() const;
     const Vehicle_node& pickup() const;
#if 0
     void setCompatibles(double speed);
#endif
     void set_compatibles(const Order order, double speed);

     /*!
      * An order is valid when:
      *   - The pickup is well formed
      *   - The delivery is well formed
      *   - isCompatibleIJ to go to delivery after inmediatly visiting pickup
      */
     bool is_valid(double speed) const;


     bool isCompatibleIJ(const Order &other, double speed) const;
#if 0
     bool isOrderCompatibleStart(const Vehicle_node &node) const;
     bool isOrderCompatibleEnd(const Vehicle_node &node) const;
#endif
     Identifiers<size_t> subsetJ(const Identifiers<size_t> &J) const;
     Identifiers<size_t> subsetI(const Identifiers<size_t> &I) const;

     friend std::ostream& operator << (std::ostream&, const Order &);

     //    void moveOrder(const int toRoute);
 private:
     size_t m_id;

     size_t pickup_id;
     size_t delivery_id;

     /*
      * I = this
      * I -> {J}
      */
     Identifiers<size_t> m_compatibleJ;

     /*
      * J = this
      * {I} -> J
      */
     Identifiers<size_t> m_compatibleI;


     /* order belongs to this problem */
     const Pgr_pickDeliver *problem;
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // SRC_PICKDELIVER_SRC_ORDER_H_
