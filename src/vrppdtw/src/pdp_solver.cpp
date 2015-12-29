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

/*                                 

 *****list of files in this dir*******
 pdp.cpp  --> Main solver
 pdp.h    ---> Structures defined in this header file
 Solution.h  -----> It contains the Solution Class and Code related to Neighborhoods  
 Route.h     -----> Explains all about Route Class.
 pdp.c --->  Contains all the details on pgRouting integration.

 The main problem is in two steps. 1.)Getting the initial solutiion and 2.)Optimizing it.

 1.) "Initial solution":
 A few heuristics are applied to find a feasible initial solution. Sequential Construction and Hill climbing. More implementation details are found here:: https://github.com/pgRouting/pgrouting/wiki/VRP-Pickup-Delivery-Problem 

 2.) "Optimizing the Solution":
 A reactive tabu search is applied on the initial solution to get a feasible optimized solution. TabuSearch comes under local search methods. We have three neighborhoods 
                       i) Single Paired Insertion  
                       ii) Swapping pairs between routes  
                       iii)Within Route Insertion. 
                        Tabu attributes plays an important role in giving the best solution(it includes TabuLength, A vector containing feasible solutions and a counter for number of solutions). 
                        Reactive part discussed in the paper is to modify TabuLength based on the solution cycle. 

 */
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


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
//Headers Include
#include "./pdp.h"
#include "./Solution.h"
#include "./Route.h"


#if 0
int PickupLength = 0;

//Depot 
//Vehicle
//Customer Data
Customer *c = NULL;
Pickup *p = NULL;
#endif
int len = 0;

#if 0
int CustomerLength;
#endif

std::vector<Solution> T;

Route *r = NULL;
//Definitions for a few functions 
int TabuSearch( std::vector<Customer> &c, Depot &d , std::vector<Pickup> &p);
//Vector containing solutions

//Initial Solution
Solution S0;

void result_struct();
int Solver(Customer *c1, int total_tuples, int VehicleLength, int capacity , char **msg, path_element **results, int *length_results_struct)
{
/*
 * capacity is not being used
 */

    std::vector<Customer> customers(c1 + 1, c1 + total_tuples);
    std::vector<Pickup> pickups;
    std::vector<Route> routes;
    Depot depot({c1[0].id, c1[0].x, c1[0].y,
            c1[0].demand,
            c1[0].Etime, c1[0].Ltime, c1[0].Stime,
            c1[0].Pindex, c1[0].Dindex
            });
#if 0
    CustomerLength  =  total_tuples-1;

    c  =  (Customer *)malloc((CustomerLength+5)*sizeof(customer));
    p  =  (Pickup *)malloc((CustomerLength+5)*sizeof(pickup));
    r  =  (Route *)malloc((CustomerLength+5)*sizeof(Route));
    //Depot Data

    d.id  =  c1[0].id;
    d.x  =  c1[0].x;
    d.y  =  c1[0].y;
    d.demand  =  c1[0].demand;
    d.Etime  =  c1[0].Etime;
    d.Ltime  =  c1[0].Ltime;
    d.Stime  =  c1[0].Stime;
    d.Pindex  =  c1[0].Pindex;
    d.Dindex  =  c1[0].Dindex;
#endif



    //Customer Data 
    for (auto c : customers) {
        c.Ddist =  CalculateDistance(c.x, c.y ,depot.x, depot.y);                        
        if (c.Pindex  == 0){
            c.P = 1;
            c.D = 0;
        }
        if (c.Dindex  == 0){
            c.D = 1;
            c.P = 0;
        }
    }

#if 0
    //Customer Data 
    for (int i = 1;i<=CustomerLength;i++)
    {
        c[i].id  =  c1[i].id;
        c[i].x  =  c1[i].x;
        c[i].y  =  c1[i].y;
        c[i].Etime  =  c1[i].Etime;
        c[i].demand  =  c1[i].demand;
        c[i].Ltime  =  c1[i].Ltime;
        c[i].Stime  =  c1[i].Stime;
        c[i].Pindex  =  c1[i].Pindex;
        c[i].Dindex  =  c1[i].Dindex;
        c[i].Ddist =  CalculateDistance(c[i].x, c[i].y ,d.x, d.y);                        
        if (c[i].Pindex ==0){
            c[i].P = 1;
            c[i].D = 0;
        }
        if (c[i].Dindex ==0){
            c[i].D = 1;
            c[i].P = 0;
        }
    }
#endif
    //Vehicle Data 

#if 0
    // warning: variable ‘Vehicle’ set but not used [-Wunused-but-set-variable]
    VehicleInfo Vehicle;
    Vehicle.given_vehicles = VehicleLength;
    Vehicle.capacity  =  capacity;
    Vehicle.speed  =  1;
    Vehicle.used_vehicles = 0;
#endif



    //From Customers put aside all the Pickup's;
#if 0
    for (int i = 1;i<=CustomerLength;i++){
#endif
        int i  =  1;
        for (auto c : customers) {
            if (c.P  == 1){
                Pickup pickup({i, c.id, c.Ddist, c.Dindex, 0});
                pickups.push_back(pickup);
                ++i;
#if 0
                PickupLength+= 1;
                p[PickupLength].id = PickupLength;
                p[PickupLength].Did = cust.Dindex;
                p[PickupLength].Ddist = cust.Ddist;
                p[PickupLength].Pid = cust.id;
#endif
            }
        }
        // printf("Pickup Length = %d \n",PickupLength);

        /* Sort Pickup's 
         * The sequential construction inserts from largest distance to smallest
         */
        std::sort(pickups.begin(), pickups.end(),
                [] (const Pickup &p1, const Pickup &p2)
                {return p1.Ddist > p2.Ddist;}
                );
        i  =  1;
        for (auto &p : pickups) {
            p.id  =  i;
            ++i;
        }
#if 0
        int swap;
        double swap1;
        for (int i = 1;i<=PickupLength;i++)
        {
            for (int j = 1;j<=PickupLength-i;j++){
                if (p[j].Ddist>p[j+1].Ddist){
                    swap1 = p[j].Ddist;
                    p[j].Ddist = p[j+1].Ddist;
                    p[j+1].Ddist = swap1;
                    swap = p[j].Did;
                    p[j].Did = p[j+1].Did;
                    p[j+1].Did = swap;
                    swap = p[j].Pid;
                    p[j].Pid = p[j+1].Pid;
                    p[j+1].Pid = swap;
                    swap = p[j].id;
                    p[j].id = p[j+1].id;
                    p[j+1].id = swap;
                }
            }
            p[i].checked = 0;
        }
#endif

#ifdef DEBUG
        for (int i = 1;i<=PickupLength;i++)
        {
            //          DBG("PickupID[%d] = %lf\n",p[i].id,p[i].Ddist);
        }
#endif



        //Sequential Construction 
        size_t v = 0;
        State S;
        Route route(capacity);
        routes.push_back(route);
        for (auto &p : pickups) {
            S = r[v].append(/*c,*/ p, /*d,CustomerLength,PickupLength,*/ S);
            int flag = r[v].HillClimbing(customers, depot /*,p[i]*/);
            if (flag == 0){
                p.checked = 1;
            }
            r[v].remove(S);
            /* in a new vehicle*/
            ++v;
            {
                Route route(capacity);
                routes.push_back(route);
            }
            S = r[v].append(/*c,*/ p, /*d,CustomerLength,PickupLength,*/ S);
            flag = r[v].HillClimbing(customers, depot /*,p[i]*/);
            if (flag == 0){
                p.checked = 1;
            }
        }
#if 0
        Vehicle.used_vehicles = r.size();
#endif

#if 0
        // int flag_complete = 0;
        int checked = 0;
        //Sequential Construction 
        for (int v = 1;v<110;v++){
            for (int i = p.size(); i >= 0; i--){
                if (p[i].checked != 1){
                    State S;
                    S = r[v].append(/*c,*/ p[i], /*d,CustomerLength,PickupLength,*/ S);
                    int flag = r[v].HillClimbing(c, d /*,p[i]*/);
                    if (flag == 1){
                        //Remove 
                        p[i].checked = 0;
                        r[v].remove(S);
                    }
                    else{
                        p[i].checked = 1;
                        checked+= 1;
                    }
                }
                //Requests complete
            }
            Vehicle.used_vehicles = v;
            if (checked == p.size();)
            {
                v = 9999;
            }
        }
#endif

        int sum = 0, rts=0;

        for (const auto &r : routes) {
            sum += r.dis;
            if (r.dis != 0){
                rts+= 1;
            }
        }

#if 0
        //   *length_results_struct  =  d.Ltime;
        int sum = 0,rts=0;

        for (int i = 1;i<=Vehicle.used_vehicles;i++){
            // printf("%d, ",i);
            // r[i].print();
            sum+= r[i].dis;
            if (r[i].dis != 0){
                rts+= 1;
            }
            Vehicle.cost = sum;
        }
        // printf("Sum = %d  Routes=%d  Vehicle.used_vehicles=%d\n",sum,rts,Vehicle.used_vehicles);
#endif


        //Storing Initial Solution (S0 is the Initial Solution)
        for (const auto &r: routes) {
            S0.cost_total+= r.cost();
            S0.dis_total+= r.dis;
            S0.twv_total+= r.twv;
            S0.cv_total+= r.cv;
        }
        S0.routes = routes;
#if 0
        //Storing Initial Solution (S0 is the Initial Solution)
        for (int i = 1;i<=Vehicle.used_vehicles;i++)
        {
            S0.cost_total+= r[i].cost();
            S0.dis_total+= r[i].dis;
            S0.twv_total+= r[i].twv;
            S0.cv_total+= r[i].cv;
        }

        S0.route_length = Vehicle.used_vehicles;
        for (int i = 1;i<=Vehicle.used_vehicles;i++)
        {
            S0.r.push_back(r[i]);
        }
        // printf("Size   = >>  S0.r.size=%ld\n", S0.r.size());
#endif

        //Starting Neighborhoods
        // printf("\nNeighborhoods From now\n");
        int sol_count = TabuSearch(customers, depot, pickups);

        //Copying back the results 
        // path_element->results , path_length   {  we need to send (results, length_results) backk ; 
        int nodes_count;
        nodes_count =  customers.size();
        *results  =  (path_element *) malloc(sizeof(path_element) * (nodes_count + 5*VehicleLength));
        int length_results = 0;



#if 0
        int *cost, *cost_nodes;
        cost  =  (int *)malloc(1000*sizeof(int));
        cost_nodes  =  (int *)malloc(1000*sizeof(int));
#endif
        /*
         * cost holds id's of customers and depot
         */
        std::vector< int > cost;
        std::vector< int > cost_nodes;
        cost.reserve(1000);
        cost_nodes.reserve(1000);
        //Cost Calculation 

        for (const auto &route : T[sol_count].routes) {
            cost.push_back(depot.id);
            for (const auto &node : route.path) {
                cost.push_back(node);
            }
            cost.push_back(depot.id);
        }

#if 0
        int copy_length = 0;
        // TAKE AN ARRAY EMBED EVERYTHING 
        for (size_t itr_route = 0; itr_route < T[sol_count].route_length; itr_route++)
        {
            cost[copy_length] = depot.id;
            copy_length++;
            for (size_t itr_node  =  0; itr_node < T[sol_count].r[itr_route].path.size(); itr_node++)
            {
                cost[copy_length] = T[sol_count].r[itr_route].path[itr_node];
                copy_length++;
            }
            cost[copy_length] = depot.id;
            copy_length++;
        }
        copy_length -= 1;
#endif
        double temp_dis = 0;
        for (size_t i = 0; i < cost.size(); i++) {                
            if (i ==0)
            {
                cost_nodes[0] = 0;
                temp_dis = 0;
            }
            else 
            {
                //Depot to first node 
                if (cost[i-1] ==depot.id && cost[i]!=depot.id )
                {
                    temp_dis = 0;
                    temp_dis += CalculateDistance(customers[cost[i]].x, customers[cost[i]].y, depot.x, depot.y);
                    if (temp_dis < customers[cost[i]].Etime) {
                        temp_dis = customers[cost[i]].Etime;
                    }
#if 0
                    temp_dis+= sqrt(((c[cost[i]].x-d.x)*(c[cost[i]].x-d.x))+((c[cost[i]].y-d.y)*(c[cost[i]].y-d.y)));
                    if (temp_dis < c[cost[i]].Etime)
                    {
                        temp_dis = c[cost[i]].Etime;
                    }
#endif

                    cost_nodes[i] = temp_dis;
                }

                //Between nodes 
                else if (cost[i-1] != depot.id && cost[i]!=depot.id)
                {
                    temp_dis += CalculateDistance(customers[cost[i]].x, customers[cost[i]].y, customers[cost[i-1]].x, customers[cost[i-1]].y);
                    if (temp_dis < customers[cost[i]].Etime) {
                        temp_dis = customers[cost[i]].Etime;
                    }
                    temp_dis += customers[cost[i-1]].Stime;

#if 0
                    temp_dis+= sqrt(((c[cost[i]].x-c[cost[i-1]].x)*(c[cost[i]].x-c[cost[i-1]].x))+((c[cost[i]].y-c[cost[i-1]].y)*(c[cost[i]].y-c    [cost[i-1]].y)));
                    if (temp_dis < c[cost[i]].Etime)
                    {
                        temp_dis = c[cost[i]].Etime;
                    }

                    temp_dis+= c[cost[i-1]].Stime;
#endif
                    cost_nodes[i] = temp_dis;
                }
                else if (cost[i] ==depot.id && cost[i-1] != depot.id)
                {
#if 0
                    temp_dis+= sqrt(((d.x-c[cost[i-1]].x)*(d.x-c[cost[i-1]].x))+((d.y-c[cost[i-1]].y)*(d.y-c[cost[i-1]].y)));
#endif
                    temp_dis += CalculateDistance(customers[cost[i-1]].x, customers[cost[i-1]].y, depot.x, depot.y);
                    cost_nodes[i] = temp_dis;
                    temp_dis = 0;
                }
                else if (cost[i] == depot.id && cost[i-1] == depot.id)
                {
                    cost_nodes[i] = 0;
                    temp_dis = 0;
                }
            }
            //Last node to deopt  
        }

        //Done cost calculation 

#if 0
        //for (size_t itr_route = 0; itr_route<T[sol_count].route_length; itr_route++)
#endif
        int itr_route = 0;
        for (const auto &route : T[sol_count].routes)
        {
            (*results)[length_results].seq  =  length_results;
            (*results)[length_results].rid  =  itr_route + 1;
            (*results)[length_results].nid  =  depot.id;
            (*results)[length_results].cost  =  cost_nodes[length_results];
            length_results++;

            //Loop for path elements.
#if 0
            for (size_t itr_node = 0; itr_node < T[sol_count].r[itr_route].path.size(); itr_node++)
#endif
                for (const auto &node : route.path) {
                    (*results)[length_results].seq  =  length_results;
                    (*results)[length_results].rid  =  itr_route + 1;
                    (*results)[length_results].nid  =  node;
                    (*results)[length_results].cost  =  cost_nodes[length_results];
                    length_results++;
                }

            (*results)[length_results].seq  =  length_results;
            (*results)[length_results].rid  =  itr_route + 1;
            (*results)[length_results].nid  =  depot.id;
            (*results)[length_results].cost  =  cost_nodes[length_results];
            length_results++;
        }

        *length_results_struct  =  length_results;
#if 0
        free(c);
        free(p);
        free(r);
#endif
        //Copying is done till here

        (*msg)  =  NULL;
        return 0;
    }
    int n = 0,maxItr=30;



    /* TABU search helps us to store the solutions after every different move. The overview of TABU search will be a list containing list of solutions*/

    int TabuSearch(const std::vector<Customer> &customers, const Depot &depot, const std::vector<Pickup> &pickups)
    {
        // printf("TABU Called\n");
        Solution S,SBest;
        double CBest;
        //Pseudo Code
        /*

         **********Before*********
         int n = 0; //Counter     

         Create Tabu List  Vector of Solutions  std::vector<Solution> T;

         **********After**********
         Solution S,S0,SBest;  //S0 is initial 
         S = S0;
         Double CBest,SBest;
         CBest  =  S.getCost();
         SBest  =  S0;
         n = 0; //Counter
         while(1)
         {
         S  =  S.getBextofNeighborhood();
         if (S ==NULL)
         break;
         if (S.getCost() < CBest){
         SBest  =  S;
         CBest  =  S.getCost();
         }
         T.push_back(S);
         n++;
         if (n>maxItr)
         break;
         }

*/

        S = S0;
        CBest  =  S.getCost();
        SBest  =  S0;
        T.clear();
        T.push_back(S0);
        while(1)
        {
            S  =  S.getBestofNeighborhood(S, customers, depot ,pickups /*CustomerLength, PickupLength*/);
            if (S.getCost() == 0)
                break;
            if (S.getCost() < CBest){
                SBest  =  S;
                CBest  =  S.getCost();
                T.push_back(S);
            }   
            else if (S.getCost()  == CBest )
            {
                // printf("\n****************Repeated Solution****************\n");
                int k =  ((12)*maxItr)/100;
                maxItr  =  maxItr-k;
                // printf("Maxitr after repeating %d k  = %d\n",maxItr,k);
            }
            n++;
            if (n > maxItr)
                break;
        }
#if 0
        printf("Size of Tabu = %ld &&& n=%d  maxItr=%d\n",T.size(),n,maxItr);
        for (unsigned int itr = 0;itr<T.size();itr++)
        {
            T[itr].dump();
        }
#endif
        return T.size()-1;
    }

