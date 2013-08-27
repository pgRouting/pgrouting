#include "VRP_Solver.h"

#define DOUBLE_MAX 1e50

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

CSolutionInfo::CSolutionInfo()
{
}
CSolutionInfo::~CSolutionInfo()
{
}

bool CSolutionInfo::init(std::vector<int> vecOrder, int iTotalOrder, std::vector<int> vecVehicle)
{
	m_vUnservedOrderId = vecOrder;
	m_iTotalOrders = iTotalOrder;
	m_vUnusedVehicles =  vecVehicle;

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


CVRPSolver::CVRPSolver()
{
}
CVRPSolver::~CVRPSolver()
{
}

bool CVRPSolver::solveVRP(std::string& strError)
{
	if(!m_bIsReadyToSolve)
	{
		strError = "Scenario is not ready to solve. Configure all parameter";
		return false;
	}

	std::vector<int> vecOrders, vecVehicles;
	for(int i = 0; i < m_vOrderInfos.size(); i++)
	{
		vecOrders.push_back(m_vOrderInfos[i].getOrderId());
	}

	for(int i = 0; i < m_vVehicleInfos.size(); i++)
	{
		vecVehicles.push_back(m_vVehicleInfos[i].getId());
	}
	
	m_solutionFinal.init(vecOrders, vecOrders.size(), vecVehicles);
	int iAttemtCount = 0;
	while(iAttemtCount < MAXIMUM_TRY)
	{
		bool bUpdateFound = false;
		CSolutionInfo initialSolution = generateInitialSolution();
		iAttemtCount++;
		bUpdateFound = updateFinalSolution(initialSolution);
		
		bool bUpdateFound2 = tabuSearch(initialSolution);

		if((bUpdateFound == true) || (bUpdateFound2 == true))
		{
			iAttemtCount = 0;
		}
	}
	return true;

}

CSolutionInfo CVRPSolver::generateInitialSolution()
{
	CSolutionInfo initialSolution;

	std::vector<int> vecOrders, vecVehicles;
	for(int i = 0; i < m_vOrderInfos.size(); i++)
	{
		vecOrders.push_back(m_vOrderInfos[i].getOrderId());
	}

	for(int i = 0; i < m_vVehicleInfos.size(); i++)
	{
		vecVehicles.push_back(m_vVehicleInfos[i].getId());
	}

	initialSolution.init(vecOrders, vecOrders.size(), vecVehicles);

	int iUnusedVehicles = m_viUnusedVehicleIndex.size();
	int iUnservedOrders = m_viUnservedOrderIndex.size();

	while( iUnusedVehicles && iUnservedOrders )
	{
		CTourInfo curTour;

		int vehicleIndex = rand() % iUnusedVehicles--;
		curTour.setVehicleInfo(m_vVehicleInfos[m_viUnusedVehicleIndex[vehicleIndex]]);
		removeVehicle(vehicleIndex);
		curTour.setStartDepot(m_vDepotInfos[0].getDepotId());
		curTour.setEndDepot(m_vDepotInfos[0].getDepotId());
		
		//use a random seed to start to tour. (we can use better approach in future)
		
		bool insertAvailable = true;

		while( insertAvailable )
		{
			insertAvailable = false;
			std::pair<int,int> PotentialInsert; // first = insert_index, second = removed_order_index;
			std::pair<int,double> bestInsert = std::make_pair(-1,DOUBLE_MAX);//first = order_insert_index, second = cost;

			for(int i = 0;i<iUnservedOrders;++i)
			{
				COrderInfo curOrder = m_vOrderInfos[m_viUnservedOrderIndex[i]];
				std::pair<int,double> curInsert = getPotentialInsert(curTour, curOrder);

				if( curInsert.second < bestInsert.second )
				{
					insertAvailable = true;
					bestInsert = curInsert;
					PotentialInsert = std::make_pair(curInsert.first,i);
				}
			}
			if( insertAvailable )
			{
				iUnservedOrders--;
				insertOrder(curTour, m_viUnservedOrderIndex[PotentialInsert.second], PotentialInsert.first);
				removeOrder(PotentialInsert.second);
			}
		}
		initialSolution.addTour(curTour);
	}
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

std::pair<int,double> CVRPSolver::getPotentialInsert(CTourInfo& curTour, COrderInfo& curOrder)
{
	std::pair<int,double> bestInsert = std::make_pair(-1,DOUBLE_MAX);
	if( curOrder.getOrderUnit() > curTour.getRemainingCapacity() )
	{
		return bestInsert;
	}
	//check if ith position insert is fisible.
	std::vector<int> vecOrderId = curTour.getOrderVector();
	for(int i = 0; i <= vecOrderId.size();++i)
	{
		CostPack costToOrder, costFromOrder;
		
		if(!i)
		{
			costToOrder = getDepotToOrderCost( curTour.getStartDepot(), curOrder.getOrderId());
		}
		else costToOrder = getOrderToOrderCost( vecOrderId[i-1], curOrder.getOrderId());

		double dArrivalTime = costToOrder.traveltime + curTour.getStartTime(i);

		if( dArrivalTime > curOrder.getCloseTime() )
		{
			continue;
		}

		if (i == vecOrderId.size())
		{
			costFromOrder = getOrderToDepotCost( curOrder.getOrderId(), curTour.getEndDepot() );
		}
		else costFromOrder = getOrderToOrderCost( curOrder.getOrderId(), vecOrderId[i] );

		dArrivalTime += curOrder.getServiceTime() + costFromOrder.traveltime;

		if( i < vecOrderId.size() && dArrivalTime > m_vOrderInfos[vecOrderId[i]].getCloseTime())
		{
			continue;
		}

		CostPack totalCost = getCostForInsert(curTour, curOrder, i);

		
		if( totalCost.cost < bestInsert.second )
		{
			bestInsert = std::make_pair(i,totalCost.cost);
		}
	}
	return bestInsert;
}

bool CVRPSolver::tabuSearch(CSolutionInfo& curSolution)
{
	m_bFoundOptimal = false;
	updateFinalSolution(curSolution);

	int numberOfSearch = 0;
	m_iGeneratedSolutionCount = 0;
	m_iStepsSinceLastSolution = 0;

	while( numberOfSearch < TOTAL_NUMBER_OF_SEARCH )
	{
		applyBestMoveInCurrentSolution(curSolution, identifyPotentialMove() );	
		insertUnservedOrders(curSolution);
		attemptFeasibleNodeExchange(curSolution);
		attempVehicleExchange(curSolution);
		++numberOfSearch;
	}
	return true;
}

void CVRPSolver::applyBestMoveInCurrentSolution(CSolutionInfo& curSolution, CMoveInfo& bestMove)
{
	++m_iGeneratedSolutionCount;
	++m_iStepsSinceLastSolution;

	updateTabuCount(bestMove);

	int totalTour = bestMove.getModifiedTourCount();
	for(int i = 0;i<totalTour;++i)
	{
		curSolution.replaceTour( bestMove.getModifiedTourAt(i) );
	}
	updateFinalSolution(curSolution);
	
}

void CVRPSolver::insertUnservedOrders(CSolutionInfo& curSolution)
{
	++m_iGeneratedSolutionCount;
	++m_iStepsSinceLastSolution;
	bool insertAvailable = true;
	CMoveInfo curMove;
	int totalUnservedOrder = m_vOrderInfos.size() - curSolution.getOrderServed();

	while( insertAvailable && totalUnservedOrder > 0 )
	{
		int insertTourId = -1;
		insertAvailable = false;
		int totalTour = curSolution.getTourInfoVector().size();
		std::pair<int,int> PotentialInsert; // first = insert_index, second = removed_customer_index;
		std::pair<int,double> bestInsert = std::make_pair(-1,DOUBLE_MAX);//first = customer_insert_index, second = cost;

		for(int j = 0;j<totalTour;++j)
		{
			CTourInfo curTour = curSolution.getTour(j);
			curMove.setInitialTour(curTour);

			for(int i = 0;i<totalUnservedOrder;++i)
			{
				COrderInfo curOrder = m_vOrderInfos[curSolution.getUnservedOrderAt(i)];
				std::pair<int,double> curInsert = curTour.getPotentialInsert( curOrder);

				insertOrder(curTour, i,curInsert.first);
				curMove.setModifiedTour(curTour);
				curMove.getInitialTour(curTour);

				//check if current move is tabu.
				if( isTabuMove(curMove) )
				{
					continue;
				}

				if( curInsert.second < bestInsert.second )
				{
					insertTourId = j;
					insertAvailable = true;
					bestInsert = curInsert;
					PotentialInsert = std::make_pair(curInsert.first,i);
				}
			}
		}
		if( insertAvailable )
		{
			totalUnservedOrder--;
			curMove.setInitialTour(curSolution.getTour(insertTourId));

			addOrderAtTour(curSolution, insertTourId,
				PotentialInsert.first,
				PotentialInsert.second);

			curMove.setModifiedTour(curSolution.getTour(insertTourId));
			this->updateTabuCount(curMove);
			this->updateFinalSolution(curSolution);//this->evaluateCurrentSolution();
		}
	}
	
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
	if(m_bIsSoultionReady == true)
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

bool CVRPSolver::insertOrder(CTourInfo& tourInfo, int orderId, int pos)
{
	if(pos < 0 || pos > tourInfo.getOrderVector().size())
		return false;

	tourInfo.insertOrder(orderId, pos);
	int orderIndex = m_mapOrderIdToIndex[orderId];
	tourInfo.getVehicleInfo().loadUnit(m_vOrderInfos[orderIndex].getOrderUnit());
	
	updateTourCosts(tourInfo);

	return true;
}

bool CVRPSolver::updateTourCosts(CTourInfo& tourInfo)
{
	std::vector<int> vecOrderId = tourInfo.getOrderVector();

	double dCost, dDistance, dTravelTime;
	dCost = dDistance = dTravelTime = 0.0;

	CostPack cPack = getDepotToOrderCost(tourInfo.getStartDepot(), vecOrderId[0]);

	dCost += cPack.cost;
	dDistance += cPack.distance;

	int ind = m_mapOrderIdToIndex[vecOrderId[0]];

	if(dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
		return false;

	dTravelTime = max(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(), 
						m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime());

	int i;
	for(i = 1; i < vecOrderId.size(); i++)
	{
		cPack = getOrderToOrderCost(vecOrderId[i - 1], vecOrderId[i]);
		dCost += cPack.cost;
		dDistance += cPack.distance;

		ind = m_mapOrderIdToIndex[vecOrderId[i]];

		if(dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
			return false;

		dTravelTime = max(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(), 
			m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime());
	
	}
	
	cPack = getOrderToDepotCost(vecOrderId[i - 1], tourInfo.getEndDepot());
	dCost += cPack.cost;
	dDistance += cPack.distance;

	dTravelTime += cPack.traveltime;

	return true;
}

bool CVRPSolver::addOrderAtTour(CSolutionInfo &solutionInfo, int tourIndex, int insertIndex, int orderIndex)
{
	return(insertOrder(solutionInfo.getTour(tourIndex), m_vOrderInfos[orderIndex].getOrderId(), insertIndex));
}
