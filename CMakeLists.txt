#
# Format convention
# - command (lower case)
# - VARIABLE (upper case)
# (http://www.cmake.org/cmake/help/v2.8.12/cmake.html)
#

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(POLICY CMP0046)
    cmake_policy(SET CMP0046 OLD)
endif()

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
        message(FATAL_ERROR "In-source builds not allowed.
        Please make a new directory (called a build directory) and run CMake from there.
        You may need to remove CMakeCache.txt." )
endif()

#---------------------------------------------
# Setting kind of build
#---------------------------------------------
#
# for developing
# cmake -DMAKE_BUILD_TYPE=Develop
#----------------------

message(STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
if (CMAKE_BUILD_TYPE MATCHES Develop)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo
        MinSizeRel."
        FORCE)
    set(WITH_INTERNAL_TESTS)
endif()
message(STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")


#---------------------------------------------
#---------------------------------------------
#---------------------------------------------

project(PGROUTING)

#---------------------------------------------
#---------------------------------------------
#---------------------------------------------

if(POLICY CMP0054)
   cmake_policy(SET CMP0054 OLD)
endif()

if(MSVC)
    cmake_policy(SET CMP0046 OLD)
endif()

#=============================================
# Set the working directories
#=============================================

#---------------------------------------------
#  PgRouting_SOURCE_NAMES
#---------------------------------------------
#
# Name of the directories that have C/C++ code
#  - <directory>/src/CMakelists.txt must exist
#  - <directory>/sql/CMakeLists.txt  must exist
#----------------------
configure_file("configuration.conf" "configuration.conf")
file(STRINGS configuration.conf PgRouting_CONFIGURATION_FILE)

set(PgRouting_SOURCE_NAMES "")
set(PgRouting_SQL_DIRECTORIES "")
set(PgRouting_DOC_DIRECTORIES "")
foreach(line ${PgRouting_CONFIGURATION_FILE})
    string(REGEX REPLACE "^(#).*" "\\1" comment ${line})
    if("${comment}" MATCHES "#")
        continue()
    endif()
    string(REGEX REPLACE "^(.*)\\|(.*)\\|(.*)\\|(.*)" "\\1" directory ${line})
    string(REGEX REPLACE "^(.*)\\|(.*)\\|(.*)\\|(.*)" "\\2" has_code ${line})
    string(REGEX REPLACE "^(.*)\\|(.*)\\|(.*)\\|(.*)" "\\3" has_sql ${line})
    string(REGEX REPLACE "^(.*)\\|(.*)\\|(.*)\\|(.*)" "\\4" has_doc ${line})

    string(STRIP ${directory} directory)
    string(STRIP ${has_code} has_code)
    string(STRIP ${has_sql} has_sql)
    string(STRIP ${has_doc} has_doc)


    if( ${has_code} MATCHES "Y")
        list(APPEND PgRouting_SOURCE_NAMES "${directory}")
    endif()
    if( ${has_sql} MATCHES "Y")
        list(APPEND PgRouting_SQL_DIRECTORIES "${directory}")
    endif()
    if( ${has_doc} MATCHES "Y")
        list(APPEND PgRouting_DOC_DIRECTORIES "${directory}")
    endif()
endforeach()

if (PGROUTING_DEBUG)
    message(STATUS "${PgRouting_SOURCE_NAMES}")
    message(STATUS "${PgRouting_SQL_DIRECTORIES}")
    message(STATUS "${PgRouting_DOC_DIRECTORIES}")
endif()



#---------------------------------------------
# minimum versions
#---------------------------------------------
set(DOXYGEN_MINIMUM_VERSION "1.7")
set(SPHINX_MINIMUM_VERSION "1.1")
set(POSTGRESQL_MINIMUM_VERSION "9.2.0")
set(BOOST_MINIMUM_VERSION "1.53")
set(CGAL_CXX_MINIMUM_VERSION "4.4")
set(POSTGIS_MINIMUM_VERSION "2.0.0")
set(GNU_CXX_MINIMUM_VERSION "4.6")
set(MSVC_CXX_MINIMUM_VERSION "18.0")

message(STATUS "DOXYGEN_MINIMUM_VERSION=${DOXYGEN_MINIMUM_VERSION}")
message(STATUS "SPHINX_MINIMUM_VERSION=${SPHINX_MINIMUM_VERSION}")
message(STATUS "POSTGRESQL_MINIMUM_VERSION=${POSTGRESQL_MINIMUM_VERSION}")
message(STATUS "POSTGIS_MINIMUM_VERSION=${POSTGIS_MINIMUM_VERSION}")
message(STATUS "GNU_CXX_MINIMUM_VERSION=${GNU_CXX_MINIMUM_VERSION}")
message(STATUS "CLANG_CXX_MINIMUM_VERSION=${CLANG_CXX_MINIMUM_VERSION}")
message(STATUS "MSVC_CXX_MINIMUM_VERSION=${MSVC_CXX_MINIMUM_VERSION}")


#---------------------------------------------
# adding cmake modules
#---------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
foreach(dir ${CMAKE_MODULE_PATH})
    message(STATUS "dir='${dir}'")
endforeach()

include(MacroLibrary)
include(TargetDistclean)

#---------------------------------------------
# C++ Compiler requirements
#---------------------------------------------

include(CheckCXXCompilerFlag)

CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS GNU_CXX_MINIMUM_VERSION)
        message(FATAL_ERROR "GCC version must be at least ${GNU_CXX_MINIMUM_VERSION}!
        Found version ${CMAKE_CXX_COMPILER_VERSION}")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS MSVC_CXX_MINIMUM_VERSION)
        message(FATAL_ERROR "MSVC version must be at least ${MSVC_CXX_MINIMUM_VERSION}!")
    endif()
else()
    message(STATUS "Unknown compiler minimum version for ${CMAKE_CXX_COMPILER_ID}")
endif()

if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    message(STATUS "Using  -std=c++11.")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    message(STATUS "Using  -std=c++0x.")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++0x or C++11 flags support.")
endif()


#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# PGROUTING version variables
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------

set(PGROUTING_VERSION_MAJOR "2")
set(PGROUTING_VERSION_MINOR "6")
set(PGROUTING_VERSION_PATCH "3")
set(PGROUTING_VERSION_DEV "")

set(PGROUTING_SHORT_VERSION "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}")
set(PGROUTING_VERSION "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}.${PGROUTING_VERSION_PATCH}")
set(PGROUTING_FULL_VERSION "v${PGROUTING_VERSION}${PGROUTING_VERSION_DEV}")


#---------------------------------------------
# Variables used in pgr_version()
#---------------------------------------------
file(STRINGS "${PGROUTING_SOURCE_DIR}/VERSION" PGROUTING_BUILD_STRING)

set(PGROUTING_GIT_HASH "")
string(REGEX REPLACE "^([0-9]+)-([0-9a-f]+) (.*)$" "\\2" PGROUTING_GIT_HASH ${PGROUTING_BUILD_STRING})
string(REGEX REPLACE "^([0-9]+)-([0-9a-f]+) (.*)$" "\\3" PGROUTING_GIT_BRANCH ${PGROUTING_BUILD_STRING})

#if (NOT "${PGROUTING_GIT_BRANCH}" STREQUAL "master")
#    set (PGROUTING_GIT_HASH  "")
#    set (PGROUTING_GIT_BRANCH  "")
#endif()


if (PGROUTING_DEBUG)
    message(STATUS "PGROUTING_VERSION: ${PGROUTING_FULL_VERSION}")
    message(STATUS "PGROUTING_SHORT_VERSION: ${PGROUTING_FULL_VERSION}")
    message(STATUS "PGROUTING_FULL_VERSION: ${PGROUTING_FULL_VERSION}")
    message(STATUS "VERSION file contents: ${PGROUTING_BUILD_STRING}")
    message(STATUS "PGROUTING_GIT_HASH: ${PGROUTING_GIT_HASH}")
    message(STATUS "PGROUTING_GIT_BRANCH: ${PGROUTING_GIT_BRANCH}")
    message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
    message(STATUS "PGROUTING_SOURCE_DIR: ${PGROUTING_SOURCE_DIR}")
    message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
endif()

#----------------------
# PGROUTING library name used in the pgr_functions
#  '$libdir/${PGROUTING_LIBRARY_NAME}'
#----------------------
set(PGROUTING_LIBRARY_NAME "libpgrouting-${PGROUTING_SHORT_VERSION}")


#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# Windows compiler flags
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------


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
    if (PGROUTING_DEBUG)
        message(STATUS "OS_BUILD=${OS_BUILD}")
        message(STATUS "CGAL_ROOT=${CGAL_ROOT}")
        message(STATUS "GMP_ROOT=${GMP_ROOT}")
        message(STATUS "BOOST_ROOT=${BOOST_ROOT}")
        message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
    endif()
endif(WIN32 AND NOT MSVC)

if(WIN32 AND MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_SCL_SECURE_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_SCL_SECURE_NO_WARNINGS")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_CRT_NONSTDC_NO_DEPRECATE")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -EHsc")
endif()

#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# Finding prerequisites
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------

#---------------------------------------------
#---------------------------------------------
# Perl
#---------------------------------------------
#---------------------------------------------
include(FindPerl)
if(NOT PERL_EXECUTABLE)
    message(FATAL_ERROR " Please check your Perl installation.")
endif(NOT PERL_EXECUTABLE)

#---------------------------------------------
#---------------------------------------------
# PostgreSQL
#---------------------------------------------
#---------------------------------------------
find_package(PostgreSQL)
if(NOT POSTGRESQL_FOUND)
    message(FATAL_ERROR " Please check your PostgreSQL installation.")
endif(NOT POSTGRESQL_FOUND)

if(NOT POSTGRESQL_VERSION_STRING)
    message(FATAL_ERROR "pg_config was not found. Please check your PostgreSQL installation!")
endif()

string(SUBSTRING "${POSTGRESQL_VERSION_STRING}" 11 -1 POSTGRESQL_VERSION)

# for XbetaY XalphaY XrcY -> X.Y
string(REGEX REPLACE "^([0-9]+)[beta|alpha|rc|devel].*" "\\1.0" POSTGRESQL_VERSION ${POSTGRESQL_VERSION})

#for X.Y.Z -> XY  Y<10
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1\\2" PGSQL_VERSION ${POSTGRESQL_VERSION})


if("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")
    message(FATAL_ERROR " PostgreSQL ${POSTGRESQL_MINIMUM_VERSION} or greater is required.")
endif("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")

#-------
include_directories(${POSTGRESQL_INCLUDE_DIR})
if(WIN32)
    include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32)
    if(MSVC)
        include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32_msvc/)
    endif(MSVC)
endif(WIN32)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPGSQL_VERSION=${PGSQL_VERSION}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPGSQL_VERSION=${PGSQL_VERSION}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DPGSQL_VERSION=${PGSQL_VERSION}")
message(STATUS "PGSQL_VERSION=${PGSQL_VERSION}")


#---------------------------------------------
#---------------------------------------------
# Boost
#---------------------------------------------
#---------------------------------------------
find_package(Boost ${BOOST_MINIMUM_VERSION})
if(Boost_INCLUDE_DIRS)
    message(STATUS "Boost headers were found here: ${Boost_INCLUDE_DIRS}")
else(Boost_INCLUDE_DIRS)
    message(FATAL_ERROR " Please check your Boost installation ")
endif(Boost_INCLUDE_DIRS)

include_directories(${Boost_INCLUDE_DIRS})


#---------------------------------------------
#---------------------------------------------
# CGAL
#---------------------------------------------
#---------------------------------------------
find_package(CGAL ${CGAL_MINIMUM_VERSION})
if(NOT CGAL_FOUND)
    message(FATAL_ERROR " Please check your CGAL installation, or set correct path to CGAL_INCLUDE_DIR and CGAL_LIBRARIES.")
endif(NOT CGAL_FOUND)

#-------
include_directories(${CGAL_INCLUDE_DIR})

#---------------------------------------------
#---------------------------------------------
# PgRouting includes
#---------------------------------------------
#---------------------------------------------

set(PGROUTING_INCLUDE_DIRECTORIES
    ${PGROUTING_SOURCE_DIR}/include
    )

#-------
include_directories(${PGROUTING_INCLUDE_DIRECTORIES})


#---------------------------------------------
if (PGROUTING_DEBUG)
    message(STATUS "PERL_VERSION_STRING = ${PERL_VERSION_STRING}")
    message(STATUS "POSTGRESQL_VERSION is ${PGSQL_VERSION}")
    message(STATUS "PGSQL_VERSION is ${PGSQL_VERSION}")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "CGAL_INCLUDE_DIR: ${CGAL_INCLUDE_DIR}")
    message(STATUS "PGROUTING_INCLUDE_DIRECTORIES = ${PGROUTING_INCLUDE_DIRECTORIES}")
    message(STATUS "POSTGRESQL_INCLUDE_DIR = ${POSTGRESQL_INCLUDE_DIR}")
    message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
endif()


#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# Posgres installation directories
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------



#---------------------------------------------
# library directory
#---------------------------------------------
execute_process(
    COMMAND ${POSTGRESQL_PG_CONFIG} --pkglibdir
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE LIB_DIR)

if(NOT LIB_DIR)
    message(FATAL_ERROR "pg_config --pkglibdir failed to return a value. Please check your PostgreSQL installation!")
endif()

#-------
set(LIBRARY_INSTALL_PATH ${LIB_DIR})

#---------------------------------------------
# extension directory
#---------------------------------------------

execute_process(
    COMMAND ${POSTGRESQL_PG_CONFIG} --sharedir
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE SHARE_DIR)

#-------
if(SHARE_DIR)
    set(SHARE_DIR "${SHARE_DIR}/extension")
else(SHARE_DIR)
    message(FATAL_ERROR "pg_config --sharedir failed to return a value. Please check your PostgreSQL installation!")
endif(SHARE_DIR)

#---------------------------------------------
if (PGROUTING_DEBUG)
    message(STATUS "Install directory for libraries is set to ${LIB_DIR}")
    message(STATUS "Extension directory for SQL files is set to ${SHARE_DIR}")
endif()



#---------------------------------------------
# Special cases for windows
#---------------------------------------------

if(WIN32)
    message(STATUS "LINK_DIRECTORIES = ${POSTGRESQL_LIBRARIES}")
    link_directories(${POSTGRESQL_LIBRARIES} ${LIB_DIR})
    link_libraries(postgres)
endif()

#-----------------------------------------------------------------------------

if (PGROUTING_DEBUG)
    get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    foreach(dir ${dirs})
        message(STATUS "dir='${dir}'")
    endforeach()
endif()


#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#compiler directives
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

# for UNIX & JENKINS
if(UNIX OR (WIN32 AND NOT MSVC))
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}  -std=gnu99")
endif()

if(NOT WIN32)
    CHECK_C_COMPILER_FLAG("-fPIC" C_COMPILER_SUPPORTS_FPIC)
    CHECK_CXX_COMPILER_FLAG("-fPIC" CXX_COMPILER_SUPPORTS_FPIC)
    if(C_COMPILER_SUPPORTS_FPIC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC ")
    endif()
    if(CXX_COMPILER_SUPPORTS_FPIC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")
    endif()
endif()

if(WIN32 AND MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")
endif()

CHECK_C_COMPILER_FLAG("-frounding-math" C_COMPILER_SUPPORTS_ROUNDING_MATH)
CHECK_CXX_COMPILER_FLAG("-frounding-math" CXX_COMPILER_SUPPORTS_ROUNDING_MATH)
if(C_COMPILER_SUPPORTS_ROUNDING_MATH)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -frounding-math ")
endif()
if(CXX_COMPILER_SUPPORTS_ROUNDING_MATH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frounding-math ")
endif()

# TODO use -Werror when TRSP gets rewritten
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   -Wall -Wconversion -pedantic -Wmissing-prototypes -W -Wunused -Wuninitialized -Wextra -Wdouble-promotion")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -fno-implicit-inline-templates -Wconversion -pedantic -W -Wunused -Wuninitialized -Wextra -Wdouble-promotion")



#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

if (PGROUTING_DEBUG)
    message(STATUS "CMAKE_CXX_FLAGS_DEBUG= ${CMAKE_CXX_FLAGS_DEBUG}")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE= ${CMAKE_CXX_FLAGS_RELEASE}")
    message(STATUS "CMAKE_CXX_FLAGS= ${CMAKE_CXX_FLAGS}")

    message(STATUS "CMAKE_C_FLAGS_DEBUG= ${CMAKE_C_FLAGS_DEBUG}")
    message(STATUS "CMAKE_C_FLAGS_RELEASE= ${CMAKE_C_FLAGS_RELEASE}")
    message(STATUS "CMAKE_C_FLAGS= ${CMAKE_C_FLAGS}")

    message(STATUS "UNIX=${UNIX}")
    message(STATUS "WIN32=${WIN32}")
    message(STATUS "MSVC=${MSVC}")
endif()

#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#====================================================================
# Include the working directories
#====================================================================

if(WITH_INTERNAL_TESTS)
    message("Including InternalQueryTests")
    set(PgRouting_SOURCE_NAMES ${PgRouting_SOURCE_NAMES} "internalQueryTests")
endif()

#-------------------
# add the subdirectories that have the C/C++ code
#-------------------

foreach (subdir ${PgRouting_SOURCE_NAMES})
    add_subdirectory("${PGROUTING_SOURCE_DIR}/src/${subdir}")
endforeach()


#====================================================================



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


#----------------------
# PGROUTING installation files names have the following name
#----------------------
set(PGROUTING_LIB_NAME "pgrouting-${PGROUTING_SHORT_VERSION}")


add_library(${PGROUTING_LIB_NAME}
    ${LIBRARY_MODE_TARGET}
    ${PGROUTING_OBJECTS})



if(APPLE)
    set_target_properties(${PGROUTING_LIB_NAME}
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)
if(WIN32 AND MSVC)
    set_target_properties(${PGROUTING_LIB_NAME} PROPERTIES PREFIX "lib")
endif(WIN32 AND MSVC)


#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# sql subdirectory creates the files:
#  pgrouting--(version).sql
#  pgrouting--(oldVersion)--(version).sql
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------

add_subdirectory(sql)

#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# INSTALLATION
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------


install(TARGETS ${PGROUTING_LIB_NAME} DESTINATION ${LIBRARY_INSTALL_PATH})
install(FILES
    ${PgRouting_SQL_FILES_TO_INSTALL}
    DESTINATION "${SHARE_DIR}"
    )

#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# DOCUMENTATION
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
add_subdirectory(doxygen)
add_subdirectory(doc)


if (PGROUTING_DEBUG)
    foreach (f ${PgRouting_SQL_FILES})
        message(STATUS ${f})
    endforeach()
endif()
