#
# Format convention
# - command (lower case)
# - VARIABLE (upper case)
# (http://www.cmake.org/cmake/help/v2.8.10/cmake.html)
#

cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(PGROUTING)

set(PGROUTING_VERSION_MAJOR "2")
set(PGROUTING_VERSION_MINOR "2")
set(PGROUTING_VERSION_PATCH "0")
set(POSTGRESQL_MINIMUM_VERSION "9.1.0")
set(POSTGIS_MINIMUM_VERSION "2.0.0")

#set(CMAKE_VERBOSE_MAKEFILE 1)

# Configure options

option(WITH_TEST 
    "Set ON|OFF (default=ON) to perform the tests" OFF)

option(WITH_DOC
    "Set ON|OFF (default=OFF) to build Documentation library tree" OFF)

option(BUILD_LATEX
    "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)

option(BUILD_HTML
    "Set ON|OFF (default=ON) to build Documentation library tree as HTML" ON)

option(BUILD_MAN
    "Set ON|OFF (default=ON) to build Documentation library tree as MAN page" OFF)

set(POSTGRESQL_VERSION "" CACHE STRING
    "Set version number for postgreSQL version" )

set(POSTGIS_VERSION "" CACHE STRING
    "Set version number for postGIS version")

option(PGR_DEBUG "Set ON|OFF (default=ON) to build Documentation library tree as MAN page" ON)

SET(POSTGRESQL_BIN "" CACHE STRING "Define optional path to PostgreSQL binaries an a non-standard location.")


#- ----------------------------------------------------------------------------
# USERS FLAGS

#overriding ON flags when no documentation is asked
if(NOT WITH_DOC)
  set(BUILD_HTML OFF) 
  set(BUILD_LATEX OFF) 
  set(BUILD_MAN OFF) 
endif(NOT WITH_DOC)

if(PGR_DEBUG)
  message(STATUS "WITH_TEST: ${WITH_TEST}")
  message(STATUS "WITH_DOC: ${WITH_DOC}")
  message(STATUS "BUILD_LATEX: ${BUILD_LATEX}")
  message(STATUS "BUILD_HTML: ${BUILD_HTML}")
  message(STATUS "BUILD_MAN: ${BUILD_MAN}")
endif()

#-----------------------------------------------------------------------------
# PGROUTING version number.

set(PGROUTING_VERSION_STRING "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}.${PGROUTING_VERSION_PATCH}")


file(STRINGS "${PGROUTING_SOURCE_DIR}/VERSION" PGROUTING_BUILD_STRING)

string(SUBSTRING ${PGROUTING_BUILD_STRING} 0 7 PGROUTING_GIT_HASH)
string(SUBSTRING ${PGROUTING_BUILD_STRING} 8 -1 PGROUTING_GIT_BRANCH)

set(PGROUTING_VERSION_REVISION_NAME "${PGROUTING_GIT_HASH} ${PGROUTING_GIT_BRANCH}")
set(PGROUTING_LIB_NAME "pgrouting--${PGROUTING_VERSION_STRING}")
set(PGROUTING_LIBRARY_NAME "pgrouting--${PGROUTING_VERSION_STRING}")

if(PGR_DEBUG)
  message(STATUS "PGROUTING_VERSION_STRING: ${PGROUTING_VERSION_STRING}")
  message(STATUS "PGROUTING_GIT_HASH: ${PGROUTING_GIT_HASH}")
  message(STATUS "PGROUTING_GIT_BRANCH: ${PGROUTING_GIT_BRANCH}")
  message(STATUS "PGROUTING_VERSION_REVISION_NAME: ${PGROUTING_VERSION_REVISION_NAME}")
  message(STATUS "PGROUTING_SOURCE_DIR: ${PGROUTING_SOURCE_DIR}")
  message(STATUS "PGROUTING_LIB_NAME: ${PGROUTING_LIB_NAME}")
  message(STATUS "PGROUTING_LIBRARY_NAME: ${PGROUTING_LIBRARY_NAME}")
endif()


#-----------------------------------------------------------------------------
# CMAKE configuration

add_subdirectory(cmake)
set(CMAKE_MODULE_PATH "${PGROUTING_SOURCE_DIR}/cmake")

include(Cat)
include(TargetDistclean)
include(FindPackageHandleStandardArgs)

if(PGR_DEBUG)
  message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
  message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
endif()


#-----------------------------------------------------------------------------
# Detecting C compiler
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#-----------------------------------------------------------------------------
# Detecting Operating System

if(PGR_DEBUG)
  message(STATUS "UNIX=${UNIX}")
  message(STATUS "WIN32=${WIN32}")
endif()

#-----------------------------------------------------------------------------
# WIN32
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


#-----------------------------------------------------------------------------
# pgRouting Boost requirements
# -DBoost_DEBUG=On  to debug

message(STATUS "* Checking pgRouting Boost requirements *")

#message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
if(BOOST_ROOT)
  find_package(Boost QUIET COMPONENTS  graph REQUIRED)
  if ( NOT Boost_FOUND OR NOT Boost_GRAPH_FOUND )
    message(FATAL_ERROR "  Package Boost or Boost.Graph  required, but not found!")
  endif ( NOT Boost_FOUND OR NOT Boost_GRAPH_FOUND )
else(BOOST_ROOT)

  find_package(Boost)
  if(!Boost_INCLUDE_DIRS)
    message(FATAL_ERROR "  Boost Include directory not found!!!") 
  endif(!Boost_INCLUDE_DIRS)
endif(BOOST_ROOT)


message(STATUS "pgRouting Boost requirements: ...OK")



#-----------------------------------------------------------------------------
# pgRouting CGAL requirements
#CGAL is used by A shape

message(STATUS "* Checking pgRouting CGAL requirements *")

find_package(CGAL QUIET REQUIRED)
if(NOT CGAL_FOUND)
  message(FATAL_ERROR "Package CGAL required, but not found!")
endif(NOT CGAL_FOUND)

message(STATUS "pgRouting CGAL requirements: ...OK")



#-----------------------------------------------------------------------------
# Perl is for the test environment

if(WITH_TEST)
  message(STATUS "* TESTS are requiered, Checking perl installation. *")

  include(FindPerl)
  if(NOT PERL_EXECUTABLE)
    message(FATAL_ERROR " Perl not found. Please check your perl installation.")
  endif(NOT PERL_EXECUTABLE)
  message(STATUS "pgRouting perl requirements: ...OK")

endif(WITH_TEST)


#-----------------------------------------------------------------------------
# POSTGRESQL is compulsory
# This module defines
#  PostgreSQL_LIBRARIES - the PostgreSQL libraries needed for linking
#  PostgreSQL_INCLUDE_DIRS - the directories of the PostgreSQL headers
#  PostgreSQL_VERSION_STRING - the version of PostgreSQL found (since CMake 2.8.8)


message(STATUS "* Checking PostgreSQL installation. *")

message(STATUS " POSTGRESQL_VERSION: ${POSTGRESQL_VERSION}")
set(PostgreSQL_ADDITIONAL_VERSIONS  ${POSTGRESQL_VERSION})

#set(PostgreSQL_ROOT /usr/lib/postgresql/${POSTGRESQL_VERSION}

find_package(PostgreSQL)
if(NOT PostgreSQL_FOUND)
  message(FATAL_ERROR "... PostgreSQL: not found.") 
endif(NOT PostgreSQL_FOUND)

if(POSTGRESQL_VERSION)
  set(PostgreSQL_VERSION  ${POSTGRESQL_VERSION})
else()
  string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1.\\2" PostgreSQL_VERSION ${PostgreSQL_VERSION_STRING})
endif()

set(POSTGRESQL_PG_CONFIG /usr/lib/postgresql/${PostgreSQL_VERSION}/bin/pg_config)

# PGSQL_VERSION is a compiler directive
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1\\2" PGSQL_VERSION ${PostgreSQL_VERSION})


if("${PostgreSQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")
  message(FATAL_ERROR "PostgreSQL ${POSTGRESQL_MINIMUM_VERSION} or greater is required. PostgreSQL ${PostgreSQL_VERSION} found!!!")
endif()

execute_process(
  COMMAND ${POSTGRESQL_PG_CONFIG} --pkglibdir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE LIB_DIR)

if(NOT LIB_DIR)
  message(FATAL_ERROR "pg_config --pkglibdir failed to return a value. Please check your PostgreSQL installation!")
endif()
set(LIBRARY_INSTALL_PATH ${LIB_DIR})

execute_process(
  COMMAND ${POSTGRESQL_PG_CONFIG} --sharedir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE SHARE_DIR)

if(SHARE_DIR)
  set(SHARE_DIR "${SHARE_DIR}/extension")
else(SHARE_DIR)
  message(FATAL_ERROR "pg_config --sharedir failed to return a value. Please check your PostgreSQL installation!")
endif(SHARE_DIR)

if(PGR_DEBUG)
  message(STATUS "  PostgreSQL_FOUND ${PostgreSQL_FOUND}")
  message(STATUS "  PostgreSQL_LIBRARIES is ${PostgreSQL_LIBRARIES}")
  message(STATUS "  PostgreSQL_INCLUDE_DIRS is ${PostgreSQL_INCLUDE_DIRS}")
#  message(STATUS "  PostgreSQL_VERSION_STRING is ${PostgreSQL_VERSION_STRING}")
  message(STATUS "  PostgreSQL_VERSION is ${PostgreSQL_VERSION}")
  message(STATUS "  POSTGRESQL_PG_CONFIG is " ${POSTGRESQL_PG_CONFIG})
  message(STATUS "  PGSQL_VERSION is ${PGSQL_VERSION}")
  message(STATUS "  POSTGRESQL_BIN: ${POSTGRESQL_BIN}")
  message(STATUS "  Extension directory for SQL files is set to ${SHARE_DIR}")
  message(STATUS "  Install directory for libraries is set to ${LIBRARY_INSTALL_PATH}")
endif(PGR_DEBUG)

message(STATUS "PostgreSQL requirements: ...OK")

#---------------------------------------------------------------------------------
# Handles documentation

if(WITH_DOC)
  add_subdirectory(doc)
endif(WITH_DOC)

#---------------------------------------------------------------------------------
#  Include directories: files in alphabetical order
set(PGROUTING_INCLUDE_DIRECTORIES
    ${PGROUTING_SOURCE_DIR}/src/apsp_johnson/src
    ${PGROUTING_SOURCE_DIR}/src/apsp_warshall/src
    ${PGROUTING_SOURCE_DIR}/src/astar/src
    ${PGROUTING_SOURCE_DIR}/src/bd_astar/src
    ${PGROUTING_SOURCE_DIR}/src/bd_dijkstra/src
    ${PGROUTING_SOURCE_DIR}/src/common/src
    ${PGROUTING_SOURCE_DIR}/src/dijkstra/src
    ${PGROUTING_SOURCE_DIR}/src/driving_distance/src
    ${PGROUTING_SOURCE_DIR}/src/kdijkstra/src
    ${PGROUTING_SOURCE_DIR}/src/ksp/src
    ${PGROUTING_SOURCE_DIR}/src/trsp/src
    ${PGROUTING_SOURCE_DIR}/src/tsp/src
    ${PGROUTING_SOURCE_DIR}/src/vrp_basic/src
    ${PGROUTING_SOURCE_DIR}/src/vrppdtw/src
    ${PGROUTING_SOURCE_DIR}/src/warshall/src

# not using C/C++
    ${PGROUTING_SOURCE_DIR}/src/label_graph/src


#not suported but directory exists
#    ${PGROUTING_SOURCE_DIR}/src/shooting_star/src
 )

include_directories(${PGROUTING_INCLUDE_DIRECTORIES} ${PostgreSQL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})



if(WIN32)
  include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32)
endif(WIN32)

# we need link libpostgres.a on win32 env
if(WIN32)
  message(STATUS "LINK_DIRECTORIES = ${PostgreSQL_LIBRARIES}")
  link_directories(${PostgreSQL_LIBRARIES} ${LIB_DIR})
  link_libraries(postgres)
endif(WIN32)


#---------------------------------------------------------------------------------
#compiler directives
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPGSQL_VERSION=${PGSQL_VERSION}")
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
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIC -O2 -g -Wall -frounding-math")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -g -Wall -frounding-math -Wno-deprecated")

elseif(WIN32)
  # currently, support MinGW only
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O2 -g -Wall -frounding-math")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g  -frounding-math -Wno-deprecated")
endif()

#---------------------------------------------------------------------------------
# List variable to collect module SQL file names
set(PACKAGE_SQL_FILES "")
set(L_PACKAGE_SQL_FILES "")

# Recurse into the subdirectories.
add_subdirectory(src)
list(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")

if(PGR_DEBUG)
  message("L_PACKAGE_SQL_FILES: ${PACKAGE_SQL_FILES}")
endif()

#---------------------------------------------------------------------------------
# assemble the object files from the src functions into
# a single library librouting.so
set(LIBRARY_OUTPUT_PATH lib)

if(APPLE)
    set(LIBRARY_MODE_TARGET "MODULE")
else(APPLE)
    set(LIBRARY_MODE_TARGET "SHARED")
endif(APPLE)


LINK_LIBRARIES(${CGAL_LIBRARIES} ${GMP_LIBRARIES} ${BOOST_THREAD_LIBRARIES})

#---------------------------------------------------------------------------------
# libraries listed in alphabetical order.
#add_library(routing-2.2
add_library(${PGROUTING_LIB_NAME}
    ${LIBRARY_MODE_TARGET}
    $<TARGET_OBJECTS:apsp_johnson>
    $<TARGET_OBJECTS:apsp_warshall>
    $<TARGET_OBJECTS:astar>
    $<TARGET_OBJECTS:bd_astar>
    $<TARGET_OBJECTS:bd_dijkstra>
    $<TARGET_OBJECTS:common>
    $<TARGET_OBJECTS:dijkstra>
    $<TARGET_OBJECTS:driving_distance>
    $<TARGET_OBJECTS:kdijkstra>
    $<TARGET_OBJECTS:ksp>
    $<TARGET_OBJECTS:trsp>
    $<TARGET_OBJECTS:tsp>
    $<TARGET_OBJECTS:vrp_basic>
    $<TARGET_OBJECTS:vrppdtw>
    $<TARGET_OBJECTS:warshall>

#not supported but directory exists
#    $<TARGET_OBJECTS:shooting_star>
    )

#install(TARGETS routing-2.2 DESTINATION ${LIBRARY_INSTALL_PATH})
install(TARGETS ${PGROUTING_LIB_NAME} DESTINATION ${LIBRARY_INSTALL_PATH})


if(APPLE)
#    set_target_properties(routing-2.2
    set_target_properties(${PGROUTING_LIB_NAME}
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)


#---------------------------------------------------------------------------------
# Gather in one file all the sql files

#file(WRITE "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
file(WRITE "${CMAKE_BINARY_DIR}/lib/${PGROUTING_LIB_NAME}.sql.in"
    "-- pgRouting version '${PGROUTING_VERSION_STRING}' extension for postgresql\n")

#file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
file(APPEND "${CMAKE_BINARY_DIR}/lib/${PGROUTING_LIB_NAME}.sql.in"
     "-- Complain if script is sourced in pgsql, rather than CREATE EXTENSION\n")
#file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
#    "\\echo Use \"CREATE EXTENSION pgrouting\" to load this file. \\quit")

cat("${PGROUTING_SOURCE_DIR}/src/common/sql/pgrouting-types.sql"
    "${PGROUTING_BINARY_DIR}/lib/${PGROUTING_LIB_NAME}.sql.in")
#   "${PGROUTING_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in")

foreach(PACKAGE_SQL_FILE ${L_PACKAGE_SQL_FILES})
#    cat(${PACKAGE_SQL_FILE} "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in")
    cat(${PACKAGE_SQL_FILE} "${CMAKE_BINARY_DIR}/lib/${PGROUTING_LIB_NAME}.sql.in")
endforeach()


# copy our temporary file into a final file
# this extra set will allow us to place variables in the sql file
# that will get substituted in this step before they are installed
configure_file("${CMAKE_BINARY_DIR}/lib/${PGROUTING_LIB_NAME}.sql.in"
    "${CMAKE_BINARY_DIR}/lib/${PGROUTING_LIB_NAME}.sql")

configure_file("${PGROUTING_SOURCE_DIR}/src/common/sql/pgrouting.control.in"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control")

#---------------------------------------------------------------------------------
# create the signature file for this release
message(STATUS "Creating lib/${PGROUTING_LIB_NAME}.sig")
#message(STATUS "Creating lib/${PGROUTING_VERSION_STRING}.sig")
execute_process(
  COMMAND ${PERL_EXECUTABLE} ${PGROUTING_SOURCE_DIR}/tools/mk-signature-file ${PGROUTING_VERSION_STRING}
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  RESULT_VARIABLE SIG_RESULT
)

if(SIG_RESULT)
    message(FATAL_ERROR "    SIG_RESULT: ${SIG_RESULT}")
endif(SIG_RESULT)

#---------------------------------------------------------------------------------
# create the extension update scripts
execute_process(
  COMMAND ${PERL_EXECUTABLE} ${PGROUTING_SOURCE_DIR}/tools/build-extension-update-files ${PGROUTING_VERSION_STRING} ${PGROUTING_SOURCE_DIR}
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
  RESULT_VARIABLE UPDATE_RESULT
)
if(UPDATE_RESULT)
    message(FATAL_ERROR "   UPDATE_RESULT: ${UPDATE_RESULT}")
endif(UPDATE_RESULT)

#---------------------------------------------------------------------------------
# make a list of the files we need to install for version updates
file(GLOB VERSION_UPDATE_FILES "${CMAKE_BINARY_DIR}/lib/pgrouting--*--*.sql")


# this will create the commands in the makefile under the install target
if(UNIX)
    file(GLOB_RECURSE LIBS_TO_INSTALL "${CMAKE_BINARY_DIR}/lib/*.so")
else(UNIX)
    file(GLOB_RECURSE LIBS_TO_INSTALL "${CMAKE_BINARY_DIR}/lib/*.dll")
endif(UNIX)

message(STATUS "LIBS_TO_INSTALL: ${PGROUTING_LIBRARY_NAME}")

#install(FILES ${LIBS_TO_INSTALL}  DESTINATION ${LIB_DIR})

install(FILES
    "${CMAKE_BINARY_DIR}/lib/${PGROUTING_LIB_NAME}.sql"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control"
    ${VERSION_UPDATE_FILES}
#    "${CMAKE_BINARY_DIR}/lib/pgrouting_legacy.sql"
    DESTINATION "${SHARE_DIR}")

