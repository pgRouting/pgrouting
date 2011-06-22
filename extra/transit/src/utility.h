#ifndef _UTILITY_H
#define _UTILITY_H
#include "postgres.h"
#include "executor/spi.h"

char *text2char(text *in);
int finish(int code, int ret);

#endif
