#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"
#include "./pdp.h"


Datum vrppdtw(PG_FUNCTION_ARGS);


//#undef DEBUG
#define DEBUG 1

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

/*
    * Define this to have profiling enabled
     */
//#define PROFILE

#ifdef PROFILE
#include <sys/time.h>

struct timeval prof_astar, prof_store, prof_extract, prof_total;
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





typedef struct Customer_type{
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
}customer_t;


static char *
text2char(text *in)
{
        char *out = (char*)palloc(VARSIZE(in));

        memcpy(out, VARDATA(in), VARSIZE(in) - VARHDRSZ);
        out[VARSIZE(in) - VARHDRSZ] = '\0';
        return out;
}

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

static int prepare_query(Portal *SPIportal, char* sql)
{
        int res = 0;

        void* SPIplan = SPI_prepare(sql, 0, NULL);

        if (SPIplan  == NULL)
        {
                elog(ERROR, "vrppdtw: couldn't create query plan via SPI");
                res = -1;
        }

        if ((*SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL)
        {
                elog(ERROR, "vrppdtw: SPI_cursor_open('%s') returns NULL", sql);
                res = -1;
        }

        return res;
}


static int fetch_customer_columns(SPITupleTable *tuptable, customer_t *c)
{
        DBG("Customer Data");

        c->id = SPI_fnumber(SPI_tuptable->tupdesc, "id");
        c->x = SPI_fnumber(SPI_tuptable->tupdesc, "x");
        c->y = SPI_fnumber(SPI_tuptable->tupdesc, "y");
        c->demand = SPI_fnumber(SPI_tuptable->tupdesc, "demand");
        c->Etime = SPI_fnumber(SPI_tuptable->tupdesc, "etime");
        c->Ltime = SPI_fnumber(SPI_tuptable->tupdesc, "ltime");
        c->Stime = SPI_fnumber(SPI_tuptable->tupdesc, "stime");
        c->Pindex = SPI_fnumber(SPI_tuptable->tupdesc, "pindex");
        c->Dindex = SPI_fnumber(SPI_tuptable->tupdesc, "dindex");
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







static void fetch_customer(HeapTuple *tuple, TupleDesc *tupdesc, customer_t *c_all, customer *c_single)
   {
   Datum binval;
   bool isnull;

   binval = SPI_getbinval(*tuple, *tupdesc, c_all->id, &isnull);
   if (isnull) elog(ERROR, "id contains a null value");
   c_single->id = DatumGetInt32(binval);


   binval = SPI_getbinval(*tuple, *tupdesc, c_all->x, &isnull);
   if (isnull) elog(ERROR, "x contains a null value");
   c_single->x = DatumGetInt32(binval);

   binval = SPI_getbinval(*tuple, *tupdesc, c_all->y, &isnull);
   if (isnull) elog(ERROR, "y contains a null value");
   c_single->y = DatumGetInt32(binval);


   binval = SPI_getbinval(*tuple, *tupdesc, c_all->demand, &isnull);
   if (isnull) elog(ERROR, "demand contains a null value");
   c_single->demand = DatumGetInt32(binval);


   binval = SPI_getbinval(*tuple, *tupdesc, c_all->Etime, &isnull);
   if (isnull) elog(ERROR, "Etime contains a null value");
   c_single->Etime = DatumGetInt32(binval);


   binval = SPI_getbinval(*tuple, *tupdesc, c_all->Ltime, &isnull);
   if (isnull) elog(ERROR, "Ltime contains a null value");
   c_single->Ltime = DatumGetInt32(binval);


   binval = SPI_getbinval(*tuple, *tupdesc, c_all->Stime, &isnull);
   if (isnull) elog(ERROR, "Stime contains a null value");
   c_single->Stime = DatumGetInt32(binval);


   binval = SPI_getbinval(*tuple, *tupdesc, c_all->Pindex, &isnull);
   if (isnull) elog(ERROR, "pindex contains a null value");
   c_single->Pindex = DatumGetInt32(binval);

   binval = SPI_getbinval(*tuple, *tupdesc, c_all->Dindex, &isnull);
   if (isnull) elog(ERROR, "dindex contains a null value");
   c_single->Dindex = DatumGetInt32(binval);

   
   }




//Note:: edge_colums = total , //ly customer_t = total ....

static int compute_shortest_path(char* sql, int  vehicle_count, int capacity ) 
{

        int SPIcode;
        void *SPIplan;
        Portal SPIportal;
        bool moredata = TRUE;
        int ntuples;
        customer *customer_single = NULL;
        int total_tuples = 0;
        customer_t customer_all = {.id= -1, .x=-1, .y=-1 , .demand=-1 , .Etime=-1, .Ltime=-1 , .Stime=-1 , .Pindex=-1 , .Dindex=-1 };   // write this 

        char *err_msg;
        int ret = -1;
        register int z;

        DBG("start shortest_path\n");

        SPIcode = SPI_connect();
        if (SPIcode  != SPI_OK_CONNECT) {
                elog(ERROR, "shortest_path: couldn't open a connection to SPI");
                return -1;
        }

        SPIplan = SPI_prepare(sql, 0, NULL);
        if (SPIplan  == NULL) {
                elog(ERROR, "shortest_path: couldn't create query plan via SPI");
                return -1;
        }

        if ((SPIportal = SPI_cursor_open(NULL, SPIplan, NULL, NULL, true)) == NULL) {
                elog(ERROR, "shortest_path: SPI_cursor_open('%s') returns NULL", sql);
                return -1;
        }

        while (moredata == TRUE) {
                SPI_cursor_fetch(SPIportal, TRUE, TUPLIMIT);

                if (customer_all.id == -1) {
                        if (fetch_customer_columns(SPI_tuptable, &customer_all) == -1)
                                return finish(SPIcode, ret);
                }

                ntuples = SPI_processed;
                total_tuples += ntuples;
                /*
                if (customer_all!=NULL)
                        customer_all = palloc(total_tuples * sizeof(customer_t));
                else
                        customer_all = repalloc(customer_all, total_tuples * sizeof(customer_t));

                        */

                if (ntuples > 0) {
                        int t;
                        SPITupleTable *tuptable = SPI_tuptable;
                        TupleDesc tupdesc = SPI_tuptable->tupdesc;

                        for (t = 0; t < ntuples; t++) {
                                HeapTuple tuple = tuptable->vals[t];
                                fetch_customer(&tuple, &tupdesc, &customer_all , 
                                                &customer_single[total_tuples - ntuples + t]);
                        }
                        SPI_freetuptable(tuptable);
                } 
                else {
                        moredata = FALSE;
                }
        }


        DBG("Calling Solver Instance\n");


        ret = Solver(customer_single, total_tuples, vehicle_count, capacity , &err_msg);

        if (ret < 0) {
                //elog(ERROR, "Error computing path: %s", err_msg);
                ereport(ERROR, (errcode(ERRCODE_E_R_E_CONTAINING_SQL_NOT_PERMITTED), 
                                        errmsg("Error computing path: %s", err_msg)));
        } 



        DBG("ret = %i\n", ret);


        DBG("ret = %i\n", ret);

        return finish(SPIcode, ret);
}



PG_FUNCTION_INFO_V1(vrppdtw);

Datum

vrppdtw(PG_FUNCTION_ARGS)

{

        FuncCallContext     *funcctx;

        int                  call_cntr;

        int                  max_calls;

        TupleDesc            tuple_desc;




        /* stuff done only on the first call of the function */

        if (SRF_IS_FIRSTCALL())

        {

                MemoryContext   oldcontext;

                //int path_count;

                int ret=-1;

                int path_count = 0;



                // XXX profiling messages are not thread safe

                profstart(prof_total);

                profstart(prof_extract);



                /* create a function context for cross-call persistence */

                funcctx = SRF_FIRSTCALL_INIT();



                /* switch to memory context appropriate for multiple function calls */

                oldcontext = MemoryContextSwitchTo(funcctx->multi_call_memory_ctx);


                //path = (vrp_result_element_t *)palloc(sizeof(vrp_result_element_t)*(MAX_ORDERS-1)*2*MAX_VEHICLES);

                DBG("Calling compute_shortes_path");



                ret = compute_shortest_path(//text2char(PG_GETARG_TEXT_P(0)), // points sql

                                text2char(PG_GETARG_TEXT_P(0)),  // orders sql

                              PG_GETARG_INT32(1),  // vehicles sql

                               PG_GETARG_INT32(2)  // distances query
);




                //elog(NOTICE, "Back from solve_vrp, path_count:%d", path_count);



                /* total number of tuples to be returned */

                //DBG("Counting tuples number\n");


                /*

                 * generate attribute metadata needed later to produce tuples from raw

                 * C strings

                 */

                //attinmeta = TupleDescGetAttInMetadata(tuple_desc);

                //funcctx->attinmeta = attinmeta;




                //elog(NOTICE, "table formed");

        }



        //  till line: 832  in VRP.c 

}

