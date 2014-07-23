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
                Solution getBestofNeighborhood(Solution S, customer *c, depot d, Pickup *p, int OrderNumber, double CBest, int CustomerLength, int PickupLength);

};
class Neighborhoods{
        public:
                Neighborhoods(){}
                Solution RemoveOrder(Solution S, customer *c, depot d, Pickup *p, int OrderNumber, double CBest);
                Solution BestSPI(Solution S, customer *c, depot d, Pickup *p, int OrderNumber, double CBest, int CustomerLength, int PickupLength);                
};



//Methods in Solution
double Solution::getCost(){
        return cost_total;
}

Solution Solution::getBestofNeighborhood(Solution S, customer *c, depot d, Pickup *p, int OrderNumber, double CBest, int CustomerLength, int PickupLength){
        printf("twv_total=%lf\n",S.cost_total);
        Neighborhoods N;
        Solution S1;
        S1=N.BestSPI(S,c,d,p, OrderNumber,CBest,CustomerLength,PickupLength);
        return S;
}



Solution Neighborhoods::BestSPI(Solution S, customer *c, depot d, Pickup *p, int OrderNumber, double CBest,int CustomerLength, int PickupLength){
    printf("Printing Pickup's");
    for(int i=0;i<PickupLength;i++){
        printf("PickupOrder Checked Variable =%d\n",p[i].checked);
    }
    return S;
};

#endif 
