cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

if(POLICY CMP0144)
    cmake_policy(SET CMP0144 OLD)
endif()

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif()

if(POLICY CMP0148)
    cmake_policy(SET CMP0148 OLD)
endif()

#---------------------------------------------
# Setting kind of build
#---------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(pgr/BuildType)


#---------------------------------------------
#---------------------------------------------
#---------------------------------------------

project(PGROUTING VERSION 4.0.0
    LANGUAGES C CXX )
set(PROJECT_VERSION_DEV "-beta1")
string(TOLOWER "${PROJECT_NAME}" PROJECT_NAME_LOWER)

include(pgr/GitInfo)
include(pgr/Version)

add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}${PROJECT_VERSION_DEV}")
set(PROJECT_LIB_NAME "${PROJECT_NAME_LOWER}-${PROJECT_LIB_VERSION}")

string(TIMESTAMP COMPILATION_DATE "%Y/%m/%d" UTC)

# uncomment to show the NOTICE/WARNING of deprecated internal C functions
# add_compile_definitions(SHOWMSG=1)

set(MINORS 4.0 3.8 3.7 3.6 3.5 3.4 3.3 3.2 3.1 3.0 2.6)
set(OLD_SIGNATURES
    3.8.0
    3.7.3 3.7.2 3.7.1 3.7.0
    3.6.3 3.6.2 3.6.1 3.6.0
    3.5.1 3.5.0
    3.4.2 3.4.1 3.4.0
    3.3.5 3.3.4 3.3.3 3.3.2 3.3.1 3.3.0
    3.2.2 3.2.1 3.2.0
    3.1.4 3.1.3 3.1.2 3.1.1 3.1.0
    3.0.6 3.0.5 3.0.4 3.0.3 3.0.2 3.0.1 3.0.0
    2.6.3 2.6.2 2.6.1 2.6.0
    )



#=============================================
# Set the working directories
#=============================================
include(pgr/Configure)


#---------------------------------------------
# minimum versions
#---------------------------------------------
set(DOXYGEN_MINIMUM_VERSION "1.7")
set(SPHINX_MINIMUM_VERSION "4.0")
set(POSTGRESQL_MINIMUM_VERSION "13")
set(BOOST_MINIMUM_VERSION "1.56.0")
set(POSTGIS_MINIMUM_VERSION "3.0.0")

message(STATUS "DOXYGEN_MINIMUM_VERSION=${DOXYGEN_MINIMUM_VERSION}")
message(STATUS "SPHINX_MINIMUM_VERSION=${SPHINX_MINIMUM_VERSION}")
message(STATUS "POSTGRESQL_MINIMUM_VERSION=${POSTGRESQL_MINIMUM_VERSION}")
message(STATUS "BOOST_MINIMUM_VERSION=${BOOST_MINIMUM_VERSION}")
message(STATUS "POSTGIS_MINIMUM_VERSION=${POSTGIS_MINIMUM_VERSION}")

# cmake 3.2 adds unwanted flags
if(WIN32 AND MSVC)
    set(CMAKE_C_FLAGS "")
    set(CMAKE_CXX_FLAGS "")
endif()

#---------------------------------------------
# Boost
#---------------------------------------------

find_package(Boost ${BOOST_MINIMUM_VERSION} REQUIRED)
if (NOT Boost_VERSION_MACRO)
    set(Boost_VERSION_MACRO ${Boost_VERSION})
endif()
set(BOOST_VERSION "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}")
add_definitions(-DBOOST_ALLOW_DEPRECATED_HEADERS)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

#---------------------------------------------
# Clang tidy
#---------------------------------------------

option(USE_CLANG_TIDY "Use clang-tidy." OFF)

if(USE_CLANG_TIDY)
    find_program(CLANG_TIDY_COMMAND NAMES clang-tidy NO_CACHE)
    if (CLANG_TIDY_COMMAND)
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND};--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
    else()
        message(WARNING "Set to use clang-tidy, but not found.")
    endif()
endif()

#-----------------------------------------------------------------------------
# C/C++ Compiler requirements
#-----------------------------------------------------------------------------

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard version to use (minimum requirement is c++17)")

set(COMPILER_VERSION "${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}")

#-----------------------------------------------------------------------------
# C++ language version and compilation flags
#-----------------------------------------------------------------------------
message(STATUS "pgRouting: Require C++${CMAKE_CXX_STANDARD}")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#---------------------------------------------
# Windows compiler flags
#---------------------------------------------

if(WIN32 AND NOT MSVC)
    set(OS_BUILD  $ENV{OS_BUILD})
    if(NOT OS_BUILD)
        set(OS_BUILD "64")
    endif()
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}:/c/ming${OS_BUILD}/projects/pgx${OS_BUILD}/pg92)
    if (NOT BOOST_ROOT)
        set(BOOST_ROOT c:/ming${OS_BUILD}/msys/local)
    endif()

    if (PROJECT_DEBUG)
        message(STATUS "OS_BUILD=${OS_BUILD}")
        message(STATUS "BOOST_ROOT=${BOOST_ROOT}")
        message(STATUS "CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
    endif()
endif(WIN32 AND NOT MSVC)

if(WIN32 AND MSVC)
    add_compile_options(
        -D_CRT_SECURE_NO_DEPRECATE
        -D_SCL_SECURE_NO_DEPRECATE
        -D_CRT_SECURE_NO_WARNINGS
        -D_SCL_SECURE_NO_WARNINGS
        -D_CRT_NONSTDC_NO_DEPRECATE
        -D_CRT_NONSTDC_NO_DEPRECATE
        -EHsc)
endif()

#-----------------------------------------------------------------------------
# Finding prerequisites
#-----------------------------------------------------------------------------

find_package(Perl REQUIRED)

#---------------------------------------------
#---------------------------------------------
# PostgreSQL
#---------------------------------------------
#---------------------------------------------
find_package(PostgreSQL "${POSTGRESQL_MINIMUM_VERSION}" REQUIRED)

# removing type of release on postgres version
# for XbetaY XalphaY XrcY -> X.Y
string(REGEX REPLACE "([0-9]+)[beta|alpha|rc|devel].*" "\\1.0" POSTGRESQL_VERSION_STRING "${POSTGRESQL_VERSION_STRING}")
message(STATUS "POSTGRESQL_VERSION_STRING=${POSTGRESQL_VERSION_STRING}")

# calculate major.minor
STRING(REGEX MATCH "([0-9]+)\.([0-9]+)" POSTGRESQL_VERSION "${POSTGRESQL_VERSION_STRING}")
message(STATUS "POSTGRESQL_VERSION=${POSTGRESQL_VERSION}")

# split mayor minor
string(REPLACE "." ";" VERSION_LIST ${POSTGRESQL_VERSION})
list(GET VERSION_LIST 0 POSTGRESQL_VERSION_MAYOR)
list(GET VERSION_LIST 1 POSTGRESQL_VERSION_MINOR)

# calculate math value of postgres version
math(EXPR PGSQL_VERSION "${POSTGRESQL_VERSION_MAYOR} * 1000 + ${POSTGRESQL_VERSION_MINOR}")
message(STATUS "PGSQL_VERSION=${PGSQL_VERSION}")

#-------
include_directories(SYSTEM ${POSTGRESQL_INCLUDE_DIR})
if(WIN32)
    include_directories(SYSTEM ${POSTGRESQL_INCLUDE_DIR}/port/win32)
    if(MSVC)
        include_directories(SYSTEM ${POSTGRESQL_INCLUDE_DIR}/port/win32_msvc/)
    endif()
endif()

add_definitions(-DPGSQL_VERSION=${PGSQL_VERSION})
message(STATUS "PGSQL_VERSION=${PGSQL_VERSION}")

#---------------------------------------------
if (PROJECT_DEBUG)
    message(STATUS "PERL_VERSION_STRING = ${PERL_VERSION_STRING}")
    message(STATUS "POSTGRESQL_VERSION is ${PGSQL_VERSION}")
    message(STATUS "PGSQL_VERSION is ${PGSQL_VERSION}")
    message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
    message(STATUS "POSTGRESQL_INCLUDE_DIR = ${POSTGRESQL_INCLUDE_DIR}")
endif()


#---------------------------------------------
#---------------------------------------------
# PROJECT includes
#---------------------------------------------
#---------------------------------------------
include_directories(${PROJECT_SOURCE_DIR}/include)

#---------------------------------------------
# library directory
#---------------------------------------------
execute_process(
    COMMAND ${POSTGRESQL_PG_CONFIG} --pkglibdir
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE LIBRARY_INSTALL_PATH)

if(NOT LIBRARY_INSTALL_PATH)
    message(FATAL_ERROR "pg_config --pkglibdir failed to return a value. Please check your PostgreSQL installation!")
endif()

message(STATUS "LIBRARY_INSTALL_PATH ${LIBRARY_INSTALL_PATH}")

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

if (PROJECT_DEBUG)
    message(STATUS "POSTGRESQL_LIBRARIES=${POSTGRESQL_LIBRARIES}")
    message(STATUS "LIBRARY_INSTALL_PATH=${LIBRARY_INSTALL_PATH}")
    message(STATUS "SHARE_DIR=${SHARE_DIR}")
endif()



#---------------------------------------------
# Special cases for windows
#---------------------------------------------

if(WIN32)
    link_directories(${POSTGRESQL_LIBRARIES})
    link_libraries(postgres)
endif()

#-----------------------------------------------------------------------------

if (PROJECT_DEBUG)
    get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
    message(STATUS "INCLUDE_DIRECTORIES='${dirs}'")
endif()


#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#compiler directives
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# https://www.postgresql.org/docs/10/xfunc-c.html

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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        CHECK_C_COMPILER_FLAG("-frounding-math" C_COMPILER_SUPPORTS_ROUNDING_MATH)
        CHECK_CXX_COMPILER_FLAG("-frounding-math" CXX_COMPILER_SUPPORTS_ROUNDING_MATH)
        if(C_COMPILER_SUPPORTS_ROUNDING_MATH)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -frounding-math ")
        endif()
        if(CXX_COMPILER_SUPPORTS_ROUNDING_MATH)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -frounding-math ")
        endif()
    endif()

    set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   -Wall -Wconversion -Wmissing-prototypes -W -Wunused -Wuninitialized -Wextra -Wdouble-promotion")
    add_definitions(-Wsign-conversion)
    if(BOOST_Geometry_VERSION_OK)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wconversion -pedantic -W -Wunused -Wuninitialized -Wextra -Wdouble-promotion")
    else()
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic -W -Wunused -Wuninitialized -Wextra -Wdouble-promotion")
    endif()
endif()


#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

if (PROJECT_DEBUG)
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
    set(PROJECT_SRC_DIRECTORIES ${PROJECT_SRC_DIRECTORIES} "internalQueryTests")
endif()

#-------------------
# add the subdirectories that have the C/C++ code
#-------------------

foreach (subdir ${PROJECT_SRC_DIRECTORIES})
    add_subdirectory("${PROJECT_SOURCE_DIR}/src/${subdir}")
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


LINK_LIBRARIES(${BOOST_THREAD_LIBRARIES})

#-------------------
# pgRouting objects to be linked
#-------------------
foreach (subdir ${PROJECT_SRC_DIRECTORIES} )
    set(PROJECT_OBJECTS ${PROJECT_OBJECTS} "$<TARGET_OBJECTS:${subdir}>")
endforeach()


#-----------------------------------------------------------------------------
# PROJECT Library names
#-----------------------------------------------------------------------------


#----------------------
# PGROUTING installation files names have the following name
#----------------------

if (PROJECT_DEBUG)
    message(STATUS "PROJECT_LIB_NAME ${PROJECT_LIB_NAME}")
endif()



add_library(${PROJECT_LIB_NAME}
    ${LIBRARY_MODE_TARGET}
    ${PROJECT_OBJECTS})



if(APPLE)
    set_target_properties(${PROJECT_LIB_NAME}
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)
if(WIN32 AND MSVC)
    set_target_properties(${PROJECT_LIB_NAME} PROPERTIES PREFIX "lib")
endif(WIN32 AND MSVC)


#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# sql subdirectory creates the files:
#  ${PROJECT_NAME_LOWER}--(version).sql
#  ${PROJECT_NAME_LOWER}--(oldVersion)--(version).sql
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------

add_subdirectory(sql)
if (PROJECT_DEBUG)
    message(STATUS "PROJECT_FILES_TO_INSTALL=${PROJECT_FILES_TO_INSTALL}")
endif()


#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# INSTALLATION
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------


install(TARGETS ${PROJECT_LIB_NAME} DESTINATION ${LIBRARY_INSTALL_PATH})
install(FILES
    ${PROJECT_FILES_TO_INSTALL}
    DESTINATION "${SHARE_DIR}"
    )

#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# DOCUMENTATION
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
add_subdirectory(doxygen)
add_subdirectory(doc)
