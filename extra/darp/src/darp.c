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

#include "darp.h"

#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"

#include "string.h"
#include "math.h"

#include "fmgr.h"

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#undef qsort

// ------------------------------------------------------------------------

/*
 * Define this to have profiling enabled
 */
//#define PROFILE

#ifdef PROFILE
#include <sys/time.h>

struct timeval prof_darp, prof_store, prof_extract, prof_total;
long proftime[5];
long profipts1, profipts2, profopts;
#define profstart(x) do { gettimeofday(&x, NULL); } while (0);
#define profstop(n, x) do { struct timeval _profstop;   \
        long _proftime;                         \
        gettimeofday(&_profstop, NULL);                         \
        _proftime = ( _profstop.tv_sec*1000000+_profstop.tv_usec) -     \
                ( x.tv_sec*1000000+x.tv_usec); \
        elog(NOTICE, \
                "PRF(%s) %lu (%f ms)", \
                (n), \
             _proftime, _proftime / 1000.0);    \
        } while (0);

#else

#define profstart(x) do { } while (0);
#define profstop(n, x) do { } while (0);

#endif // PROFILE

// ------------------------------------------------------------------------

Datum darp(PG_FUNCTION_ARGS);

#undef DEBUG
//#define DEBUG 1

#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif

// The number of tuples to fetch from the SPI cursor at each iteration
#define TUPLIMIT 10000

typedef struct vehicle_columns
{
  int id;
  int vehicle_id;
  int capacity1;
  int capacity2;
  int capacity3;
} vehicle_columns_t;

typedef struct order_columns
{
  //int id;
  int order_id;
  int puTime;
  int puUT;
  int puLT;
  int doTime;
  int doUT;
  int doLT;
  int size1;
  int size2;
  int size3;
  int from;
  int to;
} order_columns_t;

typedef struct distance_columns
{
  int from_order;
  int to_order;
  int from_point;
  int to_point;
  int value;
} distance_columns_t;

typedef struct penalty_columns
{
  int w1;
  int w2;
  int w3;
  int w4;
  int w5;
  int w6;
  int w7;
  int w8;
} penalty_columns_t;

//float DISTANCE[MAX_TOWNS][MAX_TOWNS];
//float x[MAX_TOWNS],y[MAX_TOWNS];
int total_tuples;
int order_num, vehicle_num;

int 
order_cmp (const void *c1, const void *c2)
{
  order_t *o1 = (order_t*)c1;
  order_t *o2 = (order_t*)c2;
  return o1->order_id - o2->order_id;
}

int 
order_cmp_asc (const void *c1, const void *c2)
{
  order_t *o1 = (order_t*)c1;
  order_t *o2 = (order_t*)c2;
  return o2->order_id - o1->order_id;
}

int find_order(int order_id, order_t *orders, int count)
{
	int id = -1;
	order_t target, *result;
	target.order_id = order_id;
	
	result = bsearch(&target, orders, count, sizeof (order_t),
                    order_cmp);
    if (result)
      id = result->id;
      
    return id;
}

static char *
text2char(text *in)
{
  char *out = (char*)palloc(VARSIZE(in));

  memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
  out[VARSIZE(in) - VARHDRSZ] = '\0';
  return out;
}

static int
finish(int *code)
{
  *code = SPI_finish();
  if (*code  != SPI_OK_FINISH )
  {
    elog(ERROR,"couldn't disconnect from SPI");
    return -1 ;
  }
  return 0;
}

static int
fetch_penalties_columns(SPITupleTable *tuptable, penalty_columns_t *penalty_columns)
{
  DBG("Fetching penalties");

  penalty_columns->w1 = SPI_fnumber(SPI_tuptable->tupdesc, "w1");
  penalty_columns->w2 = SPI_fnumber(SPI_tuptable->tupdesc, "w2");
  penalty_columns->w3 = SPI_fnumber(SPI_tuptable->tupdesc, "w3");
  penalty_columns->w4 = SPI_fnumber(SPI_tuptable->tupdesc, "w4");
  penalty_columns->w5 = SPI_fnumber(SPI_tuptable->tupdesc, "w5");
  penalty_columns->w6 = SPI_fnumber(SPI_tuptable->tupdesc, "w6");
  penalty_columns->w7 = SPI_fnumber(SPI_tuptable->tupdesc, "w7");
  penalty_columns->w8 = SPI_fnumber(SPI_tuptable->tupdesc, "w8");

  if (penalty_columns->w1 == SPI_ERROR_NOATTRIBUTE ||
      penalty_columns->w2 == SPI_ERROR_NOATTRIBUTE ||
      penalty_columns->w3 == SPI_ERROR_NOATTRIBUTE ||
      penalty_columns->w4 == SPI_ERROR_NOATTRIBUTE ||
      penalty_columns->w5 == SPI_ERROR_NOATTRIBUTE ||
      penalty_columns->w6 == SPI_ERROR_NOATTRIBUTE ||
      penalty_columns->w7 == SPI_ERROR_NOATTRIBUTE ||
      penalty_columns->w8 == SPI_ERROR_NOATTRIBUTE) 
    {
      elog(ERROR, "Error, query must return columns "
           "'w1' through 'w8'");
      return -1;
    }
	
  return 0;
}

static void
fetch_penalties(HeapTuple *tuple, TupleDesc *tupdesc,
				penalty_columns_t *penalty_columns, int *penalties)
{
  Datum binval;
  bool isnull; 

  binval = SPI_getbinval(*tuple, *tupdesc, penalty_columns->w1, &isnull);
  if (isnull)
    penalties[1] = -1;
  else
    penalties[1] = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, penalty_columns->w2, &isnull);
  if (isnull)
    penalties[2] = -1;
  else
    penalties[2] = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, penalty_columns->w3, &isnull);
  if (isnull)
    penalties[3] = -1;
  else
    penalties[3] = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, penalty_columns->w4, &isnull);
  if (isnull)
    penalties[4] = -1;
  else
    penalties[4] = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, penalty_columns->w5, &isnull);
  if (isnull)
    penalties[5] = -1;
  else
    penalties[5] = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, penalty_columns->w6, &isnull);
  if (isnull)
    penalties[6] = -1;
  else
    penalties[6] = DatumGetInt32(binval);
    
  binval = SPI_getbinval(*tuple, *tupdesc, penalty_columns->w7, &isnull);
  if (isnull)
    penalties[7] = -1;
  else
    penalties[7] = DatumGetInt32(binval);

  binval = SPI_getbinval(*tuple, *tupdesc, penalty_columns->w8, &isnull);
  if (isnull)
    penalties[8] = -1;
  else
    penalties[8] = DatumGetInt32(binval); 
    
    DBG("PENALTIES: %i, %i, %i, %i, %i, %i, %i, %i", penalties[1], penalties[2], penalties[3], penalties[4], penalties[5], penalties[6], penalties[7], penalties[8]);   

}


static int
fetch_distance_columns(SPITupleTable *tuptable, distance_columns_t *distance_columns)
{
  DBG("Fetching distance");

  distance_columns->from_order = SPI_fnumber(SPI_tuptable->tupdesc, "from_order");
  distance_columns->to_order = SPI_fnumber(SPI_tuptable->tupdesc, "to_order");
  distance_columns->from_point = SPI_fnumber(SPI_tuptable->tupdesc, "from_point");
  distance_columns->to_point = SPI_fnumber(SPI_tuptable->tupdesc, "to_point");  
  distance_columns->value = SPI_fnumber(SPI_tuptable->tupdesc, "value");
  if (distance_columns->from_order == SPI_ERROR_NOATTRIBUTE ||
      distance_columns->to_order == SPI_ERROR_NOATTRIBUTE ||
      distance_columns->from_point == SPI_ERROR_NOATTRIBUTE ||
      distance_columns->to_point == SPI_ERROR_NOATTRIBUTE ||
      distance_columns->value == SPI_ERROR_NOATTRIBUTE) 
    {
      elog(ERROR, "Error, query must return columns "
           "'from_order', 'to_order', 'from_point', 'to_point' and 'value'");
      return -1;
    }

  return 0;
}


static void
fetch_distance(HeapTuple *tuple, TupleDesc *tupdesc, 
            distance_columns_t *distance_columns, double *dist, int num_rows, order_t *orders)
{
  Datum binval;
  bool isnull;
  
  int from_order, to_order;
  int from_point, to_point;
  float value;
  
  //DBG("Inside distance fetched\n");

  binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->from_order, &isnull);

  if (isnull)
    elog(ERROR, "from_order contains a null value");

  from_order = DatumGetInt32(binval);
  
  //DBG("from_order=%i",from_order);

  binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->to_order, &isnull);
  if (isnull)
    elog(ERROR, "to_order contains a null value");

  to_order = DatumGetInt32(binval);

  //DBG("to_order=%i",to_order);
  
  binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->from_point, &isnull);

  if (isnull)
    elog(ERROR, "from_point contains a null value");

  from_point = DatumGetInt32(binval);

  //DBG("from_point=%i",from_point);

  binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->to_point, &isnull);
  if (isnull)
    elog(ERROR, "to_point contains a null value");

  to_point = DatumGetInt32(binval);
  
  //DBG("to_point=%i",to_point);

  binval = SPI_getbinval(*tuple, *tupdesc, distance_columns->value, &isnull);

  if (isnull)
    elog(ERROR, "value contains a null value");

  value = DatumGetFloat8(binval);
  
  //DBG("value=%f",value);
  
  //DBG("dist[%i][%i] = %f\n", from_point, to_point, value);
    
  int from = find_order(from_order, orders, order_num+1);
  
  DBG("found ford %i for %i", from, from_order);
  
  order_t ford = orders[from];
  
  if(from_order!= 0 && from_point == ford.to)
  {
	  from += order_num;
  }
  
  int to = find_order(to_order, orders, order_num+1);
  
  DBG("found tord %i for %i", to, to_order);
  
  order_t tord = orders[to];

  if(to_order!= 0 && to_point == tord.to)
  {
	  to += order_num;
  }
    
  //dist[from][to] = value;
  if(from > 0 && to > 0)
	*(dist + (num_rows * from) + to) = value;
  
  DBG("dist[%i(%i:%i)][%i(%i:%i)] = %f\n", from, from_order, from_point, to, to_order, to_point, *(dist + (num_rows * from) + to));

}

static int
fetch_order_columns(SPITupleTable *tuptable, order_columns_t *order_columns)
{
  DBG("Fetching order");

  //order_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
  order_columns->order_id = SPI_fnumber(SPI_tuptable->tupdesc, "order_id");
  order_columns->puTime = SPI_fnumber(SPI_tuptable->tupdesc, "pu_time");
  order_columns->doTime = SPI_fnumber(SPI_tuptable->tupdesc, "do_time");
  order_columns->puLT = SPI_fnumber(SPI_tuptable->tupdesc, "pu_lt");
  order_columns->doLT = SPI_fnumber(SPI_tuptable->tupdesc, "do_lt");
  order_columns->puUT = SPI_fnumber(SPI_tuptable->tupdesc, "pu_ut");
  order_columns->doUT = SPI_fnumber(SPI_tuptable->tupdesc, "do_ut");  
  order_columns->from = SPI_fnumber(SPI_tuptable->tupdesc, "from_point");
  order_columns->to = SPI_fnumber(SPI_tuptable->tupdesc, "to_point");
  order_columns->size1 = SPI_fnumber(SPI_tuptable->tupdesc, "size1");
  order_columns->size2 = SPI_fnumber(SPI_tuptable->tupdesc, "size2");
  order_columns->size3 = SPI_fnumber(SPI_tuptable->tupdesc, "size3");
  if (//order_columns->id == SPI_ERROR_NOATTRIBUTE ||
      order_columns->order_id == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->puTime == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->doTime == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->puLT == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->doLT == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->puUT == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->doUT == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->from == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->to == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->size1 == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->size2 == SPI_ERROR_NOATTRIBUTE ||
	  order_columns->size3 == SPI_ERROR_NOATTRIBUTE
  )
    {
//      elog(ERROR, "Error, query must return columns "
//           "'id', 'order_id', 'pu_time', 'do_time', 'pu_time_window', 'do_time_window', 'from_x', 'to_x', 'from_y', 'to_y' and 'size'");
      elog(ERROR, "Error, query must return columns "
           //"'id', 
           "'order_id', 'pu_lt', 'do_lt', 'pu_ut', 'do_ut', 'from_point', 'to_point', 'size1', 'size2' and 'size3'");
       return -1;
    }

  return 0;
}

static void
fetch_order(HeapTuple *tuple, TupleDesc *tupdesc,
            order_columns_t *order_columns, order_t *order, int t)
{
  Datum binval;
  bool isnull;

  DBG("inside fetch_order\n");

  //binval = SPI_getbinval(*tuple, *tupdesc, order_columns->id, &isnull);
  //
  //  DBG("got binval\n");
  //
  //if (isnull)
  //  elog(ERROR, "id contains a null value");
  //
  //order->id = DatumGetInt32(binval);
  order->id = t+1;

  DBG("id = %i\n", order->id);
  
  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->order_id, &isnull);
  if (isnull)
    elog(ERROR, "order_id contains a null value");

  order->order_id = DatumGetInt32(binval);

  DBG("order_id = %i\n", order->order_id);  


  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->puTime, &isnull);
  if (isnull)
    elog(ERROR, "puTime contains a null value");

  order->puTime = DatumGetTimestamp(binval)/1000000;

  DBG("puTime = %i\n", order->puTime);

  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->doTime, &isnull);
  if (isnull)
    elog(ERROR, "doTime contains a null value");

  order->doTime = DatumGetTimestamp(binval)/1000000;

  DBG("doTime = %i\n", order->doTime);

  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->puLT, &isnull);
  if (isnull)
    elog(ERROR, "puLT contains a null value");

  order->puLT = DatumGetIntervalP(binval);

  DBG("puLT = %Ld\n", order->puLT->time);

  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->doLT, &isnull);
  if (isnull)
    elog(ERROR, "doLT contains a null value");

  order->doLT = DatumGetIntervalP(binval);

  DBG("doLT = %Ld\n", order->doLT->time);
  
  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->puUT, &isnull);
  if (isnull)
    elog(ERROR, "puUT contains a null value");

  order->puUT = DatumGetIntervalP(binval);

  DBG("puUT = %Ld\n", order->puUT->time);

  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->doUT, &isnull);
  if (isnull)
    elog(ERROR, "doUT contains a null value");

  order->doUT = DatumGetIntervalP(binval);

  DBG("doUT = %Ld\n", order->doUT->time);

  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->from, &isnull);
  if (isnull)
    elog(ERROR, "from contains a null value");

  order->from = DatumGetInt32(binval);

  DBG("from = %i\n", order->from);

  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->to, &isnull);
  if (isnull)
    elog(ERROR, "to contains a null value");

  order->to = DatumGetInt32(binval);

  DBG("to = %i\n", order->to);


  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->size1, &isnull);

  if (isnull)
    elog(ERROR, "size1 contains a null value");

  order->size1 = DatumGetFloat8(binval);

  DBG("size1 = %f\n", order->size1);
  
  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->size2, &isnull);

  if (isnull)
    elog(ERROR, "size2 contains a null value");

  order->size2 = DatumGetFloat8(binval);

  DBG("size2 = %f\n", order->size2);
  
  binval = SPI_getbinval(*tuple, *tupdesc, order_columns->size3, &isnull);

  if (isnull)
    elog(ERROR, "size3 contains a null value");

  order->size3 = DatumGetFloat8(binval);

  DBG("size3 = %f\n", order->size3);
}

static int
fetch_vehicle_columns(SPITupleTable *tuptable, vehicle_columns_t *vehicle_columns)
{
  DBG("Fetching order");

  vehicle_columns->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
  vehicle_columns->vehicle_id = SPI_fnumber(SPI_tuptable->tupdesc, "vehicle_id");
  vehicle_columns->capacity1 = SPI_fnumber(SPI_tuptable->tupdesc, "capacity1");
  vehicle_columns->capacity2 = SPI_fnumber(SPI_tuptable->tupdesc, "capacity2");
  vehicle_columns->capacity3 = SPI_fnumber(SPI_tuptable->tupdesc, "capacity3");
  if (vehicle_columns->id == SPI_ERROR_NOATTRIBUTE ||
		  vehicle_columns->capacity1 == SPI_ERROR_NOATTRIBUTE ||
		  vehicle_columns->capacity2 == SPI_ERROR_NOATTRIBUTE ||
		  vehicle_columns->capacity3 == SPI_ERROR_NOATTRIBUTE
  )
    {
      elog(ERROR, "Error, query must return columns "
           "'id', 'capacity1', 'capacity2' and 'capacity3'");
      return -1;
    }

  return 0;
}

static void
fetch_vehicle(HeapTuple *tuple, TupleDesc *tupdesc,
		vehicle_columns_t *vehicle_columns, vehicle_t *vehicle, int t)
{
  Datum binval;
  bool isnull;

  //DBG("inside fetch_vehicle\n");

  //binval = SPI_getbinval(*tuple, *tupdesc, vehicle_columns->id, &isnull);
  //DBG("Got id\n");
  //
  //if (isnull)
  //  elog(ERROR, "id contains a null value");
  //
  //vehicle->id = DatumGetInt32(binval);
  
  vehicle->id = t;

  //DBG("id = %i\n", vehicle->id);
  
  binval = SPI_getbinval(*tuple, *tupdesc, vehicle_columns->vehicle_id, &isnull);
  //DBG("Got vehicle_id\n");

  if (isnull)
    elog(ERROR, "vehicle_id contains a null value");

  vehicle->vehicle_id = DatumGetInt32(binval);

  //DBG("vehicle_id = %i\n", vehicle->vehicle_id);

  binval = SPI_getbinval(*tuple, *tupdesc, vehicle_columns->capacity1, &isnull);
  if (isnull)
    elog(ERROR, "capacity1 contains a null value");

  vehicle->capacity1 = DatumGetFloat8(binval);

  //DBG("capacity1 = %f\n", vehicle->capacity1);

  binval = SPI_getbinval(*tuple, *tupdesc, vehicle_columns->capacity2, &isnull);
  if (isnull)
    elog(ERROR, "capacity2 contains a null value");

  vehicle->capacity2 = DatumGetFloat8(binval);

  //DBG("capacity2 = %f\n", vehicle->capacity2);
  
  binval = SPI_getbinval(*tuple, *tupdesc, vehicle_columns->capacity3, &isnull);
  if (isnull)
    elog(ERROR, "capacity3 contains a null value");

  vehicle->capacity3 = DatumGetFloat8(binval);

  //DBG("capacity3 = %f\n", vehicle->capacity3);  
}

static int conn(int *SPIcode)
{
	int res = 0;

	*SPIcode = SPI_connect();

	if (*SPIcode  != SPI_OK_CONNECT)
	{
	  elog(ERROR, "darp: couldn't open a connection to SPI");
	  res = -1;
	}
	
	return res;	
}

static int prepare_query(Portal *SPIportal, char* sql)
{
	int res = 0;

	void* SPIplan = SPI_prepare(sql, 0, NULL);

	if (SPIplan  == NULL)
	{
	  elog(ERROR, "darp: couldn't create query plan via SPI");
	  res = -1;
	}

	if ((*SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL)
	{
	  elog(ERROR, "darp: SPI_cursor_open('%s') returns NULL", sql);
	  res = -1;
	}

	return res;
}

static int solve_darp(char* orders_sql, char* vehicles_sql,
                     char* dist_sql,
                     int depot,
                     int depot_point,
                     char* penalties_sql,
                     itinerary_t* path)
{
  int SPIcode;
  
  Portal SPIportal_o; 
  Portal SPIportal_v; 
  Portal SPIportal_d; 
  Portal SPIportal_p;
  
  bool moredata = TRUE;
  int ntuples;

  vehicle_t *vehicles=NULL;
  vehicle_columns_t vehicle_columns = {id: -1, capacity1: -1, capacity2: -1, capacity3: -1};

  order_t *orders=NULL;
  order_columns_t order_columns = {
		  order_id: -1, puLT: -1, doLT:-1,  puUT: -1, doUT:-1, size1:-1, size2:-1, size3:-1, from:-1, to:-1};
		  
  distance_columns_t distance_columns = {from_order: -1, from_point: -1, to_order: -1, to_point: -1, value: -1};
  
  int penalties[9];
  penalty_columns_t penalty_columns = {w1: -1, w2: -1, w3: -1, w4: -1, w5: -1, w6: -1, w7: -1, w8: -1};
  
  char *err_msg = NULL;
  int ret = -1;
    
  int   z = 0;
  
  int    tt, cc;
  double dx, dy;
  float  fit=0.0;
  
  int prep = -1, con = -1;

  DBG("inside darp\n");

  //int total_tuples = 0;
  total_tuples = 0;
  order_num = 0;
  vehicle_num = 0;

  DBG("start darp\n");
  
  order_t depot_ord = {id:0, order_id:depot, from:depot_point, to:depot_point};
  orders = palloc(1 * sizeof(order_t));
  orders[0] = depot_ord;
  
  con = conn(&SPIcode);
  
    if (con < 0)
	  return ret;
	  
	    
  // Fetching orders
  
  prep = prepare_query(&SPIportal_o, orders_sql);

  if (prep < 0)
	  return ret;
    
  DBG("Query: %s\n",orders_sql);
  DBG("Query executed\n");
  
        DBG("Orders before: %i\n", order_num);
  
  while (moredata == TRUE)  
    {
      SPI_cursor_fetch(SPIportal_o, TRUE, TUPLIMIT);
      
        DBG("cursor fetched\n");

      if (order_columns.order_id == -1)
        {
          if (fetch_order_columns(SPI_tuptable, &order_columns) == -1)
        	  return finish(&SPIcode);
        }

      ntuples = SPI_processed;

      order_num += ntuples;

      DBG("Tuples: %i\n", order_num);

      //if (!orders)
    	//  orders = palloc(order_num * sizeof(order_t));
      //else
    	  orders = repalloc(orders, (order_num + 1) * sizeof(order_t));

      if (orders == NULL)
        {
          elog(ERROR, "Out of memory");
	  return finish(&SPIcode);
        }

      if (ntuples > 0)
        {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          DBG("Got tuple desc\n");

          for (t = 0; t < ntuples; t++)
            {
              HeapTuple tuple = tuptable->vals[t];
              //DBG("Before order fetched [%i]\n", order_num - ntuples + t);
              fetch_order(&tuple, &tupdesc, &order_columns,
                          &orders[order_num - ntuples + t+1], t);
                          //&orders[t+1], t);
              DBG("Order fetched\n");
            }

          SPI_freetuptable(tuptable);
        }
      else
        {
          moredata = FALSE;
        }
    }// end of fetching orders
    //finish(&SPIcode_o);
  
  DBG ("order_num = %i", order_num); 
    
  qsort (orders, order_num+1, sizeof (order_t), order_cmp);

  int o;
  for(o=0; o<order_num+1;++o)
  {
	  DBG("ORDERS[%i] = {id=%i,order_id=%i,from=%i,to=%i}",o,orders[o].id,orders[o].order_id,orders[o].from,orders[o].to);
  }
    
  // Fetching vehicles
  
  moredata = TRUE;
  prep = prepare_query(&SPIportal_v, vehicles_sql);

  if (prep < 0)
	  return ret;
	  
  DBG("Query: %s\n",vehicles_sql);
  DBG("Query executed\n");

  	  
  while (moredata == TRUE)
    {
      SPI_cursor_fetch(SPIportal_v, TRUE, TUPLIMIT);
  
      if (vehicle_columns.id == -1)
        {
          if (fetch_vehicle_columns(SPI_tuptable, &vehicle_columns) == -1)
        	  return finish(&SPIcode);
        }
        

      ntuples = SPI_processed;

      vehicle_num += ntuples;

      DBG("Tuples: %i\n", vehicle_num);

      if (!vehicles)
    	  vehicles = palloc(vehicle_num * sizeof(vehicle_t));
      else
    	  vehicles = repalloc(vehicles, vehicle_num * sizeof(vehicle_t));

      if (vehicles == NULL)
        {
          elog(ERROR, "Out of memory");
	  return finish(&SPIcode);
        }

      if (ntuples > 0)
        {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          DBG("Got tuple desc\n");

          for (t = 0; t < ntuples; t++)
            {
              HeapTuple tuple = tuptable->vals[t];
              //DBG("Before vehicle fetched\n");
              fetch_vehicle(&tuple, &tupdesc, &vehicle_columns,
                          &vehicles[vehicle_num - ntuples + t], t);
              //DBG("Vehicle fetched\n");
            }

          SPI_freetuptable(tuptable);
        }
      else
        {
          moredata = FALSE;
        }
    }// end of fetching vehicles
    //finish(&SPIcode_v);
    
  double dist[order_num*2+1][order_num*2+1];

  // Fetching distances
  
  moredata = TRUE;
  prep = prepare_query(&SPIportal_d, dist_sql);

  if (prep < 0)
	  return ret;
	  
  DBG("Query: %s\n",dist_sql);
  DBG("Query executed\n");
  
  while (moredata == TRUE)
    {
      SPI_cursor_fetch(SPIportal_d, TRUE, TUPLIMIT);

      if (distance_columns.value == -1)
        {
          if (fetch_distance_columns(SPI_tuptable, &distance_columns) == -1)
        	  return finish(&SPIcode);
        }

      ntuples = SPI_processed;

      if (ntuples > 0)
        {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          DBG("Got tuple desc\n");

          for (t = 0; t < ntuples; t++)
            {
              HeapTuple tuple = tuptable->vals[t];
              //DBG("Before distance fetched\n");
              fetch_distance(&tuple, &tupdesc, &distance_columns,
                          &dist[0][0], order_num *2-1, &orders[0]);
            }

          SPI_freetuptable(tuptable);
        }
      else
        {
          moredata = FALSE;
        }
    }// end of fetching distances
    
    		
  // Fetching penalties
  
  moredata = TRUE;
  prep = prepare_query(&SPIportal_p, penalties_sql);

  if (prep < 0)
	  return ret;
	  
  DBG("Query: %s\n",penalties_sql);
  DBG("Query executed\n");

  	  
  while (moredata == TRUE)
    {
      SPI_cursor_fetch(SPIportal_p, TRUE, TUPLIMIT);
  
      if (penalty_columns.w1 == -1)
        {
          if (fetch_penalties_columns(SPI_tuptable, &penalty_columns) == -1)
        	  return finish(&SPIcode);
        }
        

      ntuples = SPI_processed;

      DBG("Tuples: %i\n", ntuples);

      if (ntuples > 0)
        {
          int t;
          SPITupleTable *tuptable = SPI_tuptable;
          TupleDesc tupdesc = SPI_tuptable->tupdesc;

          DBG("Got tuple desc\n");

          for (t = 0; t < ntuples; t++)
            {
              HeapTuple tuple = tuptable->vals[t];
              //DBG("Before penalties fetched\n");
              fetch_penalties(&tuple, &tupdesc, &penalty_columns,
                          penalties);
              //DBG("Penalties fetched\n");
            }

          SPI_freetuptable(tuptable);
        }
      else
        {
          moredata = FALSE;
        }
    }// end of fetching penalties
		
		    

  DBG("Calling DARP\n");
        
  profstop("extract", prof_extract);
  profstart(prof_darp);

  DBG("Total orders: %i\n", order_num);
  DBG("Total vehicles: %i\n", vehicle_num);

  
  //qsort (orders, order_num+1, sizeof (order_t), order_cmp_asc);


  //int o;
  //for(o=0; o<order_num+1;++o)
  //{
//	  DBG("ORDERS[%i] = {id=%i,order_id=%i,from=%i,to=%i}",o,orders[o].id,orders[o].order_id,orders[o].from,orders[o].to);
  //}
  
  //return 0;

  
  //itinerary = (itinerary_t *)palloc(sizeof(itinerary_t)*(order_num*2-1)*vehicle_num);
  
  DBG("Calling DARP solver\n");
  
  ret = find_darp_solution(order_num+1, vehicle_num,
			    vehicles,
			    orders,
			    path,
			    &dist[0][0], //2D array containing pre-calculated distances
			    depot,
			    penalties,
			    &fit, &err_msg);
    
  
  total_tuples = ret;
  
  DBG("DARP solved!\n");
  DBG("Score: %f\n", fit);

  profstop("darp", prof_darp);
  profstart(prof_store);

  DBG("Profile changed and ret is %i", ret);

  if (ret < 0)
    {
      //elog(ERROR, "Error computing path: %s", err_msg);
      ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), errmsg("Error computing path: %s", err_msg)));
    } 
    
  //pfree(vehicles);
  //pfree(orders);
  finish(&SPIcode);    
}

PG_FUNCTION_INFO_V1(darp);
Datum
darp(PG_FUNCTION_ARGS)
{
  FuncCallContext     *funcctx;
  int                  call_cntr;
  int                  max_calls;
  TupleDesc            tuple_desc;
  itinerary_t         *path;
    
  /* stuff done only on the first call of the function */
  if (SRF_IS_FIRSTCALL())
    {
      MemoryContext   oldcontext;
      //int path_count;
      int ret=-1;

      // XXX profiling messages are not thread safe
      profstart(prof_total);
      profstart(prof_extract);

      /* create a function context for cross-call persistence */
      funcctx = SRF_FIRSTCALL_INIT();

      /* switch to memory context appropriate for multiple function calls */
      oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);
        

      path = (itinerary_t *)palloc(sizeof(itinerary_t)*(MAX_ORDERS-1)*2*MAX_VEHICLES);


      ret = solve_darp(//text2char(PG_GETARG_TEXT_P(0)), // points sql
                      text2char(PG_GETARG_TEXT_P(0)),  // orders sql
                      text2char(PG_GETARG_TEXT_P(1)),  // vehicles sql
                      text2char(PG_GETARG_TEXT_P(2)),  // distances query
                      PG_GETARG_INT32(3),  // depot
                      PG_GETARG_INT32(4),  // depot_point
                      text2char(PG_GETARG_TEXT_P(5)),  // penalties query
                      path);

      /* total number of tuples to be returned */
      //DBG("Counting tuples number\n");

      funcctx->max_calls = total_tuples;

      funcctx->user_fctx = path;
  
      funcctx->tuple_desc = BlessTupleDesc(
                              RelationNameGetTupleDesc("itinerary"));
      MemoryContextSwitchTo(oldcontext);
    }

  /* stuff done on every call of the function */
  funcctx = SRF_PERCALL_SETUP();

  call_cntr = funcctx->call_cntr;
  max_calls = funcctx->max_calls;
  tuple_desc = funcctx->tuple_desc;

  path = (itinerary_t *)funcctx->user_fctx;
  
  //DBG("Trying to allocate some memory\n");
  //DBG("call_cntr = %i, max_calls = %i\n", call_cntr, max_calls);

  if (call_cntr < max_calls)    /* do when there is more left to send */
    {
      HeapTuple    tuple;
      Datum        result;
      Datum *values;
      char* nulls;
   
      values = palloc(5 * sizeof(Datum));
      nulls = palloc(5 * sizeof(char));

      values[0] = Int32GetDatum(path[call_cntr].id);
      nulls[0] = ' ';
      values[1] = Int32GetDatum(path[call_cntr].order);
      nulls[1] = ' ';
      values[2] = Int32GetDatum(path[call_cntr].vehicle);
      nulls[2] = ' ';
      values[3] = Int32GetDatum(path[call_cntr].point);
      nulls[3] = ' ';
      //values[4] = TimeTzADTPGetDatum(&path[call_cntr].time);
      values[4] = TimestampGetDatum(path[call_cntr].time);
      nulls[4] = ' ';
      
     // DBG("Heap making\n");

      tuple = heap_formtuple(tuple_desc, values, nulls);

      //DBG("Datum making\n");

      /* make the tuple into a datum */
      result = HeapTupleGetDatum(tuple);

      //DBG("Trying to free some memory\n");
    
      /* clean up */
      pfree(values);
      pfree(nulls);
        

      SRF_RETURN_NEXT(funcctx, result);
    }
  else    /* do when there is no more left */
    {
      DBG("Ending function\n");
      profstop("store", prof_store);
      profstop("total", prof_total);
      DBG("Profiles stopped\n");

      pfree(path);

      DBG("Itinerary cleared\n");

      SRF_RETURN_DONE(funcctx);
    }
}
