
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.10)
PROJECT(PGROUTING)

#-----------------------------------------------------------------------------
# PGROUTING version number.
SET(PGROUTING_VERSION_MAJOR "1")
SET(PGROUTING_VERSION_MINOR "0")
SET(PGROUTING_VERSION_PATCH "7")

SET(PGROUTING_VERSION_STRING "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}.${PGROUTING_VERSION_PATCH}")

SET(POSTGRESQL_MINIMUM_VERSION "8.3.0")

# This is for my mingw64 environment, it should go away
SET(CMAKE_PREFIX_PATH /c/ming64/projects/pgx64/pg92)

add_subdirectory(cmake)
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include (MacroLibrary)
include (TargetDistclean)

#macro_optional_find_package(PostgreSQL)
#  POSTGRESQL_INCLUDE_DIR, where to find POSTGRESQL.h
#  POSTGRESQL_LIBRARIES, the libraries needed to use POSTGRESQL.
FIND_PACKAGE(PostgreSQL)
IF(NOT POSTGRESQL_FOUND)
  MESSAGE(FATAL_ERROR " Please check your PostgreSQL installation.") 
ENDIF(NOT POSTGRESQL_FOUND)

FIND_PACKAGE(Boost)
IF(Boost_INCLUDE_DIRS)
  MESSAGE("Boost headers were found here: ${Boost_INCLUDE_DIR}")
ELSE(Boost_INCLUDE_DIRS)
  MESSAGE(FATAL_ERROR " Please check your Boost installation ") 
ENDIF(Boost_INCLUDE_DIRS)

IF(NOT POSTGRESQL_VERSION_STRING)
  MESSAGE(FATAL_ERROR "pg_config was not found. Please check your PostgreSQL installation!")
ENDIF(NOT POSTGRESQL_VERSION_STRING)

MESSAGE("POSTGRESQL_VERSION_STRING is ${POSTGRESQL_VERSION_STRING}")
STRING(SUBSTRING "${POSTGRESQL_VERSION_STRING}" 11 -1 POSTGRESQL_VERSION)
MESSAGE("POSTGRESQL_VERSION is ${POSTGRESQL_VERSION}")

IF("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")
  MESSAGE(FATAL_ERROR " PostgreSQL ${POSTGRESQL_MINIMUM_VERSION} or greater is required.")
ENDIF("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")

IF("${POSTGRESQL_VERSION}" VERSION_LESS "9.1")
  SET(USE_PG_EXTENSION FALSE)
ELSE("${POSTGRESQL_VERSION}" VERSION_LESS "9.1")
  SET(USE_PG_EXTENSION TRUE)
ENDIF("${POSTGRESQL_VERSION}" VERSION_LESS "9.1")

EXECUTE_PROCESS(
  COMMAND ${POSTGRESQL_PG_CONFIG} --pkglibdir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE LIB_DIR)

IF(LIB_DIR)
  MESSAGE("Install directory for libraries is set to ${LIB_DIR}")
ELSE(LIB_DIR)
  MESSAGE(FATAL_ERROR "pg_config --pkglibdir failed to return a value. Please check your PostgreSQL installation!")
ENDIF(LIB_DIR)

EXECUTE_PROCESS(
  COMMAND ${POSTGRESQL_PG_CONFIG} --sharedir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE SHARE_DIR)

IF(SHARE_DIR)
  IF(USE_PG_EXTENSION)
    SET(SHARE_DIR "${SHARE_DIR}/extension")
  ELSE(USE_PG_EXTENSION)
    SET(SHARE_DIR "/usr/share/pgrouting")
  ENDIF(USE_PG_EXTENSION)
  MESSAGE("Install directory for SQL files is set to ${SHARE_DIR}")
ELSE(SHARE_DIR)
  MESSAGE(FATAL_ERROR "pg_config --sharedir failed to return a value. Please check your PostgreSQL installation!")
ENDIF(SHARE_DIR)

OPTION(WITH_DOC "Build Documentation" OFF)
OPTION(WITH_TSP "Build TSP library" OFF)
OPTION(WITH_DD "Build Driving distance library" OFF)

IF(UNIX)
  SET(LIBRARY_INSTALL_PATH ${LIB_DIR})
  SET(SQL_INSTALL_PATH /usr/share/pgrouting)
  MESSAGE("Installation directory for libraries is set to ${LIBRARY_INSTALL_PATH} and for SQL files is set to ${SQL_INSTALL_PATH}")
ELSE(UNIX)
  SET(LIBRARY_INSTALL_PATH ${PGROUTING_BINARY_DIR}/lib)
  SET(SQL_INSTALL_PATH ${PGROUTING_BINARY_DIR}/lib)
ENDIF(UNIX)

SET(PGROUTING_INCLUDE_DIRECTORIES
    ${PGROUTING_SOURCE_DIR}/core/astar/src
    ${PGROUTING_SOURCE_DIR}/core/dijkstra/src
    ${PGROUTING_SOURCE_DIR}/core/shooting_star/src
    ${PGROUTING_SOURCE_DIR}/core/tsp/src
    ${PGROUTING_SOURCE_DIR}/core/driving_distance/src
    ${PGROUTING_SOURCE_DIR}/core/trsp/src)

INCLUDE_DIRECTORIES(. ${PGROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
IF(WIN32)
  INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR}/port/win32)
ENDIF(WIN32)

# we need link libpostgres.a on win32 env
IF(WIN32)
  MESSAGE(STATUS "LINK_DIRECTORIES = ${POSTGRESQL_LIBRARIES}")
  LINK_DIRECTORIES(${POSTGRESQL_LIBRARIES} ${LIB_DIR})
  LINK_LIBRARIES(postgres)
ENDIF(WIN32)

IF(UNIX)
  SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIC -O2 -g -frounding-math")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -g -frounding-math -Wno-deprecated")
ELSE(UNIX)
  SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O2 -g -frounding-math")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -frounding-math -Wno-deprecated")
ENDIF(UNIX)

# List variable to collect module SQL file names
SET(PACKAGE_SQL_FILES "")
SET(L_PACKAGE_SQL_FILES "")

# Recurse into the subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
ADD_SUBDIRECTORY(core)
LIST(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")

# assemble the object files from the core functions into
# a single library librouting.so
SET(LIBRARY_OUTPUT_PATH lib)

IF(APPLE)
    SET(LIBRARY_MODE_TARGET "MODULE")
ELSE(APPLE)
    SET(LIBRARY_MODE_TARGET "SHARED")
ENDIF(APPLE)

ADD_LIBRARY(routing
    ${LIBRARY_MODE_TARGET}
    $<TARGET_OBJECTS:astar>
    $<TARGET_OBJECTS:dijkstra>
    $<TARGET_OBJECTS:shooting_star>
    $<TARGET_OBJECTS:trsp>
    )

INSTALL(TARGETS routing DESTINATION ${LIBRARY_INSTALL_PATH})

IF(APPLE)
    SET_TARGET_PROPERTIES(routing
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
ENDIF(APPLE)

IF(WITH_DOC)
  ADD_SUBDIRECTORY(doc)
ENDIF(WITH_DOC)

IF(WITH_TSP)
  macro_optional_find_package(GAUL)
  IF(NOT GAUL_FOUND)
    message(FATAL ERROR " Please check your gaul installation, or set correct path to GAUL_INCLUDE_DIR and GAUL_LIBRARIES.")
  ENDIF(NOT GAUL_FOUND)
  INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${GAUL_INCLUDE_DIR})
  ADD_SUBDIRECTORY(core/tsp)
  LIST(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")
ENDIF(WITH_TSP)

IF(WITH_DD)
  macro_optional_find_package(CGAL)
  IF(NOT CGAL_FOUND)
    message(FATAL ERROR " Please check your CGAL installation, or set correct path to CGAL_INCLUDE_DIR and CGAL_LIBRARIES.")
  ENDIF(NOT CGAL_FOUND)
  #ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
  #ADD_DEFINITIONS(-DBOOST_THREAD_USE_LIB)
  INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CGAL_INCLUDE_DIR})
  ADD_SUBDIRECTORY(core/driving_distance)
  LIST(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")
ENDIF(WITH_DD)

#MESSAGE("PACKAGE_SQL_FILES are ${L_PACKAGE_SQL_FILES}")

FILE(WRITE "${PGROUTING_SOURCE_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
"-- pgRouting version '${PGROUTING_VERSION_STRING}' extension for postgresql\n")
FILE(APPEND "${PGROUTING_SOURCE_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
"-- built WITH_TSP = ${WITH_TSP}, WITH_DD = ${WITH_DD}\n")
IF(USE_PG_EXTENSION)
    FILE(APPEND "${PGROUTING_SOURCE_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
        "-- Complain if script is sourced in pgsql, rather than CREATE EXTENSION\n")
    FILE(APPEND "${PGROUTING_SOURCE_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
        "\\echo Use \"CREATE EXTENSION pgrouting\" to load this file. \\quit")
ENDIF(USE_PG_EXTENSION)

Cat("${PGROUTING_SOURCE_DIR}/core/common/sql/pgrouting-types.sql"
    "${PGROUTING_SOURCE_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in")


FOREACH(PACKAGE_SQL_FILE ${L_PACKAGE_SQL_FILES})
    CAT(${PACKAGE_SQL_FILE} "${PGROUTING_SOURCE_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in")
ENDFOREACH()

# copy our temporary file into a final file
# this extra set will allow us to place variables in the sql file
# that will get substituted in this step before they are installed
CONFIGURE_FILE("${PGROUTING_SOURCE_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
    "${PGROUTING_SOURCE_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql" COPYONLY)

CONFIGURE_FILE("${PGROUTING_SOURCE_DIR}/core/common/sql/pgrouting.control.in"
    "${PGROUTING_SOURCE_DIR}/lib/pgrouting.control")

# this will create the commands in the makefile under the install target
IF(UNIX)
    FILE(GLOB_RECURSE LIBS_TO_INSTALL lib/*.so)
ELSE(UNIX)
    FILE(GLOB_RECURSE LIBS_TO_INSTALL lib/*.dll)
ENDIF(UNIX)

INSTALL(FILES ${LIBS_TO_INSTALL}  DESTINATION ${LIB_DIR})

INSTALL(FILES
    "${PGROUTING_SOURCE_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql"
    "${PGROUTING_SOURCE_DIR}/lib/pgrouting.control"
    DESTINATION "${SHARE_DIR}")


