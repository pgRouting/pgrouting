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

// #include <cassert>
#include "./../../common/src/pgr_assert.h"

#include "./vehicle_node.h"
#include "./vehicle_pickDeliver.h"
#include "./order.h"
#include "./solution.h"
#include "./initial_solution.h"
#include "./optimize.h"
#include "./pgr_pickDeliver.h"

void
Pgr_pickDeliver::solve() {

#if 0
    solutions.push_back(Initial_solution(0, this));
    solutions.push_back(Initial_solution(1, this));
    solutions.push_back(Initial_solution(2, this));
    solutions.push_back(Initial_solution(3, this));
    solutions.push_back(Initial_solution(4, this));
#endif

    solutions.push_back(Initial_solution(5, this));

    // solutions.push_back(Optimize(1,solutions[0]));


    std::sort(solutions.begin(), solutions.end(), []
            (const Solution &lhs, const Solution &rhs) -> bool {
            return rhs < lhs;
            } );

#if 0
    for (size_t i = 0; i < solutions.size(); i++) {
        log << solutions[i];
    }
    for (size_t i = 0; i < solutions.size(); i++) {
        log << solutions[i].tau();
    }
#endif 
}



/***** Constructor *******/

Pgr_pickDeliver::Pgr_pickDeliver(
        const Customer_t *customers_data, size_t total_customers,
        int p_max_vehicles,
        double p_capacity,
        int p_max_cycles,
        std::string &error) :
    /* Set the depot to be the first ddata found */
    max_capacity(p_capacity),
    max_cycles(p_max_cycles),
    max_vehicles(p_max_vehicles),
    m_starting_site({0, customers_data[0], Tw_node::NodeType::kStart}),
    m_ending_site({0, customers_data[0], Tw_node::NodeType::kEnd}),
    m_original_data(customers_data, customers_data + total_customers) {
        std::ostringstream tmplog;
        error = "";

        log << "\n *** Constructor of problem ***\n";

        /* sort data by id */
        std::sort(m_original_data.begin(), m_original_data.end(),
                [] (const Customer_t &c1, const Customer_t &c2)
                {return c1.id < c2.id;});

        /*
         * starting node:
         * id must be 0
         */
        if (m_original_data[0].id != 0) {
            error = "Depot node not found";
            return;
        }

        m_starting_site = Vehicle_node({0, customers_data[0], Tw_node::NodeType::kStart});
        m_ending_site = Vehicle_node({1, customers_data[0], Tw_node::NodeType::kEnd});
        if (!m_starting_site.is_start()) {
            log << "DEPOT" << m_starting_site;
            error = "Illegal values found on the starting site";
            return;
        }
        pgassert(m_starting_site.is_start());
        pgassert(m_ending_site.is_end());
        m_nodes.push_back(m_starting_site);
        m_nodes.push_back(m_ending_site);


        ID order_id(0);
        ID node_id(2);
        for (const auto p : m_original_data) {
            /*
             * skip Starting site
             */
            if (p.id == 0) continue; 

            /*
             *  SAMPLE CORRECT INFORMATION
             *
             * The Pickup is 11 (pindex == 0)
             * The Deliver is 1 (dindex == 0)
             *
             * id | x  | y  | demand | etime | Ltime | Stime | pindex | dindex
             *  1 | 45 | 68 |    -10 |   912 |   967 |    90 |     11 |      0
             * 11 | 35 | 69 |     10 |   448 |   505 |    90 |      0 |      1
             *
             */ 

            /*
             *  skip deliveries
             */
            if (p.Dindex == 0) continue;

            /*
             * pickup is found
             */
            Tw_node pickup({node_id++, p, Tw_node::NodeType::kPickup});
            if (!pickup.is_pickup()) {
                log << "PICKUP" << pickup;
                tmplog << "Illegal values found on Pickup " << p.id;
                error = tmplog.str();
                return;
            }
            pgassert(pickup.is_pickup());


            /*
             *  look for corresponding the delivery of the pickup
             */
            auto deliver_ptr = std::lower_bound(m_original_data.begin(), m_original_data.end(), p,
                    [] (const Customer_t &delivery, const Customer_t &pick) -> bool
                    {return delivery.id < pick.Dindex;}
                    );

            if (deliver_ptr == m_original_data.end()
                    || deliver_ptr->id != p.Dindex) {
                tmplog << "For Pickup " <<  p.id << " the corresponding Delivery was not found";
                error = tmplog.str();
                return;
            }


            /*
             * delivery is found
             */
            Tw_node delivery(node_id++, (*deliver_ptr), Tw_node::NodeType::kDelivery);
            if (!delivery.is_delivery()) {
                log << "DELIVERY" << delivery;
                tmplog << "Illegal values found on Delivery " << deliver_ptr->id;
                error = tmplog.str();
                return;
            }
            pgassert(delivery.is_delivery());


            /*
             *  add into an order & check the order
             */
            pickup.set_Did(delivery.id());
            delivery.set_Pid(pickup.id());
            m_nodes.push_back(pickup);
            m_nodes.push_back(delivery);

            m_orders.push_back( Order(order_id, node(node_id - 2), node(node_id - 1), this) );
            pgassert(m_orders.back().pickup().is_pickup());
            pgassert(m_orders.back().delivery().is_delivery());
            pgassert(static_cast<Tw_node> (m_orders.back().pickup()) == pickup);

            /*
             *  check the (S,P,D,E) order
             */
            { 
                Vehicle_pickDeliver truck(order_id, m_starting_site, m_ending_site, max_capacity, this);
                truck.push_back(m_orders.back());

                if (!truck.is_feasable()) {
                    log << truck << "\n";
                    tmplog << "The (pickup, delivery) = ("
                        << m_orders.back().pickup().original_id() << ", "
                        << m_orders.back().delivery().original_id() << ") is not feasable";
                    error =  tmplog.str();
                    return;
                };
                pgassert(truck.is_feasable());
            }  // check

            ++order_id;
        }  // for

        /*
         *  double check we found all orders
         */
        if (((m_orders.size() * 2 + 1) - m_original_data.size()) != 0 ) {
            error =  "A pickup was not found";
            return;
        }

        for (auto &o : m_orders) {
            o.setCompatibles();
        }

        for (auto o : m_orders) {
            log << o;
        }
    }  // constructor


const Order
Pgr_pickDeliver::order_of(const Vehicle_node &node) const {
    assert(node.is_pickup() || node.is_delivery());
    if (node.is_pickup()) {
        for (const auto o : m_orders) {
            if (o.pickup().id() == node.id()) {
                return o;
            };
        }
    }
    assert(node.is_delivery());

    for (const auto o : m_orders) {
        if (o.delivery().id() == node.id()) {
            return o;
        }
    }
    assert(false);
}


const Vehicle_node&
Pgr_pickDeliver::node(ID id) const {
    assert (id < m_nodes.size());
    assert (id == m_nodes[id].id());
    return m_nodes[id];
}



#if 0


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


void
Pgr_pickDeliver::TabuSearch(
        const std::vector<Customer_t> &customers,
        const std::vector<Pickup> &pickups,
        int maxItr/*,
                    std::vector<Solution> &T*/) {
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
void
Pgr_pickDeliver::get_result(
        Solution &solution,
        const Customers &customers,
        const Depot &depot,
        int64_t VehicleLength,
        std::vector< General_vehicle_orders_t > &result) {
#if 0
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
#endif
}
#endif
