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

#ifndef SRC_PICKDELIVER_SRC_FLEET_H
#define SRC_PICKDELIVER_SRC_FLEET_H
#pragma once

#include <deque>
#include <tuple>
#include <string>
#include <vector>


#include "./vehicle_pickDeliver.h"
#include "./../../common/src/identifiers.hpp"
namespace pgrouting {
namespace vrp {

class Pgr_pickDeliver;

class Fleet {
#if 0
    friend Pgr_pickDeliver;
#endif
     Pgr_pickDeliver* problem;
     typedef typename std::vector<Vehicle_pickDeliver> Trucks;
 public:
     typedef typename Trucks::iterator iterator;
     Trucks m_trucks;
 protected:
     Identifiers<size_t> used; 
     Identifiers<size_t> un_used; 



 public:
     /* @brief constructor
      *
      * @params [in] p_problem \t pointer to problem
      *
      */
     explicit Fleet(Pgr_pickDeliver *p_problem) :
         problem(p_problem) {}

     void build_fleet(
             const std::vector<Vehicle_t> &vehicles,
             size_t &node_id 
             );



     bool is_fleet_ok() const;
     bool is_order_ok(const Order &order) const;

     //! name vector like functions
     //@{
     size_t size() const {return m_trucks.size();}
     Vehicle_pickDeliver& operator[](size_t i);
     iterator begin() {return m_trucks.begin();};
     iterator end() {return m_trucks.end();};
     //@}
#if 0
     std::string cost_str() const;
     std::string tau(const std::string &title = "Tau") const;
     friend std::ostream& operator<<(
             std::ostream &log,
             const Fleet &solution);
     bool operator <(const Fleet &s_rhs) const;

     bool is_feasable() const;
     double duration() const;
     double wait_time() const;
     double total_travel_time() const;
     double total_service_time() const;
     int twvTot() const;
     int cvTot() const;
     size_t fleet_size() const {return fleet.size();}
     Vehicle::Cost cost() const;
#endif
};


}  //  namespace vrp
}  //  namespace pgrouting

#endif  // SRC_PICKDELIVER_SRC_FLEET_H
