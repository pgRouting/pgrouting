/*PGR

Copyright (c) 2014 Manikata Kondeti
mani.iiit123@gmail.com

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

*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "postgres.h"

#ifndef _PDP_H
#define _PDP_H


// Structures and variables which help us in storing the data

typedef struct Depot{
        int id;
        int x;
        int y;
        int demand;
        int Etime;
        int Ltime;
        int Stime;
        int Pindex;
        int Dindex;
}depot;



typedef struct Customer{
        int id;
        int x;
        int y;
        int demand;
        int Etime;
        int Ltime;
        int Stime;
        int Pindex;
        int Dindex;
        double Ddist;
        int P;
        int D;
}customer;


typedef struct Pickup{
        int id;
        int Pid;
        double Ddist;
        int Did;
        int checked;
}pickup; 



typedef struct Vehicle{
        int capacity;
        int used_vehicles;
        int given_vehicles;
        int speed;
        int cost;
}VehicleInfo;


// A module which calculates distance 
double CalculateDistance(int x1,int y1,int x2,int y2)
{
        return sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
}

/*
// DEPOT:  With id=0
depot ScanDepot(depot d)
{

scanf("%d",&d.id);
scanf("%d",&d.x);
scanf("%d",&d.y);
scanf("%d",&d.demand);
scanf("%d",&d.Etime);
scanf("%d",&d.Ltime);
scanf("%d",&d.Stime);
scanf("%d",&d.Pindex);
scanf("%d",&d.Dindex);
return d;
}

// CUSTOMER: WITH id>=1
customer ScanCustomer(int id,customer c,depot d)
{
c.id=id;
scanf("%d",&c.x);
scanf("%d",&c.y);
scanf("%d",&c.demand);
scanf("%d",&c.Etime);
scanf("%d",&c.Ltime);
scanf("%d",&c.Stime);
scanf("%d",&c.Pindex);
scanf("%d",&c.Dindex);
if(c.Pindex==0)
{ 
c.P=1;
c.D=0;
}  
else if(c.Dindex==0)
{
c.D=1;
c.P=0;
}
c.Ddist=CalculateDistance(c.x,c.y,d.x,d.y);
return c;
}


//VEHICLE: First Line contains vehicle data
VehicleInfo ScanVehicle(VehicleInfo Vehicle)
{
scanf("%d",&Vehicle.given_vehicles);
scanf("%d",&Vehicle.capacity);
scanf("%d",&Vehicle.speed);
Vehicle.used_vehicles=0;
return Vehicle;

}


 */


// Part of code: It is used to save some variables and helps if we need to revisit previous state.
typedef struct statesave{
        int twv;
        int cv;
        int dis;
        //       vector<int> path;
        //      vector<int> order;
        int path[1000];
        int order[1000];
        int path_length;
}State;


typedef struct PathElement {
         int seq;
         int rid;
         int nid;
         int nseq;
         int cost;
} path_element;



#ifdef __cplusplus
extern "C" {
#endif
int Solver(customer *c, int total_tuples, int vehicle_count, int capacity , char **msg, path_element **results, int *length_results);

#ifdef __cplusplus
}
#endif



#endif










