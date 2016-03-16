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


#include <sstream>
#include <vector>
#include <algorithm>
extern "C" {
#include "./../../common/src/pgr_types.h"
}


#include "./Solution.h"
#include "./pdp_solver.h"
#include "./tw_node.h"
#include "./order.h"


//forward declaration
static
void
TabuSearch(
        const std::vector<Customer_t> &customers,
        const std::vector<Pickup> &pickups,
        int maxIter,
        std::vector<Solution> &T);

static
void
get_result(
        Solution &solution,
        const std::vector < Customer_t > &customers,
        const Depot &depot,
        int64_t VehicleLength,
        std::vector< General_vehicle_orders_t > &result);



int64_t Solver(
        Customer_t *customers_data, size_t total_customers,
        int VehicleLength,
        double capacity,
        int max_cycles,
        General_vehicle_orders_t **results,
        size_t &length_results_struct,
        std::ostringstream &log){
    log << "\n *** Solver ***\n";


    std::vector<Customer_t> customers(customers_data, customers_data + total_customers);
    std::vector<Pickup> pickups;
    std::vector<Route> routes;

    /* sort customenrs by id */
    std::sort(customers.begin(), customers.end(),
            [] (const Customer_t &c1, const Customer_t &c2)
            {return c1.id < c2.id;});

    


#if 1
    for (const auto c : customers) {
        log << c.id << "("
            << c.x << ","
            << c.y << ","
            << c.demand << ","
            << c.Etime << ","
            << c.Ltime << ","
            << c.Stime << ","
            << c.Pindex << ","
            << c.Dindex << "\n";
    };
#endif

    /* starting node:
     * id must be 0
     */
    if (customers[0].id != 0) {
        log << "ERROR: Depot node not found\n";
        return 400;
    }


    /* starting node:
     * This are ignored
     * Dindex must be 0
     * Pindex must be 0
     * demand must be 0 (truck starts empty)
     */
    Tw_node depot1({0, customers_data[0], Tw_node::NodeType::kStart});

    log << "DEPOT: " << depot1 << "\n";


#ifndef OLDCODE
    Depot depot({customers_data[0].id, customers_data[0].x, customers_data[0].y,
            customers_data[0].demand,
            customers_data[0].Etime, customers_data[0].Ltime, customers_data[0].Stime,
            customers_data[0].Pindex, customers_data[0].Dindex
            });
#endif

    for (const auto p : customers) {
        if (p.id == 0) continue; 
        if (p.Dindex == 0) continue;
        /* pickup is found */
        Tw_node pickup({0, p, Tw_node::NodeType::kPickup});
        auto deliver_ptr = std::lower_bound(customers.begin(), customers.end(), p,
                [] (const Customer_t &delivery, const Customer_t &pick) -> bool
                //[] (const Customer_t &c1, int64_t val) -> bool
                {return delivery.id < pick.Dindex;}
                );
        if (deliver_ptr == customers.end()) {
            log << "ERROR: NOT FOUND corresponding delivery of: " << p.id <<"\n";
            return 0;
        }
        Tw_node delivery(0, (*deliver_ptr), Tw_node::NodeType::kDelivery);
        Order order(1, pickup, delivery);
        log << order << "\n";
        // log << "ORDER: " << order.pickup() << "\n" << order.delivery();
    }



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

    if (pickups.size() != 53) {
        (*results) = NULL;
        length_results_struct = 0;
        return 0;
    }

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

    TabuSearch(customers, pickups, max_cycles, T);


    std::vector< General_vehicle_orders_t > result;
#ifdef DEBUG
    for (auto &solution: T) {
        get_result(solution, customers, depot, VehicleLength, result);
    }
#else
    T.back().UpdateSol(customers);
    get_result(T.back(), customers, depot, VehicleLength, result);
#endif



    // Getting memory to store results
    *results = static_cast<General_vehicle_orders_t *>(malloc(sizeof(General_vehicle_orders_t) * (result.size())));

    //store the results
    int seq = 0;
    for (const auto &row : result) {
        (*results)[seq] = row;
        ++seq;
    }

    length_results_struct = result.size();

    // log << "FINISH";;
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
void
TabuSearch(const std::vector<Customer_t> &customers,
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
}


/*
 * For each route in the solution:
 *    For each node in the route in the solution:
 *       this is the route.
 *       example for 1 vehicle
 *       1 1 1 id=0 deltaLoad=0 departingload = 0 travelTime= 0 arrivalTime=0   serviceTime=0 waitTime=0 departureTime=0 totalLoad
 *       2 2 1 id=1 deltaLoad arrivalTime=d01 travelTime=d01 serviceTime=service(1) waitTime=(0 or waittime)  departureTime = arrivalTime + waitTime + service(1)  totalLoad
 *
 */
static
void
get_result(
        Solution &solution,
        const Customers &customers,
        const Depot &depot,
        int64_t VehicleLength,
        std::vector< General_vehicle_orders_t > &result) {
#if DEBUG
    double last_cost = 0;
    int twv = 0;
    int cv = 0;
#endif
    int vehicle_id = 1;
    solution.UpdateSol(customers);
    double agg_cost = 0;

    /* each route is a vehicle */
    for (const auto &route : solution.routes) {
        double departureTime = 0;
        double travelTime = 0;
        double agg_load = 0;

        /* starting a new vehicle */
        int vehicle_seq = 1;

        /* first stop is the depot */
        result.push_back({vehicle_seq, vehicle_id, depot.id, travelTime, 0, 0, 0, departureTime});

        ++vehicle_seq;

        int64_t prev_node = -1;
        for (const auto &node : route.path) {

            /****************
             *  travelTime
             * *********** */
            if (node == route.path.front()) {
                /*
                 * Is the first node or last node in the path
                 */
                travelTime = CalculateDistance(depot, customers[node]);
            } else {
                /*
                 * Between nodes
                 */
                travelTime = CalculateDistance(customers[prev_node], customers[node]);
            }

            /****************
             *  arrivalTime
             * *********** */
            double arrivalTime = departureTime + travelTime;



            /****************
             *  waitTime
             * *********** */
            double waitTime(0);
            if (arrivalTime < customers[node].Etime) {
                /*
                 * Arrving before the opening time, wait until it opens
                 */
                waitTime = customers[node].Etime - arrivalTime;
            }

            /* *************
             * serviceTime
             * ********** */
            double serviceTime = customers[node].Stime;

            /* *************
             * departureTime
             * ********** */
            departureTime = arrivalTime + waitTime + serviceTime;

            agg_load +=  customers[node].demand;

            result.push_back({
                    vehicle_seq,
                    vehicle_id,
                    customers[node].id,
                    travelTime,
                    arrivalTime,
                    waitTime,
                    serviceTime,
                    departureTime});
#ifdef DEBUG
            result.push_back({
                    customers[node].id,
                    customers[node].Etime,
                    customers[node].Ltime,
                    distance, distance});
            result.push_back({
                    vehicle_seq,
                    agg_cost > customers[node].Ltime? ++twv: twv,
                    agg_load > 200? ++cv: cv,
                    0, 0});
            last_cost = agg_cost;
#endif
            agg_cost +=  customers[node].Stime;
            prev_node = node;
            ++vehicle_seq;
        }
        /*
         * Going back to the depot
         */
        travelTime = CalculateDistance(customers[prev_node], depot);
        double arrivalTime = departureTime + travelTime;

        result.push_back({vehicle_seq, vehicle_id, depot.id, travelTime, arrivalTime, 0, 0, 0});
        ++vehicle_seq;
        ++vehicle_id;
#if 1
        if (VehicleLength < vehicle_id) break;
#endif
    }
    // result.push_back({0, 0, 0, solution.getCost(), solution.getCost()});
}

