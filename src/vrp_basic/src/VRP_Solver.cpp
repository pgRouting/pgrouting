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

#include "./VRP_Solver.h"
#include <algorithm>
#include <math.h>
#include <utility>
#include <map>
#include <set>
#include <string>
#include <vector>

#undef PGR_LOGGER_ON
#define PGR_LOGGER_LOC
#define PGR_LOGGER_FILE "/tmp/vrp-debug.log"
#include "../../common/src/pgr_logger.h"

#define DOUBLE_MAX 1e50

bool operator != (const CVehicleInfo& cur, const CVehicleInfo& that) {
    return(cur.m_iVehicleId != that.m_iVehicleId);
}

bool operator == (const CTourInfo& cur, const CTourInfo& that) {
    if (cur.m_vehicleInfo != that.m_vehicleInfo)
        return false;
    if (cur.m_viOrderIds.size() != that.m_viOrderIds.size())
        return false;
    auto tot = cur.m_viOrderIds.size();
    for (size_t i = 0; i < tot; i++) {
        if (cur.m_viOrderIds[i] != that.m_viOrderIds[i])
            return false;
    }
    return true;
}

bool operator == (const CMoveInfo& cur, const CMoveInfo& that) {
    if (!(cur.m_vInitialTour == that.m_vInitialTour))
        return false;
    if (!(cur.m_vModifiedTour == that.m_vModifiedTour))
        return false;
    return true;
}



CVehicleInfo::CVehicleInfo() {
    m_iCurrentLoad = 0;
}
CVehicleInfo::~CVehicleInfo() {
}

bool CVehicleInfo::loadUnit(int lUnit) {
    if (m_iCurrentLoad + lUnit > m_iCapacity)
        return false;
    m_iCurrentLoad += lUnit;
    return true;
}

COrderInfo::COrderInfo() { }
COrderInfo::~COrderInfo() { }

CDepotInfo::CDepotInfo() { }
CDepotInfo::~CDepotInfo() { }

CTourInfo::CTourInfo() {
    m_dTotalCost = 0.0;
    m_dTotalDistance = 0.0;
    m_dTotalTraveltime = 0.0;
}
CTourInfo::~CTourInfo() { }

bool CTourInfo::insertOrder(int orderId, int pos) {
    m_viOrderIds.insert(m_viOrderIds.begin() + pos, orderId);
    return true;
}

int CTourInfo::getRemainingCapacity() {
    return(m_vehicleInfo.getRemainingCapacity());
}

bool CTourInfo::removeOrder(int pos) {
    m_viOrderIds.erase(m_viOrderIds.begin() + pos);
    return true;
}

void CTourInfo::updateCost(double cost, double distance, double travelTime) {
    m_dTotalCost = cost;
    m_dTotalDistance = distance;
    m_dTotalTraveltime = travelTime;
}

CSolutionInfo::CSolutionInfo() { }
CSolutionInfo::~CSolutionInfo() { }

void CSolutionInfo::replaceTour(CTourInfo curTour) {
    unsigned int i;
    for (i = 0; i < m_vtourAll.size(); i++) {
        if (m_vtourAll[i].getVehicleId() == curTour.getVehicleId()) {
            m_vtourAll[i] = curTour;
            return;
        }
    }
    return;
}

void CSolutionInfo::replaceTourAt(int index, CTourInfo curTour) {
    if (index < 0 ||  (unsigned int) index >= m_vtourAll.size())
        return;
    m_vtourAll[index] = curTour;
}

bool CSolutionInfo::init(std::vector<int> vecOrder, int iTotalOrder, std::vector<int> vecVehicle) {
    m_vUnservedOrderId = vecOrder;
    m_iTotalOrders = iTotalOrder;
    m_vUnusedVehicles = vecVehicle;

    m_vtourAll.clear();
    m_iVehicleUsed = 0;
    m_iOrdersServed = 0;
    m_iTotalOrders = 0;
    m_dTotalCost = 0;
    m_dTotalDistance = 0;
    m_dTotalTravelTime = 0;
    return true;
}

bool CSolutionInfo::addTour(CTourInfo& tour) {
    m_vtourAll.push_back(tour);
    int vid = tour.getVehicleId();
    std::vector<int>::iterator it;
    it = std::find(m_vUnusedVehicles.begin(), m_vUnusedVehicles.end(), vid);
    if (it != m_vUnusedVehicles.end()) {
        m_vUnusedVehicles.erase(it);
    }
    m_iVehicleUsed++;
    m_dTotalDistance += tour.getDistance();
    m_dTotalTravelTime += tour.getTravelTime();
    m_dTotalCost += tour.getCost();

    std::vector<int> vecOrders = tour.getOrderVector();

    m_iOrdersServed += static_cast<int>(vecOrders.size());

    for (unsigned int i = 0; i < vecOrders.size(); i++) {
        int oid = vecOrders[i];
        it = std::find(m_vUnservedOrderId.begin(), m_vUnservedOrderId.end(), oid);
        if (it != m_vUnservedOrderId.end()) {
            m_vUnservedOrderId.erase(it);
        }
    }

    return true;
}

CMoveInfo::CMoveInfo() { }
CMoveInfo::~CMoveInfo() { }

void CMoveInfo::setInitialTour(CTourInfo tourData) {
    m_vInitialTour.clear();
    m_vInitialTour.push_back(tourData);
}

void CMoveInfo::setInitialTour(CTourInfo tourData1, CTourInfo tourData2) {
    m_vInitialTour.clear();
    m_vInitialTour.push_back(tourData1);
    m_vInitialTour.push_back(tourData2);
}

void CMoveInfo::setModifiedTour(CTourInfo tourData) {
    m_vModifiedTour.clear();
    m_vModifiedTour.push_back(tourData);
}

void CMoveInfo::setModifiedTour(CTourInfo tourData1, CTourInfo tourData2) {
    m_vModifiedTour.clear();
    m_vModifiedTour.push_back(tourData1);
    m_vModifiedTour.push_back(tourData2);
}

void CMoveInfo::getInitialTour(CTourInfo &tourData) {
    tourData = m_vInitialTour[0];
}

void CMoveInfo::getInitialTour(CTourInfo &tourData1, CTourInfo &tourData2) {
    tourData1 = m_vInitialTour[0];
    tourData2 = m_vInitialTour[1];
}

bool CMoveInfo::getModifiedTourAt(int index, CTourInfo& tourInfo) {
    if (index < 0 ||  (unsigned int) index >= m_vModifiedTour.size())
        return false;
    tourInfo = m_vModifiedTour[index];
    return true;
}



CVRPSolver::CVRPSolver() {
    // set a seed for the random number generator
    // so it will generate consistent results for the same input
    // otherwise we can not test it :(
    srand(1726354);
}
CVRPSolver::~CVRPSolver() { }

bool CVRPSolver::solveVRP(std::string& strError) {
    // if (!m_bIsReadyToSolve)
    // {
    //      strError = "Scenario is not ready to solve. Configure all parameter";
    //      return false;
    // }
    PGR_LOG("Inside Solve VRP");
    std::vector<int> vecOrders, vecVehicles;
    for (unsigned int i = 0; i < m_vOrderInfos.size(); i++) {
        vecOrders.push_back(m_vOrderInfos[i].getOrderId());
    }

    for (unsigned int i = 0; i < m_vVehicleInfos.size(); i++) {
        vecVehicles.push_back(m_vVehicleInfos[i].getId());
    }

    m_solutionFinal.init(vecOrders, static_cast<int>(vecOrders.size()), vecVehicles);
    PGR_LOG("After init solution");
    int iAttemtCount = 0;
    while (iAttemtCount < MAXIMUM_TRY) {
        bool bUpdateFound = false;
        CSolutionInfo initialSolution = generateInitialSolution();
        PGR_LOG("After Generate initial Solution");
        iAttemtCount++;
        bUpdateFound = updateFinalSolution(initialSolution);
        PGR_LOG("After update final Solution");
        bool bUpdateFound2 = tabuSearch(initialSolution);
        PGR_LOG("After Tabu Search");
        if ((bUpdateFound == true) ||  (bUpdateFound2 == true)) {
            iAttemtCount = 0;
        }
    }
    m_bIsSolutionReady = true;
    strError += " ";
    return true;
}

CSolutionInfo CVRPSolver::generateInitialSolution() {
    CSolutionInfo initialSolution;
    PGR_LOG("Inside gen ini sol");
    std::vector<int> vecOrders, vecVehicles;
    for (unsigned int i = 0; i < m_vOrderInfos.size(); i++) {
        vecOrders.push_back(m_vOrderInfos[i].getOrderId());
    }

    for (unsigned int i = 0; i < m_vVehicleInfos.size(); i++) {
        vecVehicles.push_back(m_vVehicleInfos[i].getId());
    }

    initialSolution.init(vecOrders, static_cast<int>(vecOrders.size()), vecVehicles);

    int iUnusedVehicles = static_cast<int>(initialSolution.getUnusedVehicleCount());
    int iUnservedOrders = static_cast<int>(initialSolution.getUnservedOrderCount());  // m_viUnservedOrderIndex.size();
    PGR_LOG("before while");
    while (iUnusedVehicles &&  iUnservedOrders) {
        CTourInfo curTour;

        int vehicleIndex = rand() % iUnusedVehicles--;
        int vehicleInd = m_mapVehicleIdToIndex[initialSolution.getUnusedVehicleAt(vehicleIndex)];
        curTour.setVehicleInfo(m_vVehicleInfos[vehicleInd]);  // m_viUnusedVehicleIndex[vehicleIndex]
        initialSolution.removeVehicle(vehicleIndex);

        curTour.setStartDepot(m_vDepotInfos[0].getDepotId());
        curTour.setEndDepot(m_vDepotInfos[0].getDepotId());

        // use a random seed to start to tour. (we can use better approach in future)

        bool insertAvailable = true;

        while (insertAvailable) {
            insertAvailable = false;
            std::pair<int, int> PotentialInsert;  //  first = insert_index, second = removed_order_index;
            std::pair<int, double> bestInsert = std::make_pair(-1, DOUBLE_MAX);  // first = order_insert_index, second = cost;

            for (int i = 0; i < iUnservedOrders; ++i) {
                int orderInd = m_mapOrderIdToIndex[initialSolution.getUnservedOrderAt(i)];
                COrderInfo curOrder = m_vOrderInfos[orderInd];
                std::pair<int, double> curInsert = getPotentialInsert(curTour, curOrder);

                if (curInsert.second < bestInsert.second) {
                    insertAvailable = true;
                    bestInsert = curInsert;
                    PotentialInsert = std::make_pair(curInsert.first, i);
                }
            }
            if (insertAvailable) {
                if (insertOrder(curTour, initialSolution.getUnservedOrderAt(PotentialInsert.second), PotentialInsert.first)) {
                    iUnservedOrders--;
                    initialSolution.removeOrder(PotentialInsert.second);
                }
            }
        }

        initialSolution.addTour(curTour);
    }

    return initialSolution;
}

bool CVRPSolver::updateFinalSolution(CSolutionInfo& curSolution) {
    bool callUpdate = false;
    if (curSolution.getOrderServed() > m_solutionFinal.getOrderServed()) {
        callUpdate = true;
    } else if (curSolution.getOrderServed() == m_solutionFinal.getOrderServed()) {
        if (curSolution.getTotalCost() < m_solutionFinal.getTotalCost()) {
            callUpdate = true;
        } else if (curSolution.getTotalCost() == m_solutionFinal.getTotalCost()) {
            if (curSolution.getTotalTravelTime() < m_solutionFinal.getTotalTravelTime()) {
                callUpdate = true;
            } else if (curSolution.getTotalTravelTime() == m_solutionFinal.getTotalTravelTime()) {
                if (curSolution.getTotalDistance() < m_solutionFinal.getTotalDistance()) {
                    callUpdate = true;
                }
            }
        }
    }
    if (callUpdate) {
        // m_iStepsSinceLastSolution = 0;
        m_solutionFinal = curSolution;

        // clear map and delete objects
        // m_mpTabuCount.clear();
        // for (std::map< CVRPTWMove*, int >::iterator it = m_mpMoveFrequency.begin();it!= m_mpMoveFrequency.end();++it)
        // {
        //  delete (*it).first;
        // }
        // m_mpMoveFrequency.clear();
        return true;
    }
    return false;
}

std::pair<int, double> CVRPSolver::getPotentialInsert(CTourInfo& curTour, COrderInfo& curOrder) {
    std::pair<int, double> bestInsert = std::make_pair(-1, DOUBLE_MAX);
    if (curOrder.getOrderUnit() > curTour.getRemainingCapacity()) {
        return bestInsert;
    }
    // check if ith position insert is fisible.
    std::vector<int> vecOrderId = curTour.getOrderVector();
    for (unsigned int i = 0; i <= vecOrderId.size(); ++i) {
        CostPack costToOrder, costFromOrder;

        if (!i) {
            costToOrder = getDepotToOrderCost(curTour.getStartDepot(), curOrder.getOrderId());
        } else {
            costToOrder = getOrderToOrderCost(vecOrderId[i-1], curOrder.getOrderId());
        }

        double dArrivalTime = costToOrder.traveltime + curTour.getStartTime(i);

        if (dArrivalTime > curOrder.getCloseTime()) {
            continue;
        }

        if (i == vecOrderId.size()) {
            costFromOrder = getOrderToDepotCost(curOrder.getOrderId(), curTour.getEndDepot());
        } else {
            costFromOrder = getOrderToOrderCost(curOrder.getOrderId(), vecOrderId[i]);
        }

        dArrivalTime += curOrder.getServiceTime() + costFromOrder.traveltime;

        if (i < vecOrderId.size() &&  dArrivalTime > m_vOrderInfos[m_mapOrderIdToIndex[vecOrderId[i]]].getCloseTime()) {
            continue;
        }

        CostPack totalCost = getCostForInsert(curTour, curOrder, i);

        if (totalCost.cost < bestInsert.second) {
            bestInsert = std::make_pair(i, totalCost.cost);
        }
    }
    return bestInsert;
}

bool CVRPSolver::tabuSearch(CSolutionInfo& curSolution) {
    m_bFoundOptimal = false;
    updateFinalSolution(curSolution);

    int numberOfSearch = 0;
    m_iGeneratedSolutionCount = 0;
    m_iStepsSinceLastSolution = 0;

    while (numberOfSearch < TOTAL_NUMBER_OF_SEARCH) {
        // applyBestMoveInCurrentSolution(curSolution, identifyPotentialMove(curSolution) );
        insertUnservedOrders(curSolution);
        // attemptFeasibleNodeExchange(curSolution);
        attempVehicleExchange(curSolution);
        ++numberOfSearch;
    }
    return false;
}

void CVRPSolver::applyBestMoveInCurrentSolution(CSolutionInfo& curSolution, CMoveInfo& bestMove) {
    ++m_iGeneratedSolutionCount;
    ++m_iStepsSinceLastSolution;

    updateTabuCount(bestMove);

    int totalTour = static_cast<int>(bestMove.getModifiedTourCount());
    for (int i = 0; i < totalTour; ++i) {
        CTourInfo tourInfo;
        bool bIsValid = bestMove.getModifiedTourAt(i, tourInfo);

        if (bIsValid)
            curSolution.replaceTour(tourInfo);
    }
    updateFinalSolution(curSolution);
}

void CVRPSolver::insertUnservedOrders(CSolutionInfo& curSolution) {
    ++m_iGeneratedSolutionCount;
    ++m_iStepsSinceLastSolution;
    bool insertAvailable = true;
    CMoveInfo curMove;
    int totalUnservedOrder = static_cast<int>(m_vOrderInfos.size() - curSolution.getOrderServed());

    while (insertAvailable &&  totalUnservedOrder > 0) {
        int insertTourId = -1;
        insertAvailable = false;
        int totalTour = static_cast<int>(curSolution.getTourInfoVector().size());
        std::pair<int, int> PotentialInsert;  //  first = insert_index, second = removed_customer_index;
        std::pair<int, double> bestInsert = std::make_pair(-1, DOUBLE_MAX);  // first = customer_insert_index, second = cost;

        for (int j = 0; j < totalTour; ++j) {
            CTourInfo curTour = curSolution.getTour(j);
            curMove.setInitialTour(curTour);

            for (int i = 0; i < totalUnservedOrder; ++i) {
                int ordIndex = m_mapOrderIdToIndex[curSolution.getUnservedOrderAt(i)];
                COrderInfo curOrder = m_vOrderInfos[ordIndex];
                std::pair<int, double> curInsert = getPotentialInsert(curTour, curOrder);

                insertOrder(curTour, i, curInsert.first);
                curMove.setModifiedTour(curTour);
                curMove.getInitialTour(curTour);

                // check if current move is tabu.
                if (isTabuMove(curMove)) {
                    continue;
                }

                if (curInsert.second < bestInsert.second) {
                    insertTourId = j;
                    insertAvailable = true;
                    bestInsert = curInsert;
                    PotentialInsert = std::make_pair(curInsert.first, i);
                }
            }
        }
        if (insertAvailable) {
            totalUnservedOrder--;
            curMove.setInitialTour(curSolution.getTour(insertTourId));

            addOrderAtTour(curSolution, insertTourId,
                    PotentialInsert.first,
                    PotentialInsert.second);

            curMove.setModifiedTour(curSolution.getTour(insertTourId));
            this->updateTabuCount(curMove);
            this->updateFinalSolution(curSolution);  // this->evaluateCurrentSolution();
        }
    }
}

bool CVRPSolver::addDepot(CDepotInfo depotInfo) {
    int id = depotInfo.getDepotId();
    if (m_mapDepotIdToIndex.find(id) != m_mapDepotIdToIndex.end())
        return false;
    m_mapDepotIdToIndex.insert(std::make_pair(id, m_vDepotInfos.size()));
    m_vDepotInfos.push_back(depotInfo);

    return true;
}

bool CVRPSolver::addOrder(COrderInfo orderInfo) {
    int id = orderInfo.getOrderId();
    if (m_mapOrderIdToIndex.find(id) != m_mapOrderIdToIndex.end()) {
        return false;
    }
    int index = static_cast<int>(m_vOrderInfos.size());
    m_mapOrderIdToIndex.insert(std::make_pair(id, index));
    m_vOrderInfos.push_back(orderInfo);
    m_viUnservedOrderIndex.push_back(index);
    return true;
}

bool CVRPSolver::addVehicle(CVehicleInfo vehicleInfo) {
    int id = vehicleInfo.getId();
    if (m_mapVehicleIdToIndex.find(id) != m_mapVehicleIdToIndex.end()) {
        return false;
    }
    int index = static_cast<int>(m_vVehicleInfos.size());
    m_mapVehicleIdToIndex.insert(std::make_pair(id, index));
    m_vVehicleInfos.push_back(vehicleInfo);
    m_viUnusedVehicleIndex.push_back(index);
    return true;
}

bool CVRPSolver::addDepotToOrderCost(int depotId, int orderId, CostPack cost) {
    PII depo_order = std::make_pair(depotId, orderId);
    if (m_mapDepotToOrderrCost.find(depo_order) != m_mapDepotToOrderrCost.end()) {
        return false;
    }
    m_mapDepotToOrderrCost.insert(make_pair(depo_order, cost));
    return true;
}

bool CVRPSolver::addOrderToDepotCost(int depotId, int orderId, CostPack cost) {
    PII depo_order = std::make_pair(orderId, depotId);
    if (m_mapOrderToDepotCost.find(depo_order) != m_mapOrderToDepotCost.end()) {
        return false;
    }
    m_mapOrderToDepotCost.insert(std::make_pair(depo_order, cost));
    return true;
}

bool CVRPSolver::addOrderToOrderCost(int firstOrder, int secondOrder, CostPack cost) {
    PII order_order = std::make_pair(firstOrder, secondOrder);
    if (m_mapOrderToOrderCost.find(order_order) != m_mapOrderToOrderCost.end()) {
        return false;
    }
    m_mapOrderToOrderCost.insert(std::make_pair(order_order, cost));
    return true;
}

bool CVRPSolver::getSolution(CSolutionInfo& solution, std::string& strError) {
    if (m_bIsSolutionReady == true) {
        solution = m_solutionFinal;
        return true;
    } else {
        bool ret = solveVRP(strError);
        if (ret == true) {
            solution = m_solutionFinal;
            return true;
        }
        return false;
    }
}

CostPack CVRPSolver::getDepotToOrderCost(int depotId, int orderId) {
    PII depo_order = std::make_pair(depotId, orderId);

    if (m_mapDepotToOrderrCost.find(depo_order) != m_mapDepotToOrderrCost.end()) {
        return(m_mapDepotToOrderrCost[depo_order]);
    }
    CostPack ret;
    ret.cost = ret.distance = ret.traveltime = 1e15;
    return ret;
}

CostPack CVRPSolver::getOrderToOrderCost(int orderId1, int orderId2) {
    PII order_order = std::make_pair(orderId1, orderId2);

    if (m_mapOrderToOrderCost.find(order_order) != m_mapOrderToOrderCost.end()) {
        return(m_mapOrderToOrderCost[order_order]);
    }
    CostPack ret;
    ret.cost = ret.distance = ret.traveltime = 1e15;
    return ret;
}


CostPack CVRPSolver::getOrderToDepotCost(int depotId, int orderId) {
    PII depo_order = std::make_pair(orderId, depotId);

    if (m_mapOrderToDepotCost.find(depo_order) != m_mapOrderToDepotCost.end()) {
        return(m_mapOrderToDepotCost[depo_order]);
    }
    CostPack ret;
    ret.cost = ret.distance = ret.traveltime = 1e15;
    return ret;
}

bool CVRPSolver::insertOrder(CTourInfo& tourInfo, int orderId, int pos) {
    if (pos < 0 ||  (unsigned int) pos > tourInfo.getOrderVector().size())
        return false;

    int orderIndex = m_mapOrderIdToIndex[orderId];
    if (!tourInfo.getVehicleInfo().loadUnit(m_vOrderInfos[orderIndex].getOrderUnit()))
        return false;
    tourInfo.insertOrder(orderId, pos);

    if (!updateTourCosts(tourInfo)) {
        tourInfo.removeOrder(pos);
        return false;
    }

    return true;
}

bool CVRPSolver::updateTourCosts(CTourInfo& tourInfo) {
    std::vector<int> vecOrderId = tourInfo.getOrderVector();
    std::vector<int> vecStartTimes;

    double dCost, dDistance, dTravelTime;
    dCost = dDistance = dTravelTime = 0.0;

    CostPack cPack = getDepotToOrderCost(tourInfo.getStartDepot(), vecOrderId[0]);

    dCost += cPack.cost;
    dDistance += cPack.distance;

    int ind = m_mapOrderIdToIndex[vecOrderId[0]];
    vecStartTimes.push_back(0);

    if (dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
        return false;

    dTravelTime = (std::max)(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(),
            static_cast<double>(m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime()));
    vecStartTimes.push_back(static_cast<int>(ceil(dTravelTime)));

    unsigned int i;
    for (i = 1; i < vecOrderId.size(); i++) {
        cPack = getOrderToOrderCost(vecOrderId[i - 1], vecOrderId[i]);
        dCost += cPack.cost;
        dDistance += cPack.distance;

        ind = m_mapOrderIdToIndex[vecOrderId[i]];

        if (dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
            return false;

        dTravelTime = (std::max)(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(),
                static_cast<double>(m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime()));

        vecStartTimes.push_back(static_cast<int>(ceil(dTravelTime)));
    }

    cPack = getOrderToDepotCost(vecOrderId[i - 1], tourInfo.getEndDepot());
    dCost += cPack.cost;
    dDistance += cPack.distance;

    dTravelTime += cPack.traveltime;

    vecStartTimes.push_back(static_cast<int>(ceil(dTravelTime)));
    ind = m_mapDepotIdToIndex[tourInfo.getEndDepot()];
    if (dTravelTime > m_vDepotInfos[ind].getCloseTime())
        return false;

    tourInfo.updateCost(dCost, dDistance, dTravelTime);

    tourInfo.setStartTime(vecStartTimes);

    return true;
}

bool CVRPSolver::addOrderAtTour(CSolutionInfo &solutionInfo, int tourIndex, int insertIndex, int orderIndex) {
    return(insertOrder(solutionInfo.getTour(tourIndex), m_vOrderInfos[orderIndex].getOrderId(), insertIndex));
}

CostPack CVRPSolver::getCostForInsert(CTourInfo& curTour, COrderInfo& curOrder, int pos) {
    std::vector<int> vecOrderId = curTour.getOrderVector();

    vecOrderId.insert(vecOrderId.begin() + pos, curOrder.getOrderId());
    double dCost, dDistance, dTravelTime;
    dCost = dDistance = dTravelTime = 0.0;
    CostPack costRet;

    costRet.cost = INF;
    costRet.distance = INF;
    costRet.traveltime = INF;

    CostPack cPack = getDepotToOrderCost(curTour.getStartDepot(), vecOrderId[0]);

    dCost += cPack.cost;
    dDistance += cPack.distance;

    int ind = m_mapOrderIdToIndex[vecOrderId[0]];

    if (dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
        return costRet;

    dTravelTime = (std::max)(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(),
            static_cast<double>(m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime()));

    unsigned int i;
    for (i = 1; i < vecOrderId.size(); i++) {
        cPack = getOrderToOrderCost(vecOrderId[i - 1], vecOrderId[i]);
        dCost += cPack.cost;
        dDistance += cPack.distance;

        ind = m_mapOrderIdToIndex[vecOrderId[i]];

        if (dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
            return costRet;

        dTravelTime = (std::max)(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(),
                static_cast<double>(m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime()));
    }

    cPack = getOrderToDepotCost(vecOrderId[i - 1], curTour.getEndDepot());
    dCost += cPack.cost;
    dDistance += cPack.distance;

    dTravelTime += cPack.traveltime;

    ind = m_mapDepotIdToIndex[curTour.getEndDepot()];
    if (dTravelTime > m_vDepotInfos[ind].getCloseTime())
        return costRet;

    costRet.cost = dCost - curTour.getCost();
    costRet.distance = dDistance - curTour.getDistance();
    costRet.traveltime = dTravelTime - curTour.getTravelTime();

    return costRet;
}

void CVRPSolver::attempVehicleExchange(CSolutionInfo& solutionInfo) {
    ++m_iGeneratedSolutionCount;
    ++m_iStepsSinceLastSolution;
    CMoveInfo curMove;
    CMoveInfo bestMove;

    int bestFreeCapacity = 0;
    std::pair<int, int> bestSwapIndex;
    int totalTour = static_cast<int>(solutionInfo.getTourCount());

    for (int i = 0; i < totalTour; ++i) {
        CTourInfo firstTour = solutionInfo.getTour(i);
        int firstTourLoad = firstTour.getVehicleInfo().getCurrentLoad();
        int firstVehicleCapacity = firstTour.getVehicleInfo().getCapacity();

        for (int j = i + 1; j < totalTour; ++j) {
            CTourInfo secondTour = solutionInfo.getTour(j);
            curMove.setInitialTour(firstTour, secondTour);

            int FirstTourRemainingCapacity = firstVehicleCapacity - secondTour.getVehicleInfo().getCurrentLoad();

            int SecondTourRemainingCapacity = secondTour.getVehicleInfo().getCapacity() - firstTourLoad;

            // int prevFreeCapacity = max(secondTour.getRemainingCapacity(), firstTour.getRemainingCapacity() );

            int curFreeCapacity = (std::max)(FirstTourRemainingCapacity, SecondTourRemainingCapacity);

            if ((FirstTourRemainingCapacity > 0) &&  (SecondTourRemainingCapacity > 0) &&
                    // curFreeCapacity > curFreeCapacity autological compare evaluates to false (error on MAC)
                    (curFreeCapacity > bestFreeCapacity)) {
                CVehicleInfo tempVehicle = m_vVehicleInfos[firstTour.getVehicleId()];
                firstTour.setVehicleInfo(m_vVehicleInfos[secondTour.getVehicleId()]);
                secondTour.setVehicleInfo(tempVehicle);

                curMove.setModifiedTour(firstTour, secondTour);

                if (!isTabuMove(curMove)) {
                    bestMove = curMove;
                    bestFreeCapacity = curFreeCapacity;
                    bestSwapIndex = std::make_pair(i, j);
                }

                curMove.getInitialTour(firstTour, secondTour);
            }
        }
    }
    if (bestFreeCapacity > 0) {
        CTourInfo tempTour;
        bestMove.getModifiedTourAt(0, tempTour);
        solutionInfo.replaceTourAt(bestSwapIndex.first, tempTour);
        bestMove.getModifiedTourAt(1, tempTour);
        solutionInfo.replaceTourAt(bestSwapIndex.second, tempTour);
        updateTabuCount(bestMove);
        updateFinalSolution(solutionInfo);
    }
}
#if 0
void CVRPSolver::attemptFeasibleNodeExchange(CSolutionInfo& solutionInfo) {
    ++m_iGeneratedSolutionCount;
    ++m_iStepsSinceLastSolution;
    CMoveInfo bestMove, curMove;

    int totalTour = solutionInfo.getTourCount();

    for (int i = 0; i < totalTour; ++i) {
        CTourInfo curTour = solutionInfo.getTour(i);
        std::vector<int> vecOrderId = curTour.getOrderVector();
        curMove.setInitialTour(curTour);
        int totalCustomer = curTour.getServedOrderCount();
        std::pair<int, int> bestSwapIndex;
        double lowestCost = DOUBLE_MAX;

        for (int j = 0; j < totalCustomer; ++j) {
            for (int k = j + 1; k < totalCustomer; ++k) {
                COrderInfo firstCustomer = m_vOrderInfos[m_mapOrderIdToIndex[vecOrderId[j]]];
                COrderInfo secondCustomer = m_vOrderInfos[m_mapOrderIdToIndex[vecOrderId[k]]];

                if (curTour->isFeasibleReplace(j, pSecondCustomer) &&  pCurTour->isFeasibleReplace(k, pFirstCustomer)) {
                    pCurTour->removeCustomer(j, false);
                    pCurTour->addCustomer(pSecondCustomer, j);

                    pCurTour->removeCustomer(k, false);
                    pCurTour->addCustomer(pFirstCustomer, k);

                    pCurMove->setModifiedTour(pCurTour);
                    if (isTabuMove(pCurMove)) {
                        pCurMove->getInitialTour(pCurTour);
                        continue;
                    }

                    double curTourCost = pCurTour->getTourData()->calcCost(pCurTour->getAssignedVehicle());
                    if (curTourCost < lowestCost) {
                        *pBestMove = *pCurMove;
                        lowestCost = curTourCost;
                        bestSwapIndex = std::make_pair(j, k);
                    }
                    pCurMove->getInitialTour(pCurTour);
                }
            }
        }

        if (lowestCost!= DOUBLE_MAX) {
            m_pCurrentSolution->replaceTourAt(i, pBestMove->getModifiedTourAt(0));
            this->updateTabuCount(pBestMove);
            this->evaluateCurrentSolution();
        }
    }
    delete pCurMove;
    delete pBestMove;
}
#endif

void CVRPSolver::updateTabuCount(CMoveInfo& bestMove) {
    m_veMoves.push_back(bestMove);
#if 0
    bestMove.reverseMove();
    CMoveInfo curMove;

    std::map< CMoveInfo, int >::iterator mpIt = m_mapMoveFrequency.find(bestMove);

    if (mpIt == m_mapMoveFrequency.end()) {
        curMove = bestMove;
    } else {
        curMove = (*mpIt).first;
    }

    m_mapMoveFrequency[curMove]++;

    if (m_mapMoveFrequency[curMove] >= MAXIMUM_MOVE_FREQUENCY) {
        CMoveInfo tmpMove;
        std::set<CMoveInfo>::iterator sIt = m_sTabuList.find(curMove);

        CMoveInfo tmpMove2;
        if (sIt == m_sTabuList.end()) {
            tmpMove2 = curMove;
        } else {
            tmpMove2 = (*sIt);
        }
        m_sTabuList.insert(tmpMove2);
    }
    m_mapTabuCount[curMove] = std::make_pair(m_iGeneratedSolutionCount, m_iStepsSinceLastSolution);
    bestMove.reverseMove();
    */
#endif
}

bool CVRPSolver::isTabuMove(CMoveInfo& curMove) {
    size_t i, tot = m_veMoves.size();
    for (i = 0; i < tot; i++) {
        if (curMove == m_veMoves[i])
            return true;
    }
    return false;
}


