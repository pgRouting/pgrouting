
#pragma once

#include <set>

class Solution;
class Pgr_pickDeliver;


class Initial_solution : public Solution {
 public:
     Initial_solution(
             int kind,
             const Pgr_pickDeliver *problem);
     /*
      * one truck per order
      */
     void one_truck_per_order();
     void one_truck_all_orders();
     void push_back_while_feasable();
     void push_front_while_feasable();
     void insert_while_feasable();
     void invariant() const;



 private:
     std::set<ID> all_orders;
     std::set<ID> unassigned;
     std::set<ID> assigned;

};

