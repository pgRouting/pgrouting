/*
    pgr_logger.h

    Author: Stephen Woodbridge
    Date: 2014-05-04
    License: MIT-X

    A simple logging mechanism the can be disabled for production
    and should work in C or C++ code.

    USAGE:

    #define PGR_LOGGERR_ON
    #undef PGR_LOGGER_ON  // turn off logging for production

    // override the default log file "/tmp/pgr_logger.log"
    #define PGR_LOGGER_FILE "mylogger.log"

    #undef PGR_LOGGER_LOC  // don't log filename and line number
    #define PGR_LOGGER_LOC  // log filename and line number with log message

    // include the logger macros and configure based on defines above
    #include pgr_logger.h
    
    // log a message to the log file
    PGR_LOGF(format, args);
    PGR_LOGF("%s at %d\n", "this is a message", time());
    PGR_LOG("just print a string to the log");

    This will log something like:

    myfile.cpp:123: this is a message at 1399216875
    myfile.cpp:124: just print a string to the log

*/
#ifndef PGR_LOGGER_H
#define PGR_LOGGER_H

#include <stdio.h>

#if defined(PGR_LOGGER_LOC)
#  undef PGR_LOGGER_LOC_
#  define PGR_LOGGER_LOC_ 1
#else
#  undef PGR_LOGGER_LOC_
#  define PGR_LOGGER_LOC_ 0
#endif

#ifndef PGR_LOGGER_FILE
#  define PGR_LOGGER_FILE "/tmp/pgr_logger.log"
#endif

#ifdef PGR_LOGGER_ON
#  define PGR_LOGF(format, ...) { \
     FILE *fp = fopen(PGR_LOGGER_FILE, "a+"); \
     if (PGR_LOGGER_LOC_) fprintf(fp, "%s:%d: ", __FILE__, __LINE__); \
     fprintf(fp, format, __VA_ARGS__); \
     fclose(fp); }
#else  // ifndef LOGGER_ON
#  define PGR_LOGF(format, ...)
#endif  // ifndef LOGGER_ON

#define PGR_LOG(str) PGR_LOGF("%s\n", str)

#endif  // ifndef PGR_LOGGER_H
