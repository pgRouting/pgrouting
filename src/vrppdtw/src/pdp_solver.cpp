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

#include "./pdp_types.h"
#include "./pdp_solver.h"
#include "./pdp_types.hpp"

#include "./Solution.h"
#include "./Route.h"
#include "../../common/src/memory_func.hpp"


//forward declaration
static
size_t
TabuSearch(
        const std::vector<Customer> &customers,
        const std::vector<Pickup> &pickups,
        int maxIter,
        std::vector<Solution> &T);

static
void
get_result(
        Solution &solution,
        const std::vector < Customer > &customers,
        const Depot &depot,
        int64_t VehicleLength,
        std::vector< path_element > &result);

int64_t Solver(Customer *c1,
        size_t total_tuples,
        int64_t VehicleLength,
        double capacity,
        char **msg,
        path_element **results,
        size_t *length_results_struct) {
    std::vector<Customer> customers(c1, c1 + total_tuples);
    std::vector<Pickup> pickups;
    std::vector<Route> routes;

    Depot depot({c1[0].id, c1[0].x, c1[0].y,
            c1[0].demand,
            c1[0].Etime, c1[0].Ltime, c1[0].Stime,
            c1[0].Pindex, c1[0].Dindex
            });
    return 0;

#if 0
    if (total_tuples != 107) {
        return 0;
    }
#endif

    // Customer Data
    for (auto &c : customers) {
        if (c.id == 0) continue; 
        c.Ddist = CalculateDistance(c, depot);
        if (c.Pindex == 0) {
            // From Customers put aside all the Pickup's;
            Pickup pickup({c.id, c.Ddist, c.Dindex});
            pickups.push_back(pickup);
        }
    }

#if 0
    if (pickups.size() != 53) {
        (*results) = NULL;
        (*length_results_struct) = 0;
        return 0;
    }
#endif

    /* Sort Pickup's
     * The sequential construction inserts from largest distance to smallest
     * but he had it ordered from smallest to largest
     */
    std::sort(pickups.begin(), pickups.end(),
            [] (const Pickup &p1, const Pickup &p2)
            {return p2.Ddist < p1.Ddist;});


    // Sequential Construction
    size_t v = 0;
    Route route(capacity, depot);
    routes.push_back(route);
    for (auto &pickup : pickups) {
        int OK = 0;
        OK = routes[v].insertOrder(customers, pickup);
        if (OK) continue;
        Route route(capacity, depot);
        routes.push_back(route);
        /* adding a new vehicle*/
        ++v;
        routes[v].insertOrder(customers, pickup);
    }


    std::sort(pickups.begin(), pickups.end(),
            [] (const Pickup &p1, const Pickup &p2)
            {return p1.Ddist < p2.Ddist;});

    // Initial Solution
    Solution S0;
    S0.routes = routes;
    //S0.UpdateSol(customers);

    std::vector<Solution> T;
    T.push_back(S0);

    // Starting the TABU SEARCH

    TabuSearch(customers, pickups, 30, T);


    std::vector< path_element > result;
#ifdef DEBUG
    for (auto &solution: T) {
        get_result(solution, customers, depot, VehicleLength, result);
    }
#else
    T.back().UpdateSol(customers);
    get_result(T.back(), customers, depot, VehicleLength, result);
#endif



    // Getting memory to store results
    //*results = static_cast<path_element *>(malloc(sizeof(path_element) * (result.size())));
    *results = NULL;
    *results = get_memory(result.size(), (*results));


    //store the results
    int seq = 0;
    for (const auto &row : result) {
        (*results)[seq] = row;
        ++seq;
    }

    *length_results_struct = result.size();

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
static
size_t
TabuSearch(const std::vector<Customer> &customers,
        const std::vector<Pickup> &pickups,
        int maxItr,
        std::vector<Solution> &T) {
    Solution S;
    Solution SBest;
    double CBest;
    S = T[0];
    CBest = S.getCost();
    SBest = S;

    S.UpdateSol(customers);

    int n = 0;
    while (n++ < maxItr) {
        S = S.getBestofNeighborhood(S, customers, pickups);
        S.UpdateSol(customers);
        T.push_back(S);
        if (S.getCost() == 0) break;
        if (S.getCost() < CBest) {
            SBest = S;
            CBest = S.getCost();
        } else if (S.getCost() == CBest) {
            // printf("\n****************Repeated Solution****************\n");
            int k = ((12)*maxItr)/100;
            maxItr = maxItr-k;
            // printf("Maxitr after repeating %d k = %d\n",maxItr,k);
        }
    }
    T.push_back(SBest);
    return T.size()-1;
}


/*
 * For each route in the solution:
 *    For each node in the route in the solution:
 *       this is the route.
 *       d, p0, p1 .... pn, d
 *       (rid, nid, cost) 
 *       (1, d, 0)
 *       (1, p0, 0 + dist(d, p0) OR etime)
 */
static
void
get_result(
        Solution &solution,
        const Customers &customers,
        const Depot &depot,
        int64_t VehicleLength,
        std::vector< path_element > &result) {
#if DEBUG
    double last_cost = 0;
    int twv = 0;
    int cv = 0;
#endif
    int route_id = 0;
    int seq = 1;
    solution.UpdateSol(customers);
    for (const auto &route : solution.routes) {
        double agg_cost = 0;
        double distance = 0;
        double agg_load = 0;
        result.push_back({seq, route_id, depot.id, agg_cost});
        ++seq;

        int64_t prev_node = -1;
        for (const auto &node : route.path) {

            if (node == route.path.front()) {
                /*
                 * Is the first node or last node in the path
                 */
                distance = CalculateDistance(depot, customers[node]);
                agg_cost += distance;
            } else {
                /*
                 * Between nodes
                 */
                distance = CalculateDistance(customers[prev_node], customers[node]);
                agg_cost += distance;
            }

            if (agg_cost < customers[node].Etime) {
                /*
                 * Arrving before the opening time, adjust time, moving it to the opening time
                 */
                agg_cost = customers[node].Etime;
            }

            agg_load +=  customers[node].demand;

            result.push_back({
                    seq,
                    route_id,
                    customers[node].id,
                    agg_cost});
#ifdef DEBUG
            result.push_back({
                    customers[node].id,
                    customers[node].Etime,
                    customers[node].Ltime,
                    distance});
            result.push_back({
                    seq,
                    agg_cost > customers[node].Ltime? ++twv: twv,
                    agg_load > 200? ++cv: cv,
                    0});
            last_cost = agg_cost;
#endif
            agg_cost +=  customers[node].Stime;
            prev_node = node;
            ++seq;
        }
        /*
         * Going back to the depot
         */
        agg_cost += CalculateDistance(customers[prev_node], depot);
        result.push_back({seq, route_id, depot.id, agg_cost});
        ++seq;
        ++route_id;
#if 1
        if (VehicleLength < route_id) break;
#endif
    }
    result.push_back({0, 0, 0, solution.getCost()});
}

