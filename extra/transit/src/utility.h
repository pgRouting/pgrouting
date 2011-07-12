#pragma once

#include <postgres.h>
#include <executor/spi.h>

#ifdef DEBUG
#define DBG(format, arg...)                     \
    elog(NOTICE, format , ## arg)
#else
#define DBG(format, arg...) do { ; } while (0)
#endif

char *text2char(text *in);
int finish(int code, int ret);
