

#pragma once

#include "vehicle_node.h"

class Order {
 public:

     Order(ID p_id, const Tw_node &p_pickup, const Tw_node &p_deliver);



     /*************accesosrs*/

     inline ID id() const {return m_id;};
     Vehicle_node  delivery() const;
     Vehicle_node  pickup() const;

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

     Vehicle_node m_pickup;
     Vehicle_node m_delivery;

};
