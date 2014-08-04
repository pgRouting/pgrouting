#include "postgres.h"
#include "executor/spi.h"
#include "funcapi.h"
#include "catalog/pg_type.h"
#if PGSQL_VERSION > 92
#include "access/htup_details.h"
#endif

#include "fmgr.h"



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



static int fetch_customer_columns(SPITupleTable *tuptable, customer *c)
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


/*

        static void
fetch_customer(HeapTuple *tuple, TupleDesc *tupdesc, 
                edge_columns_t *edge_columns, edge_t *target_edge)
{
        Datum binval;
        bool isnull;

        binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->id, &isnull);
        if (isnull) elog(ERROR, "id contains a null value");
        target_edge->id = DatumGetInt32(binval);

        binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->source, &isnull);
        if (isnull) elog(ERROR, "source contains a null value");
        target_edge->source = DatumGetInt32(binval);

        binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->target, &isnull);
        if (isnull) elog(ERROR, "target contains a null value");
        target_edge->target = DatumGetInt32(binval);

        binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->cost, &isnull);
        if (isnull) elog(ERROR, "cost contains a null value");
        target_edge->cost = DatumGetFloat8(binval);

        if (edge_columns->reverse_cost != -1) {
                binval = SPI_getbinval(*tuple, *tupdesc, edge_columns->reverse_cost, 
                                &isnull);
                if (isnull) elog(ERROR, "reverse_cost contains a null value");
                target_edge->reverse_cost =  DatumGetFloat8(binval);
        }
}


static int compute_shortest_path(char* sql, int start_vertex, 
                int end_vertex, bool directed, 
                bool has_reverse_cost, 
                path_element_t **path, int *path_count) 
{

        int SPIcode;
        void *SPIplan;
        Portal SPIportal;
        bool moredata = TRUE;
        int ntuples;
        edge_t *edges = NULL;
        int total_tuples = 0;
        edge_columns_t edge_columns = {.id= -1, .source= -1, .target= -1, 
                .cost= -1, .reverse_cost= -1};
        int v_max_id=0;
        int v_min_id=INT_MAX;

        int s_count = 0;
        int t_count = 0;

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

                if (edge_columns.id == -1) {
                        if (fetch_edge_columns(SPI_tuptable, &edge_columns, 
                                                has_reverse_cost) == -1)
                                return finish(SPIcode, ret);
                }

                ntuples = SPI_processed;
                total_tuples += ntuples;
                if (!edges)
                        edges = palloc(total_tuples * sizeof(edge_t));
                else
                        edges = repalloc(edges, total_tuples * sizeof(edge_t));

                if (edges == NULL) {
                        elog(ERROR, "Out of memory");
                        return finish(SPIcode, ret);        
                }

                if (ntuples > 0) {
                        int t;
                        SPITupleTable *tuptable = SPI_tuptable;
                        TupleDesc tupdesc = SPI_tuptable->tupdesc;

                        for (t = 0; t < ntuples; t++) {
                                HeapTuple tuple = tuptable->vals[t];
                                fetch_edge(&tuple, &tupdesc, &edge_columns, 
                                                &edges[total_tuples - ntuples + t]);
                        }
                        SPI_freetuptable(tuptable);
                } 
                else {
                        moredata = FALSE;
                }
        }
}
*/

