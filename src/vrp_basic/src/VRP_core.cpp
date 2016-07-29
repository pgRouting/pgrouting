/*PGR-GNU*****************************************************************

Copyright (c) 2013 Khondoker Md. Razequl Islam
ziboncsedu@gmail.com

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

#if defined(__MINGW32__) ||  defined(_MSC_VER)
#include <winsock2.h>
#include <windows.h>
#endif

#include "VRP.h"
#include "VRP_Solver.h"
#include <vector>
#include <string>
#include <exception>

#undef PGR_LOGGER_ON
#define PGR_LOGGER_LOC
#define PGR_LOGGER_FILE "/tmp/vrp-debug.log"
#include "../../common/src/pgr_logger.h"

CVRPSolver solver;

void loadOrders(vrp_orders_t *orders, int order_count, int depotId) {
    int i;
    PGR_LOGF("%s: %d\n", "Depot ID", id);
    for (i = 0; i < order_count; i++) {
        int id = orders[i].id;
        PGR_LOGF("%s: %d\n", "Order ID", id);
        if (id == depotId) {
            PGR_LOG("Got depot");
            // This order represents Deopot
            CDepotInfo depot;

            depot.setDepotId(id);

            Point pt;

            pt.X = orders[i].x;
            pt.Y = orders[i].y;

            depot.setDepotLocation(pt);

            int openTime = orders[i].open_time;
            depot.setOpenTime(openTime);

            int closeTime = orders[i].close_time;
            depot.setCloseTime(closeTime);

            solver.addDepot(depot);

        } else {
            // This is an order
            COrderInfo order;

            order.setOrderId(id);

            Point pt;

            pt.X = orders[i].x;
            pt.Y = orders[i].y;

            order.setOrderLocation(pt);

            int demand = orders[i].order_unit;
            order.setOrderUnit(demand);

            int openTime = orders[i].open_time;
            order.setOpenTime(openTime);

            int closeTime = orders[i].close_time;
            order.setCloseTime(closeTime);

            int serviceTime = orders[i].service_time;
            order.setServiceTime(serviceTime);

            solver.addOrder(order);
        }
    }
}

void loadVehicles(vrp_vehicles_t *vehicles, int vehicle_count) {
    int i;
    for (i = 0; i < vehicle_count; i++) {
        CVehicleInfo vehicle;

        int id = vehicles[i].id;
        vehicle.setId(id);

        int capcity = vehicles[i].capacity;
        vehicle.setCapacity(capcity);

        vehicle.setCostPerKM(1);

        solver.addVehicle(vehicle);
    }
}

void loadDistanceMatrix(vrp_cost_element_t *costmatrix, int cost_count, int depotId) {
    int i;
    for (i = 0; i < cost_count; i++) {
        int fromId = costmatrix[i].src_id;
        int toId = costmatrix[i].dest_id;
        CostPack cpack;
        cpack.cost = costmatrix[i].cost;
        cpack.distance = costmatrix[i].distance;
        cpack.traveltime = costmatrix[i].traveltime;

        if (fromId == depotId)
            solver.addDepotToOrderCost(fromId, toId, cpack);
        else if (toId == depotId)
            solver.addOrderToDepotCost(fromId, toId, cpack);
        else
            solver.addOrderToOrderCost(fromId, toId, cpack);
    }
}


int find_vrp_solution(vrp_vehicles_t *vehicles, size_t vehicle_count,
                      vrp_orders_t *orders, size_t order_count,
                      vrp_cost_element_t *costmatrix, size_t cost_count,
                      int depot_id,
                      vrp_result_element_t **results, size_t *result_count, char **err_msg) {
    int res;

    std::string strError;
    try {
        PGR_LOG("Before load order");
        loadOrders(orders, static_cast<int>(order_count), depot_id);
        PGR_LOG("After load order");
        loadVehicles(vehicles, static_cast<int>(vehicle_count));
        PGR_LOG("After load vehicles");
        loadDistanceMatrix(costmatrix, static_cast<int>(cost_count), depot_id);
        PGR_LOG("After load distance matrix");
        res = solver.solveVRP(strError);
        PGR_LOG("After VRP Solve");
    }
    catch(std::exception& e) {
        *err_msg = (char *) e.what();
        return -1;
    }
    catch(...) {
        *err_msg = (char *) "Caught unknown exception!";
        return -1;
    }


    if (res < 0) {
        return res;
    } else {
        try {
        CSolutionInfo solution;
        CTourInfo ctour;
        // bool bOK =
                solver.getSolution(solution, strError);
        auto totalRoute = solution.getTourInfoVector().size();
        size_t totRows = 0;
        for (size_t i = 0; i < totalRoute; i++) {
            totRows += (solution.getTour(static_cast<int>(i)).getServedOrderCount() + 2);
        }
        *results = (vrp_result_element_t *) malloc(totRows * sizeof(vrp_result_element_t));
        *result_count = totRows;
        int cnt = 0;
        for (size_t i = 0; i < totalRoute; i++) {
            ctour = solution.getTour(static_cast<int>(i));
            std::vector<int> vecOrder = ctour.getOrderVector();
            auto totalOrder = vecOrder.size();

            // For start depot
            (*results)[cnt].order_id = ctour.getStartDepot();
            (*results)[cnt].order_pos = 0;
            (*results)[cnt].vehicle_id = ctour.getVehicleId();
            (*results)[cnt].arrival_time = -1;
            (*results)[cnt].depart_time = ctour.getStartTime(0);
            cnt++;

            // For each order
            for (size_t j = 0; j < totalOrder; j++) {
                (*results)[cnt].order_id = vecOrder[j];
                (*results)[cnt].order_pos = static_cast<int>(j) + 1;
                (*results)[cnt].vehicle_id = ctour.getVehicleId();
                (*results)[cnt].depart_time = ctour.getStartTime(static_cast<int>(j) + 1);
                (*results)[cnt].arrival_time = ctour.getStartTime(static_cast<int>(j) + 1) - solver.getServiceTime(vecOrder[j]);
                cnt++;
            }

            // For return depot
            (*results)[cnt].order_id = ctour.getEndDepot();
            (*results)[cnt].order_pos = static_cast<int>(totalOrder) + 1;
            (*results)[cnt].vehicle_id = ctour.getVehicleId();
            (*results)[cnt].arrival_time = ctour.getStartTime(static_cast<int>(totalOrder) + 1);
            (*results)[cnt].depart_time = -1;
            cnt++;
        }
        }
        catch(std::exception& e) {
        *err_msg = (char *) e.what();
        return -1;
        }
        catch(...) {
        *err_msg = (char *) "Caught unknown exception!";
        return -1;
        }
    }
    return EXIT_SUCCESS;
}

