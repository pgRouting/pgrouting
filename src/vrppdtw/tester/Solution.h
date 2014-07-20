#ifndef  _SOLUTION_H
#define _SOLUTION_H

#include <vector>
#include <map>
#include <queue>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <set>

class Solution{
        public:
                Solution(){
                        route_length=0,cost_total=0;
                }
                //Variables
                int twv_total;
                int cv_total;
                double cost_total;
                std::vector<Route> r;
                int route_length;
                //Methods
                double getCost();
                int getBestofNeoghborhood();

};

double Solution::getCost(){
        return cost_total;
}

#endif 
