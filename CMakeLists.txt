#
# Format convention
# - command (lower case)
# - VARIABLE (upper case)
# (http://www.cmake.org/cmake/help/v2.8.10/cmake.html)
#

cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(PGROUTING)

#set(CMAKE_VERBOSE_MAKEFILE 1)

# Configure options
#option(WITH_TSP
#    "Set ON|OFF (default=OFF) to build TSP library" OFF)

option(WITH_DD
    "Set ON|OFF (default=ON) to build Driving distance library" ON)

option(WITH_DOC
    "Set ON|OFF (default=OFF) to build Documentation library tree" OFF)

option(BUILD_LATEX
    "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)

option(BUILD_HTML
    "Set ON|OFF (default=ON) to build Documentation library tree as HTML" ON)

option(BUILD_MAN
    "Set ON|OFF (default=ON) to build Documentation library tree as MAN page" ON)

#-----------------------------------------------------------------------------
# PGROUTING version number.
set(PGROUTING_VERSION_MAJOR "2")
set(PGROUTING_VERSION_MINOR "0")
set(PGROUTING_VERSION_PATCH "0")

set(PGROUTING_VERSION_STRING "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}.${PGROUTING_VERSION_PATCH}")

set(POSTGRESQL_MINIMUM_VERSION "8.4.0")

file(STRINGS "${PGROUTING_SOURCE_DIR}/VERSION" PGROUTING_BUILD_STRING)

string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\1" PGROUTING_GIT_TAG ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\2" PGROUTING_GIT_BUILD ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\3" PGROUTING_GIT_HASH ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\4" PGROUTING_GIT_BRANCH ${PGROUTING_BUILD_STRING})

set(PGROUTING_VERSION_REVISION_NAME "${PGROUTING_GIT_HASH} ${PGROUTING_GIT_BRANCH}")
set(PGROUTING_VERSION_REVISION_HASH "${PGROUTING_GIT_HASH}")

message(STATUS "PGROUTING_GIT_TAG: ${PGROUTING_GIT_TAG}")
message(STATUS "PGROUTING_GIT_BUILD: ${PGROUTING_GIT_BUILD}")
message(STATUS "PGROUTING_GIT_HASH: ${PGROUTING_GIT_HASH}")
message(STATUS "PGROUTING_GIT_BRANCH: ${PGROUTING_GIT_BRANCH}")
message(STATUS "PGROUTING_VERSION_REVISION_NAME: ${PGROUTING_VERSION_REVISION_NAME}")
message(STATUS "PGROUTING_VERSION_REVISION_HASH: ${PGROUTING_VERSION_REVISION_HASH}")

#-----------------------------------------------------------------------------
# CMAKE configuration
# This is for my mingw64 environment, it should go away
set(CMAKE_PREFIX_PATH /usr:/usr/local)
set(CMAKE_INCLUDE_PATH /usr:/usr/local)
set(CMAKE_LIBRARY_PATH /usr:/usr/local)

add_subdirectory(cmake)
set(CMAKE_MODULE_PATH "${PGROUTING_SOURCE_DIR}/cmake")

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
  if (NOT BOOST_ROOT)
    set(BOOST_ROOT c:/ming${OS_BUILD}/msys/local)
  endif(NOT BOOST_ROOT)
   
  if (NOT CGAL_ROOT)
    set(CGAL_ROOT c:/ming${OS_BUILD}/msys/local)
  endif(NOT CGAL_ROOT)
  
  if (NOT GMP_ROOT)
    set(GMP_ROOT c:/ming${OS_BUILD}/mingw${OS_BUILD})
  endif(NOT GMP_ROOT)
  message(STATUS "OS_BUILD=${OS_BUILD}")
  message(STATUS "CGAL_ROOT=${CGAL_ROOT}")
  message(STATUS "GMP_ROOT=${GMP_ROOT}")
  message(STATUS "BOOST_ROOT=${BOOST_ROOT}")
  message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
endif(WIN32)

# Check if source contains .git directory
#if(EXISTS "${PGROUTING_SOURCE_DIR}/.git")
#  # Find Git to collect version information
#  find_package(Git QUIET)
#  if(GIT_FOUND)
#    GIT_WC_INFO(${PGROUTING_SOURCE_DIR} PGROUTING)
#    set(PGROUTING_VERSION_REVISION_NAME "${PGROUTING_WC_REVISION_NAME}")
#    set(PGROUTING_VERSION_REVISION_HASH "${PGROUTING_WC_REVISION_HASH}")
#  else()
#    message(STATUS "Could NOT find GIT (optional)")
#    set(PGROUTING_VERSION_REVISION_NAME "")
#    set(PGROUTING_VERSION_REVISION_HASH "")
#  endif()
#else()
#  message(STATUS "Source is not a GIT repository")
#  set(PGROUTING_VERSION_REVISION_NAME "")
#  set(PGROUTING_VERSION_REVISION_HASH "")
#endif()

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
  message(STATUS "Boost headers were found here: ${Boost_INCLUDE_DIR}")
else(Boost_INCLUDE_DIRS)
  message(FATAL_ERROR " Please check your Boost installation ") 
endif(Boost_INCLUDE_DIRS)

if(NOT POSTGRESQL_VERSION_STRING)
  message(FATAL_ERROR "pg_config was not found. Please check your PostgreSQL installation!")
endif(NOT POSTGRESQL_VERSION_STRING)

message(STATUS "POSTGRESQL_VERSION_STRING is ${POSTGRESQL_VERSION_STRING}")
string(SUBSTRING "${POSTGRESQL_VERSION_STRING}" 11 -1 POSTGRESQL_VERSION)
message(STATUS "POSTGRESQL_VERSION is ${POSTGRESQL_VERSION}")
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1\\2" PGSQL_VERSION ${POSTGRESQL_VERSION})
message(STATUS "PGSQL_VERSION is ${PGSQL_VERSION}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPGSQL_VERSION=${PGSQL_VERSION}")

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

set(LIBRARY_INSTALL_PATH ${LIB_DIR})

execute_process(
  COMMAND ${POSTGRESQL_PG_CONFIG} --sharedir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE SHARE_DIR)

if(SHARE_DIR)
  # Always install in "contrib" directory of PostgreSQL
  set(CONTRIB_DIR "${SHARE_DIR}/contrib/pgrouting-${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}")
  message(STATUS "Contrib directory for SQL files is set to ${CONTRIB_DIR}")

  # Install as extension if supported by the version of PostgreSQL
  if(USE_PG_EXTENSION)
    set(SHARE_DIR "${SHARE_DIR}/extension")
    message(STATUS "Extension directory for SQL files is set to ${SHARE_DIR}")
  #else(USE_PG_EXTENSION)
  #  set(SHARE_DIR "/usr/share/pgrouting")
  endif(USE_PG_EXTENSION)
else(SHARE_DIR)
  message(FATAL_ERROR "pg_config --sharedir failed to return a value. Please check your PostgreSQL installation!")
endif(SHARE_DIR)

# Handles documentation
add_subdirectory(doc)

set(PGROUTING_INCLUDE_DIRECTORIES
    ${PGROUTING_SOURCE_DIR}/src/astar/src
    ${PGROUTING_SOURCE_DIR}/src/dijkstra/src
    ${PGROUTING_SOURCE_DIR}/src/kdijkstra/src
    ${PGROUTING_SOURCE_DIR}/src/apsp_johnson/src
    ${PGROUTING_SOURCE_DIR}/src/apsp_warshall/src
#    ${PGROUTING_SOURCE_DIR}/src/shooting_star/src
    ${PGROUTING_SOURCE_DIR}/src/tsp/src
    ${PGROUTING_SOURCE_DIR}/src/driving_distance/src
    ${PGROUTING_SOURCE_DIR}/src/trsp/src
    )

include_directories(${PGROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
if(WIN32)
  include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32)
endif(WIN32)

# we need link libpostgres.a on win32 env
if(WIN32)
  message(STATUS "LINK_DIRECTORIES = ${POSTGRESQL_LIBRARIES}")
  link_directories(${POSTGRESQL_LIBRARIES} ${LIB_DIR})
  link_libraries(postgres)
endif(WIN32)

if(APPLE)
  # support GNU(Xcode<4) and Clang(Xcode>=4)
  if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIC -O2 -g -frounding-math")
  else()
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIC -O2 -g")
  endif()
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -g -frounding-math -Wno-deprecated")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -g -Wno-deprecated")
  endif()
elseif(UNIX) # UNIX system variable include UNIX like system(i.e. APPLE and CYGWIN)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIC -O2 -g -frounding-math")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -g -frounding-math -Wno-deprecated")
elseif(WIN32)
  # currently, support MinGW only
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O2 -g -frounding-math")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -frounding-math -Wno-deprecated")
endif()

# List variable to collect module SQL file names
set(PACKAGE_SQL_FILES "")
set(L_PACKAGE_SQL_FILES "")

# Recurse into the subdirectories.
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

add_library(routing_bd
    ${LIBRARY_MODE_TARGET}
    $<TARGET_OBJECTS:bd_astar>
    $<TARGET_OBJECTS:bd_dijkstra>
    )

install(TARGETS routing_bd DESTINATION ${LIBRARY_INSTALL_PATH})

add_library(routing
    ${LIBRARY_MODE_TARGET}
    $<TARGET_OBJECTS:astar>
    $<TARGET_OBJECTS:dijkstra>
    $<TARGET_OBJECTS:kdijkstra>
    $<TARGET_OBJECTS:apsp_johnson>
    $<TARGET_OBJECTS:apsp_warshall>
#    $<TARGET_OBJECTS:shooting_star>
    $<TARGET_OBJECTS:trsp>
    )

install(TARGETS routing DESTINATION ${LIBRARY_INSTALL_PATH})

add_library(routing_vrp
    ${LIBRARY_MODE_TARGET}
    $<TARGET_OBJECTS:vrp_single>
    )

install(TARGETS routing_vrp DESTINATION ${LIBRARY_INSTALL_PATH})


if(APPLE)
    set_target_properties(routing
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
    set_target_properties(routing_bd
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
    set_target_properties(routing_vrp
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)

#include_directories(${PGROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})
#if(WIN32)
#    include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32)
#endif(WIN32)
#add_subdirectory(src/tsp)
#list(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")

if(WITH_DD)
  find_package(CGAL)
  if(NOT CGAL_FOUND)
    message(FATAL_ERROR " Please check your CGAL installation, or set correct path to CGAL_INCLUDE_DIR and CGAL_LIBRARIES.")
  endif(NOT CGAL_FOUND)
  #add_definitions(-DBOOST_ALL_NO_LIB)
  #add_definitions(-DBOOST_THREAD_USE_LIB)
  include_directories(${CGAL_INCLUDE_DIR})
  add_subdirectory(src/driving_distance)
  list(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")
endif(WITH_DD)

#MESSAGE("PACKAGE_SQL_FILES are ${L_PACKAGE_SQL_FILES}")

file(WRITE "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
"-- pgRouting version '${PGROUTING_VERSION_STRING}' extension for postgresql\n")
file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
"-- built WITH_DD = ${WITH_DD}\n")
if(USE_PG_EXTENSION)
    file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
        "-- Complain if script is sourced in pgsql, rather than CREATE EXTENSION\n")
    #file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
    #    "\\echo Use \"CREATE EXTENSION pgrouting\" to load this file. \\quit")
endif(USE_PG_EXTENSION)

cat("${PGROUTING_SOURCE_DIR}/src/common/sql/pgrouting-types.sql"
    "${PGROUTING_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in")


foreach(PACKAGE_SQL_FILE ${L_PACKAGE_SQL_FILES})
    cat(${PACKAGE_SQL_FILE} "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in")
endforeach()

# copy our temporary file into a final file
# this extra set will allow us to place variables in the sql file
# that will get substituted in this step before they are installed
configure_file("${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
    "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql")

configure_file("${PGROUTING_SOURCE_DIR}/src/common/sql/pgrouting.control.in"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control")

# We will copy the pgrouting_legacy.sql, but not install it as part of 
# the extension. TODO: we might want to also include a 
# pgrouting_legacy_uninstall.sql file or add it to the extension
configure_file("${PGROUTING_SOURCE_DIR}/src/common/sql/pgrouting_legacy.sql"
    "${CMAKE_BINARY_DIR}/lib/pgrouting_legacy.sql" COPYONLY)

# this will create the commands in the makefile under the install target
if(UNIX)
    file(GLOB_RECURSE LIBS_TO_INSTALL "${CMAKE_BINARY_DIR}/lib/*.so")
else(UNIX)
    file(GLOB_RECURSE LIBS_TO_INSTALL "${CMAKE_BINARY_DIR}/lib/*.dll")
endif(UNIX)

#message(STATUS "LIBS_TO_INSTALL=${LIBS_TO_INSTALL}")

install(FILES ${LIBS_TO_INSTALL}  DESTINATION ${LIB_DIR})

install(FILES
    "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control"
    "${CMAKE_BINARY_DIR}/lib/pgrouting_legacy.sql"
    DESTINATION "${SHARE_DIR}")

# The following probably could be done better
# The idea is to also have a "pgrouting-x.x" directory in PostgreSQL contrib
configure_file("${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.sql" COPYONLY)

install(FILES
    "${CMAKE_BINARY_DIR}/lib/pgrouting.sql"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control"
    "${CMAKE_BINARY_DIR}/lib/pgrouting_legacy.sql"
    DESTINATION "${CONTRIB_DIR}")

# TODO: The following probably should be done better
if(WITH_DD)
  # We will copy the routing_dd_legacy.sql, but not install it as part of 
  # the extension. TODO: we might want to also include a 
  # routing_dd_legacy_uninstall.sql file or add it to the extension
  configure_file("${PGROUTING_SOURCE_DIR}/src/driving_distance/sql/routing_dd_legacy.sql"
      "${CMAKE_BINARY_DIR}/lib/pgrouting_dd_legacy.sql" COPYONLY)

  install(FILES
      "${CMAKE_BINARY_DIR}/lib/pgrouting_dd_legacy.sql"
      DESTINATION "${SHARE_DIR}")

  install(FILES
      "${CMAKE_BINARY_DIR}/lib/pgrouting_dd_legacy.sql"
      DESTINATION "${CONTRIB_DIR}")
endif(WITH_DD)

