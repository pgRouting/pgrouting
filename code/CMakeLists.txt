cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
project(MYROUTING)
message(STATUS "MYROUTING_SOURCE_DIR: ${MYROUTING_SOURCE_DIR}")

set(CMAKE_PREFIX_PATH /usr:/usr/local)
set(CMAKE_INCLUDE_PATH /usr:/usr/local)
set(CMAKE_LIBRARY_PATH /usr:/usr/local)

add_subdirectory(cmake)
set(CMAKE_MODULE_PATH "${MYROUTING_SOURCE_DIR}/cmake")

# loads and runs the code given in the cmake files from ${PGROUTING_SOURCE_DIR}/cmake
include(MacroLibrary)
include(TargetDistclean)




#checking postgres
find_package(PostgreSQL)
if(NOT POSTGRESQL_FOUND)
  message(FATAL_ERROR " Please check your PostgreSQL installation.") 
endif(NOT POSTGRESQL_FOUND)

#checking boost libraries
find_package(Boost)
if(Boost_INCLUDE_DIRS)
  message(STATUS "Boost header files were found at: ${Boost_INCLUDE_DIR}")
else(Boost_INCLUDE_DIRS)
  message(FATAL_ERROR " Please check your Boost installation ") 
endif(Boost_INCLUDE_DIRS)


if(NOT POSTGRESQL_VERSION_STRING)
  message(FATAL_ERROR "pg_config was not found. Please check your PostgreSQL installation!")
endif(NOT POSTGRESQL_VERSION_STRING)

message(STATUS "POSTGRESQL_VERSION_STRING is ${POSTGRESQL_VERSION_STRING}")

#checking the version of postgresql
if("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")
  message(FATAL_ERROR " PostgreSQL ${POSTGRESQL_MINIMUM_VERSION} or greater is required.")
endif("${POSTGRESQL_VERSION}" VERSION_LESS "${POSTGRESQL_MINIMUM_VERSION}")


#will store the string from index 11 to end of ${POSTGRESQL_VERSION_STRING} to POSTGRESQL_VERSION
string(SUBSTRING "${POSTGRESQL_VERSION_STRING}" 11 -1 POSTGRESQL_VERSION)

message(STATUS "POSTGRESQL_VERSION is ${POSTGRESQL_VERSION}")
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1\\2" PGSQL_VERSION ${POSTGRESQL_VERSION})
message(STATUS "PGSQL_VERSION is ${PGSQL_VERSION}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPGSQL_VERSION=${PGSQL_VERSION}")



#setting the libraries path
execute_process(
  COMMAND ${POSTGRESQL_PG_CONFIG} --pkglibdir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE LIB_DIR)


#setting the sql extension share path
  execute_process(
  COMMAND ${POSTGRESQL_PG_CONFIG} --sharedir
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE SHARE_DIR)

#set(LIB_DIR "${MYROUTING_SOURCE_DIR}/lib")
set(MYROUTING_INCLUDE_DIRECTORIES ${MYROUTING_SOURCE_DIR}/src/mydijkstra/src)
set(MYROUTING_INCLUDE_DIRECTORIES ${MYROUTING_SOURCE_DIR}/src/contraction/src)
set(MYROUTING_INCLUDE_DIRECTORIES ${MYROUTING_SOURCE_DIR}/src/myastar/src)
set(MYROUTING_INCLUDE_DIRECTORIES ${MYROUTING_SOURCE_DIR}/src/common/src)

#including these directories for the compiler
include_directories(${MYROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS})

set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -fPIC -O2 -g -Wall -frounding-math")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O2 -g -Wall -std=c++0x -frounding-math -Wno-deprecated")

message(STATUS "CMAKE_C_FLAGS is ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS is ${CMAKE_CXX_FLAGS}")

set(LIBRARY_MODE_TARGET "SHARED")

add_subdirectory(src)

SET(PACKAGE_SQL_FILES "")
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}")


add_library( myrouting_library 
${LIBRARY_MODE_TARGET} 
$<TARGET_OBJECTS:mydijkstra>
$<TARGET_OBJECTS:contraction>
$<TARGET_OBJECTS:myastar>
$<TARGET_OBJECTS:common>
)

install(TARGETS myrouting_library DESTINATION ${LIB_DIR} )



install(FILES
    "${MYROUTING_SOURCE_DIR}/lib/myrouting--0.0.0.sql"
   "${MYROUTING_SOURCE_DIR}/lib/myrouting.control"
    DESTINATION ${SHARE_DIR}/extension)

