/*PGR-GNU*****************************************************************

FILE: fleet.h

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

#ifndef INCLUDE_VRP_FLEET_H_
#define INCLUDE_VRP_FLEET_H_
#pragma once

#include <iosfwd>
#include <vector>
#include <memory>
#include <utility>

#include "cpp_common/identifiers.hpp"
#include "vrp/pd_problem.h"
#include "vrp/vehicle_pickDeliver.h"

namespace pgrouting {
namespace vrp {

class Pgr_pickDeliver;
class PD_Orders;

class Fleet {
     friend class PD_problem;

 public:
     typedef std::vector<Vehicle_pickDeliver>::iterator iterator;


 public:
     /*! @name constructor
      * @{
      */
     Fleet() = default;

     Fleet(const std::vector<Vehicle_t> &vehicles, double factor);

     Fleet(const Fleet &fleet);
     /*!@}*/

     Fleet& operator=(const Fleet &fleet);

     void set_compatibles(const PD_Orders &orders);

     bool is_fleet_ok() const;
     bool is_order_ok(const Order &order) const;

     Vehicle_pickDeliver get_truck();
     Vehicle_pickDeliver get_truck(size_t order);

     /*! @name vector like functions
      * @{
      */
     size_t size() const {return m_trucks.size();}
     Vehicle_pickDeliver& operator[](size_t i);
     iterator begin() {return m_trucks.begin();}
     iterator end() {return m_trucks.end();}

     /*!@}*/

     friend std::ostream& operator << (std::ostream &log, const Fleet &v);

 private:
     std::vector<Vehicle_pickDeliver> m_trucks;
     Identifiers<size_t> m_used;
     Identifiers<size_t> m_un_used;

     /*! @brief build the fleet
      *
      * @param[in] vehicles of type Vehicle_t
      */
     bool build_fleet(
             std::vector<Vehicle_t> vehicles,
             double factor);

     void add_vehicle(
             Vehicle_t,
             double factor,
             const Vehicle_node&,
             const Vehicle_node&);

     /** @brief the problem message */
     static Pgr_messages& msg();

     /** The problem */
     static Pgr_pickDeliver* problem;
};


}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_FLEET_H_
