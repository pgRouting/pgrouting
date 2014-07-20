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
using namespace std;

int PickupLength=0;


depot d;
VehicleInfo Vehicle;
customer c[1000];
int len=0;

Route r[500];


//Definitions for a few functions 
void TabuSearch();

int main()
{
        pickup p[600];
        Vehicle=ScanVehicle(Vehicle);
        depot d=ScanDepot(d);
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
        for(int i=1;i<=PickupLength;i++){
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
        printf("Sum=%d  Routes=%d\n",sum,rts);


        //Storing Initial Solution


        //Starting Neighborhoods
        printf("\nNeighborhoods From now\n");
        TabuSearch();

        return 0;
}


void TabuSearch()
{
        printf("TABU Called\n");
        Solution S[100];
        Route r3;
        printf("Cost=%lf\n",S[9].getCost());
        return;
}













