#include "VRP_Solver.h"

#undef PGR_LOGGER_ON
#define PGR_LOGGER_LOC
#define PGR_LOGGER_FILE "/tmp/vrp-debug.log"
#include "../../common/src/pgr_logger.h"

#define DOUBLE_MAX 1e50

bool operator != (const CVehicleInfo& cur, const CVehicleInfo& that)
{
	return(cur.m_iVehicleId != that.m_iVehicleId);
}

bool operator== (const CTourInfo& cur, const CTourInfo& that)
{
	if(cur.m_vehicleInfo != that.m_vehicleInfo)
		return false;
	if(cur.m_viOrderIds.size() != that.m_viOrderIds.size())
		return false;
	int tot = cur.m_viOrderIds.size();
	for(int i = 0; i < tot; i++)
	{
		if(cur.m_viOrderIds[i] != that.m_viOrderIds[i])
			return false;
	}
	return true;
}

bool operator == (const CMoveInfo& cur, const CMoveInfo& that)
{
	if(!(cur.m_vInitialTour == that.m_vInitialTour))
		return false;
	if(!(cur.m_vModifiedTour == that.m_vModifiedTour))
		return false;
	return true;
}



bool Compare::operator() ( pq_pair p1, pq_pair p2)
{
	if((p1.cost).cost < (p2.cost).cost)

		return true;
	return false;
}



CVehicleInfo::CVehicleInfo()
{
	m_iCurrentLoad = 0;
}
CVehicleInfo::~CVehicleInfo()
{
}

bool CVehicleInfo::loadUnit(int lUnit)
{
	if(m_iCurrentLoad + lUnit > m_iCapacity)
		return false;
	m_iCurrentLoad += lUnit;
	return true;
}

COrderInfo::COrderInfo()
{
}
COrderInfo::~COrderInfo()
{
}

CDepotInfo::CDepotInfo()
{
}
CDepotInfo::~CDepotInfo()
{
}

CTourInfo::CTourInfo()
{
	m_dTotalCost = 0.0;
	m_dTotalDistance = 0.0;
	m_dTotalTraveltime = 0.0;
}
CTourInfo::~CTourInfo()
{
}

bool CTourInfo::insertOrder(int orderId, int pos)
{
	m_viOrderIds.insert(m_viOrderIds.begin() + pos, orderId);
	return true;
}

int CTourInfo::getRemainingCapacity()
{
	return(m_vehicleInfo.getRemainingCapacity());
}

bool CTourInfo::removeOrder(int pos)
{
	m_viOrderIds.erase(m_viOrderIds.begin() + pos);
	return true;
}

void CTourInfo::updateCost(double cost, double distance, double travelTime)
{
	m_dTotalCost = cost;
	m_dTotalDistance = distance;
	m_dTotalTraveltime = travelTime;
}

CSolutionInfo::CSolutionInfo()
{
}
CSolutionInfo::~CSolutionInfo()
{
}

void CSolutionInfo::replaceTour(CTourInfo curTour)
{
	int i;
	for(i = 0; i < m_vtourAll.size(); i++)
	{
		if(m_vtourAll[i].getVehicleId() == curTour.getVehicleId())
		{
			m_vtourAll[i] = curTour;
			return;
		}
	}
	return;
}

void CSolutionInfo::replaceTourAt(int index, CTourInfo curTour)
{
	if(index < 0 || index >= m_vtourAll.size())
		return;
	m_vtourAll[index] = curTour;
}

bool CSolutionInfo::init(std::vector<int> vecOrder, int iTotalOrder, std::vector<int> vecVehicle,std::priority_queue<pq_pair,std::vector<pq_pair>,Compare> pq)
{
	m_vUnservedOrderId = vecOrder;
	m_iTotalOrders = iTotalOrder;
	m_vUnusedVehicles =  vecVehicle;
	m_pq=pq;

	m_vtourAll.clear();
	m_iVehicleUsed = 0;
	m_iOrdersServed = 0;
	m_iTotalOrders = 0;
	m_dTotalCost = 0;
	m_dTotalDistance = 0;
	m_dTotalTravelTime = 0;
	return true;
}

bool CSolutionInfo::addTour(CTourInfo& tour)
{
	m_vtourAll.push_back(tour);
	int vid = tour.getVehicleId();
	std::vector<int>::iterator it;
	it = std::find(m_vUnusedVehicles.begin(), m_vUnusedVehicles.end(), vid);
	if(it != m_vUnusedVehicles.end())
	{
		m_vUnusedVehicles.erase(it);
	}
	m_iVehicleUsed++;
	m_dTotalDistance += tour.getDistance();
	m_dTotalTravelTime += tour.getTravelTime();
	m_dTotalCost += tour.getCost();

	std::vector<int> vecOrders = tour.getOrderVector();

	m_iOrdersServed += vecOrders.size();

	for(int i = 0; i < vecOrders.size(); i++)
	{
		int oid = vecOrders[i];
		it = std::find(m_vUnservedOrderId.begin(), m_vUnservedOrderId.end(), oid);
		if(it != m_vUnservedOrderId.end())
		{
			m_vUnservedOrderId.erase(it);
		}
	}

	return true;
}

CMoveInfo::CMoveInfo()
{

}
CMoveInfo::~CMoveInfo()
{

}

void CMoveInfo::setInitialTour(CTourInfo tourData)
{
	m_vInitialTour.clear();
	m_vInitialTour.push_back(tourData);
}

void CMoveInfo::setInitialTour(CTourInfo tourData1, CTourInfo tourData2)
{
	m_vInitialTour.clear();
	m_vInitialTour.push_back(tourData1);
	m_vInitialTour.push_back(tourData2);
}

void CMoveInfo::setModifiedTour(CTourInfo tourData)
{
	m_vModifiedTour.clear();
	m_vModifiedTour.push_back(tourData);
}

void CMoveInfo::setModifiedTour(CTourInfo tourData1, CTourInfo tourData2)
{
	m_vModifiedTour.clear();
	m_vModifiedTour.push_back(tourData1);
	m_vModifiedTour.push_back(tourData2);
}

void CMoveInfo::getInitialTour(CTourInfo &tourData)
{
	tourData = m_vInitialTour[0];
}

void CMoveInfo::getInitialTour(CTourInfo &tourData1, CTourInfo &tourData2)
{
	tourData1 = m_vInitialTour[0];
	tourData2 = m_vInitialTour[1];
}

bool CMoveInfo::getModifiedTourAt(int index, CTourInfo& tourInfo)
{
	if(index < 0 || index >= m_vModifiedTour.size())
		return false;
	tourInfo = m_vModifiedTour[index];
}



CVRPSolver::CVRPSolver()
{
    // set a seed for the random number generator
    // so it will generate consistent results for the same input
    // otherwise we can not test it :(
    srand(1726354);
}
CVRPSolver::~CVRPSolver()
{
}

bool CVRPSolver::solveVRP(std::string& strError)
{
//	if(!m_bIsReadyToSolve)
//	{
//		strError = "Scenario is not ready to solve. Configure all parameter";
//		return false;
//	}
	PGR_LOG("Inside Solve VRP");
	std::vector<int> vecOrders, vecVehicles;
	for(int i = 0; i < m_vOrderInfos.size(); i++)
	{
		vecOrders.push_back(m_vOrderInfos[i].getOrderId());
	}

	for(int i = 0; i < m_vVehicleInfos.size(); i++)
	{
		vecVehicles.push_back(m_vVehicleInfos[i].getId());
	}
	
	m_solutionFinal.init(vecOrders, vecOrders.size(), vecVehicles,do_pq);
	PGR_LOG("After init solution");
		CSolutionInfo initialSolution = generateInitialSolution();
		PGR_LOG("After Generate initial Solution");
	return true;

}

CSolutionInfo CVRPSolver::generateInitialSolution()
{
	CSolutionInfo initialSolution;
	PGR_LOG("Inside gen ini sol");
	std::vector<int> vecOrders, vecVehicles;
	for(int i = 0; i < m_vOrderInfos.size(); i++)
	{
		vecOrders.push_back(m_vOrderInfos[i].getOrderId());
	}

	for(int i = 0; i < m_vVehicleInfos.size(); i++)
	{
		vecVehicles.push_back(m_vVehicleInfos[i].getId());
	}

	initialSolution.init(vecOrders, vecOrders.size(), vecVehicles,do_pq);

	int iUnusedVehicles = initialSolution.getUnusedVehicleCount();
	int iUnservedOrders = initialSolution.getUnservedOrderCount();
	PGR_LOG("before while");


        /* The sequnetial construction will be implemented here using Hill climbing routing heuristics  */

	/* so basically this function will provide us with a feasible solution and then we will use that solution to get an optimal solution using Tabu search */
        /* pseudo code 
           
	   while ( vehicle_not_available or total_orders_have_been_assigned)
	       {           
	                                                                       
	                   select a randome vehicles from the available ones .
			   remove it form the ususedvehicle list ;
			   NEW_TOUR;	

		(for all unassigned orders)     these orders are stored in sorted manner in a protiy queue(dist between depo and order  
			             get the next order 
				     append it in the current tour ;
				     call the hill climbing routing heuristic for the current tour of that vehicle
				     if( current tour is feasible )
				             mark the order as completed 
			              else
				            remove order from the current tour;
				     
			    
	   }
          */

	return initialSolution;
}

bool CVRPSolver::updateFinalSolution(CSolutionInfo& curSolution)
{
	bool callUpdate = false;
	if ( curSolution.getOrderServed() > m_solutionFinal.getOrderServed() )
	{
		callUpdate = true;
	}
	else if ( curSolution.getOrderServed() == m_solutionFinal.getOrderServed() )
	{
		if ( curSolution.getTotalCost() < m_solutionFinal.getTotalCost() )
		{
			callUpdate = true;
		}
		else if (curSolution.getTotalCost() == m_solutionFinal.getTotalCost() )
		{
			if ( curSolution.getTotalTravelTime() < m_solutionFinal.getTotalTravelTime() )
			{
				callUpdate = true;
			}
			else if ( curSolution.getTotalTravelTime() == m_solutionFinal.getTotalTravelTime()  )
			{
				if ( curSolution.getTotalDistance() < m_solutionFinal.getTotalDistance()  )
				{
					callUpdate = true;
				}
			}
		}
	}
	if (callUpdate)
	{
		//m_iStepsSinceLastSolution = 0;
		m_solutionFinal = curSolution;

		//clear map and delete objects
		//m_mpTabuCount.clear();
		//for (std::map< CVRPTWMove*,int >::iterator it = m_mpMoveFrequency.begin();it!=m_mpMoveFrequency.end();++it)
		//{
		//	delete (*it).first;
		//}
		//m_mpMoveFrequency.clear();
		return true;
	}
	return false;
}




bool CVRPSolver::addDepot(CDepotInfo depotInfo)
{
	int id = depotInfo.getDepotId();
	if(m_mapDepotIdToIndex.find(id) != m_mapDepotIdToIndex.end())
		return false;
	m_mapDepotIdToIndex.insert(std::make_pair(id, m_vDepotInfos.size()));
	m_vDepotInfos.push_back(depotInfo);
	
	return true;
}

bool CVRPSolver::addOrder(COrderInfo orderInfo)
{
	int id = orderInfo.getOrderId();
	if(m_mapOrderIdToIndex.find(id) != m_mapOrderIdToIndex.end())
	{
		return false;
	}
	int index = m_vOrderInfos.size();
	m_mapOrderIdToIndex.insert(std::make_pair(id, index));
	m_vOrderInfos.push_back(orderInfo);
	m_viUnservedOrderIndex.push_back(index);
	return true;
}

bool CVRPSolver::addVehicle(CVehicleInfo vehicleInfo)
{
	int id = vehicleInfo.getId();
	if(m_mapVehicleIdToIndex.find(id) != m_mapVehicleIdToIndex.end())
	{
		return false;
	}
	int index = m_vVehicleInfos.size();
	m_mapVehicleIdToIndex.insert(std::make_pair(id, index));
	m_vVehicleInfos.push_back(vehicleInfo);
	m_viUnusedVehicleIndex.push_back(index);
	return true;
}

bool CVRPSolver::addDepotToOrderCost(int depotId, int orderId, CostPack cost)
{
	PII depo_order = std::make_pair(depotId, orderId);
	if(m_mapDepotToOrderrCost.find(depo_order) != m_mapDepotToOrderrCost.end())
	{
		return false;
	}
	m_mapDepotToOrderrCost.insert(make_pair(depo_order, cost));

	pq_pair pq;
	pq.d_o_pair=depo_order;
	pq.cost=cost;
	do_pq.push(pq);
	return true;
}

bool CVRPSolver::addOrderToDepotCost(int depotId, int orderId, CostPack cost)
{
	PII depo_order = std::make_pair(orderId, depotId);
	if(m_mapOrderToDepotCost.find(depo_order) != m_mapOrderToDepotCost.end())
	{
		return false;
	}
	m_mapOrderToDepotCost.insert(std::make_pair(depo_order, cost));
	return true;
}

bool CVRPSolver::addOrderToOrderCost(int firstOrder, int secondOrder, CostPack cost)
{
	PII order_order = std::make_pair(firstOrder, secondOrder);
	if(m_mapOrderToOrderCost.find(order_order) != m_mapOrderToOrderCost.end())
	{
		return false;
	}
	m_mapOrderToOrderCost.insert(std::make_pair(order_order, cost));
	return true;
}

bool CVRPSolver::getSolution(CSolutionInfo& solution, std::string& strError)
{
	if(m_bIsSolutionReady == true)
	{
		solution = m_solutionFinal;
		return true;
	}
	else
	{
		bool ret = solveVRP(strError);
		if(ret == true)
		{
			solution = m_solutionFinal;
			return true;
		}
		return false;
	}
}

CostPack CVRPSolver::getDepotToOrderCost(int depotId, int orderId)
{
	PII depo_order = std::make_pair(depotId, orderId);

	if(m_mapDepotToOrderrCost.find(depo_order) != m_mapDepotToOrderrCost.end())
	{
		return(m_mapDepotToOrderrCost[depo_order]);
	}
	CostPack ret;
	ret.cost = ret.distance = ret.traveltime = 1e15;
	return ret;
}

CostPack CVRPSolver::getOrderToOrderCost(int orderId1, int orderId2)
{
	PII order_order = std::make_pair(orderId1, orderId2);

	if(m_mapOrderToOrderCost.find(order_order) != m_mapOrderToOrderCost.end())
	{
		return(m_mapOrderToOrderCost[order_order]);
	}
	CostPack ret;
	ret.cost = ret.distance = ret.traveltime = 1e15;
	return ret;
}


CostPack CVRPSolver::getOrderToDepotCost(int depotId, int orderId)
{
	PII depo_order = std::make_pair(orderId, depotId);

	if(m_mapOrderToDepotCost.find(depo_order) != m_mapOrderToDepotCost.end())
	{
		return(m_mapOrderToDepotCost[depo_order]);
	}
	CostPack ret;
	ret.cost = ret.distance = ret.traveltime = 1e15;
	return ret;
}




