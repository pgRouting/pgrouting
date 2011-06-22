#ifndef _TRANSIT_H
#define _TRANSIT_H

#include "postgres.h"
#include "fmgr.h"
#include "funcapi.h"
#include "executor/spi.h"
#include "utils/timestamp.h"

#include "utility.h"

#define TUPLIMIT 1000

#undef DEBUG
//#define DEBUG 1

#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif

#ifdef __cplusplus
extern "C"
#endif

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

typedef struct
{
    text *stop_id;
    text *trip_id;
}gtfs_path_element_t;

static void fetch_path(HeapTuple *tuple, TupleDesc *tupdesc, gtfs_path_element_t *path_element);

#endif
