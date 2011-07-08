#pragma once

#include "postgres.h"
#include "executor/spi.h"

char *text2char(text *in);
int finish(int code, int ret);
