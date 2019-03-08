# - Find PostgreSQL
# Find the PostgreSQL includes and client library
# This module defines
#  POSTGRESQL_INCLUDE_DIR, where to find POSTGRESQL.h
#  POSTGRESQL_LIBRARIES, the libraries needed to use POSTGRESQL.
#  POSTGRESQL_FOUND, If false, do not try to use PostgreSQL.
#  POSTGRESQL_EXECUTABLE
#  POSTGRESQL_VERSION_STRING
#
# Copyright (c) 2006, Jaroslaw Staniek, <js@iidea.pl>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

# Add the postgresql and mysql include paths here

# A check condition to see if those variables are set

SET(POSTGRESQL_BIN "" CACHE STRING "non-standard path to the postgresql program executables")

if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES AND POSTGRESQL_EXECUTABLE AND POSTGRESQL_VERSION_STRING)
    set(POSTGRESQL_FOUND TRUE)
else(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES AND POSTGRESQL_EXECUTABLE)

    if(NOT "${POSTGRESQL_BIN}" STREQUAL "")
        # Checking POSTGRESQL_PG_CONFIG
        find_program(POSTGRESQL_PG_CONFIG NAMES pg_config
            PATHS
            ${POSTGRESQL_BIN}
            NO_DEFAULT_PATH
            )
    else(NOT "${POSTGRESQL_BIN}" STREQUAL "")
        # Checking POSTGRESQL_PG_CONFIG
        find_program(POSTGRESQL_PG_CONFIG NAMES pg_config
            PATHS
            /usr/lib/postgresql/*/bin/
            )
    endif(NOT "${POSTGRESQL_BIN}" STREQUAL "")

    message(STATUS "POSTGRESQL_PG_CONFIG is " ${POSTGRESQL_PG_CONFIG})
    if(POSTGRESQL_PG_CONFIG)
        execute_process(
            COMMAND ${POSTGRESQL_PG_CONFIG} --bindir
            OUTPUT_STRIP_TRAILING_WHITESPACE
            OUTPUT_VARIABLE T_POSTGRESQL_BIN)
    endif(POSTGRESQL_PG_CONFIG)

    # search for POSTGRESQL_EXECUTABLE _only_ in the dir specified by pg_config
    find_program(POSTGRESQL_EXECUTABLE NAMES postgres
        PATHS
        ${T_POSTGRESQL_BIN}
        NO_DEFAULT_PATH
        )
    # if not found continue search in the path and all the dirs listed here (questionable)
    find_program(POSTGRESQL_EXECUTABLE NAMES postgres
        PATHS
        /usr/lib/postgresql/*/bin/
        )
#    # more elegant, equivalent way if we want to keep both of above:
#    find_program(POSTGRESQL_EXECUTABLE NAMES postgres
#        HINTS
#        ${T_POSTGRESQL_BIN}
#        PATHS
#        /usr/lib/postgresql/*/bin/
#        )
    message(STATUS "POSTGRESQL_EXECUTABLE is " ${POSTGRESQL_EXECUTABLE})

    if(POSTGRESQL_PG_CONFIG)
        execute_process(
            COMMAND ${POSTGRESQL_PG_CONFIG} --version
            OUTPUT_STRIP_TRAILING_WHITESPACE
            OUTPUT_VARIABLE POSTGRESQL_VERSION_STRING)
    endif(POSTGRESQL_PG_CONFIG)

    message(STATUS "POSTGRESQL_VERSION_STRING in FindPostgreSQL.cmake is " ${POSTGRESQL_VERSION_STRING})

    if(POSTGRESQL_PG_CONFIG)
        execute_process(
            COMMAND ${POSTGRESQL_PG_CONFIG} --includedir-server
            OUTPUT_STRIP_TRAILING_WHITESPACE
            OUTPUT_VARIABLE T_POSTGRESQL_INCLUDE_DIR)
    endif(POSTGRESQL_PG_CONFIG)

    #as with POSTGRESQL_EXECUTABLE we should/could use the path specified by pg_config only
    #instead of path and our own guesses
    find_path(POSTGRESQL_INCLUDE_DIR postgres.h
        HINTS
        ${T_POSTGRESQL_INCLUDE_DIR}
        PATHS
        /usr/include/server
        /usr/include/pgsql/server
        /usr/local/include/pgsql/server
        /usr/include/postgresql/server
        /usr/include/postgresql/*/server
        /usr/local/include/postgresql/server
        /usr/local/include/postgresql/*/server
        $ENV{ProgramFiles}/PostgreSQL/*/include/server
        $ENV{SystemDrive}/PostgreSQL/*/include/server
        )

    if(POSTGRESQL_PG_CONFIG)
        execute_process(
            COMMAND ${POSTGRESQL_PG_CONFIG} --libdir
            OUTPUT_STRIP_TRAILING_WHITESPACE
            OUTPUT_VARIABLE POSTGRESQL_LIBRARIES)
    endif(POSTGRESQL_PG_CONFIG)

    if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
        set(POSTGRESQL_FOUND TRUE)
        message(STATUS "POSTGRESQL_INCLUDE_DIR: ${POSTGRESQL_INCLUDE_DIR}")
        message(STATUS "POSTGRESQL_LIBRARIES: ${POSTGRESQL_LIBRARIES}")
        include_directories(${POSTGRESQL_INCLUDE_DIR})
    else(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
        set(POSTGRESQL_FOUND FALSE)
        message(STATUS "PostgreSQL not found.")
    endif(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)

    mark_as_advanced(POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBRARIES)

endif(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES AND POSTGRESQL_EXECUTABLE AND POSTGRESQL_VERSION_STRING)

