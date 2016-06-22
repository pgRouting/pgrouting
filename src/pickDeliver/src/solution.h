#pragma once

#include <deque>

#include "vehicle_pickDeliver.h"
#include "order.h"

class Pgr_pckDeliver;

class Solution {
 protected:

     const double EPSILON = 0.001;
     std::deque<Vehicle_pickDeliver> fleet;

     /* this solution belongs to this problem*/
     const Pgr_pickDeliver *problem;


 public:

     typedef std::tuple< int, int, int, double, double > Cost;

     void get_postgres_result(
             std::vector< General_vehicle_orders_t > &result) const;


     /* @brief constructor
      *
      * @params [in] p_problem \t pointer to problem
      *
      */
     explicit Solution(const Pgr_pickDeliver *p_problem) : 
         problem(p_problem)
    {}; 


     /* @brief move constructor */
     Solution(const Solution &&sol)  noexcept :
         fleet(std::move(sol.fleet)),
         problem(std::move(sol.problem))
         {};

     /* @brief copy constructor */
     Solution(const Solution &sol) :
         fleet(sol.fleet),
         problem(sol.problem)
    {};

     /* @brief move assigment */
     Solution& operator=(const Solution&& sol) {
         fleet = sol.fleet;
         return *this;
     };

     /* @brief copy assigment */
     Solution& operator=(const Solution& sol) {
         fleet = sol.fleet;
         return *this;
     };

     std::string cost_str() const;
     std::string tau() const;
     friend std::ostream& operator<<(std::ostream &log, const Solution &solution);
     bool operator <(const Solution &s_rhs) const;

     double duration() const;
     double wait_time() const;
     int twcTot() const;
     int cvTot() const;
     size_t fleet_size() const {return fleet.size();};

     /*
      * Cost in terms of a tuple
      *  <0> duration
      *  <1> wait_time
      *  <2> fleet size TODO
      *  <3> time window violations
      *  <4> capacity violations
      */
     Cost cost() const;




};


