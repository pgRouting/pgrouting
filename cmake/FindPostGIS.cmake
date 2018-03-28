# (c) 2015  pgRouting colaborators
#
# Finds the most recent postGIS for a particular postgreSQL
# We need this for the tests
#
# Usage:
# find_package(PostGIS <version>)
#

# The following variables are set if PostGIS is found:
#  PostGIS_FOUND            - Set to true when PostGIS is found.
#  PostGIS_LIBRARY          - if we ever need to link it
#  PostGIS_EXTENSION_DIR    - usless for the moment
#  PostGIS_LIBRARY_DIR      - useless for the moment


FIND_PATH(PostGIS_EXTENSION_DIR postgis.control
    PATHS "${PostgreSQL_EXTENSION_INSTALLATION}")

find_library(PostGIS_LIBRARY_DIR NAMES postgis-2.2.so
    PATHS "${PostgreSQL_LIBRARY_INSTALLATION}")

if (NOT PostGIS_LIBRARY)
    find_library(PostGIS_LIBRARY_DIR NAMES postgis-2.1.so
        PATHS "${PostgreSQL_LIBRARY_INSTALLATION}")
endif()

if (NOT PostGIS_LIBRARY)
    find_library(PostGIS_LIBRARY_DIR NAMES postgis-2.0.so
        PATHS "${PostgreSQL_LIBRARY_INSTALLATION}")
endif()

if (NOT (PostGIS_EXTENSION_DIR STREQUAL PostgreSQL_EXTENSION_INSTALLATION))
    #message(STATUS "PostGIS not found for the given postgreSQL version")
    set(PostGIS_FOUND FALSE)
else()

    if (PostGIS_EXTENSION_DIR  AND PostGIS_LIBRARY_DIR)
        SET(PostGIS_FOUND TRUE)
        set(PostGIS_LIBRARY_DIR ${PostgreSQL_LIBRARY_INSTALLATION})
    else ()
        SET(PostGIS_FOUND FALSE)
    endif ()
endif ()

#message(STATUS PostGIS_LIBRARY ${PostGIS_LIBRARY})
#message(STATUS PostGIS_EXTENSION_DIR ${PostGIS_EXTENSION_DIR})
#message(STATUS PostgreSQL_EXTENSION_INSTALLATION ${PostgreSQL_EXTENSION_INSTALLATION})
#message(STATUS PostGIS_LIBRARY_DIR ${PostGIS_LIBRARY_DIR})

MARK_AS_ADVANCED(PostGIS_EXTENSION_DIR PostGIS_LIBRARY_DIR PostGIS_LIBRARY)

