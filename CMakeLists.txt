
CMAKE_MINIMUM_REQUIRED(VERSION 2.3)
PROJECT(PGROUTING)

#-----------------------------------------------------------------------------
# PGROUTING version number.
SET(PGROUTING_VERSION_MAJOR "1")
SET(PGROUTING_VERSION_MINOR "0")
SET(PGROUTING_VERSION_PATCH "6")

SET(PGROUTING_VERSION_STRING "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}.${PGROUTING_VERSION_PATCH}")

add_subdirectory(cmake)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include (MacroLibrary)
include (TargetDistclean)

macro_optional_find_package(PostgreSQL)
IF(NOT POSTGRESQL_FOUND)
  MESSAGE(FATAL ERROR " Please check your PostgreSQL installation.") 
ENDIF(NOT POSTGRESQL_FOUND)

FIND_PACKAGE(Boost)
IF(Boost_INCLUDE_DIRS)
  MESSAGE("Boost headers were found here: ${Boost_INCLUDE_DIR}")
ELSE(Boost_INCLUDE_DIRS)
  MESSAGE(FATAL ERROR " Please check your Boost installation ") 
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
OPTION(WITH_KDIJKSTRA "Build One_To_Many_ShortestPath (Dijkstra) function" OFF)

IF(UNIX)
  SET(LIBRARY_INSTALL_PATH ${LIB_DIR})
  SET(SQL_INSTALL_PATH /usr/share/pgrouting)
  MESSAGE("Installation directory for libraries is set to ${LIBRARY_INSTALL_PATH} and for SQL files is set to ${SQL_INSTALL_PATH}")
ELSE(UNIX)
  SET(LIBRARY_INSTALL_PATH ${PGROUTING_BINARY_DIR}/lib)
  SET(SQL_INSTALL_PATH ${PGROUTING_BINARY_DIR}/lib)
ENDIF(UNIX)

SET(PGROUTING_INCLUDE_DIRECTORIES ${PGROUTING_SOURCE_DIR}/core ${PGROUTING_SOURCE_DIR}/core/src ${PGROUTING_SOURCE_DIR}/extra ${PGROUTING_SOURCE_DIR}/extra/tsp ${PGROUTING_SOURCE_DIR}/extra/tsp/src ${PGROUTING_SOURCE_DIR}/extra/driving_distance ${PGROUTING_SOURCE_DIR}/extra/driving_distance/src ${PGROUTING_SOURCE_DIR}/extra/KDijkstra ${PGROUTING_SOURCE_DIR}/extra/KDijkstra/src
)

INCLUDE_DIRECTORIES(. ${PGROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
IF(WIN32)
  INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR}/port/win32)
ENDIF(WIN32)

# we need link libpostgres.a on win32 env
IF(WIN32)
  LINK_DIRECTORIES(${LIB_DIR})
  LINK_LIBRARIES(postgres)
ENDIF(WIN32)

SET(CMAKE_C_FLAGS   "-O2 -g -lgmp -frounding-math")
SET(CMAKE_CXX_FLAGS "-O2 -g -lgmp -frounding-math")

# Recurse into the subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
SUBDIRS(core)

IF(WITH_KDIJKSTRA)
  INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES})
  ADD_SUBDIRECTORY(extra/KDijkstra)
ENDIF(WITH_KDIJKSTRA)

IF(WITH_TSP)
  macro_optional_find_package(GAUL)
  IF(NOT GAUL_FOUND)
    message(FATAL ERROR " Please check your gaul installation, or set correct path to GAUL_INCLUDE_DIR and GAUL_LIBRARIES.")
  ENDIF(NOT GAUL_FOUND)
  INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${GAUL_INCLUDE_DIR})
  ADD_SUBDIRECTORY(extra/tsp)
ENDIF(WITH_TSP)

IF(WITH_DD)
  macro_optional_find_package(CGAL)
  IF(NOT CGAL_FOUND)
    message(FATAL ERROR " Please check your CGAL installation, or set correct path to CGAL_INCLUDE_DIR and CGAL_LIBRARIES.")
  ENDIF(NOT CGAL_FOUND)
  INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CGAL_INCLUDE_DIR})
  ADD_SUBDIRECTORY(extra/driving_distance)
ENDIF(WITH_DD)




