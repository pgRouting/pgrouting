
#pragma once

#include "optimize.h"

class Solution;
class Pgr_pickDeliver;


class Optimize : public Solution {
 public:

     Optimize(int kind,
             const Solution &solution);

     /* @brief decrease_truck
      * 
      * Optimization by decreasing one truck
      */
     void decrease_truck();



};

