#
# Format convention
# - command (lower case)
# - VARIABLE (upper case)
# (http://www.cmake.org/cmake/help/v2.8.12/cmake.html)
#

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#---------------------------------------------
# Setting kind of build
#---------------------------------------------
#
# On Master cmake -DMAKE_BUILD_TYPE=Release
#----------------------


message(STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
if(NOT CMAKE_BUILD_TYPE)
    if (UNIX)
        set(CMAKE_BUILD_TYPE DEBUG CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo
            MinSizeRel."
            FORCE)
        message("Running as: Debug")
    else()
        set(CMAKE_BUILD_TYPE RELEASE CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo
            MinSizeRel."
            FORCE)
        message("Running as: Release")
    endif()
endif()
message(STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")


#---------------------------------------------
#---------------------------------------------
#---------------------------------------------

project(PGROUTING)

#---------------------------------------------
#---------------------------------------------
#---------------------------------------------


#---------------------------------------------
# minimum versions
#---------------------------------------------
set(DOXYGEN_MINIMUM_VERSION "1.8.6")
set(SPHINX_MINIMUM_VERSION "1.1")

message(STATUS "DOXYGEN_MINIMUM_VERSION=${DOXYGEN_MINIMUM_VERSION}")
message(STATUS "SPHINX_MINIMUM_VERSION=${SPHINX_MINIMUM_VERSION}")


#---------------------------------------------
# C++ Compiler requirements
#---------------------------------------------


include(CheckCXXCompilerFlag)

if (NOT MSVC) 
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    message(STATUS "COMPILER_SUPPORTS_CXX0X= ${COMPILER_SUPPORTS_CXX0X}")
    message(STATUS "CMAKE_CXX_COMPILER= ${CMAKE_CXX_COMPILER}")
    if(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
        CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
        message(STATUS "COMPILER_SUPPORTS_CXX11= ${COMPILER_SUPPORTS_CXX11}")
        if(COMPILER_SUPPORTS_CXX11)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        else()
            message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++0x nor C++11 support.
            Please use a different C++ compiler.")
        endif()
    endif()
endif()

#-----------------------------------------------------------------------------
# PGROUTING version number.
set(PGROUTING_VERSION_MAJOR "2")
set(PGROUTING_VERSION_MINOR "3")
set(PGROUTING_VERSION_PATCH "0")

set(PGROUTING_VERSION_STRING "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}.${PGROUTING_VERSION_PATCH}")
set(PGROUTING_VERSION "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}")

set(POSTGRESQL_MINIMUM_VERSION "9.1.0")
set(POSTGIS_MINIMUM_VERSION "2.0.0")

file(STRINGS "${PGROUTING_SOURCE_DIR}/VERSION" PGROUTING_BUILD_STRING)

set(PGROUTING_GIT_TAG "pgrouting-2.3.0-dev")

#string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\1" PGROUTING_GIT_TAG ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^(.+)-([^-]+)-[0-9]+-g([0-9a-f]+) (.*)$" "\\2" PGROUTING_GIT_BUILD ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\3" PGROUTING_GIT_HASH ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\4" PGROUTING_GIT_BRANCH ${PGROUTING_BUILD_STRING})

set(PGROUTING_VERSION_REVISION_NAME "${PGROUTING_GIT_HASH} ${PGROUTING_GIT_BRANCH}")
set(PGROUTING_VERSION_REVISION_HASH "${PGROUTING_GIT_HASH}")

message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
message(STATUS "PGROUTING_GIT_TAG: ${PGROUTING_GIT_TAG}")
message(STATUS "PGROUTING_GIT_BUILD: ${PGROUTING_GIT_BUILD}")
message(STATUS "PGROUTING_GIT_HASH: ${PGROUTING_GIT_HASH}")
message(STATUS "PGROUTING_GIT_BRANCH: ${PGROUTING_GIT_BRANCH}")
message(STATUS "PGROUTING_VERSION_REVISION_NAME: ${PGROUTING_VERSION_REVISION_NAME}")

message(STATUS "PGROUTING_SOURCE_DIR: ${PGROUTING_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

message(STATUS "PGROUTING_VERSION_REVISION_HASH: ${PGROUTING_VERSION_REVISION_HASH}")
message(STATUS "POSTGRESQL_BIN: ${POSTGRESQL_BIN}")



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

if(WIN32 AND NOT MSVC)
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
endif(WIN32 AND NOT MSVC)


include(FindPerl)
if(NOT PERL_EXECUTABLE)
    message(FATAL_ERROR " Please check your Perl installation.")
endif(NOT PERL_EXECUTABLE)

find_package(PostgreSQL)
if(NOT POSTGRESQL_FOUND)
    message(FATAL_ERROR " Please check your PostgreSQL installation.") 
endif(NOT POSTGRESQL_FOUND)

#set(Boost_DEBUG ON)
find_package(Boost)
if(Boost_INCLUDE_DIRS)
    message(STATUS "Boost headers were found here: ${Boost_INCLUDE_DIRS}")
    #find_file(MYVAR geometry.hpp)
    #message(STATUS "geometry.hpp  found here: ${MYVAR}")
else(Boost_INCLUDE_DIRS)
    message(FATAL_ERROR " Please check your Boost installation ") 
endif(Boost_INCLUDE_DIRS)

find_package(CGAL)
if(NOT CGAL_FOUND)
    message(FATAL_ERROR " Please check your CGAL installation, or set correct path to CGAL_INCLUDE_DIR and CGAL_LIBRARIES.")
endif(NOT CGAL_FOUND)

if(NOT POSTGRESQL_VERSION_STRING)
    message(FATAL_ERROR "pg_config was not found. Please check your PostgreSQL installation!")
endif(NOT POSTGRESQL_VERSION_STRING)

message(STATUS "POSTGRESQL_VERSION_STRING is ${POSTGRESQL_VERSION_STRING}")
string(SUBSTRING "${POSTGRESQL_VERSION_STRING}" 11 -1 POSTGRESQL_VERSION)
message(STATUS "POSTGRESQL_VERSION is ${POSTGRESQL_VERSION}")
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1\\2" PGSQL_VERSION ${POSTGRESQL_VERSION})
message(STATUS "PGSQL_VERSION is ${PGSQL_VERSION}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPGSQL_VERSION=${PGSQL_VERSION}")

#TODO maybe needs a wrapper for visual studio
if(WIN32 AND MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_SCL_SECURE_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_SCL_SECURE_NO_WARNINGS")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_NONSTDC_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -EHsc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")
endif()

if("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")
    message(FATAL_ERROR " PostgreSQL ${POSTGRESQL_MINIMUM_VERSION} or greater is required.")
endif("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")


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
    set(SHARE_DIR "${SHARE_DIR}/extension")
    message(STATUS "Extension directory for SQL files is set to ${SHARE_DIR}")
else(SHARE_DIR)
    message(FATAL_ERROR "pg_config --sharedir failed to return a value. Please check your PostgreSQL installation!")
endif(SHARE_DIR)

#-------------------------------------
# Handles documentation
#-------------------------------------
set(PROJECT_NAME  "pgRouting")
set(PROJECT_BRIEF "pgRouting extends the PostGIS / PostgreSQL geospatial database to provide geospatial routing functionality.")
set(PROJECT_NUMBER  "${PGROUTING_GIT_TAG}")
add_subdirectory(doxygen)
add_subdirectory(doc)
#-------------------------------------

#include directories: files in alphabetical order
set(PGROUTING_INCLUDE_DIRECTORIES
    ${PGROUTING_SOURCE_DIR}/src/common/src
    ${PGROUTING_SOURCE_DIR}/src/dijkstra/src
    )

include_directories(${PGROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${CGAL_INCLUDE_DIR})
message(STATUS "PGROUTING_INCLUDE_DIRECTORIES = ${PGROUTING_INCLUDE_DIRECTORIES}")
message(STATUS "POSTGRESQL_INCLUDE_DIR = ${POSTGRESQL_INCLUDE_DIR}")
message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()



if(WIN32)
    include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32)
    if(MSVC)
        include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32_msvc/)
    endif(MSVC)
endif(WIN32)

# we need link libpostgres.a on win32 env
if(WIN32)
    message(STATUS "LINK_DIRECTORIES = ${POSTGRESQL_LIBRARIES}")
    link_directories(${POSTGRESQL_LIBRARIES} ${LIB_DIR})
    link_libraries(postgres)
endif(WIN32)

# TODO default compilation with -DNDEBUG 

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#compiler directives
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

if(UNIX)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}  -std=gnu99 ")
endif()


CHECK_C_COMPILER_FLAG("-frounding-math" C_COMPILER_SUPPORTS_ROUNDING_MATH)
CHECK_CXX_COMPILER_FLAG("-frounding-math" CXX_COMPILER_SUPPORTS_ROUNDING_MATH)
if(C_COMPILER_SUPPORTS_ROUNDING_MATH)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -frounding-math")
endif()
if(CXX_COMPILER_SUPPORTS_ROUNDING_MATH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frounding-math")
endif()


if(NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")
endif()


set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}  -Wall -Wconversion -pedantic -fmax-errors=10  -Wmissing-prototypes")
#TODO remove the optimizer falg when pgr_bdijkstra gets fixed
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -O1 -fno-implicit-inline-templates -Wconversion -pedantic -fmax-errors=10 -Wunused -Wuninitialized -Wextra")

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if(CDEBUG)
message(STATUS "CMAKE_CXX_FLAGS_DEBUG= ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE= ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "CMAKE_CXX_FLAGS= ${CMAKE_CXX_FLAGS}")

message(STATUS "CMAKE_C_FLAGS_DEBUG= ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "CMAKE_C_FLAGS_RELEASE= ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "CMAKE_C_FLAGS= ${CMAKE_C_FLAGS}")

message(STATUS "UNIX=${UNIX}")
message(STATUS "WIN32=${WIN32}")
message(STATUS "MSVC=${MSVC}")
#endif()

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


# List variable to collect module SQL file names
set(PACKAGE_SQL_FILES "")
set(L_PACKAGE_SQL_FILES "")

#==========================
#  PgRouting_SOURCE_NAMES
#==========================

set(PgRouting_SOURCE_NAMES
    "common"


    "dijkstra"
    "driving_distance"
    "ksp"
    "allpairs"
    "withPoints"

    #"internalQueryTests"
    #"funnyDijkstra"

    "alpha_shape"

    "astar"
    "bd_astar"
    "bd_dijkstra"
    "trsp" #2.0
    "tsp" #2.0

    "vrp_basic"
    "pickDeliver"
    "contraction"

    )

#-------------------
# add the subdirectories that have the C/C++ code 
#-------------------

foreach (subdir ${PgRouting_SOURCE_NAMES})
    add_subdirectory("${PGROUTING_SOURCE_DIR}/src/${subdir}/src")
endforeach()

#-----------------------------------------------------------------------------
# List variable to collect module SQL file names
#-----------------------------------------------------------------------------
set(PgRouting_SQL_DIRECTORIES ${PgRouting_SOURCE_NAMES}
    "apsp_johnson"  #V2.0
    "apsp_warshall" #V2.0
    "kdijkstra"     #V2.0
    "vrppdtw" #2.1

    "convenience"
    "topology"
    "label_graph"
    "costMatrix"

    )


set(PACKAGE_SQL_FILES "")
set(LIST_PACKAGE_SQL_FILES "")

foreach (subdir ${PgRouting_SQL_DIRECTORIES})
    add_subdirectory("${PGROUTING_SOURCE_DIR}/src/${subdir}/sql")
endforeach()

list(APPEND LIST_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")

if (CDEBUG)
    foreach (files ${LIST_PACKAGE_SQL_FILES})
        message("${files}")
    endforeach()
endif()


#add_subdirectory(src)
#list(APPEND L_PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")


# assemble the object files from the src functions into
# a single library librouting.so
set(LIBRARY_OUTPUT_PATH lib)

if(APPLE)
    set(LIBRARY_MODE_TARGET "MODULE")
else(APPLE)
    set(LIBRARY_MODE_TARGET "SHARED")
endif(APPLE)


LINK_LIBRARIES(${CGAL_LIBRARIES} ${GMP_LIBRARIES} ${BOOST_THREAD_LIBRARIES})

#-------------------
# pgRouting objects to be linked
#-------------------
foreach (subdir ${PgRouting_SOURCE_NAMES} )
    set(PGROUTING_OBJECTS ${PGROUTING_OBJECTS} "$<TARGET_OBJECTS:${subdir}>")
endforeach()


#-----------------------------------------------------------------------------
# PGROUTING Library names
#-----------------------------------------------------------------------------

message("Building the library names")

#----------------------
# PGROUTING installation files names have the following name
#----------------------
set(PGROUTING_LIB_NAME "pgrouting-${PGROUTING_VERSION}")


#----------------------
# PGROUTING library name used in the pgr_functions
#  '$libdir/${PGROUTING_LIBRARY_NAME}'
#----------------------
set(PGROUTING_LIBRARY_NAME "libpgrouting-${PGROUTING_VERSION}")

add_library(${PGROUTING_LIB_NAME}
    ${LIBRARY_MODE_TARGET}
    ${PGROUTING_OBJECTS})

install(TARGETS ${PGROUTING_LIB_NAME} DESTINATION ${LIBRARY_INSTALL_PATH})



if(APPLE)
    set_target_properties(${PGROUTING_LIB_NAME}
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)
if(WIN32 AND MSVC)
    set_target_properties(${PGROUTING_LIB_NAME} PROPERTIES PREFIX "lib")
endif(WIN32 AND MSVC)


file(WRITE "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
    "-- pgRouting version '${PGROUTING_VERSION_STRING}' extension for postgresql\n")

file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
    "-- Complain if script is sourced in pgsql, rather than CREATE EXTENSION\n")
#file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
#    "\\echo Use \"CREATE EXTENSION pgrouting\" to load this file. \\quit")



foreach(PACKAGE_SQL_FILE ${LIST_PACKAGE_SQL_FILES})
    cat(${PACKAGE_SQL_FILE} "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in")
endforeach()

# copy our temporary file into a final file
# this extra set will allow us to place variables in the sql file
# that will get substituted in this step before they are installed
configure_file("${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
    "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql")

configure_file("${PGROUTING_SOURCE_DIR}/src/common/sql/pgrouting.control.in"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control")

# create the signature file for this release
message(STATUS "Creating lib/pgrouting--${PGROUTING_VERSION_STRING}.sig")
execute_process(
    COMMAND ${PERL_EXECUTABLE} ${PGROUTING_SOURCE_DIR}/tools/mk-signature-file ${PGROUTING_VERSION_STRING}
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    RESULT_VARIABLE SIG_RESULT
    )
if(SIG_RESULT)
    message(FATAL_ERROR "    SIG_RESULT: ${SIG_RESULT}")
endif(SIG_RESULT)

# create the extension update scripts
execute_process(
    COMMAND ${PERL_EXECUTABLE} ${PGROUTING_SOURCE_DIR}/tools/build-extension-update-files ${PGROUTING_VERSION_STRING} ${PGROUTING_SOURCE_DIR}
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    RESULT_VARIABLE UPDATE_RESULT
    )
if(UPDATE_RESULT)
    message(FATAL_ERROR "   UPDATE_RESULT: ${UPDATE_RESULT}")
endif(UPDATE_RESULT)

# make a list of the files we need to install for version updates
file(GLOB VERSION_UPDATE_FILES "${CMAKE_BINARY_DIR}/lib/pgrouting--*--*.sql")

# this will create the commands in the makefile under the install target
if(UNIX)
    file(GLOB_RECURSE LIBS_TO_INSTALL "${CMAKE_BINARY_DIR}/lib/*.so")
else(UNIX)
    file(GLOB_RECURSE LIBS_TO_INSTALL "${CMAKE_BINARY_DIR}/lib/*.dll")
endif(UNIX)

message(STATUS "LIBS_TO_INSTALL=${LIBS_TO_INSTALL}")

install(FILES ${LIBS_TO_INSTALL}  DESTINATION ${LIB_DIR})

install(FILES
    "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control"
    ${VERSION_UPDATE_FILES}
    DESTINATION "${SHARE_DIR}")

