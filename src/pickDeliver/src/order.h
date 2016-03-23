

#pragma once

#include <iostream>
#include "vehicle_node.h"


class Pgr_pickDeliver;


class Order {
 public:

     Order(ID p_id,
             const Vehicle_node &p_pickup,
             const Vehicle_node &p_deliver,
             const Pgr_pickDeliver &p_problem);



     /*************accesosrs*/

     inline ID id() const {return m_id;};
     const Vehicle_node&  delivery() const;
     const Vehicle_node&  pickup() const;

     /*!
      * An order is valid when:
      *   - The pickup is well formed
      *   - The delivery is well formed
      *   - isCompatibleIJ to go to delivery after inmediatly visiting pickup
      */
     bool is_valid() const;


     bool isOrderCompatibleIJ(const Order &other) const;
     bool isOrderCompatibleStart(const Vehicle_node &node) const;
     bool isOrderCompatibleEnd(const Vehicle_node &node) const;

     friend std::ostream& operator<<(std::ostream&, const Order &);

 //    void  moveOrder(const int toRoute);
 private:

     ID m_id;

     const Vehicle_node &m_pickup;
     const Vehicle_node &m_delivery;

     /* order belongs to this problem */
     const Pgr_pickDeliver &problem;

};
