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
	//if((p1.cost).cost > (p2.cost).cost)
        if(p1.cost > p2.cost)
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

void CTourInfo::setOrderVector(std::vector<int> order_vector)
{
	m_viOrderIds.clear();
	m_viStartTime.clear();
	m_viOrderIds=order_vector;
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
//	m_vUnservedOrderId = vecOrder;
	m_iTotalOrders = iTotalOrder;
	m_vUnusedVehicles =  vecVehicle;
	m_pq=pq;

	while(!m_pq.empty())
	{
		pq_pair p;
		p=m_pq.top();
		m_vUnservedOrderId.push_back(p.d_o_pair.second);  // orders in increasing order of their closing time 
//                printf("order_id %d  close time %d \n",p.d_o_pair.second,p.cost);
	m_pq.pop();
	}

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

	m_bIsSolutionReady = true;
//        m_solutionFinal = Reduce_Vehicles(initialSolution);
//	m_solutionFinal=Tabu_Search(m_solutionFinal);
       m_solutionFinal=Tabu_Search(initialSolution);
       m_solutionFinal=Reduce_Vehicles(m_solutionFinal);
	return true;
	
}



CSolutionInfo CVRPSolver::Reduce_Vehicles(CSolutionInfo InitialSolution)
{
  
     std::vector<CTourInfo> Tour_Vector=InitialSolution.getTourInfoVector();

     int count=Tour_Vector.size();

     for(int i=0;i<count;i++)                                // sort the tours in descending order of their orders served
     {
	     for(int j=0;j<count-1;j++)
	     {
		     if(Tour_Vector[j].getServedOrderCount()<Tour_Vector[j+1].getServedOrderCount())
		     {
			     CTourInfo temp;
			     temp=Tour_Vector[j];
			     Tour_Vector[j]=Tour_Vector[j+1];                                                
			     Tour_Vector[j+1]=temp;
		     }
	     }
     }
      

    int start=0,end=count-1;
    bool possible=true;

    int c=10000;
    while(end>Tour_Vector.size()/2)
    {
	    CTourInfo end_tour=Tour_Vector[end];

	    std::vector<int>end_order= end_tour.getOrderVector();

	    while(!end_order.empty() && start<end)
	    {
	    CTourInfo start_tour=Tour_Vector[start];
	    
            
	    for(int i=0;i<end_order.size();i++)
	    {
                  int orderInd = m_mapOrderIdToIndex[end_order[i]];
                  COrderInfo order= m_vOrderInfos[orderInd];
		  start_tour=Modified_Hill_Climbing(start_tour,order);

		  if(updateTourCosts(start_tour))
		  {
			  Tour_Vector[start]=start_tour;
			  end_order.erase(end_order.begin()+i);
                          printf("here ");			 
		  }
				  				  
	    }
	    start++;
	    

	    }

           if(end_order.empty())
	   {
                  Tour_Vector.erase(Tour_Vector.begin() + end);
		  end--;
		  start=0;
	   }
	   else
	   {
		   end_tour.setOrderVector(end_order);
                   bool yes=updateTourCosts(end_tour);
		   Tour_Vector[end]=end_tour;
		   end--;
		   start=0;

	   }

    }  


	CSolutionInfo NewSolution;

	std::vector<int> vecOrders, vecVehicles;
	for(int i = 0; i < m_vOrderInfos.size(); i++)
	{
		vecOrders.push_back(m_vOrderInfos[i].getOrderId());
	}

	for(int i = 0; i < m_vVehicleInfos.size(); i++)
	{
		vecVehicles.push_back(m_vVehicleInfos[i].getId());
	}

	NewSolution.init(vecOrders, vecOrders.size(), vecVehicles,do_pq);

	for(int i=0;i<Tour_Vector.size();i++)
	{
                     NewSolution.addTour(Tour_Vector[i]);
	}
 

	return NewSolution;

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
	while(iUnusedVehicles && iUnservedOrders)
	{

		CTourInfo curTour;
		int vehicleIndex = rand() % iUnusedVehicles--;
		int vehicleInd = m_mapVehicleIdToIndex[initialSolution.getUnusedVehicleAt(vehicleIndex)];
		curTour.setVehicleInfo(m_vVehicleInfos[vehicleInd]);//m_viUnusedVehicleIndex[vehicleIndex]
		initialSolution.removeVehicle(vehicleIndex);
		
		curTour.setStartDepot(m_vDepotInfos[0].getDepotId());
		curTour.setEndDepot(m_vDepotInfos[0].getDepotId());


		for(int i=0;i<iUnservedOrders;i++)
		{
			CTourInfo original_tour=curTour;
			int orderInd = m_mapOrderIdToIndex[initialSolution.getUnservedOrderAt(i)];
			COrderInfo curOrder = m_vOrderInfos[orderInd];	     

			curTour=Hill_Climbing(curTour,curOrder);
                        
		/*	if(updateTourCosts(curTour))
				printf("updated\n");
			else
				printf("not_updated\n");*/
			if(updateTourCosts(curTour))
			{
				iUnservedOrders--;
				initialSolution.removeOrder(i);
			}
			else
				curTour=original_tour;

		}	
		
		initialSolution.addTour(curTour);
	}

	return initialSolution;
}


CSolutionInfo CVRPSolver::Tabu_Search(CSolutionInfo InitialSolution)
{
     CSolutionInfo BestSolution=InitialSolution;
     int count=10000;
     while(count--)
     {  
	
	std::vector<CTourInfo> TourVector=BestSolution.getTourInfoVector();
	int tour_size=TourVector.size();
	int t1,t2;

	t1=rand()%tour_size;
	t2=rand()%tour_size;
//	printf("%d %d   ",t1,t2);
	while(t1==t2)
	{
            t2=rand()%tour_size;
	}


	CTourInfo tour1=TourVector[t1];
	CTourInfo tour2=TourVector[t2];
	CTourInfo O_Tour1,O_Tour2;
	O_Tour1=tour1;
	O_Tour2=tour2;

	std::vector<int> order1=tour1.getOrderVector();
	std::vector<int> order2=tour2.getOrderVector();


	for(int i=0;i<order1.size();i++)
	{

		CMoveInfo Move;
		Move.setInitialTour(O_Tour1,O_Tour2);
		std::vector<int> t_order1=order1;
		std::vector<int> t_order2=order2;
		for(int j=0;j<order2.size();j++)
		{
			t_order2.insert(t_order2.begin()+j,t_order1[i]);
			tour2.setOrderVector(t_order2);
			if(updateTourCosts(tour2))
			{

				t_order1.erase(t_order1.begin()+i);
				tour1.setOrderVector(t_order1);
				if(updateTourCosts(tour1)||t_order1.size()==0)
				{
					TourVector[t1]=tour1;
					TourVector[t2]=tour2;

					Move.setModifiedTour(tour1,tour2);
					CSolutionInfo NewSolution;

					std::vector<int> vecOrders, vecVehicles;
					for(int i = 0; i < m_vOrderInfos.size(); i++)
						vecOrders.push_back(m_vOrderInfos[i].getOrderId());


					for(int i = 0; i < m_vVehicleInfos.size(); i++)
						vecVehicles.push_back(m_vVehicleInfos[i].getId());

					NewSolution.init(vecOrders, vecOrders.size(), vecVehicles,do_pq);

					for(int i=0;i<TourVector.size();i++)
					{

						std::vector<int> order3=TourVector[i].getOrderVector();
						if(order3.size()>0)
							NewSolution.addTour(TourVector[i]);
					}

					if(NewSolution.getTotalTravelTime()< BestSolution.getTotalTravelTime())
					{
						if(!isTabuMove(Move))
						{
							BestSolution=NewSolution;
							m_veMoves.push_back(Move);
							if( m_veMoves.size()>30)
								m_veMoves.erase( m_veMoves.begin());
						
							printf("New_Hit ");
						}
					}

				}

			}
		}
	}


	for(int i=0;i<order2.size();i++)
	{

		CMoveInfo Move;
		Move.setInitialTour(O_Tour1,O_Tour2);
		std::vector<int> t_order1=order1;
		std::vector<int> t_order2=order2;
		for(int j=0;j<order1.size();j++)
		{
			t_order1.insert(t_order1.begin()+j,t_order2[i]);
			tour1.setOrderVector(t_order1);
			if(updateTourCosts(tour1))
			{

				t_order2.erase(t_order2.begin()+i);
				tour2.setOrderVector(t_order2);
				if(updateTourCosts(tour2)||t_order2.size()==0)
				{
					TourVector[t1]=tour1;
					TourVector[t2]=tour2;

					Move.setModifiedTour(tour1,tour2);
					CSolutionInfo NewSolution;

					std::vector<int> vecOrders, vecVehicles;
					for(int i = 0; i < m_vOrderInfos.size(); i++)
						vecOrders.push_back(m_vOrderInfos[i].getOrderId());


					for(int i = 0; i < m_vVehicleInfos.size(); i++)
						vecVehicles.push_back(m_vVehicleInfos[i].getId());

					NewSolution.init(vecOrders, vecOrders.size(), vecVehicles,do_pq);

					for(int i=0;i<TourVector.size();i++)
					{

						std::vector<int> order3=TourVector[i].getOrderVector();
						if(order3.size()>0)
							NewSolution.addTour(TourVector[i]);
					}

					if(NewSolution.getTotalTravelTime()< BestSolution.getTotalTravelTime())
					{
						if(!isTabuMove(Move))
						{
							BestSolution=NewSolution;
							m_veMoves.push_back(Move);
							if( m_veMoves.size()>30)
								m_veMoves.erase( m_veMoves.begin());
						
							printf("other_Hit ");
						}
					}

				}

			}
		}
	}


       int id1,id2;
       double min=100000000.00;

       for(int i=0;i<order1.size();i++)
       {
		for(int j=0;j<order2.size();j++)
	       {
		     CostPack  cPack = getOrderToOrderCost(order1[i], order2[j]);

		     if(cPack.cost<min)
		     {
			     min=cPack.cost; id1=i;id2=j;

		     }
			 cPack = getOrderToOrderCost(order1[j], order2[i]);
		      if(cPack.cost<min)
		      {
			      min=cPack.cost;id1=i;id2=j;
		      }

	        }
       }
       int flag=0;
       std::vector<int> temp;
       CMoveInfo Move1;
       Move1.setInitialTour(O_Tour1,O_Tour2);

       if(flag==0)
       {
	       temp=order1;
	       temp.insert(temp.begin()+id1,order2[id2]);
	       tour1.setOrderVector(temp);
	       if(updateTourCosts(tour1))
	       {
		       order2.erase(order2.begin()+id2);
		       tour2.setOrderVector(order2);
		       if(updateTourCosts(tour2)||order2.size()==0)
		       {
		       TourVector[t1]=tour1;
		       TourVector[t2]=tour2;
		       flag=1;
		       }

	       }
	       else{
		       tour1=TourVector[t1];
	       }
		       
       }
       if(flag==0)
       {

	       temp=order1;
	       temp.insert(temp.begin()+id1+1,order2[id2]);
	       tour1.setOrderVector(temp);
	       if(updateTourCosts(tour1))
	       {
		       order2.erase(order2.begin()+id2);
		       tour2.setOrderVector(order2);
		       if(updateTourCosts(tour2)||order2.size()==0)
		       {
		       TourVector[t1]=tour1;
		       TourVector[t2]=tour2;
		       flag=1;
		       }

	       }
	       else{
		       tour1=TourVector[t1];
	       }
	}
       if(flag==0)
       {

	       temp=order2;
	       temp.insert(temp.begin()+id2,order1[id1]);
	       tour2.setOrderVector(temp);
	       if(updateTourCosts(tour2))
	       {
		       order1.erase(order1.begin()+id1);
		       tour1.setOrderVector(order1);
		       if(updateTourCosts(tour1)||order1.size()==0)
		       {
		       TourVector[t1]=tour1;
		       TourVector[t2]=tour2;
		       flag=1;
		       }

	       }
	       else{
		       tour2=TourVector[t2];
	       }
		       

       }
       if(flag==0)
       {

	       temp=order2;
	       temp.insert(temp.begin()+id2+1,order1[id1]);
	       tour2.setOrderVector(temp);
	       if(updateTourCosts(tour2))
	       {
		       order1.erase(order1.begin()+id1);
		       tour1.setOrderVector(order1);
		       if(updateTourCosts(tour1)||order1.size()==0)
		       {
		       TourVector[t1]=tour1;
		       TourVector[t2]=tour2;
		       flag=1;
		       }

	       }
	       else{
		       tour2=TourVector[t2];
	       }
		       
       }
      int next_move=0; 
       if(flag==1)
       {
        
	Move1.setModifiedTour(tour1,tour2);
	CSolutionInfo NewSolution;

	std::vector<int> vecOrders, vecVehicles;
	for(int i = 0; i < m_vOrderInfos.size(); i++)
	{
		vecOrders.push_back(m_vOrderInfos[i].getOrderId());
	}

	for(int i = 0; i < m_vVehicleInfos.size(); i++)
	{
		vecVehicles.push_back(m_vVehicleInfos[i].getId());
	}

	NewSolution.init(vecOrders, vecOrders.size(), vecVehicles,do_pq);

	for(int i=0;i<TourVector.size();i++)
	{
                    std::vector<int> order3=TourVector[i].getOrderVector();
		    if(order3.size()>0)
			NewSolution.addTour(TourVector[i]);
	}

	if(NewSolution.getTotalTravelTime()< BestSolution.getTotalTravelTime())
	{
              if(!isTabuMove(Move1))
	      {
		BestSolution=NewSolution;
		
		  m_veMoves.push_back(Move1);

		  if( m_veMoves.size()>30)
			   m_veMoves.erase( m_veMoves.begin());
		  next_move=1;
		  printf("done ");
	      }
	}

       }
       //       if(next_move==0||next_move==1)
       {
//	    printf("Hit inside");

       order1=O_Tour1.getOrderVector();
       order2=O_Tour2.getOrderVector();
       int temp1;
      
        for(int i=0;i<order1.size();i++)
         {	
		CMoveInfo Move;
		Move.setInitialTour(O_Tour1,O_Tour2);
	       	 for(int j=0;j<order2.size();j++)
	       {
                       temp1=order1[i];
		       order1[i]=order2[j];
		       order2[j]=temp1;
		       tour1.setOrderVector(order1);
		       tour2.setOrderVector(order2);
		       int swap=0;
		       if(updateTourCosts(tour1) && updateTourCosts(tour2))
		       {


		       TourVector[t1]=tour1;
		       TourVector[t2]=tour2;

			       Move.setModifiedTour(tour1,tour2);
			       CSolutionInfo NewSolution;

			       std::vector<int> vecOrders, vecVehicles;
			       for(int i = 0; i < m_vOrderInfos.size(); i++)
			      	       vecOrders.push_back(m_vOrderInfos[i].getOrderId());
			      

			       for(int i = 0; i < m_vVehicleInfos.size(); i++)
			  	       vecVehicles.push_back(m_vVehicleInfos[i].getId());
			      
				     NewSolution.init(vecOrders, vecOrders.size(), vecVehicles,do_pq);

			       for(int i=0;i<TourVector.size();i++)
			       	       NewSolution.addTour(TourVector[i]);
			      

			       if(NewSolution.getTotalTravelTime()< BestSolution.getTotalTravelTime())
			       {
				       if(!isTabuMove(Move))
				       {
					       BestSolution=NewSolution;
					       m_veMoves.push_back(Move);
					       if( m_veMoves.size()>30)
						       m_veMoves.erase( m_veMoves.begin());
					       swap=1;
					       printf("Hit ");
				       }
			       }
		       }
		       if(swap==0)
		       {

                       temp1=order1[i];
		       order1[i]=order2[j];
		       order2[j]=temp1;
		       }
	       }
	 }
       }
       



    }
     return BestSolution;
}

CTourInfo CVRPSolver::Modified_Hill_Climbing(CTourInfo curTour,COrderInfo curOrder){

	std::vector<int> order_vector=curTour.getOrderVector();

	int size=order_vector.size();


       if(size==0)
       {
	       order_vector.push_back(curOrder.getOrderId());
	       curTour.setOrderVector(order_vector);
	       return curTour;

       }  

       else
       {

	       order_vector.push_back(curOrder.getOrderId());
	       size=order_vector.size();
	for(int i=0;i<size-1 ;i++)
	{
		int flag=0;
		for(int j=i+1;j<size;j++)
		{
			//double cost1=hill_climbing_cost(curTour,order_vector);
                      
//			if( m_vOrderInfos[m_mapOrderIdToIndex[order_vector[i]]].getCloseTime() >  m_vOrderInfos[m_mapOrderIdToIndex[order_vector[j]]].getCloseTime())
		{
				std::vector<int> temp_order_vector=order_vector;

				int temp=temp_order_vector[i];
				temp_order_vector[i]=temp_order_vector[j];
				temp_order_vector[j]=temp_order_vector[i];
			//	double cost2=hill_climbing_cost(curTour,temp_order_vector);

			//	if(cost2-cost1 <0)
				{
					order_vector=temp_order_vector;
					flag=1;
				}
		
            	}
	// 	if(flag==0)
	//		break;
		}
          		
	}
        curTour.setOrderVector(order_vector);
        return curTour;	

       }
	

}



CTourInfo CVRPSolver::Hill_Climbing(CTourInfo curTour,COrderInfo curOrder){

	std::vector<int> order_vector=curTour.getOrderVector();

	int size=order_vector.size();


       if(size==0)
       {
	       order_vector.push_back(curOrder.getOrderId());
	       curTour.setOrderVector(order_vector);
	       return curTour;

       }  

       else
       {

	       order_vector.push_back(curOrder.getOrderId());
	       size=order_vector.size();
	for(int i=0;i<size-1 ;i++)
	{
		int flag=0;
		for(int j=i+1;j<size;j++)
		{
			double cost1=hill_climbing_cost(curTour,order_vector);
                      
			if( m_vOrderInfos[m_mapOrderIdToIndex[order_vector[i]]].getCloseTime() >  m_vOrderInfos[m_mapOrderIdToIndex[order_vector[j]]].getCloseTime())
		{
				std::vector<int> temp_order_vector=order_vector;

				int temp=temp_order_vector[i];
				temp_order_vector[i]=temp_order_vector[j];
				temp_order_vector[j]=temp_order_vector[i];
				double cost2=hill_climbing_cost(curTour,temp_order_vector);

				if(cost2-cost1 <0)
				{
					order_vector=temp_order_vector;
					flag=1;
				}
		
            	}
	 	if(flag==0)
			break;
		}
          		
	}
        curTour.setOrderVector(order_vector);
        return curTour;	

       }
	

}




double CVRPSolver::hill_climbing_cost(CTourInfo curTour,std::vector<int> order_vector)
{
         
        int total_capacity=  m_vVehicleInfos[ m_mapVehicleIdToIndex[curTour.getVehicleId()]].getCapacity();
	int TWV=0,CV=0;

	double dCost, dDistance, dTravelTime;
	dCost = dDistance = dTravelTime = 0.0;
	CostPack cPack = getDepotToOrderCost(curTour.getStartDepot(), order_vector[0]);
	
	dCost += cPack.cost;
	dDistance += cPack.distance;
	int ind = m_mapOrderIdToIndex[order_vector[0]];

	if(dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
			TWV++;
	dTravelTime = max(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(),m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime());

	total_capacity=total_capacity - m_vOrderInfos[ind].getOrderUnit();
	if(total_capacity <0 )
		CV++;

	int i;

	for(i = 1; i < order_vector.size(); i++)
	{
		cPack = getOrderToOrderCost(order_vector[i - 1], order_vector[i]);
		dCost += cPack.cost;
		dDistance += cPack.distance;

		ind = m_mapOrderIdToIndex[order_vector[i]];

		if(dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
			TWV++;

		dTravelTime = max(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(),m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime());
                 
	total_capacity=total_capacity - m_vOrderInfos[ind].getOrderUnit();
	if(total_capacity <0 )
		CV++;

	}

	cPack = getOrderToDepotCost(order_vector[i - 1], curTour.getEndDepot());
	
	dCost += cPack.cost;
	dDistance += cPack.distance;
	dTravelTime += cPack.traveltime;


	ind = m_mapDepotIdToIndex[curTour.getEndDepot()];

	if(dTravelTime > m_vDepotInfos[ind].getCloseTime())
		TWV++;


	return (0.2*dTravelTime)+(0.7*TWV)+(0.1*CV);
        
}


bool CVRPSolver::updateTourCosts(CTourInfo& tourInfo)
{
	std::vector<int> vecOrderId = tourInfo.getOrderVector();
	std::vector<int> vecStartTimes;


	if(vecOrderId.size()==0)
	{   

	       return false;	
	}

       int vehicle_ind= m_mapVehicleIdToIndex[tourInfo.getVehicleId()];
       tourInfo.setVehicleInfo(m_vVehicleInfos[vehicle_ind]);

	double dCost, dDistance, dTravelTime;
	dCost = dDistance = dTravelTime = 0.0;

	CostPack cPack = getDepotToOrderCost(tourInfo.getStartDepot(), vecOrderId[0]);

	dCost += cPack.cost;
	dDistance += cPack.distance;
	int ind = m_mapOrderIdToIndex[vecOrderId[0]];

	vecStartTimes.push_back(0);

	if(dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
	{      
//		printf("twv ");
		return false;
	}

	dTravelTime = max(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(), m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime());
	
	vecStartTimes.push_back(ceil(dTravelTime));
       
	if(!tourInfo.getVehicleInfo().loadUnit(m_vOrderInfos[ind].getOrderUnit()))
	{
//	     printf("cv ");		
		return false;
	}
	int i;
	for(i = 1; i < vecOrderId.size(); i++)
	{
		cPack = getOrderToOrderCost(vecOrderId[i - 1], vecOrderId[i]);
		dCost += cPack.cost;
		dDistance += cPack.distance;
		ind = m_mapOrderIdToIndex[vecOrderId[i]];
		if(dTravelTime + cPack.traveltime > m_vOrderInfos[ind].getCloseTime())
		{ // printf("twv ");	
			return false;}

		dTravelTime = max(dTravelTime + cPack.traveltime + m_vOrderInfos[ind].getServiceTime(), m_vOrderInfos[ind].getOpenTime() + m_vOrderInfos[ind].getServiceTime());

		vecStartTimes.push_back(ceil(dTravelTime));
         
	if(!tourInfo.getVehicleInfo().loadUnit(m_vOrderInfos[ind].getOrderUnit()))
	{  //  printf("cv ");		
		return false;}
         
	}

	cPack = getOrderToDepotCost(vecOrderId[i - 1], tourInfo.getEndDepot());
	dCost += cPack.cost;
	dDistance += cPack.distance;
	dTravelTime += cPack.traveltime;
	vecStartTimes.push_back(ceil(dTravelTime));

	ind = m_mapDepotIdToIndex[tourInfo.getEndDepot()];

	if(dTravelTime > m_vDepotInfos[ind].getCloseTime())
	{//printf("twv ");
		return false;
	}

	tourInfo.updateCost(dCost, dDistance, dTravelTime);

	tourInfo.setStartTime(vecStartTimes);

	return true;
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
//	pq.cost=cost;
	pq.cost=m_vOrderInfos[ m_mapOrderIdToIndex[orderId]].getCloseTime();

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


bool CVRPSolver::isTabuMove(CMoveInfo& curMove)

{

	int i, tot = m_veMoves.size();
	for(i = 0; i < tot; i++)
	{
	if(curMove == m_veMoves[i])
		return true;
	}
	return false;

}
