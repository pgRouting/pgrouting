
#ifndef _PDP_H
#define _PDP_H


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


using namespace std;



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


double CalculateDistance(int x1,int y1,int x2,int y2)
{
        return sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
}

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

VehicleInfo ScanVehicle(VehicleInfo Vehicle)
{
        scanf("%d",&Vehicle.given_vehicles);
        scanf("%d",&Vehicle.capacity);
        scanf("%d",&Vehicle.speed);
        Vehicle.used_vehicles=0;
        return Vehicle;

}

//temp variables
int temp=0;

int CustomerLength=0;
int OrderLength=0;

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

// Class 
class Route
{
        public:
                int twv;
                int cv;
                int dis;
                //       vector<int> path;
                //      vector<int> order;
                int path[1000];
                int order[1000];
                int path_length;
                Route()
                {
                        twv=0;
                        cv=0;
                        dis=0;
                        path_length=0;
                        for(int i=0;i<1000;i++)
                        {
                                path[i]=0;
                                order[i]=0;
                        }
                }
                State append(customer *c, Pickup p, depot d,int CustomerLength, int PickupLength, State S);
                void update(customer *c,depot d);
                double cost();
                int HillClimbing(customer *c,depot d,Pickup p);
                int insertOrder(customer *c,depot d,Pickup p);
                void remove(State S);
                void print();
                int RemoveOrder(Pickup p);
};



int  Route::RemoveOrder(Pickup p){
        int flag=0;
        printf("Remove Order with Pid=%d  Did=%d\n",p.Pid,p.Did);
        for(int i=0;i<path_length;i++)
        {
                if(path[i]==p.Pid || path[i]==p.Did)
                {
                        flag=1;
                        path[i]=0;
                        order[i]=0;
                }
        }
        int new_path[path_length+1],new_length=0,new_order[path_length+1];
        if(flag==1)
        {
        //copy 
        for(int i=0;i<path_length;i++)
        {
                if(path[i]!=0)
                {
                        printf("path[%d]=%d \n",new_length,path[i]);
                        new_path[new_length]=path[i];
                        new_order[new_length]=order[i];
                        new_length++;
                }
        }
        //Reverse Copy
        for(int i=0;i<new_length;i++)
        {
                path[i]=new_path[i];
                order[i]=new_order[i];
        }
        path_length=new_length;
        printf("!!!!!!!!!!!!!Removed Order baby  with Pid=%d    New Length = %d  !!!!!!!!!!!!!!!!!\n",p.Pid,path_length);
        return 1;
        }
        else
        {
                return 0;
        }
}
State  Route::append(customer *c, Pickup p, depot d, int CustomerLength, int PickupLength, State S){
  
        //Save State;
        S.twv=twv;
        S.cv=cv;
        S.dis=dis;
        S.path_length=path_length;
        for(int i=0;i<path_length;i++)
        {
                S.path[i]=path[i];
                S.order[i]=order[i];
        }

        // Insert Order 
        path[path_length]=p.Pid;
        order[path_length]=p.id;
        path[path_length+1]=p.Did;
        order[path_length+1]=p.id;
        path_length+=2;

        return S;
}
void Route::update(customer *c, depot d)
{
        dis=0,twv=0,cv=0;
        int load=0;
        for(int i=-1;i<path_length;i++)
        {
                //depot to first customer
                if(i==-1)
                {
                        dis+=sqrt(((d.x-c[path[i+1]].x)*(d.x-c[path[i+1]].x))+((d.y-c[path[i+1]].y)*(d.y-c[path[i+1]].y)));
                        if(dis<c[path[i+1]].Etime)
                        {
                                dis=c[path[i+1]].Etime;
                        }
                        else if(dis>c[path[i+1]].Ltime)
                        {
                                         twv+=1;
                        }
                        dis+=c[path[i+1]].Stime;
                        load+=c[path[i+1]].demand;
                }
                //Last cusotmer to depot 
                if(i==path_length-1)
                {
                        dis+=sqrt(((d.x-c[path[i]].x)*(d.x-c[path[i]].x))+((d.y-c[path[i]].y)*(d.y-c[path[i]].y)));
                        if(dis>d.Ltime)
                        {
                                twv+=1;
                        }
                }
                //Middle customers
                if(i>=0 && i< path_length-1)
                {
                        dis+=sqrt(((c[path[i]].x-c[path[i+1]].x)*(c[path[i]].x-c[path[i+1]].x))+((c[path[i]].y-c[path[i+1]].y)*(c[path[i]].y-c[path[i+1]].y)));
                        if(dis<c[path[i+1]].Etime)
                        {
                                dis=c[path[i+1]].Etime;
                        }
                        else if(dis>c[path[i+1]].Ltime)
                        {
                                twv+=1;
                        }
                        dis+=c[path[i+1]].Stime;
                        load+=c[path[i+1]].demand;
                }

                if(load>200 || load<0)
                {
                        cv+=1;
                }
        }
        return;
}
double Route::cost()
{
        return (0.3*dis)+(0.5*twv)+(0.2*cv);
}
int Route::insertOrder(customer *c, depot d, Pickup p)
{
        double cost1=0,cost2=0;
        twv=0,cv=0,dis=0;
        int swap=0;
        update(c,d);
        if(twv==0 && cv==0 && dis<d.Ltime)        
                return 0;
        
        for(int i=0;i<path_length;i++)
        {
                for(int j=0;j<path_length;j++)
                {
                        if((c[path[i]].Ltime > c[path[j]].Ltime) ){
                                //Swap Path
                                swap=path[i];
                                path[i]=path[j];
                                path[j]=swap;
                                //Swap order
                                swap=order[i];
                                order[i]=order[j];
                                order[j]=swap;

                        }
                }
        }
        //After complete sort
        int temp[10000],tempo[10000];
        for(int i=0;i<path_length;i++)
        {
                temp[i]=path[path_length-i-1];
                tempo[i]=order[path_length-i-1];
        }
        for(int i=0;i<path_length;i++)
        {
                path[i]=temp[i];
                order[i]=tempo[i];
        }

        twv=0,cv=0,dis=0;
        print(); 
        update(c,d);        
        if(twv>0 || cv>0 || dis> d.Ltime)
        { 
                return 1;
        }
        return 0;
}

int Route::HillClimbing(customer *c, depot d, Pickup p)
{
        double cost1=0,cost2=0;
        twv=0,cv=0,dis=0;
        int swap=0;
        update(c,d);
        cost1=cost();
        if(twv==0 && cv==0 && dis<d.Ltime)        
                return 0;
        
        for(int i=0;i<path_length;i++){
                for(int j=0;j<path_length;j++){
                        int swap_flag=0,count_flag=0;
                        if((c[path[i]].Ltime > c[path[j]].Ltime) &&  (count_flag==0) ){
                                swap_flag=1;
                                count_flag=1;
                                //Swap Path
                                swap=path[i];
                                path[i]=path[j];
                                path[j]=swap;
                                //Swap order
                                swap=order[i];
                                order[i]=order[j];
                                order[j]=swap;

                        }
                        update(c,d);
                        cost2=cost();
                        if(cost2>cost1){
                                if(swap_flag==1){
                                        //Swap Path
                                        swap=path[i];
                                        path[i]=path[j];
                                        path[j]=swap;
                                        //Swap order
                                        swap=order[i];
                                        order[i]=order[j];
                                        order[j]=swap;
                                   //     update(c,d);
                                }
                        }
                }
        }
        //After complete sort
        int temp[10000],tempo[10000];
        for(int i=0;i<path_length;i++)
        {
                temp[i]=path[path_length-i-1];
                tempo[i]=order[path_length-i-1];
        }
        for(int i=0;i<path_length;i++)
        {
                path[i]=temp[i];
                order[i]=tempo[i];
        }


        update(c,d);
        if(twv>0 || cv>0 || dis> d.Ltime)
        { 
                return 1;
        }
        return 0;
}


void Route::print(){
        printf("%d ",dis);
        printf("%d ",twv);
        printf("%d ",cv);
        printf("%lf ",cost());
        printf("[");
        for(int i=0;i<path_length;i++)
        {
                printf("%d ",path[i]);
        }
        printf("] ");
        printf("[");
        for(int i=0;i<path_length;i++)
        {
                printf("%d ",order[i]);
        }
        printf("] \n");
        return;

}

void Route::remove( State S){
        twv=S.twv;
        cv=S.cv;
        dis=S.dis;
        path_length=S.path_length;
        for(int i=0;i<path_length;i++)
        {
                path[i]=S.path[i];
                order[i]=S.order[i];
        }
        return;
}



#endif










