


#include "postgres.h"
#include <time.h>

#include "time_msg.h"

void time_msg(char *msg, clock_t start_t, clock_t end_t) {
    double elapsed_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    elog(DEBUG1, "Elapsed time for %s: %lfsec = (%lf - %f) / CLOCKS_PER_SEC ", msg, elapsed_t, (double) end_t, (double) start_t);
}
