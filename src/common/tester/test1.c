
#define LOGGER_ON
#define LOGGER_LOC
#define LOGGER_FILE "test-log.log"

#include "../src/pgr_logger.h"

#include <time.h>

int main () {
    LOG("%s\n", "message 1");
    LOG("%s at epoch %d\n", "message 2", (int)time(NULL));
    LOG("%s\n", "message 3");

    return 0;
}
