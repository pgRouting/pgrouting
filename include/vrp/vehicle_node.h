/*PGR-GNU*****************************************************************

FILE: vehicle_node.h

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

#ifndef INCLUDE_VRP_VEHICLE_NODE_H_
#define INCLUDE_VRP_VEHICLE_NODE_H_
#pragma once


#include <string>

#include "vrp/tw_node.h"

namespace pgrouting {
namespace vrp {


/*! \class Vehicle_node;
 * \brief Extend Tw_node to evaluate the vehicle at node level
 *
 * This class extends Twnode by adding attributes to store information
 * about the node in a path and provides the tools evaluate the node
 * and to set and get these attribute values.
 */
class Vehicle_node: public Tw_node {
 public:
     /** @name log */
     ///@ {

     friend std::ostream& operator<<(
             std::ostream &log, const Vehicle_node &node);

     ///@}

     /** @name Node evaluation accessors */
     ///@ {

     /*! \brief Truck's travel_time from previous node to this node. */
     inline double travel_time() const {return m_travel_time;}

     /*! \brief Truck's arrival_time to this node. */
     inline double arrival_time() const {return m_arrival_time;}

     /*! \brief Truck's wait_time at this node. */
     inline double wait_time() const {return m_wait_time;}

     /*! \brief Truck's departure_time from this node. */
     inline double departure_time() const {return m_departure_time;}

     /*! \brief delta_time = departure_time(this) - departure_time(previous) */
     inline double delta_time() const {return m_delta_time;}

     ///@}






     /** @name Accumulated evaluation accessors */
     ///@ {

     /*! \brief Truck's total times it has violated time windows. */
     inline int twvTot() const {return m_twvTot;}

     /*! \brief Truck's total times it has violated cargo limits. */
     inline int cvTot() const {return m_cvTot;}

     /*! \brief Truck's total cargo after the node was served. */
     inline double cargo() const {return m_cargo;}

     /*! \brief Truck's travel duration up to this node. */
     inline double total_time() const {return m_departure_time;}

     /*! \brief _time spent moving between nodes by the truck */
     inline double total_travel_time() const {return m_tot_travel_time;}

     /*! \brief _time spent by the truck waiting for nodes to open */
     inline double total_wait_time() const {return m_tot_wait_time;}

     /*! \brief _time spent by the truck servicing the nodes */
     inline double total_service_time() const {return m_tot_service_time;}

     ///@}

     /*! \brief True when \b arrival_time + \b delta_time generates TWV.*/
     bool deltaGeneratesTWV(double delta_time) const;

     /** @name State */
     ///@ {

     /*! \brief True when the total count for violations are 0 */
     bool feasible() const {return m_twvTot == 0 &&  m_cvTot == 0;}

     /*! \brief True doesn't have twc nor cv (including total counts) */
     bool feasible(double cargoLimit) const {
         return feasible() &&  !has_twv() &&  !has_cv(cargoLimit);
     }

     /*! \brief True when at this node does not violate time windows */
     bool has_twv() const {
         return is_late_arrival(m_arrival_time);
     }

     /*! \brief True when not violation
      *
      * Ending's or start's cargo should be 0
      **/
     bool has_cv(double cargoLimit) const {
         return is_end() ||  is_start() ? m_cargo != 0
             : m_cargo > cargoLimit ||  m_cargo < 0;
     }
     ///@}

     /** @name mutators */
     ///@ {
     /*! @todo TODO evaluate with matrix also*/
     void evaluate(double cargoLimit);
     void evaluate(const Vehicle_node &pred, double cargoLimit, double speed);
     ///@}

     /** @name Document*/
     ///@ {

     /*! \brief returns the arrval time at \b this visited after \b other */
     double arrival_i_arrives_j(
             const Vehicle_node &other,
             double speed) const;

     ///@}



     /*! \brief Construct from parameters */
     Vehicle_node() : Tw_node() {}
     Vehicle_node(const Vehicle_node &) = default;
     explicit Vehicle_node(const Tw_node &node);

 private:
     /** @name Node evaluation members */
     ///@ {

     double m_travel_time;     ///< Travel time from last node
     double m_arrival_time;    ///< Arrival time at this node
     double m_wait_time;       ///< Wait time at this node when early arrival
     double m_departure_time;  // /< Departure time from this node
     double m_delta_time;      ///< Departure time - last nodes departure time

     ///@}

     /** @name Accumulated evaluation members */
     ///@ {

     double m_cargo;           ///< Accumulated cargo
     int    m_twvTot;          ///< Total count of TWV
     int    m_cvTot;           ///< Total count of CV
     double m_tot_wait_time;     ///< Accumulated wait time
     double m_tot_travel_time;   ///< Accumulated travel time
     double m_tot_service_time;  // /< Accumulated service time

     ///@}
};

}  //  namespace vrp
}  //  namespace pgrouting

#endif  // INCLUDE_VRP_VEHICLE_NODE_H_
