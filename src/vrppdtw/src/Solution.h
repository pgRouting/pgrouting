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
         cost_total = 0,twv_total = 0,cv_total = 0,dis_total = 0;
     }
     //Variables
     int twv_total;
     int cv_total;
     int dis_total;
     double cost_total;
     std::vector<Route> routes;
#if 0
     size_t route_length;
#endif
     //Methods
     void dump() const;
     double getCost() const ;
     Solution getBestofNeighborhood(const Solution S, const std::vector<Customer> &c, const Depot &d, const std::vector<Pickup> &p /* int CustomerLength, int PickupLength*/) const;
     void UpdateSol();

};

class Neighborhoods{
 public:
     Neighborhoods(){}
     Solution BestSPI(const Solution S, const std::vector<Customer> &c, const Depot &d, const std::vector<Pickup> &pickups);   
};



void
Solution::UpdateSol() {
    cost_total = 0,twv_total = 0,cv_total = 0,dis_total = 0;
#if 0
    int routes_del = 0;
#endif
    routes.erase( std::remove_if(routes.begin(), routes.end(),
            [](const Route &r) 
            {return r.path.empty();}
            ), routes.end());
    for (const auto &r : routes) {
        twv_total  +=  r.twv;
        dis_total  +=  r.dis;
        cv_total   +=  r.cv;
        cost_total +=  r.cost();
    }


#if 0
    for (size_t i = 0; i < route_length; i++) {
        twv_total  +=  r[i].twv;
        dis_total  +=  r[i].dis;
        cv_total   +=  r[i].cv;
        cost_total +=  r[i].cost();
        if(r[i].path.size()  ==  0) {
            routes_del++;
            r.erase(r.begin()+i);   
        }
    }
    route_length = route_length-routes_del;
#endif
    return;
}


//Methods in Solution

#if 0
void Solution::dump(){
    printf("Routes Length = %d   ",route_length);
    printf("Total Cost = %lf   ",cost_total);
    printf("Total Distance = %d   \n",dis_total);
    printf("Routes \n");
    for(int i = 0;i<route_length;i++)
    {
        printf("Route::%d  ",i);
        printf("TWV = %d  CV = %d DIS = %d [ ",r[i].twv,r[i].cv,r[i].dis);
        for(int j = 0;j<r[i].path_length;j++)
        {
            printf("%d ",r[i].path[j]);
        }
        printf("]\n");
    }
    return;
}
#endif

double
Solution::getCost() const{
    double cost_total  =  0;
    for (const auto &r : routes) {
        cost_total +=  r.cost();
    }
#if 0
    for (unsigned int i = 0;i<r.size();i++)
    {
        cost_total+= r[i].cost();
    }
#endif
    return cost_total;
}

Solution
Solution::getBestofNeighborhood(const Solution S, const std::vector<Customer> &customers, const Depot &depot, const std::vector<Pickup> &pickups) const  {
    //    printf("twv_total = %lf\n",S.cost_total);
    Neighborhoods N;
    Solution S1;
    S1  =  N.BestSPI(S, customers, depot, pickups);
    return S1;
}


Solution
Neighborhoods::BestSPI(const Solution S, const std::vector<Customer> &customers, const Depot &depot, const std::vector<Pickup> &pickups) {


    Solution CurrSol,BestSol,TempSol;
    CurrSol = BestSol = S;
    std::vector<Pickup> OrderRequests = pickups;
#if 0
    Pickup *OrderRequests = NULL;
    OrderRequests =  (Pickup *)malloc(2000*sizeof(Pickup));
#endif
    int Ro_flag,Hc_flag;
    State TempState;

#if 0
    //Copy Order requests from pickup's
    for(size_t order = 1;order<= p.size();order++)
    {
        OrderRequests[order] = p[order];
    }
#endif
    //Main SPI
    for(size_t order = 1;order<= pickups.size();order++)
    {
        //Order Find and Remove it! 
        for(unsigned int route_remove = 0;route_remove<CurrSol.routes.size();route_remove++)
        {                                
            Ro_flag = CurrSol.routes[route_remove].RemoveOrder(OrderRequests[order]);
            TempSol = CurrSol;
            if(Ro_flag != 1) continue;
#if 0
            if(Ro_flag == 1)
            {
#endif
                //Insert, Total Cost, (if it is more copy back the solution) ,  (else store best = temp, checked = 2, break )          
#if 0
                for(unsigned int route = 0;route<CurrSol.routes.size();route++)
#endif
                for(auto &route : CurrSol.routes) {
                    TempState = route.append(/*c,*/ OrderRequests[order], /*d,CustomerLength, PickupLength,*/ TempState);
                    Hc_flag = route.insertOrder(customers, depot /*,OrderRequests[order]*/ );
                    // Hc flag tells us about insertion
                    if(Hc_flag == 0) {
#if 0
                        if(route.cost() <=  BestSol.routes[route].cost())
#endif
                        if(route.cost() <=  BestSol.routes[&route-&CurrSol.routes[0]].cost()) {
                            CurrSol.UpdateSol();
                            BestSol = CurrSol;
                        }
                    }
                    TempSol.UpdateSol();
                    CurrSol = TempSol;
                    //   CurrSol  =  BestSol;
                }        
                BestSol.UpdateSol();
                CurrSol  =  BestSol;
                break;
#if 0
            }
            else
            {
                continue;
            }
#endif
        }
    }
    BestSol.UpdateSol();
    return BestSol;
}



#endif 
