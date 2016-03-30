
#pragma once

#include <set>

class Solution;
class Pgr_pickDeliver;


class Initial_solution : public Solution {
 public:
     Initial_solution(
             int kind,
             const Pgr_pickDeliver *problem);

     void invariant() const;

 private:
     /*
      * one truck per order
      */
     void one_truck_per_order();
     void one_truck_all_orders();
     void push_back_while_feasable();
     void push_front_while_feasable();
     void insert_while_feasable();

     void insert_while_compatibleJ();
     void fill_truck_while_compatibleJ(
             Vehicle_pickDeliver &truck,
             std::set<ID> &possible_orders);
     std::deque<ID> first_ordersIJ() const;

     void insert_while_compatibleI();
     void fill_truck_while_compatibleI(
             Vehicle_pickDeliver &truck,
             std::set<ID> &possible_orders);
     std::deque<ID> first_ordersJI() const;


 private:
     std::set<ID> all_orders;
     std::set<ID> unassigned;
     std::set<ID> assigned;

};

