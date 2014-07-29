
#ifndef _PDP_H
#define _PDP_H




#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#include "fmgr.h"
#endif

#include <vector>
#include <map>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <search.h>

#include "string.h"
#include "math.h"


//
#ifdef DEBUG
#define DBG(format, arg...)                     \
        elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif

// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 1000

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif


//


using namespace std;


        static int
finish(int code, int ret)
{
        code = SPI_finish();
        if (code  != SPI_OK_FINISH ) {
                elog(ERROR,"couldn't disconnect from SPI");
                return -1 ;
        }

        return ret;
}



// Structures and variables which help us in storing the data

int VehicleCount;
int Capacity;
int Speed;

typedef struct Depot{
        int id;
        int x;
        int y;
        int demand;
        int Etime;
        int Ltime;
        int Stime;
        int Pindex;
        int Dindex;
}depot;



typedef struct Customer{
        int id;
        int x;
        int y;
        int demand;
        int Etime;
        int Ltime;
        int Stime;
        int Pindex;
        int Dindex;
        double Ddist;
        int P;
        int D;
}customer;


typedef struct Pickup{
        int id;
        int Pid;
        double Ddist;
        int Did;
        int checked;
}pickup; 



typedef struct Vehicle{
        int capacity;
        int used_vehicles;
        int given_vehicles;
        int speed;
        int cost;
}VehicleInfo;


// A module which calculates distance 
double CalculateDistance(int x1,int y1,int x2,int y2)
{
        return sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
}


// DEPOT:  With id=0
depot ScanDepot(depot d)
{

        scanf("%d",&d.id);
        scanf("%d",&d.x);
        scanf("%d",&d.y);
        scanf("%d",&d.demand);
        scanf("%d",&d.Etime);
        scanf("%d",&d.Ltime);
        scanf("%d",&d.Stime);
        scanf("%d",&d.Pindex);
        scanf("%d",&d.Dindex);
        return d;
}

static int fetch_distance_columns(SPITupleTable *tuptable, customer *c)
{
        DBG("Customer Data");

        c->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
        c->x = SPI_fnumber(SPI_tuptable->tupdesc, "x");
        c->y = SPI_fnumber(SPI_tuptable->tupdesc, "y");
        c->demand = SPI_fnumber(SPI_tuptable->tupdesc, "demand");
        c->Etime = SPI_fnumber(SPI_tuptable->tupdesc, "Etime");
        c->Ltime = SPI_fnumber(SPI_tuptable->tupdesc, "Ltime");
        c->Stime = SPI_fnumber(SPI_tuptable->tupdesc, "Stime");
        c->Pindex = SPI_fnumber(SPI_tuptable->tupdesc, "Pindex");
        c->Dindex = SPI_fnumber(SPI_tuptable->tupdesc, "Dindex");
        if (c->id == SPI_ERROR_NOATTRIBUTE ||
                        c->x == SPI_ERROR_NOATTRIBUTE ||
                        c->y == SPI_ERROR_NOATTRIBUTE ||
                        c->demand == SPI_ERROR_NOATTRIBUTE ||
                        c->Ltime == SPI_ERROR_NOATTRIBUTE ||
                        c->Stime == SPI_ERROR_NOATTRIBUTE ||
                        c->Pindex == SPI_ERROR_NOATTRIBUTE ||
                        c->Dindex == SPI_ERROR_NOATTRIBUTE ||
                        c->Etime == SPI_ERROR_NOATTRIBUTE)
        {
                elog(ERROR, "Error, query must return columns "
                                "'id', 'x','y','demand', 'Etime',  'Ltime', 'Stime', 'Pindex',  and 'Dindex'");
                return -1;
        }

        return 0;
}




// CUSTOMER: WITH id>=1
customer ScanCustomer(int id,customer c,depot d)
{
        c.id=id;
        scanf("%d",&c.x);
        scanf("%d",&c.y);
        scanf("%d",&c.demand);
        scanf("%d",&c.Etime);
        scanf("%d",&c.Ltime);
        scanf("%d",&c.Stime);
        scanf("%d",&c.Pindex);
        scanf("%d",&c.Dindex);
        if(c.Pindex==0)
        { 
                c.P=1;
                c.D=0;
        }  
        else if(c.Dindex==0)
        {
                c.D=1;
                c.P=0;
        }
        c.Ddist=CalculateDistance(c.x,c.y,d.x,d.y);
        return c;
}


//VEHICLE: First Line contains vehicle data
VehicleInfo ScanVehicle(VehicleInfo Vehicle)
{
        scanf("%d",&Vehicle.given_vehicles);
        scanf("%d",&Vehicle.capacity);
        scanf("%d",&Vehicle.speed);
        Vehicle.used_vehicles=0;
        return Vehicle;

}

//temp variables
int temp=0;

int CustomerLength=0;
int OrderLength=0;


// Part of code: It is used to save some variables and helps if we need to revisit previous state.
typedef struct statesave{
        int twv;
        int cv;
        int dis;
        //       vector<int> path;
        //      vector<int> order;
        int path[1000];
        int order[1000];
        int path_length;
}State;


//SPI code 

static int conn(int *SPIcode)
{
        int res = 0;

        *SPIcode = SPI_connect();

        if (*SPIcode  != SPI_OK_CONNECT)
        {
                elog(ERROR, "vrppdtw: couldn't open a connection to SPI");
                res = -1;
        }

        return res;
}




#endif










