#include<stdio.h>
#include<stdlib.h>

#ifndef _PDP_H
#define _PDP_H


// Structures and variables which help us in storing the data

int VehicleCount;
int Capacity;
int Speed;

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

//temp variables


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



#endif










