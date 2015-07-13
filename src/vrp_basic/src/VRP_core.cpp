#include "VRP.h"
#include "VRP_Solver.h"
#include <exception>
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#undef PGR_LOGGER_ON
#define PGR_LOGGER_LOC
#define PGR_LOGGER_FILE "/tmp/vrp-debug.log"
#include "../../common/src/pgr_logger.h"

CVRPSolver solver;

void loadOrders(vrp_orders_t *orders, int order_count, int depotId)
{
	int i;
    PGR_LOGF("%s: %d\n", "Depot ID", id)
	for(i = 0; i < order_count; i++)
	{
		int id = orders[i].id;
        PGR_LOGF("%s: %d\n", "Order ID", id)
		if (id == depotId)
		{
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
			
		}
		else
		{
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

void loadVehicles(vrp_vehicles_t *vehicles, int vehicle_count)
{
	int i;
	for(i = 0; i < vehicle_count; i++)
	{
		CVehicleInfo vehicle;

		int id = vehicles[i].id;
		vehicle.setId(id);

		int capcity = vehicles[i].capacity;
		vehicle.setCapacity(capcity);

		vehicle.setCostPerKM(1);

		solver.addVehicle(vehicle);
	}
	
}

void loadDistanceMatrix(vrp_cost_element_t *costmatrix, int cost_count, int depotId)
{
	int i;
	for(i = 0; i < cost_count; i++)
	{
		int fromId = costmatrix[i].src_id;
		int toId = costmatrix[i].dest_id;
		CostPack cpack;
		cpack.cost = costmatrix[i].cost;
		cpack.distance = costmatrix[i].distance; 
		cpack.traveltime = costmatrix[i].traveltime;

		if(fromId == depotId)
			solver.addDepotToOrderCost(fromId, toId, cpack);
		else if(toId == depotId)
			solver.addOrderToDepotCost(fromId, toId, cpack);
		else
			solver.addOrderToOrderCost(fromId, toId, cpack);
	}
	
}


int find_vrp_solution(vrp_vehicles_t *vehicles, int vehicle_count,
					  vrp_orders_t *orders, int order_count,
					  vrp_cost_element_t *costmatrix, int cost_count,
					  int depot_id,
					  vrp_result_element_t **results, int *result_count, char **err_msg)
{
	int res;
	
	std::string strError;
	try {
		PGR_LOG("Before load order");
		loadOrders(orders, order_count, depot_id);
		PGR_LOG("After load order");
		loadVehicles(vehicles, vehicle_count);
		PGR_LOG("After load vehicles");
		loadDistanceMatrix(costmatrix, cost_count, depot_id);
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

	
	if (res < 0)
		return res;
	else
	{
		try {
		CSolutionInfo solution;
		CTourInfo ctour;
		bool bOK = solver.getSolution(solution, strError);
		int totalRoute = solution.getTourInfoVector().size();
		int totRows = 0;
		int i;
		for(i = 0; i < totalRoute; i++)
		{
			totRows += (solution.getTour(i).getServedOrderCount() + 2);
		}
		*results = (vrp_result_element_t *) malloc(totRows * sizeof(vrp_result_element_t));
		*result_count = totRows;
		int cnt = 0;
		for(int i = 0; i < totalRoute; i++)
		{
			ctour = solution.getTour(i);
			std::vector<int> vecOrder = ctour.getOrderVector();
			int totalOrder = vecOrder.size();

			// For start depot
			(*results)[cnt].order_id = ctour.getStartDepot();
			(*results)[cnt].order_pos = 0;
			(*results)[cnt].vehicle_id = ctour.getVehicleId();
			(*results)[cnt].arrival_time = -1;
			(*results)[cnt].depart_time = ctour.getStartTime(0);
			cnt++;
			
			// For each order
			for(int j = 0; j < totalOrder; j++)
			{
				(*results)[cnt].order_id = vecOrder[j];
				(*results)[cnt].order_pos = j + 1;
				(*results)[cnt].vehicle_id = ctour.getVehicleId();
				(*results)[cnt].depart_time = ctour.getStartTime(j + 1);
				(*results)[cnt].arrival_time = ctour.getStartTime(j + 1) - solver.getServiceTime(vecOrder[j]);
				cnt++;
			}
			
			// For return depot
			(*results)[cnt].order_id = ctour.getEndDepot();
			(*results)[cnt].order_pos = totalOrder + 1;
			(*results)[cnt].vehicle_id = ctour.getVehicleId();
			(*results)[cnt].arrival_time = ctour.getStartTime(totalOrder + 1);
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

