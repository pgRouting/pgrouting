/*                                 

 *****list of files in this dir*******
 pdp.cpp  --> Main solver
 pdp.h    ---> Structures defined in this header file
 Solution.h  -----> It contains the Solution Class and Code related to Neighborhoods  

 The main problem is in two steps. 1.)Getting the initial solutiion and 2.)Optimizing it.

 1.) "Initial solution":
 A few heuristics are applied to find a feasible initial solution. Sequential Construction and Hill climbing. More implementation details are found here:: https://github.com/pgRouting/pgrouting/wiki/VRP-Pickup-Delivery-Problem 

 2.) "Optimizing the Solution":
 A reactive tabu search is applied on the initial solution to get a feasible optimized solution. TabuSearch comes under local search methods. We have three neighborhoods i) Single Paired Insertion  ii) Swapping pairs between routes  iii)Within Route Insertion. Tabu attributes plays an important role in giving the best solution(it includes TabuLength, A vector containing feasible solutions and a counter for number of solutions) Reactive part discussed in the paper is to modify TabuLength based on the solution cycle. 

 */

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

int PickupLength=0;

//Depot 
depot d;
//Vehicle
VehicleInfo Vehicle;
//Customer Data
customer c[1000];
pickup p[600];
int len=0;

Route r[500];

//Definitions for a few functions 
void TabuSearch();

//Initial Solution
Solution S0;

int main()
{
        Vehicle=ScanVehicle(Vehicle);
        d=ScanDepot(d);
        while((scanf("%d",&temp))!=EOF){
                CustomerLength+=1;
                len=CustomerLength;
                c[CustomerLength]=ScanCustomer(temp,c[CustomerLength],d);
        }

        //From customers put aside all the pickup's;
        for(int i=1;i<=CustomerLength;i++){
                if(c[i].P==1){
                        PickupLength+=1;
                        p[PickupLength].id=PickupLength;
                        p[PickupLength].Did=c[i].Dindex;
                        p[PickupLength].Ddist=c[i].Ddist;
                        p[PickupLength].Pid=c[i].id;
                }
        }
        printf("Pickup Length=%d \n",PickupLength);

        //Sort Pickup's
        int swap;
        double swap1;
        for(int i=1;i<=PickupLength;i++)
        {
                for(int j=1;j<=PickupLength-i;j++){
                        if(p[j].Ddist>p[j+1].Ddist){
                                swap1=p[j].Ddist;
                                p[j].Ddist=p[j+1].Ddist;
                                p[j+1].Ddist=swap1;
                                swap=p[j].Did;
                                p[j].Did=p[j+1].Did;
                                p[j+1].Did=swap;
                                swap=p[j].Pid;
                                p[j].Pid=p[j+1].Pid;
                                p[j+1].Pid=swap;
                                swap=p[j].id;
                                p[j].id=p[j+1].id;
                                p[j+1].id=swap;
                        }
                }
                p[i].checked=0;
        }

        /*
           for(int i=1;i<=PickupLength;i++)
           {
           printf("PickupID[%d]=%lf\n",p[i].id,p[i].Ddist);
           }
         */


        int flag_complete=0,checked=0;
        //Sequential Construction 
        for(int v=1;v<110;v++){
                for(int i=PickupLength;i>=1;i--){
                        if(p[i].checked!=1){
                                State S;
                                S=r[v].append(c,p[i],d,CustomerLength,PickupLength,S);
                                int flag=r[v].HillClimbing(c,d,p[i]) ;
                                if(flag==1){
                                        //Remove 
                                        p[i].checked=0;
                                        r[v].remove(S);
                                }
                                else{
                                        p[i].checked=1;
                                        checked+=1;
                                }
                        }
                        //Requests complete
                }
                Vehicle.used_vehicles=v;
                if(checked==PickupLength)
                {
                        v=9999;
                }
        }
        int sum=0,rts=0;

        for(int i=1;i<=Vehicle.used_vehicles;i++){
                printf("%d, ",i);
                r[i].print();
                sum+=r[i].dis;
                if(r[i].dis!=0){
                        rts+=1;
                }
                Vehicle.cost=sum;
        }
        printf("Sum=%d  Routes=%d  Vehicle.used_vehicles=%d\n",sum,rts,Vehicle.used_vehicles);

        //Storing Initial Solution (S0 is the Initial Solution)
        for(int i=1;i<=Vehicle.used_vehicles;i++)
        {
                S0.cost_total+=r[i].cost();
                S0.dis_total+=r[i].dis;
                S0.twv_total+=r[i].twv;
                S0.cv_total+=r[i].cv;
        }
        S0.route_length=Vehicle.used_vehicles;
        for(int i=1;i<=Vehicle.used_vehicles;i++)
        {
                S0.r.push_back(r[i]);
        }
        printf("Size  =>>  S0.r.size=%d\n",S0.r.size());

        //Starting Neighborhoods
        printf("\nNeighborhoods From now\n");
        TabuSearch();
        return 0;
}
int n=0,maxItr=30;

void TabuSearch()
{
        printf("TABU Called\n");
        Solution S,SBest;
        double CBest;
        std::vector<Solution> T;
        //Pseudo Code
        /*

         **********Before*********
         int n=0; //Counter     
         Create Tabu List  Vector of Solutions  std::vector<Solution> T;

         **********After**********
         Solution S,S0,SBest;  //S0 is initial 
         S=S0;
         Double CBest,SBest;
         CBest = S.getCost();
         SBest = S0;
         n=0; //Counter
         while(1)
         {
         S = S.getBextofNeighborhood();
         if(S==NULL)
         break;
         if(S.getCost() < CBest){
         SBest = S;
         CBest = S.getCost();
         }
         T.push_back(S);
         n++;
         if(n>maxItr)
         break;
         }

         */

        S=S0;
        CBest = S.getCost();
        SBest = S0;
        T.clear();
        T.push_back(S0);
        while(1)
        {
                S = S.getBestofNeighborhood(S,c,d,p,CustomerLength,PickupLength);
                if(S.getCost()==0)
                        break;
                if (S.getCost() < CBest){
                        SBest = S;
                        CBest = S.getCost();
                        T.push_back(S);
                }   
                else if(S.getCost() == CBest )
                {
                        printf("\n****************Repeated Solution****************\n");
                        int k= ((12)*maxItr)/100;
                        maxItr = maxItr-k;
                        printf("Maxitr after repeating %d k =%d\n",maxItr,k);
                }
                n++;
                if (n > maxItr)
                        break;
        }
        printf("Size of Tabu=%d &&& n=%d  maxItr=%d\n",T.size(),n,maxItr);
        for(int itr=0;itr<T.size();itr++)
        {
                T[itr].dump();
        }
        return;
}

