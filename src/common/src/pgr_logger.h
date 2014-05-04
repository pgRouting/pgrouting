/*
    pgr_logger.h

    Author: Stephen Woodbridge
    Date: 2014-05-04
    License: MIT-X

    A simple logging mechanism the can be disabled for production
    and should work in C or C++ code.

    USEAGE:

    #define LOGGERR_ON
    #udef LOGGER_ON  // turn off logging for production

    // override the default log file "/tmp/pgr_logger.log"
    #define LOGGER_FILE "mylogger.log"

    #undef LOGGER_LOC  // dont log filename and line number
    #define LOGGER_LOC  // log filename and line number with log message

    // include the logger macros and configure based on defines above
    #include pgr_logger.h
    
    // log a message to the log file
    LOG(format, args)
    LOG("%s at %d\n", "this is a message", time())

    This will log something like:

    myfile:123: this is a message at 1399216875

*/
#ifndef PGR_LOGGER_H
#define PGR_LOGGER_H

#include <stdio.h>

#if defined(LOGGER_LOC)
#  undef LOGGER_LOC_
#  define LOGGER_LOC_ 1
#else
#  undef LOGGER_LOC_
#  define LOGGER_LOC_ 0
#endif

#ifndef LOGGER_FILE
#  define LOGGER_FILE "/tmp/pgr_logger.log"
#endif

#ifdef LOGGER_ON
#  define LOG(format, ...) { \
     FILE *fp = fopen(LOGGER_FILE, "a+"); \
     if (LOGGER_LOC_) fprintf(fp, "%s:%d: ", __FILE__, __LINE__); \
     fprintf(fp, format, __VA_ARGS__); \
     fclose(fp); }
#else  // ifndef LOGGER_ON
#  define LOG(format, ...)
#endif  // ifndef LOGGER_ON

#endif  // ifndef PGR_LOGGER_H
