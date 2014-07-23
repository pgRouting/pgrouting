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
                void dump();
                double getCost();
                Solution getBestofNeighborhood(Solution S, customer *c, depot d, Pickup *p,int CustomerLength, int PickupLength);

};
class Neighborhoods{
        public:
                Neighborhoods(){}
                Solution BestSPI(Solution S, customer *c, depot d, Pickup *p, int CustomerLength, int PickupLength);                
};



//Methods in Solution

void Solution::dump(){
        printf("Routes Length=%d   ",route_length);
        printf("Total Cost=%lf   ",cost_total);
        printf("Total Distance=%d   \n",dis_total);
        printf("Routes \n",route_length);
        for(int i=0;i<route_length;i++)
        {
                printf("Route::%d   [ ",i);
                for(int j=0;j<r[i].path_length;j++)
                {
                        printf("%d ",r[i].path[j]);
                }
                printf("]\n");
        }
        return;
}
double Solution::getCost(){
        return cost_total;
}

Solution Solution::getBestofNeighborhood(Solution S, customer *c, depot d, Pickup *p,  int CustomerLength, int PickupLength){
    //    printf("twv_total=%lf\n",S.cost_total);
        Neighborhoods N;
        Solution S1;
        S1=N.BestSPI(S,c,d,p,CustomerLength,PickupLength);
        return S1;
}



Solution Neighborhoods::BestSPI(Solution S, customer *c, depot d, Pickup *p,  int CustomerLength, int PickupLength){
                Solution CurrSol,BestSol;
                CurrSol=BestSol=S;
                Pickup OrderRequests[1005];
                        int RO;
                        State TempState;
                //Copy Order requests from pickup's
                for(int order=1;order<=PickupLength;order++)
                {
                        OrderRequests[order]=p[order];
                }
                printf("Routes Size  %d\n",BestSol.r.size());
                
                //Main SPI
                for(int order=PickupLength;order>=1;order--)
                {
                        //Order Find and Remove it! 
                        for(int route=0;route<CurrSol.r.size();route++){                                
                               RO=CurrSol.r[route].RemoveOrder(OrderRequests[order]);
                        }
                }
                return BestSol;

}

#endif 
