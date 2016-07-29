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

#ifndef SRC_VRP_BASIC_SRC_VRP_SOLVER_H_
#define SRC_VRP_BASIC_SRC_VRP_SOLVER_H_

#pragma once

#include <cstdlib>
#include <vector>
#include <map>
#include <utility>
#include <string>
#include <math.h>

#define MAXIMUM_TRY 15
#define TOTAL_NUMBER_OF_SEARCH 15
#define MAXIMUM_MOVE_FREQUENCY 15

#define INF (1e15)

typedef std::pair<int, int> PII;

//  Structure for Point, Geo coordinates can be represented with it
typedef struct {
  double X, Y;
}Point;

//  Structure to keep cost, distance and traveltime. If distance/ traveltime is missing, there may be a negative flag
typedef struct {
    double cost, distance, traveltime;
}CostPack;


//  Class for holding vehicle information which consist of capacity, and cost_per_km
//  For first version we will use homogeneous cost
class CVehicleInfo {
 public:
     CVehicleInfo();
     ~CVehicleInfo();

     bool init();

     bool loadUnit(int lUnit);
     bool unloadUnit(int lUnit);
     int getRemainingCapacity() {return (m_iCapacity - m_iCurrentLoad);}

     int getCurrentLoad() {return m_iCurrentLoad;}

     int getCapacity() {return m_iCapacity;}
     void setCapacity(int capacity) {m_iCapacity = capacity;}

     int getId() {return (this->m_iVehicleId);}
     void setId(int id) {m_iVehicleId = id;}

     double getCostPerKM() {return m_dCostPerKM;}
     void setCostPerKM(double cost) {m_dCostPerKM = cost;}

     friend bool operator != (const CVehicleInfo& cur, const CVehicleInfo& that);


     // CVehicleInfo( CVehicleInfo const& );
     // CVehicleInfo& operator = (const CVehicleInfo& vehicleInfo);

 private:
     int m_iCapacity;
     int m_iCurrentLoad;
     int m_iVehicleId;
     double m_dCostPerKM;
};



//  Class to represent Orders. Each order is consist of open_time, close_time and sevice_time, its location and number of units ordered.
class COrderInfo {
 public:
     COrderInfo();
     ~COrderInfo();

     int getOpenTime() {return m_iOrderOpenTime;}
     void setOpenTime(int openTime) {m_iOrderOpenTime = openTime;}

     int getCloseTime() {return m_iOrderCloseTime;}
     void setCloseTime(int closeTime) {m_iOrderCloseTime = closeTime;}

     int getServiceTime() {return m_iOrderServiceTime;}
     void setServiceTime(int serviceTime) {m_iOrderServiceTime = serviceTime;}

     int getOrderUnit() {return m_iOrderUnitCount;}
     void setOrderUnit(int orderUnit) {m_iOrderUnitCount = orderUnit;}

     Point getOrderLocation() {return m_ptOrderLocation;}
     void setOrderLocation(Point location) {m_ptOrderLocation = location;}

     int getOrderId() {return m_iOrderId; }
     void setOrderId(int orderId) {m_iOrderId = orderId;}


     // COrderInfo( COrderInfo const& );
     // COrderInfo& operator = (const COrderInfo& solution);

 private:
     int m_iOrderOpenTime;
     int m_iOrderCloseTime;
     int m_iOrderServiceTime;
     int m_iOrderUnitCount;
     int m_iOrderId;

     Point m_ptOrderLocation;
};


//  Class to represent Depot information. Each depot will have it's Open_Time and Close_Time. The Depot that will open earliest will have open time 0
//  and all other time will be normalized with respect to it. For the first version there will be only one depot
class CDepotInfo {
 public:
     CDepotInfo();
     ~CDepotInfo();

     int getOpenTime() {return m_iDepotOpenTime;}
     void setOpenTime(int openTime) {m_iDepotOpenTime = openTime;}

     int getCloseTime() {return m_iDepotCloseTime;}
     void setCloseTime(int closeTime) {m_iDepotCloseTime = closeTime;}

     int getDepotId() {return m_iDepotId;}
     void setDepotId(int id) {m_iDepotId = id;}

     Point getDepotLocation() {return m_ptDepotLocation;}
     void setDepotLocation(Point location) {m_ptDepotLocation = location;}

     // CDepotInfo( CDepotInfo const& );
     // CDepotInfo& operator = (const CDepotInfo& solution);

 private:
     int m_iDepotOpenTime;
     int m_iDepotCloseTime;

     int m_iDepotId;

     Point m_ptDepotLocation;
};

//  Class to represent information of a Tour. A Tour starts from a depot and ends in a depot. On the way it serves several orders.
//  Each Tour has a vehicle ID and the list of Orders it serves in appropriate order. It also has the total Distance, Cost and Time assciated.
class CTourInfo {
 public:
     CTourInfo();
     ~CTourInfo();

     void init();

     int getRemainingCapacity();

     int getVehicleId() {return m_vehicleInfo.getId();}

     CVehicleInfo& getVehicleInfo() {return m_vehicleInfo;}
     void setVehicleInfo(CVehicleInfo vehicleInfo) {m_vehicleInfo = vehicleInfo;}

     int getStartDepot() {return m_iStartDepotId;}
     void setStartDepot(int depotId) {m_iStartDepotId = depotId;}

     int getEndDepot() {return m_iEndDepotId;}
     void setEndDepot(int depotId) {m_iEndDepotId = depotId;}

     size_t getServedOrderCount() {return m_viOrderIds.size();}

     void updateCost(double cost, double distance, double travelTime);

     void setStartTime(std::vector<int> vStartTime) {m_viStartTime = vStartTime;}


     bool insertOrder(int orderId, int pos);
     bool removeOrder(int pos);


     double getDistance() {return m_dTotalDistance;}

     double getCost() {return m_dTotalCost;}

     double getTravelTime() {return m_dTotalTraveltime;}

     std::vector<int> getOrderVector() {return m_viOrderIds;}

     int getStartTime(int pos) {
         if ((unsigned int) pos >= m_viStartTime.size())
             return 0;
         else
             return m_viStartTime[pos];
     }

     friend bool operator == (const CTourInfo& cur, const CTourInfo& that);


     // bool operator != (const CTourInfo& that)
     //  {
     //  return(!(*this == that));
     // }

     // CTourInfo( CTourInfo const& );
     // CTourInfo& operator = (const CTourInfo& solution);


 private:
     CVehicleInfo m_vehicleInfo;
     int m_iStartDepotId;
     int m_iEndDepotId;
     int m_iOrdersServed;
     std::vector<int> m_viOrderIds;
     std::vector<int> m_viStartTime;
     double m_dTotalCost;
     double m_dTotalDistance;
     double m_dTotalTraveltime;
};



//  This class will represent a solution of a VRP problem. A solution will be consist of multiple tour.
//  It also contains the number of vehicle used, number of orders served and total cost, distance and traveltime.
class CSolutionInfo {
 public:
     CSolutionInfo();
     ~CSolutionInfo();

     int getVehicleUsed() {return m_iVehicleUsed;}
     int getOrderServed() {return m_iOrdersServed;}
     // int getVehicleUsed() {return m_iVehicleUsed;}

     bool addTour(CTourInfo& tour);
     CTourInfo& getTour(int pos) {return m_vtourAll[pos];}

     size_t getTourCount() {return (m_vtourAll.size());}

     size_t getUnservedOrderCount() {return m_vUnservedOrderId.size();}
     size_t getUnusedVehicleCount() {return m_vUnusedVehicles.size();}

     int getUnusedVehicleAt(int pos) {return m_vUnusedVehicles[pos];}

     void removeVehicle(int pos) {m_vUnusedVehicles.erase(m_vUnusedVehicles.begin() + pos);}
     void removeOrder(int pos) {m_vUnservedOrderId.erase(m_vUnservedOrderId.begin() + pos);}

     double getTotalCost() {return m_dTotalCost;}
     double getTotalDistance() {return m_dTotalDistance;}
     double getTotalTravelTime() {return m_dTotalTravelTime;}
     int getUnservedOrderAt(int pos) {return m_vUnservedOrderId[pos];}
     // void addOrderAtTour(int tourIndex, int insertIndex, int orderIndex);

     void replaceTourAt(int index, CTourInfo curTour);
     void replaceTour(CTourInfo curTour);

     bool init(std::vector<int> vecOrder, int iTotalOrder, std::vector<int> vecVehicle);

     std::vector<CTourInfo> getTourInfoVector() {return m_vtourAll;}

     // CTourInfo( CTourInfo const& );
     // CTourInfo& operator = (const CTourInfo& solution);

 private:
     std::vector<CTourInfo> m_vtourAll;
     std::vector<int> m_vUnservedOrderId;
     std::vector<int> m_vUnusedVehicles;
     int m_iVehicleUsed;
     int m_iOrdersServed;
     int m_iTotalOrders;
     double m_dTotalCost;
     double m_dTotalDistance;
     double m_dTotalTravelTime;
};

class CMoveInfo {
 public:
     CMoveInfo();
     ~CMoveInfo();

     bool isBetter(CMoveInfo *pVRPMove);

     void reverseMove();
     void setInitialTour(CTourInfo pTourData);
     void setInitialTour(CTourInfo pTourData1, CTourInfo pTourData2);
     void setModifiedTour(CTourInfo pTourData);
     void setModifiedTour(CTourInfo pTourData1, CTourInfo pTourData2);

     bool getModifiedTourAt(int index, CTourInfo& tourInfo);
     size_t getModifiedTourCount() const { return m_vModifiedTour.size();}
     double getModifiedTourCost() const;
     void getInitialTour(CTourInfo &TourData);
     void getInitialTour(CTourInfo &TourData1, CTourInfo &TourData2);

     friend bool operator == (const CMoveInfo& cur, const CMoveInfo& that);


     // bool operator != (const CMoveInfo& that)
     //  {
     //  return(!(*this == that));
     // }

     // CMoveInfo( CMoveInfo const& );
     // CMoveInfo& operator = (const CMoveInfo& solution);

 private:
     void clearInitialTour();
     void clearModifiedTour();

     std::vector<CTourInfo> m_vInitialTour;
     std::vector<CTourInfo> m_vModifiedTour;
};




//  This is the main class that will solve the VRP problem. It will use the previous classes to represent the problem and the solution.
//  It will also have pre generated point to point distance/ cost/ traveltime information in maps.
class CVRPSolver {
 public:
     CVRPSolver();
     ~CVRPSolver();

     bool init();

     bool addDepot(CDepotInfo depotInfo);
     bool addOrder(COrderInfo orderInfo);
     bool addVehicle(CVehicleInfo vehicleInfo);

     CostPack getOrderToOrderCost(int firstOrder, int secondOrder);
     CostPack getDepotToOrderCost(int depotId, int orderId);
     CostPack getOrderToDepotCost(int depotId, int orderId);

     bool addOrderToOrderCost(int firstOrder, int secondOrder, CostPack cost);
     bool addDepotToOrderCost(int depotId, int orderId, CostPack cost);
     bool addOrderToDepotCost(int depotId, int orderId, CostPack cost);

     void removeVehicle(int vehicleIndex)
     {m_viUnusedVehicleIndex.erase(m_viUnusedVehicleIndex.begin() + vehicleIndex, m_viUnusedVehicleIndex.begin() + vehicleIndex+1);}

     void removeOrder(int orderIndex)
     {m_viUnservedOrderIndex.erase(m_viUnservedOrderIndex.begin() + orderIndex, m_viUnservedOrderIndex.begin() + orderIndex + 1);}

     bool solveVRP(std::string& strError);

     bool getSolution(CSolutionInfo& solution, std::string& strError);
     CSolutionInfo generateInitialSolution();
     bool updateFinalSolution(CSolutionInfo& solutionInfo);
     std::pair<int, double> getPotentialInsert(CTourInfo& curTour, COrderInfo& curOrder);
     CostPack getCostForInsert(CTourInfo& curTour, COrderInfo& curOrder, int pos);
     bool tabuSearch(CSolutionInfo& solutionInfo);
     int getServiceTime(int order_id) {return (m_vOrderInfos[m_mapOrderIdToIndex[order_id]].getServiceTime());}

     bool insertOrder(CTourInfo& tourInfo, int orderId, int pos);
     void applyBestMoveInCurrentSolution(CSolutionInfo& solutionInfo, CMoveInfo& bestMove);
     void insertUnservedOrders(CSolutionInfo& solutionInfo);
     // void attemptFeasibleNodeExchange(CSolutionInfo& solutionInfo);
     void attempVehicleExchange(CSolutionInfo& solutionInfo);
     // CMoveInfo identifyPotentialMove();
     void updateTabuCount(CMoveInfo& bestMove);

     bool isTabuMove(CMoveInfo& curMove);
     bool updateTourCosts(CTourInfo& tourInfo);
     bool addOrderAtTour(CSolutionInfo& solutionInfo, int tourIndex, int insertIndex, int orderIndex);


 private:
     bool m_bIsReadyToSolve;
     std::vector<CVehicleInfo> m_vVehicleInfos;
     std::vector<COrderInfo> m_vOrderInfos;
     std::vector<CDepotInfo> m_vDepotInfos;

     std::map<int, int> m_mapOrderIdToIndex;
     std::map<int, int> m_mapVehicleIdToIndex;
     std::map<int, int> m_mapDepotIdToIndex;

     std::map<std::pair<int, int>, CostPack> m_mapOrderToOrderCost;
     std::map<std::pair<int, int>, CostPack> m_mapDepotToOrderrCost;
     std::map<std::pair<int, int>, CostPack> m_mapOrderToDepotCost;

     /*
        std::map<CMoveInfo, int> m_mapMoveFrequency;
        std::map<CMoveInfo, std::pair<int, int> > m_mapTabuCount;
        std::set<CMoveInfo> m_sTabuList;
        */

     std::vector<CMoveInfo> m_veMoves;

     bool m_bIsSolutionReady;
     CSolutionInfo m_solutionFinal;

 private:
     std::vector<int> m_viUnservedOrderIndex;
     std::vector<int> m_viUnusedVehicleIndex;
     int m_iGeneratedSolutionCount;
     int m_iStepsSinceLastSolution;
     bool m_bFoundOptimal;
};

#endif  // SRC_VRP_BASIC_SRC_VRP_SOLVER_H_
