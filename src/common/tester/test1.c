
#define PGR_LOGGER_ON
#define PGR_LOGGER_LOC
#define PGR_LOGGER_FILE "test-log.log"

#include "../src/pgr_logger.h"

#include <time.h>

int main () {
    PGR_LOG("message 1");
    PGR_LOGF("%s at epoch %d\n", "message 2", (int)time(NULL));
    PGR_LOG("message 3");

    return 0;
}
