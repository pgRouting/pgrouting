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

if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES AND POSTGRESQL_EXECUTABLE AND POSTGRESQL_VERSION_STRING)
   set(POSTGRESQL_FOUND TRUE)
else(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES AND POSTGRESQL_EXECUTABLE)

#  find_path(POSTGRESQL_INCLUDE_DIR libpq-fe.h

 FIND_PROGRAM(POSTGRESQL_EXECUTABLE NAMES postgres
    PATHS
    /usr/lib/postgresql/*/bin/
    )
 MESSAGE(STATUS "POSTGRESQL_EXECUTABLE is " ${POSTGRESQL_EXECUTABLE})

 FIND_PROGRAM(POSTGRESQL_PG_CONFIG NAMES pg_config
    PATHS
    /usr/lib/postgresql/*/bin/
    )
 MESSAGE(STATUS "POSTGRESQL_PG_CONFIG is " ${POSTGRESQL_PG_CONFIG})

 if(POSTGRESQL_PG_CONFIG)
      EXECUTE_PROCESS(
          COMMAND pg_config --version
          OUTPUT_STRIP_TRAILING_WHITESPACE
          OUTPUT_VARIABLE POSTGRESQL_VERSION_STRING)
 endif(POSTGRESQL_PG_CONFIG)

 if(POSTGRESQL_PG_CONFIG)
      EXECUTE_PROCESS(
          COMMAND pg_config --includedir-server
          OUTPUT_STRIP_TRAILING_WHITESPACE
          OUTPUT_VARIABLE T_POSTGRESQL_INCLUDE_DIR)
 endif(POSTGRESQL_PG_CONFIG)
 FIND_PATH(POSTGRESQL_INCLUDE_DIR postgres.h
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
      EXECUTE_PROCESS(
          COMMAND pg_config --libdir
          OUTPUT_STRIP_TRAILING_WHITESPACE
          OUTPUT_VARIABLE POSTGRESQL_LIBRARIES)
 endif(POSTGRESQL_PG_CONFIG)
# find_library(POSTGRESQL_LIBRARIES NAMES pq libpq
#          PATHS
#          ${T_POSTGRESQL_LIBRARIES}
#          /usr/lib
#          /usr/local/lib
#          /usr/lib/postgresql
#          /usr/lib64
#          /usr/local/lib64
#          /usr/lib64/postgresql
#          $ENV{ProgramFiles}/PostgreSQL/*/lib/ms
#          $ENV{SystemDrive}/PostgreSQL/*/lib/ms
#          )
      
  if(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
    set(POSTGRESQL_FOUND TRUE)
    message(STATUS "Found PostgreSQL: ${POSTGRESQL_INCLUDE_DIR}, ${POSTGRESQL_LIBRARIES}")
    INCLUDE_DIRECTORIES(${POSTGRESQL_INCLUDE_DIR})
  else(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)
    set(POSTGRESQL_FOUND FALSE)
    message(STATUS "PostgreSQL not found.")
  endif(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES)

  mark_as_advanced(POSTGRESQL_INCLUDE_DIR POSTGRESQL_LIBRARIES)

endif(POSTGRESQL_INCLUDE_DIR AND POSTGRESQL_LIBRARIES AND POSTGRESQL_EXECUTABLE AND POSTGRESQL_VERSION_STRING)
