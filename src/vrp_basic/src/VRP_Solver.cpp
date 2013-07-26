#include "VRP_Solver.h"

CVehicleInfo::CVehicleInfo()
{
}
CVehicleInfo::~CVehicleInfo()
{
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

CSolutionInfo::CSolutionInfo()
{
}
CSolutionInfo::~CSolutionInfo()
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
	
	m_solutionFinal.init();
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
	initialSolution.init();

	int iUnusedVehicles = m_viUnusedVehicleIndex.size();
	int iUnservedOrders = m_viUnservedOrderIndex.size();

	while( iUnusedVehicles && iUnservedOrders )
	{
		CTourInfo curTour;

		int vehicleIndex = rand() % iUnusedVehicles--;
		curTour.setVehicleId(m_viUnusedVehicleIndex[vehicleIndex]);
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
				curTour.insertOrder(m_viUnservedOrderIndex[PotentialInsert.second], PotentialInsert.first);
				removeOrder(PotentialInsert.second);
			}
		}
		m_pInitialSolution.addTour(curTour);
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

		dArrivalTime += curOrder->getServiceTime() + costFromOrder.traveltime;

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

void CVRPSolver::tabuSearch(CSolutionInfo curSolution)
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
		attemptFeasibleNodeExchange();
		attempVehicleExchange();
		++numberOfSearch;
	}
}

void CVRPSolver::applyBestMoveInCurrentSolution(CSolutionInfo& curSolution, CMoveInfo& bestMove)
{
	++m_iGeneratedSolutionCount;
	++m_iStepsSinceLastSolution;

	updateTabuCount(bestMove);

	int totalTour = bestMove->getModifiedTourCount();
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
			CTourInfo curTour = curSolution->getTour(j);
			curMove.setInitialTour(curTour);

			for(int i = 0;i<totalUnservedOrder;++i)
			{
				COrderInfo curOrder = m_vOrderInfos[curSolution.getUnservedOrderAt(i)];
				std::pair<int,double> curInsert = curTour.getPotentialInsert( curOrder);

				curTour.addOrder(curOrder,curInsert.first);
				curMove.setModifiedTour(curTour);
				curMove.getInitialTour(curTour);

				//check if current move is tabu.
				if( isTabuMove(pCurMove) )
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

			curSolution.addOrderAtTour(insertTourId,
				PotentialInsert.first,
				PotentialInsert.second));

			curMove.setModifiedTour(curSolution.getTour(insertTourId));
			this->updateTabuCount(curMove);
			this->evaluateCurrentSolution();
		}
	}
	
}

