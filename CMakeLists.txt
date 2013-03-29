#
# Format convention
# - command (lower case)
# - VARIABLE (upper case)
# (http://www.cmake.org/cmake/help/v2.8.10/cmake.html)
#

cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)
project(PGROUTING)

# Configure options
option(WITH_TSP
    "Set ON|OFF (default=OFF) to build TSP library" OFF)

option(WITH_DD
    "Set ON|OFF (default=OFF) to build Driving distance library" OFF)

option(WITH_DOC
    "Set ON|OFF (default=OFF) to build Documentation library tree" OFF)

#-----------------------------------------------------------------------------
# PGROUTING version number.
set(PGROUTING_VERSION_MAJOR "1")
set(PGROUTING_VERSION_MINOR "0")
set(PGROUTING_VERSION_PATCH "7")

set(PGROUTING_VERSION_STRING "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}.${PGROUTING_VERSION_PATCH}")

set(POSTGRESQL_MINIMUM_VERSION "8.3.0")

#-----------------------------------------------------------------------------
# CMAKE configuration
# This is for my mingw64 environment, it should go away
set(CMAKE_PREFIX_PATH /usr:/usr/local)
set(CMAKE_INCLUDE_PATH /usr:/usr/local)
set(CMAKE_LIBRARY_PATH /usr:/usr/local)

add_subdirectory(cmake)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(MacroLibrary)
include(TargetDistclean)

message(STATUS "UNIX=${UNIX}")
message(STATUS "WIN32=${WIN32}")
if(WIN32)
  set(OS_BUILD  $ENV{OS_BUILD})
  if(NOT OS_BUILD)
    set(OS_BUILD "64")
  endif(NOT OS_BUILD)
  set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}:/c/ming${OS_BUILD}/projects/pgx${OS_BUILD}/pg92)
  set(BOOST_ROOT c:/ming${OS_BUILD}/msys/local)
  set(GAUL_ROOT c:/ming${OS_BUILD}/msys/local)
  set(CGAL_ROOT c:/ming${OS_BUILD}/msys/local)
  set(GMP_ROOT c:/ming${OS_BUILD}/mingw${OS_BUILD})
  message(STATUS "OS_BUILD=${OS_BUILD}")
  message(STATUS "GAUL_ROOT=${GAUL_ROOT}")
  message(STATUS "CGAL_ROOT=${CGAL_ROOT}")
  message(STATUS "GMP_ROOT=${GMP_ROOT}")
  message(STATUS "BOOST_ROOT=${BOOST_ROOT}")
  message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
endif(WIN32)

# Find Git to collect version information
find_package(Git)
if(GIT_FOUND)
  GIT_WC_INFO(${PGROUTING_SOURCE_DIR} PGROUTING)
  set(PGROUTING_VERSION_REVISION_NAME "${PGROUTING_WC_REVISION_NAME}")
  set(PGROUTING_VERSION_REVISION_HASH "${PGROUTING_WC_REVISION_HASH}")
else()
  set(PGROUTING_VERSION_REVISION_NAME "")
  set(PGROUTING_VERSION_REVISION_HASH "")
endif()

#macro_optional_find_package(PostgreSQL)
#  POSTGRESQL_INCLUDE_DIR, where to find POSTGRESQL.h
#  POSTGRESQL_LIBRARIES, the libraries needed to use POSTGRESQL.
find_package(PostgreSQL)
if(NOT POSTGRESQL_FOUND)
  message(FATAL_ERROR " Please check your PostgreSQL installation.") 
endif(NOT POSTGRESQL_FOUND)

#set(Boost_DEBUG ON)
find_package(Boost)
if(Boost_INCLUDE_DIRS)
  message("Boost headers were found here: ${Boost_INCLUDE_DIR}")
else(Boost_INCLUDE_DIRS)
  message(FATAL_ERROR " Please check your Boost installation ") 
endif(Boost_INCLUDE_DIRS)

if(NOT POSTGRESQL_VERSION_STRING)
  message(FATAL_ERROR "pg_config was not found. Please check your PostgreSQL installation!")
endif(NOT POSTGRESQL_VERSION_STRING)

message(STATUS "POSTGRESQL_VERSION_STRING is ${POSTGRESQL_VERSION_STRING}")
string(SUBSTRING "${POSTGRESQL_VERSION_STRING}" 11 -1 POSTGRESQL_VERSION)
message(STATUS "POSTGRESQL_VERSION is ${POSTGRESQL_VERSION}")

if("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")
  message(FATAL_ERROR " PostgreSQL ${POSTGRESQL_MINIMUM_VERSION} or greater is required.")
endif("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")

if("${POSTGRESQL_VERSION}" VERSION_LESS "9.1")
  set(USE_PG_EXTENSION FALSE)
else("${POSTGRESQL_VERSION}" VERSION_LESS "9.1")
  set(USE_PG_EXTENSION TRUE)
endif("${POSTGRESQL_VERSION}" VERSION_LESS "9.1")

execute_process(
  COMMAND ${POSTGRESQL_PG_CONFIG} --pkglibdir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE LIB_DIR)

if(LIB_DIR)
  message(STATUS "Install directory for libraries is set to ${LIB_DIR}")
else(LIB_DIR)
  message(FATAL_ERROR "pg_config --pkglibdir failed to return a value. Please check your PostgreSQL installation!")
endif(LIB_DIR)

execute_process(
  COMMAND ${POSTGRESQL_PG_CONFIG} --sharedir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE SHARE_DIR)

if(SHARE_DIR)
  if(USE_PG_EXTENSION)
    set(SHARE_DIR "${SHARE_DIR}/extension")
  else(USE_PG_EXTENSION)
    set(SHARE_DIR "/usr/share/pgrouting")
  endif(USE_PG_EXTENSION)
  message(STATUS "Install directory for SQL files is set to ${SHARE_DIR}")
else(SHARE_DIR)
  message(FATAL_ERROR "pg_config --sharedir failed to return a value. Please check your PostgreSQL installation!")
endif(SHARE_DIR)

# Handles documentation
add_subdirectory(doc)

if(UNIX)
  set(LIBRARY_INSTALL_PATH ${LIB_DIR})
  set(SQL_INSTALL_PATH /usr/share/pgrouting)
else(UNIX)
  set(LIBRARY_INSTALL_PATH ${PGROUTING_BINARY_DIR}/lib)
  set(SQL_INSTALL_PATH ${PGROUTING_BINARY_DIR}/lib)
endif(UNIX)
message(STATUS "Installation directory for libraries is set to ${LIBRARY_INSTALL_PATH}")
message(STATUS "Installation directory for SQL files is set to ${SQL_INSTALL_PATH}")

set(PGROUTING_INCLUDE_DIRECTORIES
    ${PGROUTING_SOURCE_DIR}/src/astar/src
    ${PGROUTING_SOURCE_DIR}/src/dijkstra/src
    ${PGROUTING_SOURCE_DIR}/src/shooting_star/src
    ${PGROUTING_SOURCE_DIR}/src/tsp/src
    ${PGROUTING_SOURCE_DIR}/src/driving_distance/src
    ${PGROUTING_SOURCE_DIR}/src/trsp/src)

include_directories(. ${PGROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
if(WIN32)
  include_directories(${INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR}/port/win32)
endif(WIN32)

# we need link libpostgres.a on win32 env
if(WIN32)
  message(STATUS "LINK_DIRECTORIES = ${POSTGRESQL_LIBRARIES}")
  link_directories(${POSTGRESQL_LIBRARIES} ${LIB_DIR})
  link_libraries(postgres)
endif(WIN32)

if(UNIX)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIC -O2 -g -frounding-math")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -g -frounding-math -Wno-deprecated")
else(UNIX)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O2 -g -frounding-math")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -frounding-math -Wno-deprecated")
endif(UNIX)

# List variable to collect module SQL file names
set(PACKAGE_SQL_FILES "")
set(L_PACKAGE_SQL_FILES "")

# Recurse into the subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
add_subdirectory(src)
list(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")

# assemble the object files from the src functions into
# a single library librouting.so
set(LIBRARY_OUTPUT_PATH lib)

if(APPLE)
    set(LIBRARY_MODE_TARGET "MODULE")
else(APPLE)
    set(LIBRARY_MODE_TARGET "SHARED")
endif(APPLE)

add_library(routing
    ${LIBRARY_MODE_TARGET}
    $<TARGET_OBJECTS:astar>
    $<TARGET_OBJECTS:dijkstra>
    $<TARGET_OBJECTS:shooting_star>
    $<TARGET_OBJECTS:trsp>
    )

install(TARGETS routing DESTINATION ${LIBRARY_INSTALL_PATH})

if(APPLE)
    set_target_properties(routing
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)

IF(WITH_TSP)
  find_package(GAUL)
  if(NOT GAUL_FOUND)
    message(FATAL_ERROR " Please check your gaul installation, or set correct path to GAUL_INCLUDE_DIR and GAUL_LIBRARIES.")
  endif(NOT GAUL_FOUND)
  include_directories(${INCLUDE_DIRECTORIES} ${GAUL_INCLUDE_DIR})
  add_subdirectory(src/tsp)
  list(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")
endif(WITH_TSP)

if(WITH_DD)
  find_package(CGAL)
  if(NOT CGAL_FOUND)
    message(FATAL_ERROR " Please check your CGAL installation, or set correct path to CGAL_INCLUDE_DIR and CGAL_LIBRARIES.")
  endif(NOT CGAL_FOUND)
  #add_definitions(-DBOOST_ALL_NO_LIB)
  #add_definitions(-DBOOST_THREAD_USE_LIB)
  include_directories(${INCLUDE_DIRECTORIES} ${CGAL_INCLUDE_DIR})
  add_subdirectory(src/driving_distance)
  list(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")
endif(WITH_DD)

#MESSAGE("PACKAGE_SQL_FILES are ${L_PACKAGE_SQL_FILES}")

file(WRITE "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
"-- pgRouting version '${PGROUTING_VERSION_STRING}' extension for postgresql\n")
file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
"-- built WITH_TSP = ${WITH_TSP}, WITH_DD = ${WITH_DD}\n")
if(USE_PG_EXTENSION)
    file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
        "-- Complain if script is sourced in pgsql, rather than CREATE EXTENSION\n")
    file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
        "\\echo Use \"CREATE EXTENSION pgrouting\" to load this file. \\quit")
endif(USE_PG_EXTENSION)

Cat("${PGROUTING_SOURCE_DIR}/src/common/sql/pgrouting-types.sql"
    "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in")


foreach(PACKAGE_SQL_FILE ${L_PACKAGE_SQL_FILES})
    cat(${PACKAGE_SQL_FILE} "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in")
endforeach()

# copy our temporary file into a final file
# this extra set will allow us to place variables in the sql file
# that will get substituted in this step before they are installed
configure_file("${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
    "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql" COPYONLY)

configure_file("${PGROUTING_SOURCE_DIR}/src/common/sql/pgrouting.control.in"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control")

# this will create the commands in the makefile under the install target
if(UNIX)
    file(GLOB_RECURSE LIBS_TO_INSTALL lib/*.so)
else(UNIX)
    file(GLOB_RECURSE LIBS_TO_INSTALL lib/*.dll)
    message(STATUS "LIBS_TO_INSTALL=${LIBS_TO_INSTALL}")
endif(UNIX)

install(FILES ${LIBS_TO_INSTALL}  DESTINATION ${LIB_DIR})

install(FILES
    "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control"
    DESTINATION "${SHARE_DIR}")


