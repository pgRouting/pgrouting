#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"

#include "./pdp.h"


#undef DEBUG
//#define DEBUG 1

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

//From Dijkstra
static char *
text2char(text *in)
{
          char *out = palloc(VARSIZE(in));

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



static int fetch_distance_columns(SPITupleTable *tuptable, customer *c)
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




