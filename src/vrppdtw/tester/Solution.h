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
                        route_length=0,cost_total=0,twv_total=0,cv_total=0,dis_total=0;
                }
                //Variables
                int twv_total;
                int cv_total;
                int dis_total;
                double cost_total;
                std::vector<Route> r;
                int route_length;
                //Methods
                double getCost();
                Solution getBestofNeighborhood(Solution S);

};

double Solution::getCost(){
        return cost_total;
}


Solution Solution::getBestofNeighborhood(Solution S){
        printf("twv_total=%lf\n",S.cost_total);
        return S;
}


#endif 
