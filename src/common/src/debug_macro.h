
#ifndef DEBUG_MACRO_H
#define DEBUG_MACRO_H


#ifdef DEBUG
#include "postgres.h"
#define PGR_DBG(...) \
    elog(NOTICE, __VA_ARGS__)
#else
#define PGR_DBG(...) do { ; } while (0)
#endif

#endif
