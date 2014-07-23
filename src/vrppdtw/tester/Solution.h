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
                void UpdateSol();

};
class Neighborhoods{
        public:
                Neighborhoods(){}
                Solution BestSPI(Solution S, customer *c, depot d, Pickup *p, int CustomerLength, int PickupLength);                
};



void Solution::UpdateSol()
{
        route_length=0,cost_total=0,twv_total=0,cv_total=0,dis_total=0;

        for(int i=0;i<route_length;i++)
        {
                twv_total+=r[i].twv;
                dis_total+=r[i].dis;
                cv_total+=r[i].cv;
                cost_total+=r[i].cost();
        }
        return;
}


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
        cost_total=0;
        for(int i=0;i<r.size();i++)
        {
                cost_total+=r[i].dis;
        }
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
        Solution CurrSol,BestSol,TempSol;
        CurrSol=BestSol=S;
        Pickup OrderRequests[1005];
        int Ro_flag,Hc_flag;
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
                int rflag=0;
                //Order Find and Remove it! 
                for(int route_remove=0;route_remove<CurrSol.r.size();route_remove++)
                {                                
                        Ro_flag=CurrSol.r[route_remove].RemoveOrder(OrderRequests[order]);
                        TempSol=CurrSol;
                        if(Ro_flag==1)
                        {
                                //Insert, Hill climb, Total Cost, (if it is more copy back the solution) ,  (else store best=temp, checked=2, break )            
                                for(int route=0;route<CurrSol.r.size();route++)
                                {
                                        TempState=CurrSol.r[route].append(c,OrderRequests[order],d,CustomerLength, PickupLength,TempState);
                                        Hc_flag=CurrSol.r[route].insertOrder(c,d,OrderRequests[order]);
                                        printf("After Inserting into route %d with HC=%d  twv=%d \n",route,Hc_flag,CurrSol.r[route].twv);
                                        CurrSol.r[route].print();
                                        if(Hc_flag==0)
                                        {
                                                //Feasible solution
                                                printf("-->Feasible Solution\n");
                                                printf("-->CurrCost =%lf  BestCost=%lf \n",CurrSol.getCost(),BestSol.getCost());
                                                CurrSol.dump();
                                                if(CurrSol.getCost() > BestSol.getCost())
                                                {
                                                        CurrSol=TempSol;
                                                }
                                                else
                                                {
                                                        BestSol=CurrSol;
                                                        for(int z=0;z<CurrSol.r.size();z++)
                                                        {
                                                                CurrSol.UpdateSol();
                                                                BestSol.r[z]= CurrSol.r[z];

                                                                BestSol.cost_total= CurrSol.cost_total;
                                                                BestSol.dis_total=CurrSol.dis_total;
                                                                BestSol.twv_total=CurrSol.twv_total;
                                                                BestSol.cv_total=CurrSol.cv_total;
                                                        }

                                                        OrderRequests[order].checked=2;
                                                }
                                        }
                                        else
                                        {
                                                CurrSol=TempSol;
                                        }
                                }
                        }
                }
        }
        CurrSol.dump();
        return BestSol;

}

#endif 
