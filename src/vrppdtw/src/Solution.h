/*PGR-GNU*****************************************************************

Copyright (c) 2014 Manikata Kondeti
mani.iiit123@gmail.com

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

********************************************************************PGR-GNU*/

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

#include "./Route.h"

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
        cost_total=0,twv_total=0,cv_total=0,dis_total=0;
        int routes_del=0;
        for(int i=0;i<route_length;i++)
        {
                twv_total+=r[i].twv;
                dis_total+=r[i].dis;
                cv_total+=r[i].cv;
                cost_total+=r[i].cost();
                if(r[i].path_length==0)
                {
                        routes_del++;
                        r.erase(r.begin()+i);   
                }
        }
        route_length=route_length-routes_del;
        return;
}


//Methods in Solution

#if 0
void Solution::dump(){
        printf("Routes Length=%d   ",route_length);
        printf("Total Cost=%lf   ",cost_total);
        printf("Total Distance=%d   \n",dis_total);
        printf("Routes \n");
        for(int i=0;i<route_length;i++)
        {
                printf("Route::%d  ",i);
                printf("TWV=%d  CV=%d DIS=%d [ ",r[i].twv,r[i].cv,r[i].dis);
                for(int j=0;j<r[i].path_length;j++)
                {
                        printf("%d ",r[i].path[j]);
                }
                printf("]\n");
        }
        return;
}
#endif

double Solution::getCost(){
        cost_total=0;
        for(unsigned int i=0;i<r.size();i++)
        {
                cost_total+=r[i].cost();
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
        Pickup *OrderRequests=NULL;
        OrderRequests= (Pickup *)malloc(2000*sizeof(Pickup));
        int Ro_flag,Hc_flag;
        State TempState;
        //Copy Order requests from pickup's
        for(int order=1;order<=PickupLength;order++)
        {
                OrderRequests[order]=p[order];
        }

        //Main SPI
        for(int order=1;order<=PickupLength;order++)
        {
                //Order Find and Remove it! 
                for(unsigned int route_remove=0;route_remove<CurrSol.r.size();route_remove++)
                {                                
                        Ro_flag=CurrSol.r[route_remove].RemoveOrder(OrderRequests[order]);
                        TempSol=CurrSol;
                        if(Ro_flag==1)
                        {
                                //Insert, Total Cost, (if it is more copy back the solution) ,  (else store best=temp, checked=2, break )          
                                for(unsigned int route=0;route<CurrSol.r.size();route++)
                                {
                                        TempState=CurrSol.r[route].append(c,OrderRequests[order],d,CustomerLength, PickupLength,TempState);
                                        Hc_flag=CurrSol.r[route].insertOrder(c,d,OrderRequests[order]);
                                        // Hc flag tells us about insertion
                                        if(Hc_flag==0)
                                        {
                                                if(CurrSol.r[route].cost() <= BestSol.r[route].cost())
                                                {
                                                        CurrSol.UpdateSol();
                                                        BestSol=CurrSol;
                                                }
                                        }
                                        TempSol.UpdateSol();
                                        CurrSol=TempSol;
                                        //   CurrSol = BestSol;
                                }        
                                BestSol.UpdateSol();
                                CurrSol = BestSol;
                                break;
                        }
                        else
                        {
                                continue;
                        }
                }
        }
        BestSol.UpdateSol();
        return BestSol;
}



#endif 
