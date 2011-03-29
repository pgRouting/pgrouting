/*
 * Dial-a-Ride Problem solution algorithm for PostgreSQL
 *
 * Copyright (c) 2010 Anton A. Patrushev, Georepublic
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 */
                   
extern "C"
{
#include <gaul.h>
#include <postgres.h>
}

#include <vector>
#include "darp.h"

#define MAX_GENERATIONS 100 //must be over 15000
#define MAX_RIDE_TIME 7200 //2 hours

#undef DEBUG
//#define DEBUG 1

#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif

using namespace std;

int onum, vnum;
int depot;
int *pen;

vector<order_t> ORDERS;
vector<vehicle_t> VEHICLES;
vector< vector<double> > DISTANCES;

vector< vector< pair<int, double> > > ORD;


class Router
{
	public:
	vector<order_t> v;    //orders in clusters
	vector<int>     cs;   //customers served
	vector<int>     cv;     //customers in a vehicle
	vector<int>     cn;   //customers not yet served
	
	int n;            //total number of customers in all clusters
	int cun;          //number of customers in a cluster
	vector<float> w;       //weights
	//w[1], w[2] - total route and time windows violation weight
	//w[3], w[4] - ride time and route duration violation
	//w[5], w[6] - weight of ex riding and waiting time
	//w[7]       - weight on distance
	
	vector<double> load1;   //vector with a load in a vehicle after
	                       //a node has been served
	              
	vector<double> load2;
	vector<double> load3;
	
	vector<double> ctime;
	vector<double> wtime;
	
	vector<int> ord;      //order of customers in a solution 
	
	//Request r;
	//Distance d;
	vehicle_t car;
	
	Router();
	
	//marks gnode as visited by updating global data
	void visit(int gnode)
	{	
		DBG("Visiting node %i", gnode);
		for(int c=0;c<v.size();c++)
		{	
			if(gnode == cn[c])
			{
				if(gnode <= (n-1))
				{			
					//remove from non-served list
					//cn[c] = -1;
					cn[c] += n-1;
					DBG("cn[%i] = %i",c,cn[c]);
					//put to in-vehicle list
					cv[c] = gnode;
					break;				
				}
				
				else
				{			
					//put to served list
					cs[c] = gnode;
					//remove from in-vehicle list
					cv[c] = -1;
					cn[c] = -1;
					DBG("cn[%i] = %i",c,cn[c]);
					break;				
				}	
			}	
		}//for ends
		DBG("Finish visiting");
	}
	
	//marks gnode as not visited by updating global data
	void unvisit(int gnode)
	{
		for(int c=v.size()-1;c>=0;c--)
		{		
			if(gnode==v[c].id)
			{		
				cn[c] = v[c].id;
				cv[c] = -1;
				break;
			}
			
			if(gnode==v[c].id+n-1)
			{
				cs[c] = -1;
				cv[c] = v[c].id;
				break;		
			}
		}
		//for ends
	}
	
	int unserved()
	{
		int u = 0;
		for(int i = 1; i < cn.size(); ++i)
		{
			if(cn.at(i) != -1)
				++u;
		}
		return u;
	}
		
	//Returns node closest to cnode
	int closestNode(int cnode, int k)
	{		
		float minldis=3.3E+38f, newldis;
		int cstnode = -2, count;
		
		//DBG("calculating closest for %i", cnode);
		
		//destination of c not in N
		for (int c=0; c < /*cn.size()*/ cun; ++c)
		{
			//DBG("cn[%i]=%i",c,cn[c]);
			if(cn[c] > 0 && cn[c]!=cnode)
			{		
				newldis = separation(cnode, cn[c], k);
				
				if(minldis > newldis)
				{			
					minldis = newldis;
					cstnode = cn[c];		
				}			
			}
		}
		//for ends
			
		return(cstnode);	
	}	
	
	//returns space-time separation between node cnode and nnode
	float separation(int cnode, int nnode, int k)
	{		
		//DBG("separation for %i, %i", cnode, nnode);
		float ttime, timek1;
		float routedur=0, twviol=0,ridetimeviol=0, exride=0, wt=0;
		ttime = DISTANCES.at(cnode).at(nnode);
		//DBG("ttime = %f", ttime);
		timek1 = ctime[k] + ttime; //arrival time at nnode
		//DBG("timek1 = %f", timek1);
		//change in route duration, ctime[k+1]-ctime[k]
		if(nnode>n-1)
			routedur = timek1 + ORDERS.at(nnode-n+1).doTime-ctime[k];
		else
			routedur = timek1 + ORDERS.at(nnode).puTime-ctime[k];
		//DBG("routedur = %f", routedur);
		//tw violations calculated
		//for we want to get there soon (twviol>0)
		
		float nnodeUT, nnodeLT;
		
		if(nnode>n-1)
		{
			nnodeUT = ORDERS.at(nnode-n+1).doTime + ORDERS.at(nnode-n+1).doUT->time/1000000;
			nnodeLT = ORDERS.at(nnode-n+1).doTime - ORDERS.at(nnode-n+1).doLT->time/1000000;
		}
		else
		{
			nnodeUT = ORDERS.at(nnode).puTime + ORDERS.at(nnode).puUT->time;
			nnodeLT = ORDERS.at(nnode).puTime - ORDERS.at(nnode).puLT->time/1000000;
		}
		
		if(timek1>nnodeUT);//r.getUTimeWindow(nnode))
			twviol=timek1-nnodeUT;
		//tw viol - if early then increase cost - can wait(twviol&lt;0)
		if(timek1<nnodeLT)
			twviol=timek1-nnodeLT;
		if(timek1 < nnodeLT) //if arrival is to early
		
		timek1 = nnodeLT; //wait until ok
		//customers ride time violations caluclated,
		//if customer has been in the car for longer than max
		//ride time sais then ride time viol &gt; 0, service times of
		//customer not included, i.e. nnode is a drop off location
		// AND
		//customers excess ride times calculated,
		//ridetime - direct transport time
		if(nnode>n)
		{
			
			for(int i=1;i<ord.size()-2;i++)
			{
				
				if(ord[i]==nnode-n-1)
				{
					
					if(timek1-ctime[i]>MAX_RIDE_TIME)
					ridetimeviol=timek1-ctime[i];
					exride=	timek1- ctime[i] - DISTANCES.at(ord[i]).at(nnode);//d.getDistance(ord[i],nnode);
					
				}
				//if
				
			}
			//for
			
		}
		//if
		//waiting time calculated * persons in the vehicle
		int count = 0;
		for (int c=0;c<cv.size();c++)
		{
			
			if(cv[c]!= -1) count++;
			
		}
		
		wt = count*(timek1-(ctime[k] + ttime));
	
		
		return (
				w[1]*routedur - 
				w[3]*exride +  				
				w[4]*wt + 
				w[5]*ridetimeviol -
				w[6]*twviol -
				w[7]*ttime
				);
		
	}	

	//returns cost of move from current node to next node
	float movecost(int cnode, int nnode, int k)
	{
		DBG("in movecost(%i,%i,%i)",cnode,nnode,k);
		float ttime, timek1;
		
		float routedur=0, twviol=0,ridetimeviol=0, exride=0, wt=0;
		ttime = DISTANCES.at(cnode).at(nnode);
		DBG("ttime=%f",ttime);
		timek1 = ctime[k] + ttime; //arrival time at nnode
		DBG("timek1=%f",timek1);
		
		float nnodeST;
		if(nnode>n-1)
			nnodeST = ORDERS.at(nnode-n+1).doTime;
		else
			nnodeST = ORDERS.at(nnode).puTime;

		DBG("wtime[%i]=%f",k+1,wtime[k+1]);
		DBG("ctime[%i]=%f",k,ctime[k]);
		DBG("nnodeST=%f",nnodeST);
		
		//change in route duration, ctime[k+1]-ctime[k]
		routedur = timek1+/*wtime[k+1]*/+nnodeST-ctime[k];
		DBG("routedur=%f",routedur);
		
		float nnodeUT, nnodeLT;
		
		if(nnode>n-1)
		{
			nnodeUT = ORDERS.at(nnode-n+1).doTime + ORDERS.at(nnode-n+1).doUT->time/1000000;
			nnodeLT = ORDERS.at(nnode-n+1).doTime - ORDERS.at(nnode-n+1).doLT->time/1000000;
		}
		else
		{
			nnodeUT = ORDERS.at(nnode).puTime + ORDERS.at(nnode).puUT->time/1000000;
			nnodeLT = ORDERS.at(nnode).puTime - ORDERS.at(nnode).puLT->time/1000000;
		}		

		//tw violations calculated,
		if(timek1>nnodeUT)
			twviol=timek1-nnodeUT;
		if(ctime[k] + ttime < nnodeLT)
			twviol=nnodeLT-(ctime[k] + ttime);
		if(timek1 < nnodeLT) //if arrival is to early
		timek1= nnodeLT; //wait until ok
		//customers ride time violations caluclated,
		//if customer has been in the car for longer than max
		//ride time sais then ride time viol &gt; 0, service times of
		//customer not included, i.e. nnode is a drop off location
		// AND
		//customers excess ride times calculated,
		//ridetime - direct transport time
		if(nnode>n-1)
		{
			
			for(int i=1;i<ord.size()-2;i++)
			{
				
				if(ord[i]==nnode-n+1)
				{
					
					if(timek1-ctime[i]>MAX_RIDE_TIME)
					ridetimeviol=timek1-ctime[i]-MAX_RIDE_TIME;
					exride=
					timek1- ctime[i] - DISTANCES.at(ord[i]).at(nnode);//d.getDistance(ord[i],nnode);
					
				}
				//if
				
			}
			//for
			
		}
		//if
		//waiting time calculated * persons in the vehicle
		int count = 0;
		for (int c=0;c<cv.size();c++)
		{
			
			if(cv[c]!= -1) count++;
			
		}
		
		wt = count*(timek1-(ctime[k] + ttime));
		
		//return(w[1]*routedur + w[2]*twviol + w[3]*ridetimeviol
		//+ w[5]*exride + w[6]*wt + w[7]*ttime);
		
		return (
		w[1]*routedur - 
		w[3]*exride +  				
		w[4]*wt + 
		w[5]*ridetimeviol -
		w[6]*twviol -
		w[7]*ttime
		);
		
	}
	
	float cost(int nnode, int cnode, int k)
	{
		DBG("in cost(%i, %i, %i)",nnode,cnode,k);
		int vn[4]; //next four nodes considered
		
		float totcost = 0;
		float ttime; //travel time
		
		int cc = 0;
		float ertime; //earliest arrival time to node
		
		for(int i=0;i<4;i++)
		{
			vn[i]=-1;
			//N[i] = -1;
		}

		for(int i=0;i<4;i++)
		{
			
			totcost = totcost + movecost(cnode, nnode, k);
			DBG("**********************************totcost=%f", totcost);
			if (nnode>0) 
				visit(nnode);
			vn[i]=nnode;
			ttime = DISTANCES.at(cnode).at(nnode);//d.getDistance(cnode,nnode)/speed;
			ertime = ctime[k] + ttime; //arrival time at nnode
			
			float nnodeLT;
						
			if(nnode>n-1)
			{
				nnodeLT = ORDERS.at(nnode-n+1).doTime - ORDERS.at(nnode-n+1).doLT->time/1000000;
			}
			else
			{
				nnodeLT = ORDERS.at(nnode).puTime - ORDERS.at(nnode).puLT->time/1000000;
			}				
			
			//if(ertime<nnodeLT)
			//	ertime=nnodeLT;
			
			if(ertime<nnodeLT)
				ertime=nnodeLT;
			
			cnode = nnode;
			k++;
			float cnodeST;
			if(cnode>n-1)
				cnodeST = ORDERS.at(cnode-n+1).doTime;
			else
				cnodeST = ORDERS.at(cnode).puTime;
			
			ctime[k] = ertime + cnodeST;
			
			//service at nnode finished
			nnode = closestNode(cnode,k);
			DBG("nnode=%i",nnode);
			DBG("**********************************totcost=%f",totcost);
			if (nnode==-2)
			{			
				cc++;
				if (cc==1)
				{					
					nnode = 0;
					totcost = totcost + movecost(cnode, nnode, k);	
				}
				else break;
			}
			//end if
		}
		//end for
		/*
		for(int i=3;i>=0;i--)
		{
			
			if(vn[i]> 0)
			unvisit(vn[i]);			
		}
		//for ends
		*/
		DBG("**********************************totcost=%f",totcost);
		return totcost;
	}
	
	float getTotalCost()
	{
		float tcost;
		
		DBG("Calculating total cost");
		
		if(ord.size() <= 1)
			return tcost;
		
		float ttime, routeviol=0, serv, routedur=0,twviol=0;
		float rideviol=0,xride=0,wt=0;
		float capacityviol=0;
		
		int cnode, nnode;
		
		/*
		for(int i=0;i < cv.size(); i++)
		{
			cv[i]=-1;
			cs[i]=-1;
			cn[i]=v[i].id;
		}
		*/
		load1[0]=0;
		load2[0]=0;
		load3[0]=0;
		
		cnode = ord[0];
		ctime[0]=retime();
		
		for(int j=0;j < wtime.size(); j++)
			wtime[j]=0;
			
		//update ctime, wtime, load for the route found
		for(int i=1;i < ord.size(); i++)
		{			
			nnode = ord.at(i);
			DBG("nnode=%i",nnode);
			
			order_t order;
			float nnodeST, nnodeLT;
			
			if(nnode > n-1)
			{
				order = ORDERS[nnode - (n-1)];
				nnodeST = order.doTime;
				nnodeLT = nnodeST - order.doLT->time/1000000;
				
				load1[i]=load1[i-1]-order.size1;
				load2[i]=load2[i-1]-order.size2;
				load3[i]=load3[i-1]-order.size3;
			}
			else
			{
				order = ORDERS[nnode];
				nnodeST = order.puTime;
				nnodeLT = nnodeST - order.puLT->time/1000000;
				
				load1[i]=load1[i-1]+order.size1;
				load2[i]=load2[i-1]+order.size2;
				load3[i]=load3[i-1]+order.size3;
			}
			
			//load[i]=load[i-1]+order.size;
			
			
			ttime = DISTANCES.at(cnode).at(nnode);
			ctime[i] = ctime[i-1]+ttime+nnodeST+wtime[i];
			DBG(">>>>>>>>>>> ctime[%i]=%f",i,ctime[i]);
			if(ctime[i] < nnodeLT+nnodeST)
			{				
				ctime[i]=nnodeLT+nnodeST;
				wtime[i]=ctime[i]-(ctime[i-1]+ttime+nnodeST);				
			}
			//if ends
			if(i>1 && wtime[i]>0 && (load1[i-1]>load1[i-2] || load2[i-1]>load2[i-2] ||load3[i-1]>load3[i-2] ))
				waiting(i);
			//if(diff&gt;-1) totcost = diff;
			
			//cnode = nnode;
			
			//if(cnode>0)
			//	visit(cnode);
			
			
			DBG("calculate capacity violation for loads %f,%f,%f and capacities %f,%f,%f", load1[i], load2[i], load3[i], car.capacity1,car.capacity2,car.capacity3);
			
			
			if(load1[i] > car.capacity1)
			{
				//raise capacity violation constraint
				capacityviol+=load1[i] - car.capacity1;
			}
			if(load2[i] > car.capacity2)
			{
				//raise capacity violation constraint
				capacityviol+=load2[i] - car.capacity2;
			}
			if(load3[i] > car.capacity3)
			{
				//raise capacity violation constraint
				capacityviol+=load3[i] - car.capacity3;
			}
			
			DBG("capacity violation = %f", capacityviol);			
			
		}
		//for ends
		
		for(int i=1;i < ord.size();i++)
		{
			
			ttime = DISTANCES.at(ord[i-1]).at(ord[i]);
			
			order_t order;
			float nnodeUT, nnodeLT;
			
			if(ord[i] > n-1)
			{
				order = ORDERS[ord[i] - (n-1)];
				serv = order.doTime;
				nnodeLT = serv - order.doLT->time/1000000;
				nnodeUT = serv + order.doUT->time/1000000;
			}
			else
			{
				order = ORDERS[ord[i]];
				serv = order.puTime;
				nnodeLT = serv - order.puLT->time/1000000;
				nnodeUT = serv + order.puUT->time/1000000;
			}
			
			//tw violations calculated,
			if(ctime[i]-serv > nnodeUT)
				twviol=twviol+(ctime[i]-serv)-nnodeUT;
				
			if(ctime[i-1]+ttime<nnodeLT)
				twviol=twviol+nnodeLT-(ctime[i-1]+ttime);
				
			//customers ride time violations caluclated,
			//if customer has been in the car for longer than max
			
			//ride time sais then ride time viol > 0, service times of
			//customer not included, i.e. nnode is a drop off location
			// AND
			//customers excess ride times calculated,
			//ridetime - direct transport time
			if(ord[i]>n-1)
			{
				
				for(int j=1;j<ord.size()-2;j++)
				{				
					if(ord[j]==ord[i]-(n-1))
					{
						
						if(ctime[i]-serv-ctime[j]>MAX_RIDE_TIME)
						{
							
							rideviol= rideviol+ ctime[i]-serv-
							ctime[j]-MAX_RIDE_TIME;
							
						}
						
						xride=xride+ctime[i]-serv-ctime[j]
						-DISTANCES.at(ord[i]).at(ord[j]);
						
					}
					//if
					
				}
				//for
				
			}
			//if
			//waiting time calculated * persons in the vehicle
			wt = wt+(load1[i-1]+load2[i-1]+load3[i-1])*(ctime[i]-serv-(ctime[i-1]+ttime));
			
		}
		
		if(wt < 1E-5)
			wt=0;
		//total route duration
		routedur = ctime[ord.size()-1]-ctime[0];
		
		//route duration violations calculated,
		//if current time is higer than
		//max allowable route duration then route viol becomes > 0
		if(ctime[ord.size()-1]-ctime[0]>MAX_RIDE_TIME)
			routeviol = ctime[ord.size()-1]-ctime[0]-MAX_RIDE_TIME;			
			
		tcost = w.at(1)*routedur + 
				w.at(2)*capacityviol + 
				w.at(3)*xride + 	
				w.at(4)*wt +			
				w.at(5)*routeviol + 
				w.at(6)*twviol + 
				w.at(7)*rideviol +  
				w.at(8)*getDist();
		
		DBG("tcost (%f,%f,%f,%f,%f,%f,%f,%f) = %f",
				w.at(1)*routedur, 
				w.at(2)*capacityviol, 
				w.at(3)*xride, 	
				w.at(4)*wt,			
				w.at(5)*routeviol, 
				w.at(6)*twviol, 
				w.at(7)*rideviol,  
				w.at(8)*getDist(),
				tcost);
		
		return 1/tcost;
	}
	
	//returns total distance between nodes in ord
	float getDist()
	{		
		float dist=0;
		for(int i=0;i<ord.size()-1;i++)
		dist = dist+DISTANCES.at(ord[i]).at(ord[i+1]);
		return(dist);		
	}
	
	//Recalculate starting time so that time windows are not violated
	float retime()
	{
		
		float ttime, ut;
		int id = ord[ord.size()-1];
		
		if(id > n-1)
		{
			ut = ORDERS[id - (n-1)].doTime + ORDERS[id - (n-1)].doUT->time/1000000;
		}
		else
		{
			ut = ORDERS[id].puTime + ORDERS[id].puUT->time/1000000;
		}
		
		if(ctime[ctime.size()-1]>ut)
			ctime[ctime.size()-1]=ut;
						
		
		for(int i=ctime.size()-1;i<0;i--)
		{			
			ttime = DISTANCES.at(ord[i]).at(ord[i-1]);
			
			float st, lt, ut;
			int oid = ord[i-1];
			
			if(id > n-1)
			{
				st = ORDERS[id - (n-1)].doTime;
				lt = st - ORDERS[id - (n-1)].doLT->time/1000000;
				ut = st + ORDERS[id - (n-1)].doUT->time/1000000;
			}
			else
			{
				st = ORDERS[id].puTime;
				lt = st - ORDERS[id].puLT->time/1000000;
				ut = st + ORDERS[id].puUT->time/1000000;
			}
			
			ctime[i-1] = ctime[i]-ttime-st;
			
			if(ctime[i-1] < lt+st)
				ctime[i-1]=	lt+st;
				
			if(ctime[i-1]>ut+st)
			ctime[i-1]=ut+st;
			
		}
		//for ends
		return(ctime[0]);		
	}
	
	//moves waiting times to nodes where there are fewer customers
	//in the vehicle
	void waiting(int no)
	{
		
		float start=wtime[no];
		float diff=0,ut,st;
		
		for(int i=no;i>1;i--)
		{		
			int id = ord[i-1];
		
			if(id > n-1)
			{
				st = ORDERS[id - (n-1)].doTime;
				ut = st + ORDERS[id - (n-1)].doUT->time/1000000;			
			}
			else
			{
				st = ORDERS[id].puTime;
				ut = st + ORDERS[id].puUT->time/1000000;				
			}
			
			diff=ut-ctime[i-1]+st;
			
			//arrival time within tw
			if(load1[i-2]>load1[i-1]||load2[i-2]>load2[i-1]||load3[i-2]>load3[i-1]|| wtime[i]<0.01 || diff<=0)
				break;
			else
			{				
				if(diff>0 && diff<=wtime[i])
				{					
					wtime[i-1]=wtime[i-1]+diff;
					ctime[i-1]=ctime[i-1]+diff;
					wtime[i]=wtime[i]-diff;
					diff=0;					
				}
				
				
				if(diff>0 && diff>wtime[i])
				{					
					wtime[i-1]=wtime[i-1]+wtime[i];
					ctime[i-1]=ctime[i-1]+wtime[i];
					wtime[i]=0;					
				}				
			}
			//else ends		
		}
		//for ends		
	}
	
	void route()
	{
		cun = v.size();
		DBG("%i orders in cluster %i", cun, car.id);
		if(cun == 1) //depot only
		{
			//this car stays at the depot
			DBG("car %i stays at the depot", car.id);
			return;
		}
		
		ctime.resize(2*cun+2); //time at which car has
		                       //finished servicing the nodes,
		wtime.resize(2*cun+2);
		
		int nnode=-1, cnode=0; //newnode and current node
		float ttime=2000000000, mincost;
				
		int sumcs=0; //sum: cs - customers that have been serviced
		int sumcn=0; //sum: cn - customers not in vehicle
		int sumv=0;  //sum: v - all customers numbers in cluster		
		
		//find cust in v with earliest latest pick-up time
		//as first customer in route
		order_t first=ORDERS[0];
		int no=-1;
		double mini=3.3E38f;
		//vector<double> ultw(v.size());		
		//DBG("ultw.size() = %i", ultw.size());
		
		DBG("earliest latest time finding loop");
		//we assume that the depot is always in a cluster and has id=0
		for(int re=1;re<cun;++re)
		{			
			DBG("v[%i] = %i",re, v.at(re).id);
			
			double t = v.at(re).puUT->time/1000000;

			DBG("%f > %f", mini, t);
			if(mini>t)
			{
				mini=t; 
				no=re; 
				first=v.at(re);
				no = re;
			}
		
		}
		
		DBG("earliest latest order = %i", first.id);
		
		//for
		//values set as -1 to indicate no customer for cs and cv, i.e.
		//no customer is in vehicle or has been serviced in the begining,
		//all customers number set into cn, customers not served
		for(int i=0;i<cun*2-1;i++)
		{
			/*
			if(i > (n-1))
			{
				cn.push_back(v[i-(n-1)].id + (n-1));
				DBG("cn.push_back(%i)",v[i-(n-1)].id + (n-1));
			}
			else
			{
				cn.push_back(v[i].id);
				DBG("cn.push_back(%i)",v[i].id);
			}
			*/
			if(i <= (n-1))
				cn.push_back(v[i].id);
				
			cs.push_back(-1);
			cv.push_back(-1);
			//ord.push_back(-1);
			load1.push_back(-1);
			load2.push_back(-1);
			load3.push_back(-1);
		}
		
		//cv.at(no) = first.id; //first customer added to vehcile
		//cn.at(no) = -1;       //first customer deleted from cn
		ord.push_back(0);         //start in depot
		ord.push_back(first.id); //then to first customer
		load1[0] = 0;
		load1[1] = first.size1;	
		
		load2[0] = 0;
		load2[1] = first.size2;
		
		load3[0] = 0;
		load3[1] = first.size3;
		
		//current node depot, next node first customer,
		cnode = 0;
		nnode = first.id;
		
		visit(nnode);
		
		DBG("from 0 to %i",first.id);
		
		//travel time from depot to first customer
		ttime = DISTANCES.at(cnode).at(nnode);
		
		DBG("ttime = %f", ttime);
		
		//first cust has lowest upper pickup tw	
		//float L = first.doLT->time/1000000-DISTANCES.at(first.id).at(first.id+n-1)-MAX_RIDE_TIME;
		
		//DBG("L = %f", L);
		
		//ctime.at(0) = L-ttime+v.at(0).puTime; //ctime[0]
		
		//assume that service time is most desirable
		ctime.at(0) = first.puTime - ttime;
				
		//if(ctime.at(0) < 0)
		//	ctime.at(0)=v.at(0).puTime;
			
		DBG(" ===================== ctime[0]=%f",ctime[0]);	
			
		//ctime.at(1) = ctime.at(0) + ttime + ORDERS.at(nnode).puTime ; //ctime[1]
		ctime.at(1) = first.puTime;
		
		//time after first cust
		//if ctime is lower than lower time window, then wait
		if(ctime.at(1) < ORDERS.at(nnode).puLT->time/1000000 + ORDERS.at(nnode).puTime)
		{			
			ctime.at(1) = ORDERS.at(nnode).puLT->time/1000000 + ORDERS.at(nnode).puTime;
			wtime.at(1) = ctime.at(1) - ctime.at(0) - ttime - ORDERS.at(nnode).puTime;			
		}
		
		//cnode set to first customer
		cnode = nnode;
		
		int k = 1;
		//while not all customers served
		while(unserved() > 0)
		{			
			nnode = closestNode(cnode, k);
			
			if (nnode == -2)
			{				
				break;					
			}
			
			visit(nnode);
			ord.push_back(nnode);
			DBG("================ ord.push_back(%i)",nnode);
			
			ttime = DISTANCES.at(cnode).at(nnode);
			
			//ctime calculation
			
			cnode = nnode;
			++k;
		}

	}
};

Router::Router()
{
	w.resize(9); 
	
	if(pen[0] > 0)
	{
		//default weights for:
		w.at(1) = pen[1]; //driving time violation
		w.at(2) = pen[2]*1000000000; //car capacity violation
		w.at(3) = pen[3]; //excess passanger ride time
		w.at(4) = pen[4]; //passanger waiting time violation
		w.at(5) = pen[5]; //route duration
		w.at(6) = pen[6]*onum; //time window violation
		w.at(7) = pen[7]*onum; //passanger ride time violation
		w.at(8) = pen[8]*onum*1000000; //route time violation
	}
	else
	{
		//default weights for:
		w.at(1) = 8; //driving time violation
		w.at(2) = 1000000000; //car capacity violation
		w.at(3) = 3; //excess passanger ride time
		w.at(4) = 1; //passanger waiting time violation
		w.at(5) = 1; //route duration
		w.at(6) = onum; //time window violation
		w.at(7) = onum; //passanger ride time violation
		w.at(8) = onum*1000000; //route time violation
	}
	
	/*
	w.resize(9); //weights for:
	w.at(1) = 8; //driving time violation
	w.at(2) = 1000000000; //car capacity violation
	w.at(3) = 3; //excess passanger ride time
	w.at(4) = 1; //passanger waiting time violation
	w.at(5) = 1; //route duration
	w.at(6) = onum; //time window violation
	w.at(7) = onum; //passanger ride time violation
	w.at(8) = onum*1000000; //route time violation
	 */
}

boolean darp_score(population *pop, entity *entity)
{
  int           c,k;
  float         dist;
  int           cur_allele, prev_allele;
      
  entity->fitness = 0.0;
  dist = 0.0;
  
  bool depot_check = TRUE;
  int vehicle_check;
  
  char *chromo=NULL;
  size_t chromolen=0;
  
  //assign very low score to really wrong chromosomes
  for (c = 0; c < pop->num_chromosomes; c++)
  {
  	depot_check = depot_check && ((bool *)entity->chromosome[c])[0];
  }
  
  if(!depot_check)
    entity->fitness = -1;
  
  for (k = 1; k < pop->len_chromosomes; k++)
  {
  	for (c = 0; c < pop->num_chromosomes; c++)
  	{
  		vehicle_check += (int) ((bool *)entity->chromosome[c])[k];
	}
	//DBG("%i == %i", vehicle_check, 1);
	if(vehicle_check != 1)
      entity->fitness = -1;
      
    vehicle_check = 0;
  }
 
  //scoring the route of correct chromosomes
  if(entity->fitness >= 0)
  {
    chromo = ga_chromosome_boolean_to_string(pop, entity, chromo, &chromolen);
    DBG("scoring chromosome : %s", chromo);
  
 	//route(pop, entity);
	int c;
	
	ORD = vector<vector< pair<int, double> > > ( pop->num_chromosomes, vector< pair<int, double> > ( 0 ) );
	
    // for each vehicle (cluster)
	for (c = 0; c < pop->num_chromosomes; c++)
	{
		DBG("vehicle # : %i", c);
		Router router;
		DBG("router initialized");
		router.car = VEHICLES.at(c);
		router.n = onum;
		int o;
				
		DBG("clustering");

		for(o=0; o<pop->len_chromosomes;++o)
		{
			//if order is in the cluster
			if(((bool *)entity->chromosome[c])[o])
			{
			  router.v.push_back(ORDERS.at(o));
			  DBG("router.v.push_back(%i)", ORDERS.at(o).id);
			}			
		}
		
		DBG("routing");
		router.route();
		DBG("after routing");
		
		entity->fitness += router.getTotalCost();
		
		for(int i=0;i < router.ord.size(); ++i)
		{			
			DBG("router.ord[%i]=%i",i, router.ord.at(i));
			ORD.at(c).push_back(pair<int,double>( router.ord.at(i), ( i == 0 ? 
																		(router.ctime.at(1) - router.ctime.at(0)) - DISTANCES.at(0).at(router.ord.at(1))
																		//router.ctime.at(0) 
																		: router.ctime.at(i) - router.ctime.at(i-1) 
																	)*1000000 ));
		}
		
		//copy (router.ord.begin(),router.ord.end(),back_inserter(ORD.at(c)));
		
		//entity->fitness += router.getTotalCost();
	} 	
  }  
  
  return TRUE;
}

int
find_darp_solution(int order_num, int vehicle_num,
			vehicle_t *vehicles,
			order_t *orders,
			itinerary_t *path,
			double *dist,
			//point_t depot,
			int depot_id,
			int *penalties,
			float* fit, char** err_msg)

{
  int i,j;
  population    *pop=NULL;              /* Population of solutions. */
  float          score = 0.0;           /* Best score */

  onum=order_num;
  vnum = vehicle_num;
  depot = depot_id;
  
  pen = penalties;
  
  DBG("depot=%i",depot);
  
  for(int o=0; o<order_num;++o)
  {
	  DBG("orders[%i].id=%i",o,orders[o].order_id);
  }
  
  //ORDERS = new order_t[order_num];
  
  //DISTANCE = &dist;
  //ORDERS = orders;
  //VEHICLES = vehicles;
  
  copy ( orders, orders + order_num, back_inserter ( ORDERS ) );
  copy ( vehicles, vehicles + vehicle_num, back_inserter ( VEHICLES ) );
  
  for(int d = 0; d < order_num*2 - 1; ++d)
  {
	  vector<double> row;
	  //ugly
	  for(int dd = 0; dd < (order_num*2-1); ++dd)
	  {
	    *(dist + (order_num * d) + dd);
	    //DBG("dist[%i][%i]=%f",d,dd,*(dist + ((order_num * 2 - 1) * d) + dd));
	    row.push_back(*(dist + ((order_num * 2 - 1) * d) + dd));
	  }
	  
	  DISTANCES.push_back(row);
  }
  
  DBG("GA");
  
  //return 0;
  
  
  random_init();
  
      if (pop) ga_extinction(pop);
      
      //random_seed(ss);
      random_seed(14635242);
      
      pop = ga_genesis_boolean(
                30,  /* const int              population_size */
                vehicle_num, /* const int              num_chromo */
                order_num,   /* const int              len_chromo */
                NULL,   /* GAgeneration_hook      generation_hook */
                NULL,   /* GAiteration_hook       iteration_hook */
                NULL,   /* GAdata_destructor      data_destructor */
                NULL,   /* GAdata_ref_incrementor data_ref_incrementor */
                darp_score,/* GAevaluate           evaluate */
                ga_seed_boolean_random, /* GAseed               seed */
                NULL,     /* GAadapt              adapt */
                ga_select_one_bestof2,/* GAselect_one     select_one */
                ga_select_two_bestof2,/* GAselect_two     select_two */
                ga_mutate_boolean_singlepoint,    /* GAmutate        mutate */
                ga_crossover_boolean_singlepoints, /* GAcrossover     crossover */
                NULL,          /* GAreplace       replace */
                NULL           /* vpointer        User data */
                );

      ga_population_set_parameters(
               pop,                     /* population      *pop */
               GA_SCHEME_DARWIN,        /* const ga_scheme_type     scheme */
               GA_ELITISM_PARENTS_SURVIVE,  /* const ga_elitism_type   elitism */
               0.5,                     /* optimal double  crossover */
               0.05,                     /* optimal double  mutation */
               0.0                      /* unused  double  migration */
               );

      ga_evolution(
              pop,              /* population  *pop */
              MAX_GENERATIONS*order_num*vehicle_num   /* const int   max_generations */
              );

      
      entity* winner;
      if(ga_get_entity_from_rank(pop,0)->fitness > score)
        {
          score = ga_get_entity_from_rank(pop,0)->fitness;
          winner = ga_get_entity_from_rank(pop,0);
          *fit = score;
      
          /*
          for(int l=0; l<cnum; l++)
            { 
              p_ids[l] = ids[
                             ((int *)ga_get_entity_from_rank(pop,0)->
                              chromosome[0])[l]];
            }
          */
        }
        
        size_t chromolen=0;
        char* chromo = NULL;
        char* chr = ga_chromosome_boolean_to_string(pop, winner, chromo, &chromolen);
        
        DBG("Winner is %s", chr);
        DBG("Score = %f", score);
        
        darp_score(pop, winner);
        
        //DBG("ORD size is %i x %i", ORD.size(), ORD.at(0).size());
        
        vector<itinerary_t> res;
        
        for(int i = 0; i < ORD.size(); ++i)
        {
			for(int j = 0; j < ORD.at(i).size(); ++j)
			{
				//DBG(" [%i][%i]>>>>>>>>> %i", i,j,ORD.at(i).at(j));
				//DBG(" [%i][%i]>>>>>>>>>", i,j);
				
				itinerary_t it;
				int oid;
				
				oid = ORD.at(i).at(j).first;
				
				it.id = j;//oid > order_num - 1 ? oid - order_num + 1: oid;//i+j;
				it.vehicle = VEHICLES.at(i).vehicle_id;
				it.order = ORDERS.at(oid > order_num - 1 ? oid - order_num + 1: oid).order_id;//oid > order_num - 1 ? oid - order_num + 1: oid;
				it.time = ORD.at(i).at(j).second;
				DBG("-- %f  --  %f", ORD.at(i).at(j).second, it.time);
				//it.isPickUp = oid <= order_num - 1;
				it.point = oid > order_num - 1 ? ORDERS.at(oid - order_num + 1).to : ORDERS.at(oid).from;
				res.push_back(it);
				
			}
			DBG("--------------");
		}
		//DBG("res.size()=%i",res.size());
		
		//path = (itinerary_t *)palloc(sizeof(itinerary_t)*res.size());	
		
		for(int r = 0; r < res.size(); ++r)
		{
				path[r] = res.at(r);
				//DBG("itinerari[%i]=%i",r,path[r].id);
		}
    
    ga_extinction(pop); 
  
  return res.size();
  //return EXIT_SUCCESS;
}
