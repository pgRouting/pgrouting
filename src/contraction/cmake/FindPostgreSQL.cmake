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

#POSTGRESQL_BIN ia an optional commandline argument to specify a non-standard path to the postgresql program executables
SET(POSTGRESQL_BIN "" CACHE STRING "Some user-specified option")    

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


    # Checking POSTGRESQL_EXECUTABLE in all the dir (*) - implies that 
    find_program(POSTGRESQL_EXECUTABLE NAMES postgres
        PATHS
        ${T_POSTGRESQL_BIN}
        )
    message(STATUS "POSTGRESQL_EXECUTABLE is " ${POSTGRESQL_EXECUTABLE})


#    find_program(POSTGRESQL_EXECUTABLE NAMES postgres
#        PATHS
#        /usr/lib/postgresql/*/bin/
#        )
#    message(STATUS "POSTGRESQL_EXECUTABLE is " ${POSTGRESQL_EXECUTABLE})

#    find_program(POSTGRESQL_PG_CONFIG NAMES pg_config
#        PATHS
#        /usr/lib/postgresql/*/bin/
#        )
#    message(STATUS "POSTGRESQL_PG_CONFIG is " ${POSTGRESQL_PG_CONFIG})

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

    find_path(POSTGRESQL_INCLUDE_DIR postgres.h
        ${T_POSTGRESQL_INCLUDE_DIR}

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

