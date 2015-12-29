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
 pdp.cpp --> Main solver
 pdp.h  ---> Structures defined in this header file
 Solution.h -----> It contains the Solution Class and Code related to Neighborhoods
 Route.h   -----> Explains all about Route Class.
 pdp.c ---> Contains all the details on pgRouting integration.

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
#include <algorithm>

#include "./pdp.h"
#include "./Solution.h"
#include "./Route.h"


int len = 0;


std::vector<Solution> T;

Route *r = NULL;
// Definitions for a few functions
int TabuSearch(const std::vector<Customer> &c, const Depot &d , const std::vector<Pickup> &p);
// Vector containing solutions

// Initial Solution
Solution S0;

void result_struct();
int Solver(Customer *c1,
        int total_tuples,
        int VehicleLength,
        int capacity,
        char **msg,
        path_element **results,
        int *length_results_struct) {
    std::vector<Customer> customers(c1 + 1, c1 + total_tuples);
    std::vector<Pickup> pickups;
    std::vector<Route> routes;

    Depot depot({c1[0].id, c1[0].x, c1[0].y,
            c1[0].demand,
            c1[0].Etime, c1[0].Ltime, c1[0].Stime,
            c1[0].Pindex, c1[0].Dindex
            });


    // Customer Data
    for (auto c : customers) {
        c.Ddist = CalculateDistance(c.x, c.y , depot.x, depot.y);
        if (c.Pindex == 0) {
            c.P = 1;
            c.D = 0;
        }
        if (c.Dindex == 0) {
            c.D = 1;
            c.P = 0;
        }
    }


    // From Customers put aside all the Pickup's;
    int i = 1;
    for (auto c : customers) {
        if (c.P == 1) {
            Pickup pickup({i, c.id, c.Ddist, c.Dindex, 0});
            pickups.push_back(pickup);
            ++i;
        }
    }

    /* Sort Pickup's
     * The sequential construction inserts from largest distance to smallest
     * but he had it ordered from smallest to largest
     */
    std::sort(pickups.begin(), pickups.end(),
            [] (const Pickup &p1, const Pickup &p2)
            {return p1.Ddist > p2.Ddist;});
    i = 1;
    for (auto &p : pickups) {
        p.id = i;
        ++i;
    }




    // Sequential Construction
    size_t v = 0;
    State S;
    Route route(capacity);
    routes.push_back(route);
    for (auto &p : pickups) {
        S = r[v].append(p, S);
        int flag = r[v].HillClimbing(customers, depot);
        if (flag == 0) {
            p.checked = 1;
        }
        r[v].remove(S);
        /* in a new vehicle*/
        ++v;
        {
            Route route(capacity);
            routes.push_back(route);
        }
        S = r[v].append(p, S);
        flag = r[v].HillClimbing(customers, depot);
        if (flag == 0) {
            p.checked = 1;
        }
    }

    int sum = 0, rts = 0;

    for (const auto &r : routes) {
        sum += r.dis;
        if (r.dis != 0) {
            rts+= 1;
        }
    }



    // Storing Initial Solution (S0 is the Initial Solution)
    for (const auto &r : routes) {
        S0.cost_total += r.cost();
        S0.dis_total  += r.dis;
        S0.twv_total  += r.twv;
        S0.cv_total   += r.cv;
    }
    S0.routes = routes;

    // Starting Neighborhoods
    int sol_count = TabuSearch(customers, depot, pickups);

    // Copying back the results
    int nodes_count;
    nodes_count = customers.size();
    *results = static_cast<path_element *>(malloc(sizeof(path_element) * (nodes_count + 5*VehicleLength)));

    int length_results = 0;

    /*
     * cost holds id's of customers and depot
     */
    std::vector< int > cost;
    std::vector< int > cost_nodes;
    cost.reserve(1000);
    cost_nodes.reserve(1000);

    // Cost Calculation

    for (const auto &route : T[sol_count].routes) {
        cost.push_back(depot.id);
        for (const auto &node : route.path) {
            cost.push_back(node);
        }
        cost.push_back(depot.id);
    }

    double temp_dis = 0;
    for (size_t i = 0; i < cost.size(); i++) {
        if (i ==0) {
            cost_nodes[0] = 0;
            temp_dis = 0;
            continue;
        }
        // Depot to first node
        if (cost[i-1] == depot.id && cost[i] != depot.id) {
            temp_dis = 0;
            temp_dis += CalculateDistance(customers[cost[i]].x, customers[cost[i]].y, depot.x, depot.y);
            if (temp_dis < customers[cost[i]].Etime) {
                temp_dis = customers[cost[i]].Etime;
            }
            cost_nodes[i] = temp_dis;
        } else if (cost[i-1] != depot.id && cost[i] != depot.id) {
            // Between nodes
            temp_dis += CalculateDistance(customers[cost[i]].x, customers[cost[i]].y, customers[cost[i-1]].x, customers[cost[i-1]].y);
            if (temp_dis < customers[cost[i]].Etime) {
                temp_dis = customers[cost[i]].Etime;
            }
            temp_dis += customers[cost[i-1]].Stime;
            cost_nodes[i] = temp_dis;
        } else if (cost[i] ==depot.id && cost[i-1] != depot.id) {
            temp_dis += CalculateDistance(customers[cost[i-1]].x, customers[cost[i-1]].y, depot.x, depot.y);
            cost_nodes[i] = temp_dis;
            temp_dis = 0;
        } else if (cost[i] == depot.id && cost[i-1] == depot.id) {
            // Last node to deopt
            cost_nodes[i] = 0;
            temp_dis = 0;
        }
    }

    // Done cost calculation

    int itr_route = 0;
    for (const auto &route : T[sol_count].routes) {
        (*results)[length_results].seq = length_results;
        (*results)[length_results].rid = itr_route + 1;
        (*results)[length_results].nid = depot.id;
        (*results)[length_results].cost = cost_nodes[length_results];
        length_results++;

        // Loop for path elements.
        for (const auto &node : route.path) {
            (*results)[length_results].seq = length_results;
            (*results)[length_results].rid = itr_route + 1;
            (*results)[length_results].nid = node;
            (*results)[length_results].cost = cost_nodes[length_results];
            length_results++;
        }

        (*results)[length_results].seq = length_results;
        (*results)[length_results].rid = itr_route + 1;
        (*results)[length_results].nid = depot.id;
        (*results)[length_results].cost = cost_nodes[length_results];
        length_results++;
    }

    *length_results_struct = length_results;

    (*msg) = NULL;
    return 0;
}



/* TABU search helps us to store the solutions after every different move.
 * The overview of TABU search will be a list containing list of solutions

 **********Before*********
 int n = 0; //Counter

 Create Tabu List Vector of Solutions std::vector<Solution> T;

 **********After**********
 Solution S,S0,SBest; //S0 is initial
 S = S0;
 Double CBest,SBest;
 CBest = S.getCost();
 SBest = S0;
 n = 0; //Counter
 while(1)
 {
 S = S.getBextofNeighborhood();
 if (S ==NULL)
 break;
 if (S.getCost() < CBest){
 SBest = S;
 CBest = S.getCost();
 }
 T.push_back(S);
 n++;
 if (n>maxItr)
 break;
 }

*/

int TabuSearch(const std::vector<Customer> &customers,
        const Depot &depot,
        const std::vector<Pickup> &pickups) {
    int n = 0;
    int maxItr = 30;
    Solution S, SBest;
    double CBest;
    S = S0;
    CBest = S.getCost();
    SBest = S0;
    T.clear();
    T.push_back(S0);
    while (1) {
        S = S.getBestofNeighborhood(S, customers, depot , pickups);
        if (S.getCost() == 0) break;
        if (S.getCost() < CBest) {
            SBest = S;
            CBest = S.getCost();
            T.push_back(S);
        } else if (S.getCost() == CBest) {
            // printf("\n****************Repeated Solution****************\n");
            int k = ((12)*maxItr)/100;
            maxItr = maxItr-k;
            // printf("Maxitr after repeating %d k = %d\n",maxItr,k);
        }
        n++;
        if (n > maxItr)
            break;
    }
    return T.size()-1;
}

