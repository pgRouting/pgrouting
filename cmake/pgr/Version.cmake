#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# PGROUTING version variables
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------

set(PGROUTING_SHORT_VERSION "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}${PGROUTING_VERSION_DEV}")
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
    message(STATUS "PGROUTING_VERSION: ${PGROUTING_VERSION}")
    message(STATUS "PGROUTING_SHORT_VERSION: ${PGROUTING_SHORT_VERSION}")
    message(STATUS "PGROUTING_FULL_VERSION: ${PGROUTING_FULL_VERSION}")
    message(STATUS "VERSION file contents: ${PGROUTING_BUILD_STRING}")
    message(STATUS "PGROUTING_GIT_HASH: ${PGROUTING_GIT_HASH}")
    message(STATUS "PGROUTING_GIT_BRANCH: ${PGROUTING_GIT_BRANCH}")
    message(STATUS "CMAKE_VERSION: ${CMAKE_VERSION}")
    message(STATUS "PGROUTING_SOURCE_DIR: ${PGROUTING_SOURCE_DIR}")
    message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
endif()


if (PGROUTING_DEBUG)
    message(STATUS "PGROUTING_VERSION: ${PGROUTING_VERSION}")
    message(STATUS "PGROUTING_VERSION_MAJOR: ${PGROUTING_VERSION_MAJOR}")
    message(STATUS "PGROUTING_VERSION_MINOR: ${PGROUTING_VERSION_MINOR}")
    message(STATUS "PGROUTING_VERSION_PATCH: ${PGROUTING_VERSION_PATCH}")
    message(STATUS "PGROUTING_VERSION_TWEAK: ${PGROUTING_VERSION_TWEAK}")
    message(STATUS "PGROUTING_VERSION_DEV: ${PGROUTING_VERSION_DEV}")
    message(STATUS "PGROUTING_VERSION_SHORT: ${PGROUTING_VERSION_SHORT}")
    message(STATUS "PGROUTING_VERSION_FULL: ${PGROUTING_VERSION_FULL}")
endif()

