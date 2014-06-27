#include "VRP_Solver.h"
#include "Utils.h"

char buff[1005];

CVRPSolver solver;

// Load orders from the order file. The first order represents the depot.
// TODO: file names are hard coded, it should be changed to commandline argument.
void loadOrders()
{
	FILE *fp = fopen("Orders.txt", "rt");
	if(fp == NULL)
	{
		fprintf(stderr, "Order file not found!\n");
	}
	bool bGotDepot = false;
	// OrderId XCord YCord Demand StartTime EndTime ServiceTime
	while(fgets(buff, 1000, fp))
	{
		if(strlen(buff) == 0)
			break;
		StringTokenizer tokenizer;
		tokenizer.parse(buff, " \t");
		StringVector vecToken;
		tokenizer.getTokens(vecToken);

		if(vecToken.size() != 7)
			continue;

		if(!isdigit(vecToken[0][0]))
			continue;
		
		if (!bGotDepot)
		{
			// This order represents Deopot
			CDepotInfo depot;
			int id = atoi(vecToken[0].c_str());
			depot.setDepotId(id);

			Point pt;

			pt.X = atof(vecToken[1].c_str());
			pt.Y = atof(vecToken[2].c_str());

			depot.setDepotLocation(pt);

			int openTime = atoi(vecToken[4].c_str());
			depot.setOpenTime(openTime);

			int closeTime = atoi(vecToken[5].c_str());
			depot.setCloseTime(closeTime);

			solver.addDepot(depot);
			bGotDepot = true;
		}
		else
		{
			// This is an order
			COrderInfo order;
			int id = atoi(vecToken[0].c_str());
			order.setOrderId(id);

			Point pt;

			pt.X = atof(vecToken[1].c_str());
			pt.Y = atof(vecToken[2].c_str());

			order.setOrderLocation(pt);

			int demand = atoi(vecToken[3].c_str());
			order.setOrderUnit(demand);

			int openTime = atoi(vecToken[4].c_str());
			order.setOpenTime(openTime);

			int closeTime = atoi(vecToken[5].c_str());
			order.setCloseTime(closeTime);

			int serviceTime = atoi(vecToken[6].c_str());
			order.setServiceTime(serviceTime);

			solver.addOrder(order);
		}
	}
	fclose(fp);
}

// Load vehicles from vehicle file.
// TODO: file names are hard coded, it should be changed to commandline argument.

void loadVehicles()
{
	FILE *fp = fopen("Vehicles.txt", "rt");

	if(fp == NULL)
	{
		fprintf(stderr, "Vehicle file not found!\n");
	}
	// VehicleId Capacity 
	// In terms of cost all the vehicle will have default cost of 1 for the first version
	while(fgets(buff, 1000, fp))
	{
		if(strlen(buff) == 0)
			break;
		StringTokenizer tokenizer;
		tokenizer.parse(buff, " \t");
		StringVector vecToken;
		tokenizer.getTokens(vecToken);

		if(vecToken.size() != 2)
			continue;

		if(!isdigit(vecToken[0][0]))
			continue;

		CVehicleInfo vehicle;

		int id = atoi(vecToken[0].c_str());
		vehicle.setId(id);
		
		int capcity = atoi(vecToken[1].c_str());
		vehicle.setCapacity(capcity);

		vehicle.setCostPerKM(1);

		solver.addVehicle(vehicle);
	}
	fclose(fp);
}

// Load the cost matrix
// TODO: file names are hard coded, it should be changed to commandline argument.
void loadDistanceMatrix()
{
	FILE *fp = fopen("Distance.txt", "rt");
	if(fp == NULL)
	{
		fprintf(stderr, "Cost file not found!\n");
		return;
	}

	// From To Cost
	while(fgets(buff, 1000, fp))
	{
		if(strlen(buff) == 0)
			break;
		StringTokenizer tokenizer;
		tokenizer.parse(buff, " \t");
		StringVector vecToken;
		tokenizer.getTokens(vecToken);

		if(vecToken.size() != 3)
			continue;

		if(!isdigit(vecToken[0][0]))
			continue;

		int fromId = atoi(vecToken[0].c_str());
		int toId = atoi(vecToken[1].c_str());
		CostPack cpack;
		cpack.cost = cpack.distance = atof(vecToken[2].c_str());
		cpack.traveltime = cpack.cost;
		
		if(fromId == 1)
			solver.addDepotToOrderCost(fromId, toId, cpack);
		else if(toId == 1)
			solver.addOrderToDepotCost(fromId, toId, cpack);
		else
			solver.addOrderToOrderCost(fromId, toId, cpack);
	}
	fclose(fp);		
}

// Print the solution to a file.
// TODO: Currently prints on a fixed file. Later the file name will be taken as a command line argument

bool print_solution(std::string strError)
{
	FILE *fp = fopen("result.txt", "wt");
	if(fp == NULL)
	{
		strError = "Could not open file";
		return false;
	}
	CSolutionInfo solution;
	bool bOK = solver.getSolution(solution, strError);
	if(bOK == false)
		return false;


	int to=0;
	int totalRoute = solution.getTourInfoVector().size();
	CTourInfo ctour;

	fprintf(fp, "Total Number of Route: %d\n", totalRoute);
	fprintf(fp, "Total Cost: %.3lf\n", solution.getTotalCost());
	fprintf(fp, "Total Distance: %.3lf\n", solution.getTotalDistance());
	fprintf(fp, "Total TravelTime: %.3lf\n", solution.getTotalTravelTime());

	for(int i = 0; i < totalRoute; i++)
	{
		ctour = solution.getTour(i);
		fprintf(fp, "Route No. %d: \n", i + 1);
		fprintf(fp, "Vehicle Id: %d\n", ctour.getVehicleId());
		fprintf(fp, "Starting Depot Id: %d\n", ctour.getStartDepot());
		fprintf(fp, "End Depot Id: %d\n", ctour.getEndDepot());

		std::vector<int> vecOrder = ctour.getOrderVector();
		int totalOrder = vecOrder.size();
		to+=totalOrder;
		fprintf(fp, "Visited Order Ids: ");
		for(int j = 0; j < totalOrder; j++)
		{
			if(j > 0)
				fprintf(fp, " ");
			fprintf(fp, "%d", vecOrder[j]);
			if(j < totalOrder - 1)
				fprintf(fp, ",");
		}
		fprintf(fp, "\n");
	}

		fprintf(fp, "total order served %d\n", to);
	fclose(fp);
	return true;
}

int main()
{
	loadOrders();
	loadVehicles();
	loadDistanceMatrix();
	
	std::string strError;
	
	bool bIsOK = solver.solveVRP(strError);
	
	if(!bIsOK)
	{
		fprintf(stderr, "Error Occurred: %s\n", strError.c_str());
	}
	else
	{
		print_solution(strError);
	}


	return 0;
}
