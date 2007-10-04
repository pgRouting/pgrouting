CMAKE_MINIMUM_REQUIRED(VERSION 2.3)
PROJECT(PGROUTING)

#-----------------------------------------------------------------------------
# PGROUTING version number.
SET(PGROUTING_VERSION_MAJOR "1")
SET(PGROUTING_VERSION_MINOR "0")
SET(PGROUTING_VERSION_PATCH "0")

SET(PGROUTING_VERSION_STRING "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}.${PGROUTING_VERSION_PATCH}-RC1")

add_subdirectory(cmake)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include (MacroLibrary)

macro_optional_find_package(PostgreSQL)

FIND_PACKAGE(Boost)
IF(Boost_INCLUDE_DIRS)
  MESSAGE("Boost headers were found here: ${Boost_INCLUDE_DIR}")
ENDIF(Boost_INCLUDE_DIRS)

EXEC_PROGRAM(pg_config
 ARGS --pkglibdir
 OUTPUT_VARIABLE LIB_DIR)

IF(LIB_DIR)
  MESSAGE("Output directory for libraries is set to ${LIB_DIR}")
ELSE(LIB_DIR)
  SET(LIB_DIR ${PGROUTING_BINARY_DIR}/lib)
  MESSAGE("pg_config not found, output directory for libraries is set to ${PGROUTING_BINARY_DIR}/lib")
ENDIF(LIB_DIR)

if(PGROUTING_CORE_INCLUDE_DIR)
   set(PGROUTING_CORE_FOUND TRUE)

else(PGROUTING_CORE_INCLUDE_DIR)
 FIND_PATH(PGROUTING_CORE_INCLUDE_DIR dijkstra.h
      PATHS
      ${CMAKE_CURRENT_SOURCE_DIR}/core/src
      )

  if(PGROUTING_CORE_INCLUDE_DIR)
    set(PGROUTING_CORE_FOUND TRUE)
    message(STATUS "Found PGROUTING_CORE core: ${PGROUTING_CORE_INCLUDE_DIR}")
    INCLUDE_DIRECTORIES(${PGROUTING_CORE_INCLUDE_DIR})
  else(PGROUTING_CORE_INCLUDE_DIR)
    set(PGROUTING_CORE_FOUND FALSE)
    message(STATUS "PGROUTING_CORE core not found.")
  endif(PGROUTING_CORE_INCLUDE_DIR)

  mark_as_advanced(PGROUTING_CORE_INCLUDE_DIR)

endif(PGROUTING_CORE_INCLUDE_DIR)

OPTION(WITH_TSP "Build TSP library" OFF)
OPTION(WITH_DD "Build Driving distance library" OFF)

# Recurse into the subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
SUBDIRS(core/src)

IF(WITH_TSP)
ADD_SUBDIRECTORY(extra/tsp/src)
macro_optional_find_package(GAUL)
ENDIF(WITH_TSP)

IF(WITH_DD)
ADD_SUBDIRECTORY(extra/driving_distance/src)
macro_optional_find_package(CGAL)
ENDIF(WITH_DD)

#SET(LIBRARY_OUTPUT_PATH ${PGROUTING_BINARY_DIR}/lib)

IF(UNIX)
  SET(LIBRARY_OUTPUT_PATH ${LIB_DIR})
  SET(SQL_INSTALL_PATH /usr/share/postlbs)
ELSE(UNIX)
  SET(LIBRARY_OUTPUT_PATH ${PGROUTING_BINARY_DIR}/lib)
  SET(SQL_INSTALL_PATH ${PGROUTING_BINARY_DIR}/lib)
ENDIF(UNIX)

SET(PGROUTING_INCLUDE_DIRECTORIES ${PGROUTING_SOURCE_DIR}/core ${PGROUTING_SOURCE_DIR}/core/src ${PGROUTING_SOURCE_DIR}/extra ${PGROUTING_SOURCE_DIR}/extra/tsp ${PGROUTING_SOURCE_DIR}/extra/tsp/src ${PGROUTING_SOURCE_DIR}/extra/driving_distance ${PGROUTING_SOURCE_DIR}/extra/driving_distance/src
)

INCLUDE_DIRECTORIES(. ${PGROUTING_INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS}/boost)

SET(CMAKE_C_FLAGS "-O2 -g")
SET(CMAKE_CXX_FLAGS "-O2 -g")

CONFIGURE_FILE(core/sql/routing_core.sql ${SQL_INSTALL_PATH}/routing_core.sql COPYONLY)
CONFIGURE_FILE(core/sql/routing_core_wrappers.sql ${SQL_INSTALL_PATH}/routing_core_wrappers.sql COPYONLY)

IF(WITH_TSP)
CONFIGURE_FILE(extra/tsp/sql/routing_tsp.sql ${SQL_INSTALL_PATH}/routing_tsp.sql COPYONLY)
CONFIGURE_FILE(extra/tsp/sql/routing_tsp_wrappers.sql ${SQL_INSTALL_PATH}/routing_tsp_wrappers.sql COPYONLY)
ENDIF(WITH_TSP)

IF(WITH_DD)
CONFIGURE_FILE(extra/driving_distance/sql/routing_dd.sql ${SQL_INSTALL_PATH}/routing_dd.sql COPYONLY)
CONFIGURE_FILE(extra/driving_distance/sql/routing_dd_wrappers.sql ${SQL_INSTALL_PATH}/routing_dd_wrappers.sql COPYONLY)
ENDIF(WITH_DD)
