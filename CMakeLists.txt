#
# Format convention
# - command (lower case)
# - VARIABLE (upper case)
# (http://www.cmake.org/cmake/help/v2.8.10/cmake.html)
#

#TODO Handle developers documentation  to be used by: make dev-doc
#TODO Handle tests to be used by: make test
#TODO some todo around before moving to develop

project(PGROUTING)

#set(CMAKE_VERBOSE_MAKEFILE 1)

# Configure options


option(WITH_DOC
    "Set ON|OFF (default=OFF) to build Documentation library tree" OFF)

option(BUILD_LATEX
    "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)

option(BUILD_HTML
    "Set ON|OFF (default=ON) to build Documentation library tree as HTML" ON)

option(BUILD_MAN
    "Set ON|OFF (default=ON) to build Documentation library tree as MAN page" OFF)

#TODO change to OFF when debuging is finished
option(CDEBUG
    "Set ON|OFF (default=OFF) to build the test infrastucture" ON)

#TODO change to OFF when debuging is finished
option(WITH_TEST
    "Set ON|OFF (default=OFF) to build the test infrastucture" ON)

option(WITH_DEV_DOC
    "Set ON|OFF (default=OFF) to build Documentation library tree" OFF)


option(POSTGRESQL_VERSION " To set the version")


#=============================================================================
#
# PGROUTING Setting of the directories to be processed
#
# The following directories exist but are not being processed
#   shooting_star
#   vrpdptw
#=============================================================================

#==========================
#  PgRouting_SOURCE_NAMES
#==========================
#
#
#  Some ordering is here:
#  First Group
#    Line 1: V 2.1
#    Line 2: V 2.2
#
#  Middle group: V 2.0
#
#  Last group: proposed functions
#  That is directories that didnt exist on V 2.0 but do exist in V 2.1
#  but classification is as proposed (so that bug fixes are not as urgent as
#  for the Middle group
#-----------------------------------------------------------------------------

set(PgRouting_SOURCE_NAMES
    "common" "dijkstra"
    "driving_distance"
    "ksp"
    "allpairs"
    "withPoints"

    #"funnyDijkstra"
    
    "alpha_shape"

    "astar" "bd_astar"
    "bd_dijkstra" 
    "kdijkstra"
    "trsp" #2.0
    "tsp"

    "vrp_basic" "vrppdtw"
    )

#==========================
#  PgRouting_INCLUDE_DIRECTORIES
#==========================
#
# The names of the directories that have C/C++ code
#
#-----------------------------------------------------------------------------

#set(PgRouting_INCLUDE_DIRECTORIES ${PgRouting_SOURCE_NAMES})
set(PgRouting_INCLUDE_DIRECTORIES ${PgRouting_SOURCE_NAMES})

#==========================
#  PgRouting_SQL_DIRECTORIES
#==========================
#
# The names of the directories that have SQL
#
#   - Add names of directories that don't use C/C++ code
#
#-----------------------------------------------------------------------------

set(PgRouting_SQL_DIRECTORIES ${PgRouting_SOURCE_NAMES} 
    "apsp_johnson"  #V2.0
    "apsp_warshall" #V2.0
    "convinience"
    "topology"
    "label_graph")


#==========================
#  PgRouting_SOURCE_NAMES
#==========================
#
# The names of the deprecated directories
#   - but the test must work
#
#-----------------------------------------------------------------------------
set(PgRouting_TEST_DIRECTORIES ${PgRouting_SOURCE_NAMES} 
    "apsp_warshall" "apsp_johnson"
    )






#-----------------------------------------------------------------------------
# PGROUTING Setting the Minimum requirements
#-----------------------------------------------------------------------------

message("Setting the minimum requirements")
#----------------------
# Build requirements 
#----------------------

# compiler with c++0x or c++11

set(POSTGRESQL_MINIMUM_VERSION "9.1.0")
#travis has 2.8.7
set(CMAKE_MINIMUM_VERSION "2.8.7")
set(BOOST_MINIMUM_VERSION "1.46.0")

if (CDEBUG)
    message(STATUS "POSTGRESQL_MINIMUM_VERSION: ${POSTGRESQL_MINIMUM_VERSION}")
    message(STATUS "CMAKE_MINIMUM_VERSION: ${CMAKE_MINIMUM_VERSION}")
    message(STATUS "BOOST_MINIMUM_VERSION: ${BOOST_MINIMUM_VERSION}")
endif()

#----------------------
# Documentation requirements 
#----------------------
# if the requirement is not found WITH_DOC will be set to OFF
# but build will continue

#overriding ON flags when no documentation is asked
if(NOT WITH_DOC)
    set(BUILD_HTML OFF) 
    set(BUILD_LATEX OFF) 
    set(BUILD_MAN OFF) 
endif(NOT WITH_DOC)

if(WITH_DOC)

    set(SPHINX_MINIMUM_VERSION "1.0")

    if (CDEBUG)
        message(STATUS "SPHINX_MINIMUM_VERSION: ${SPHINX_MINIMUM_VERSION}")
    endif()

endif()

#----------------------
# Devlopers Documentation requirements 
#----------------------

if(WITH_DEV_DOC)

    set(DOXYGEN_MINIMUM_VERSION "1.8.7")

    if (CDEBUG)
        message(STATUS "DOXYGEN_MINIMUM_VERSION: ${DOXYGEN_MINIMUM_VERSION}")
    endif()

endif()


#----------------------
# Tests requirements 
#----------------------

if(WITH_TEST)

    set(POSTGIS_MINIMUM_VERSION "2.0.0")

    if (CDEBUG)
        message(STATUS "POSTGIS_MINIMUM_VERSION: ${POSTGIS_MINIMUM_VERSION}")
    endif()

endif()

message(".....OK")



#-----------------------------------------------------------------------------
# PGROUTING version handling.
#-----------------------------------------------------------------------------

message("Building the Version")

#----------------------
# PGROUTING version
#----------------------
set(PGROUTING_VERSION_MAJOR "2")
set(PGROUTING_VERSION_MINOR "2")
set(PGROUTING_VERSION_PATCH "0")

#----------------------
# PGROUTING version string
#----------------------
set(PGROUTING_VERSION_STRING "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}.${PGROUTING_VERSION_PATCH}")



#----------------------
# PGROUTING pgr_version gets filled with this variables
#----------------------
file(STRINGS "${PGROUTING_SOURCE_DIR}/VERSION" PGROUTING_BUILD_STRING)
string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\1" PGROUTING_GIT_TAG ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\2" PGROUTING_GIT_BUILD ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\3" PGROUTING_GIT_HASH ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^(.+)-([0-9]+)-g([0-9a-f]+) (.*)$" "\\4" PGROUTING_GIT_BRANCH ${PGROUTING_BUILD_STRING})


string(COMPARE NOTEQUAL ${PGROUTING_GIT_BRANCH}  "master" PGROUTING_DEVELOP)

set(PGROUTING_VERSION_REVISION_NAME "${PGROUTING_GIT_HASH} ${PGROUTING_GIT_BRANCH}")

if(PGROUTING_DEVELOP)
    set(PGROUTING_GIT_TAG "pgrouting-${PGROUTING_VERSION_STRING}-dev")
endif()

if (CDEBUG)
    message(STATUS "  PGROUTING_GIT_TAG: ${PGROUTING_GIT_TAG}")
    message(STATUS "  PGROUTING_GIT_BUILD: ${PGROUTING_GIT_BUILD}")
    message(STATUS "  PGROUTING_GIT_HASH: ${PGROUTING_GIT_HASH}")
    message(STATUS "  PGROUTING_GIT_BRANCH: ${PGROUTING_GIT_BRANCH}")
    message(STATUS "  PGROUTING_VERSION_REVISION_NAME: ${PGROUTING_VERSION_REVISION_NAME}")
endif()

message(".....OK")




#-----------------------------------------------------------------------------
# PGROUTING Library names
#-----------------------------------------------------------------------------

message("Building the library names")

#----------------------
# PGROUTING installation files names have the following name
#----------------------
set(PGROUTING_LIB_NAME "pgrouting--${PGROUTING_VERSION_STRING}")


#----------------------
# PGROUTING library name used in the pgr_functions
#  '$libdir/${PGROUTING_LIBRARY_NAME}'
#----------------------
set(PGROUTING_LIBRARY_NAME "libpgrouting--${PGROUTING_VERSION_STRING}")


if (CDEBUG)
    message(STATUS "  PGROUTING_LIB_NAME: ${PGROUTING_LIB_NAME}")
    message(STATUS "  PGROUTING_LIBRARY_NAME: ${PGROUTING_LIBRARY_NAME}")
endif()

message(".....OK")




#-----------------------------------------------------------------------------
# CMAKE configuration
#-----------------------------------------------------------------------------

message("Checking the cmake configuration")

cmake_minimum_required(VERSION ${CMAKE_MINIMUM_VERSION} FATAL_ERROR)

#----------------------
# Extra modules location
#----------------------
set(CMAKE_MODULE_PATH "${PGROUTING_SOURCE_DIR}/cmake")


#----------------------
# For concatenation
#----------------------
include(Cat)
include(FindPackageHandleStandardArgs)

#----------------------
# Should leave only the files that were in the distribution.
#----------------------
include(TargetDistclean)


#----------------------
# Should leave only the files that were in the distribution.
#----------------------
if (CDEBUG)
    message(STATUS "  CMAKE_VERSION: ${CMAKE_VERSION}")
    message(STATUS "  PGROUTING_SOURCE_DIR: ${PGROUTING_SOURCE_DIR}")
    message(STATUS "  PGROUTING_BINARY_DIR: ${PGROUTING_BINARY_DIR}")
endif()

message(".....OK")




#-----------------------------------------------------------------------------
# Operating system
#-----------------------------------------------------------------------------

message("Detecting Operating System")


# TODO In V3.0 we are going to store this in pgr_version
if(APPLE)
    message(STATUS "  found: APPLE")
elseif(UNIX)
    message(STATUS "  found: UNIX")
elseif(WIN32)
    message(STATUS "  found: WIN32")
endif()


message(".....OK")





#-----------------------------------------------------------------------------
# WIN32 
#-----------------------------------------------------------------------------
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

    if (CDEBUG)
        message(STATUS "OS_BUILD=${OS_BUILD}")
        message(STATUS "CGAL_ROOT=${CGAL_ROOT}")
        message(STATUS "GMP_ROOT=${GMP_ROOT}")
        message(STATUS "BOOST_ROOT=${BOOST_ROOT}")
        message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
    endif()

endif(WIN32)
#-----------------------------------------------------------------------------




#-----------------------------------------------------------------------------
# Checking BUILD requierments 
#-----------------------------------------------------------------------------

#----------------------
# Detecting C compiler
#----------------------

message("Detecting Compilers")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

message(STATUS "CMAKE_CXX_COMPILER= ${CMAKE_CXX_COMPILER}")


if(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
elseif(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++0x nor C++11 support.
    Please use a different C++ compiler.")
endif()

if (CDEBUG)
    message(STATUS "  CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
    message(STATUS "  CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
    message(STATUS "  CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
    message(STATUS "  CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
endif()

message(".....OK")


#----------------------
# Boost
#----------------------

message("Pgrouting uses: Boost graph header only")
message("CGAL uses: Boost thread library")


#set(Boost_USE_MULTITHREADED off)
set(Boost_DEBUG ON)

# pg routing doesnt need thread, but CGAL does
set (Boost_ADDITIONAL_VERSIONS "1.60" "1.59" "1.58" "1.57")

find_package(Boost ${BOOST_MINIMUM_VERSION} COMPONENTS thread REQUIRED) 


if(NOT Boost_INCLUDE_DIRS)
    message(FATAL_ERROR " Include directory was not found.
    Please check your Boost installation ") 
endif()


if (CDEBUG)
    message(STATUS "Boost headers were found here: ${Boost_INCLUDE_DIR}")
    message(STATUS "Boost_THREAD_FOUND=${Boost_THREAD_FOUND}")
    message(STATUS "Boost_THREAD_LIBRARY=${Boost_THREAD_LIBRARY}")
    message(STATUS "Boost_THREAD_LIBRARIES=${Boost_THREAD_LIBRARIES}")
    message(STATUS "Boost_LIB_DIAGNOSTIC_DEFINITIONS=${Boost_LIB_DIAGNOSTIC_DEFINITIONS}")
endif()

message(".....OK")


#----------------------
# CGAL
#----------------------

message("Checking CGAL requirements")


find_package(CGAL)

if(NOT CGAL_FOUND)
    message(FATAL_ERROR " CGAL installation not found
    Please check your CGAL installation ") 
endif(NOT CGAL_FOUND)

if (CDEBUG)
    message(STATUS "CGAL_INCLUDE_DIR=${CGAL_INCLUDE_DIR}")
    message(STATUS "CGAL_LIBRARIES=${CGAL_LIBRARIES}")
    message(STATUS "GMP_LIBRARIES=${GMP_LIBRARIES}")
endif()

message(".....OK")


#----------------------
# PostgreSQL
#----------------------

message("Detecting Postgresql requirements")

include(FindPackageHandleStandardArgs)

if (POSTGRESQL_VERSION)
    message("Setting to user setup")
    set(PostgreSQL_VERSION_STRING  "${POSTGRESQL_VERSION}.0")
    set(PostgreSQL_ADDITIONAL_VERSIONS ${POSTGRESQL_VERSION})
endif()

set (PostgreSQL_DEBUG ON)
find_package(PostgreSQL ${POSTGRESQL_MINIMUM_VERSION})

if (NOT PostgreSQL_FOUND)
    message(FATAL_ERROR " Please check your PostgreSQL installation.") 
endif ()



string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1.\\2" POSTGRESQL_VERSION ${PostgreSQL_VERSION_STRING})
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1\\2" PGSQL_VERSION ${POSTGRESQL_VERSION})


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPGSQL_VERSION=${PGSQL_VERSION}")

if (CDEBUG)
    message(STATUS "  pg_config.h is found in: ${PostgreSQL_INCLUDE_DIRS}")
    message(STATUS "  pg_type.h is found in: ${PostgreSQL_INCLUDE_DIRS}/catalog")
    message(STATUS "  Library: ${PostgreSQL_LIBRARY}")
    message(STATUS "  Libraries: ${PostgreSQL_LIBRARIES}")
    message(STATUS "  Where all library are: ${PostgreSQL_LIBRARY_DIRS}")
    message(STATUS "  PostgreSQL_VERSION_STRING is ${PostgreSQL_VERSION_STRING}")
    message(STATUS "  C flag: PGSQL_VERSION is ${PGSQL_VERSION}")
    message(STATUS "  Extension directory to install in: ${PostgreSQL_EXTENSION_DIRS}")
    message(STATUS "  Directory to install the extension: ${PostgreSQL_EXTENSION_LIBRARY_DIRS}")
endif()


message(".....OK")



#=============================================================================
# Prepare the compilation & linking
#=============================================================================


#-------------------
# The INCLUDE directories
#-------------------

#-------------------
# Generate pgRouting's
#-------------------
foreach (subdir ${PgRouting_SOURCE_NAMES} )
    set(PGROUTING_INCLUDE_DIRECTORIES ${PGROUTING_INCLUDE_DIRECTORIES} "${PGROUTING_SOURCE_DIR}/src/${subdir}/src")
endforeach()


include_directories(
    ${PGROUTING_INCLUDE_DIRECTORIES} 
    ${PostgreSQL_INCLUDE_DIRS} 
    ${Boost_INCLUDE_DIRS})


if(WIN32)
    #include_directories(${PostgreSQL_INCLUDE_DIRS}/port/win32)
endif(WIN32)


#-------------------
# Compiler directives
#-------------------
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
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -std=c++0x -g -Wno-deprecated")
    endif()

elseif(UNIX) # UNIX system variable include UNIX like system(i.e. APPLE and CYGWIN)
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -std=gnu99 -fPIC -O2 -g -Wall  -pedantic   -fmax-errors=10 -Werror -Wmissing-prototypes -frounding-math")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -g -Wall -pedantic  -fmax-errors=10 -Wextra -Werror -frounding-math -Wno-deprecated")

elseif(WIN32)
    # currently, support MinGW only
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O2 -g -Wall -frounding-math")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g  -std=c++0x -frounding-math -Wno-deprecated")
endif()



#-------------------
# add the subdirectories that have the C/C++ code 
#-------------------

foreach (subdir ${PgRouting_SOURCE_NAMES})
    add_subdirectory("${PGROUTING_SOURCE_DIR}/src/${subdir}/src")
endforeach()

#-------------------
# add the libraries to link with
#-------------------


link_libraries(${CGAL_LIBRARIES} ${GMP_LIBRARIES} ${Boost_THREAD_LIBRARIES})

if(WIN32)
    message(STATUS "LINK_DIRECTORIES =
    ${PostgreSQL_LIBRARY_DIRS}")
    message(STATUS "LINK_LIBRARIES =
    ${CGAL_LIBRARIES}
    ${GMP_LIBRARIES}
    ${Boost_THREAD_LIBRARIES}
    ${PostgreSQL_LIBRARIES}")

    link_directories(${PostgreSQL_LIBRARY_DIRS})
    link_libraries(postgres)
endif(WIN32)

#-------------------
# assemble the object files from the src functions into
# a single library librouting.so
#-------------------

#-------------------
# Type of library
#-------------------

if(APPLE)
    set(LIBRARY_MODE_TARGET "MODULE")
else(APPLE)
    set(LIBRARY_MODE_TARGET "SHARED")
endif(APPLE)

#-------------------
# pgRouting objects to be linked
#-------------------
foreach (subdir ${PgRouting_SOURCE_NAMES} )
    set(PGROUTING_OBJECTS ${PGROUTING_OBJECTS} "$<TARGET_OBJECTS:${subdir}>")
endforeach()

add_library(${PGROUTING_LIB_NAME}
    ${LIBRARY_MODE_TARGET}
    ${PGROUTING_OBJECTS})
#-----------------------------------------------------------------------------


install(TARGETS ${PGROUTING_LIB_NAME} DESTINATION ${PostgreSQL_EXTENSION_LIBRARY_DIRS})



if(APPLE)
    set_target_properties(routing-2.2
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)


file(WRITE "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
    "-- pgRouting version '${PGROUTING_VERSION_STRING}' extension for postgresql\n")

file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
    "-- Complain if script is sourced in pgsql, rather than CREATE EXTENSION\n")

#file(APPEND "${CMAKE_BINARY_DIR}/lib/pgrouting--${PGROUTING_VERSION_STRING}.sql.in"
#    "\\echo Use \"CREATE EXTENSION pgrouting\" to load this file. \\quit")

#-----------------------------------------------------------------------------
# List variable to collect module SQL file names
#-----------------------------------------------------------------------------

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

include(FindPerl)
if(NOT PERL_EXECUTABLE)
    message(FATAL_ERROR " Please check your Perl installation.")
endif(NOT PERL_EXECUTABLE)


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





message(STATUS "LIBS_TO_INSTALL=${PGROUTING_LIBRARY_NAME}")


install(FILES
    "${CMAKE_BINARY_DIR}/lib/${PGROUTING_LIB_NAME}.sql"
    "${CMAKE_BINARY_DIR}/lib/pgrouting.control"
    ${VERSION_UPDATE_FILES}
    DESTINATION "${PostgreSQL_EXTENSION_DIRS}")

#=============================================================================
# Checking optional requierments 
#=============================================================================

#-----------------------------------------------------------------------------
# Checking DEVELOPRES DOCUMENTATION requierments 
#-----------------------------------------------------------------------------

if(WITH_DEV_DOC)
    message(STATUS "Checking Developers Documentation Requierments")
    message(STATUS "NO chcking for the moment")

    #----------------------
    # Doxygen
    #----------------------
    message(STATUS "   Doxygen")
    message(STATUS "MISSING CODE")


endif()


#-----------------------------------------------------------------------------
# Checking TEST requierments 
#-----------------------------------------------------------------------------

if(WITH_TEST)

    find_path(PGTAP pgtap.control
        PATHS ${PostgreSQL_EXTENSION_DIRS})
    if (EXISTS PGTAP)
        message(STATUS "PGTAP ${PGTAP}")
    else()
        message(STATUS "PGTAP not found. 
        Please check your pgTap installation.")
    endif()
    include(FindPostGIS)
    find_package(PostGIS ${POSTGIS_MINIMUM_VERSION})

    if(NOT PostGIS_FOUND)
        set(WITH_TEST False)
        message(STATUS " postGIS not Found.
        Please check your postGIS installation.")
    endif()
endif()


#----------------------
# Perl
#----------------------

if(WITH_TEST1)
    include(FindPerl)
    if(NOT PERL_EXECUTABLE)
        message(FATAL_ERROR " Please check your Perl installation.")
    endif(NOT PERL_EXECUTABLE)
endif()

#-----------------------------------------------------------------------------



#-----------------------------------------------------------------------------
# Handles documentation
#  - checks for requirements
#  - process the documentation
#-----------------------------------------------------------------------------

if (WITH_DOC)
    message(STATUS "PGROUTING_VERSION_REVISION_NAME: ${PGROUTING_VERSION_REVISION_NAME}")
    add_subdirectory(doc)
endif()
