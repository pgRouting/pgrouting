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


#pragma once

#include  <vector> 
#include  <map> 
#include  <queue> 
#include  <string> 
#include  <stdlib.h> 
#include  <iostream> 
#include  <algorithm> 
#include  <math.h> 
#include  <stdio.h> 
#include  <string.h> 
#include  <set> 

class Route {
 public:
     int twv;
     int cv;
     int dis;
     std::vector < int >  path;
     std::vector < int >  order;
     int capacity;

#if 0
     int path_length;
#endif

     Route(int _capacity) :
         twv(0),
         cv(0),
         dis(0),
         capacity(_capacity) {
             path.clear();
             order.clear();
         }
     State append(
             /*Customer *c,*/
             const Pickup &p,
            /* Depot d,  int CustomerLength, int PickupLength,*/
             State S);
     void update(const std::vector<Customer> &c, const Depot &d);
     double cost() const;
     int HillClimbing(const std::vector<Customer> &c, const Depot &d
             /*,Pickup p*/);
     int insertOrder(const std::vector<Customer> &c, const Depot &d
             /*,Pickup p*/);
     void remove(State S);
     // void print();
     int RemoveOrder(const Pickup &p);
     double distance(double x1, double y1, double x2, double y2) const;
};


/*
 * Returns:
 * 1 - when the order was found
 * 0 - When the order was not found
 */
int
Route::RemoveOrder(const Pickup &p){

    auto Pickup_pos = find(path.begin(), path.end(),p.Pid);

    if (Pickup_pos != path.end()) {
        path.erase(Pickup_pos);
        order.erase(Pickup_pos);
    } else {
        return 0;
    }

    auto Delivery_pos = find(path.begin(), path.end(),p.Did);

    if (Delivery_pos !=  path.end()) {
        path.erase(Delivery_pos);
        order.erase(Delivery_pos);
    }
    return 1;

#if 0
    // printf("Remove Order with Pid = %d  Did=%d\n",p.Pid,p.Did);
    for (int i = 0;i < path_length;i++) {
        if (path[i] ==p.Pid || path[i]==p.Did)
        {
            flag = 1;
            path[i] = 0;
            order[i] = 0;
        }
    }
    int new_path[path_length+1],new_length = 0,new_order[path_length+1];
    if (flag ==1)
    {
        //copy 
        for (int i = 0;i < path_length;i++)
        {
            if (path[i]! = 0)
            {
                // printf("path[%d] = %d \n",new_length,path[i]);
                new_path[new_length] = path[i];
                new_order[new_length] = order[i];
                new_length++;
            }
        }
        //Reverse Copy
        for (int i = 0;i < new_length;i++)
        {
            path[i] = new_path[i];
            order[i] = new_order[i];
        }
        path_length = new_length;
        return 1;
    }
    else
    {
        return 0;
    }
#endif
}

State
Route::append(
        /*Customer *c,*/
        const Pickup &p,
        /* Depot d, int CustomerLength, int PickupLength,*/
        State S){

    //Save State;
    S.twv = twv;
    S.cv = cv;
    S.dis = dis;
#if 0
    S.path_length = path_length;
#endif
    S.path_length = path.size();

    for (size_t i = 0; i  <  path.size(); i++)
    {
        S.path[i] = path[i];
        S.order[i] = order[i];
    }

    // Insert Order 

    path.push_back(p.Pid);
    path.push_back(p.Did);

    order.push_back(p.id);
    order.push_back(p.id);
#if 0
    path[path_length] = p.Pid;
    order[path_length] = p.id;
    path[path_length+1] = p.Did;
    order[path_length+1] = p.id;
    path_length += 2;
#endif
    return S;
}

double
Route::distance(double x1, double y1, double x2, double y2) const {
    double delta_x = x1 - x2;
    double delta_y = y1 - y2;
    return sqrt(delta_x * delta_x + delta_y * delta_y);
}

void
Route::update(const std::vector<Customer> &c, const Depot &d) {
    dis = 0, twv=0, cv=0;
    int load = 0;
    for (int i = -1; i  <  (int)path.size(); i++)
    {
        //Depot to first Customer
        if (i ==-1)
        {
#if 0
            dis += sqrt(((d.x-c[path[i+1]].x)*(d.x-c[path[i+1]].x))+((d.y-c[path[i+1]].y)*(d.y-c[path[i+1]].y)));
#endif
            dis += distance(d.x, d.y, c[path[i+1]].x, c[path[i+1]].y);
            if (dis < c[path[i+1]].Etime)
            {
                dis = c[path[i+1]].Etime;
            }
            else if (dis > c[path[i+1]].Ltime)
            {
                twv += 1;
            }
            dis += c[path[i+1]].Stime;
            load += c[path[i+1]].demand;
        }
        //Last cusotmer to Depot 
        if (i == (int)(path.size() - 1)) {
#if 0
            dis += sqrt(((d.x-c[path[i]].x)*(d.x-c[path[i]].x))+((d.y-c[path[i]].y)*(d.y-c[path[i]].y)));
#endif
            dis  += distance(d.x, d.y, c[path[i]].x, c[path[i]].y);
            if (dis > d.Ltime)
            {
                twv += 1;
            }
        }
        //Middle Customers
        if (i  >= 0 && i  <  (int)(path.size() - 1))
        {
#if 0
            dis += sqrt(((c[path[i]].x-c[path[i+1]].x)*(c[path[i]].x-c[path[i+1]].x))+((c[path[i]].y-c[path[i+1]].y)*(c[path[i]].y-c[path[i+1]].y)));
#endif
            dis  += distance(c[path[i]].x, c[path[i]].y, c[path[i+1]].x, c[path[i+1]].y);
            if (dis < c[path[i+1]].Etime)
            {
                dis = c[path[i+1]].Etime;
            }
            else if (dis > c[path[i+1]].Ltime)
            {
                twv += 1;
            }
            dis += c[path[i+1]].Stime;
            load += c[path[i+1]].demand;
        }

        /*
         * works only with 200 load
         */
        if (load > 200 || load < 0) {
            cv += 1;
        }
    }
    return;
}

double
Route::cost() const {
    return (0.3*dis)+(0.5*twv)+(0.2*cv);
}

int
Route::insertOrder(const std::vector<Customer> &c, const Depot &d
        /*, Pickup p*/) {
    // double cost1 = 0,cost2=0;
    twv = 0, cv=0, dis=0;

#if 0
    int swap = 0;
#endif

    update(c,d);
    if (twv ==0 && cv==0 && dis  <  d.Ltime)        
        return 0;

    for (size_t i = 0;i  <  path.size(); i++) {
        for (size_t j = 0; j  <  path.size(); j++) {
            if ((c[path[i]].Ltime  >  c[path[j]].Ltime) ){
                std::swap(path[i], path[j]);
                std::swap(order[i], order[j]);
#if 0
                //Swap Path
                swap = path[i];
                path[i] = path[j];
                path[j] = swap;
                //Swap order
                swap = order[i];
                order[i] = order[j];
                order[j] = swap;
#endif
            }
        }
    }

    std::reverse(path.begin(),path.end());
    std::reverse(order.begin(),order.end());

#if 0
    //After complete sort
    int *temp = NULL;
    int *tempo = NULL;
    temp =  (int *)malloc(1000*sizeof(int));
    tempo =  (int *)malloc(1000*sizeof(int));
    for (int i = 0;i < path_length;i++)
    {
        temp[i] = path[path_length-i-1];
        tempo[i] = order[path_length-i-1];
    }
    for (int i = 0;i < path_length;i++)
    {
        path[i] = temp[i];
        order[i] = tempo[i];
    }
#endif

    twv = 0,cv=0,dis=0;
    update(c,d);        

    return twv  >  0 || cv > 0 || dis > d.Ltime;
#if 0
    // print(); 
    if (twv > 0 || cv>0 || dis> d.Ltime)
    { 
        return 1;
    }
    return 0;
#endif
}

int Route::HillClimbing(const std::vector<Customer> &c, const Depot &d
        /*, Pickup p*/)
{
    double cost1 = 0,cost2=0;
    twv = 0,cv=0,dis=0;
#if 0
    int swap = 0;
#endif
    update(c,d);
    cost1 = cost();
    if (twv ==0 && cv==0 && dis < d.Ltime)        
        return 0;

    for (size_t i = 0; i  <  path.size(); i++){
        for (size_t j = 0;j < path.size(); j++){
            int swap_flag = 0,count_flag=0;
            if ((c[path[i]].Ltime  >  c[path[j]].Ltime) &&  (count_flag ==0) ){
                swap_flag = 1;
                count_flag = 1;
                std::swap(path[i], path[j]);
                std::swap(order[i], order[j]);
#if 0
                //Swap Path
                swap = path[i];
                path[i] = path[j];
                path[j] = swap;
                //Swap order
                swap = order[i];
                order[i] = order[j];
                order[j] = swap;
#endif
            }
            update(c,d);
            cost2 = cost();
#if 0
            if (cost2  > cost1){
                if (swap_flag ==1){
#endif
                    if (cost2  >  cost1 && swap_flag == 1) {
                        //Swap Path
                        std::swap(path[i], path[j]);
                        std::swap(order[i], order[j]);
#if 0
                        swap = path[i];
                        path[i] = path[j];
                        path[j] = swap;
                        //Swap order
                        swap = order[i];
                        order[i] = order[j];
                        order[j] = swap;
                        //     update(c,d);
#endif
                    }
#if 0
                }
#endif
            }
        }

        std::reverse(path.begin(),path.end());
        std::reverse(order.begin(),order.end());

#if 0
        //After complete sort
        int *temp = NULL;
        int *tempo = NULL;
        temp =  (int *)malloc(1000*sizeof(int));
        tempo =  (int *)malloc(1000*sizeof(int));
        for (int i = 0;i < path_length;i++)
        {
            temp[i] = path[path_length-i-1];
            tempo[i] = order[path_length-i-1];
        }
        for (int i = 0;i < path_length;i++)
        {
            path[i] = temp[i];
            order[i] = tempo[i];
        }
#endif


        twv = 0,cv=0,dis=0;
        update(c,d);        

        return twv  >  0 || cv > 0 || dis > d.Ltime;
#if 0
        update(c,d);
        if (twv > 0 || cv>0 || dis> d.Ltime)
        { 
            return 1;
        }
        return 0;
#endif
    }


#if 0
    void Route::print(){
        printf("%d ",dis);
        printf("%d ",twv);
        printf("%d ",cv);
        printf("%lf ",cost());
        printf("[");
        for (int i = 0;i < path_length;i++)
        {
            printf("%d ",path[i]);
        }
        printf("] ");
        printf("[");
        for (int i = 0;i < path_length;i++)
        {
            printf("%d ",order[i]);
        }
        printf("] \n");
        return;

    }
#endif

    void Route::remove(State S) {
        twv = S.twv;
        cv = S.cv;
        dis = S.dis;

        path.clear();
        order.clear();
        for (int i = 0; i  <  S.path_length; i++) {
            path[i] = S.path[i];
            order[i] = S.order[i];
        }
    }


