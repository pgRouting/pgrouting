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

class Fleet : public PD_problem {
 public:
     typedef std::vector<Vehicle_pickDeliver>::iterator iterator;
     std::vector<Vehicle_pickDeliver> m_trucks;


 public:
     /*! @name constructor
      * @{
      */
     Fleet() = default;

     Fleet(const std::vector<Vehicle_t> &vehicles, double factor);

     Fleet(const Fleet &fleet);
     /*!@}*/

     /* TODO move code to .cpp */
     Fleet& operator=(const Fleet &fleet) {
         m_trucks = fleet.m_trucks;
         used = fleet.used;
         un_used = fleet.un_used;
         return *this;
     }

     void set_compatibles(const PD_Orders &orders);

     bool is_fleet_ok() const;
     bool is_order_ok(const Order &order) const;

     Vehicle_pickDeliver get_truck();
     void release_truck(size_t id);
     Vehicle_pickDeliver get_truck(size_t order);
     Vehicle_pickDeliver get_truck(const Order order);

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
     Identifiers<size_t> used;
     Identifiers<size_t> un_used;

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
             std::unique_ptr<Base_node>,
             const Vehicle_node&,
             std::unique_ptr<Base_node>,
             const Vehicle_node&);

     template <typename T> std::unique_ptr<Base_node> create_b_start(
             const Vehicle_t &vehicle,
             size_t node_id) {
         std::unique_ptr<Base_node> b_start(new T(
                     node_id,
                     vehicle.start_node_id,
                     vehicle.start_x,
                     vehicle.start_y));
         return std::move(b_start);
     }

     template <typename T> std::unique_ptr<Base_node> create_b_end(
             const Vehicle_t &vehicle,
             size_t node_id) {
         std::unique_ptr<Base_node> b_end(new T(
                     node_id,
                     vehicle.end_node_id,
                     vehicle.end_x,
                     vehicle.end_y));
         return std::move(b_end);
     }
};


}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_FLEET_H_
