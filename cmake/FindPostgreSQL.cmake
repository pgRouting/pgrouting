
#-----------------------------------------------------------------------------
# Find the PostgreSQL installation for PGROUTING.
#
#  (c) 2015 pgRouting contributors
#
# Limitations: Doesn't work for postgrSQL 9.0 and 8.X
# Currently, the following version numbers are known to this module:
#    "9.5" "9.4" "9.3" "9.2" "9.1"
#
# Benefits:
#  - works only for postgreSQL versions that have
#    CREATE EXTENSION
#  - Code grouped by OS
#  
# Note: postgreSQL gets installed in some regular places, so this is based on that
# regularity
#
# ----------------------------------------------------------------------------
# Usage:
# ----------------------------------------------------------------------------
# In your CMakeLists.txt file do something like this:
# ...
# # PostgreSQL
# FIND_PACKAGE(PostgreSQL 9.1)
# ...
# if( PostgreSQL_FOUND )
#   include_directories(${PostgreSQL_INCLUDE_DIRS})
#   link_directories(${PostgreSQL_LIBRARY_DIRS})
# endif( PostgreSQL_FOUND )
# ...
# Remember to include ${PostgreSQL_LIBRARIES} in the target_link_libraries() statement.
#
#
#  
# ----------------------------------------------------------------------------
# The following variables are set if PostgreSQL is found:
# ----------------------------------------------------------------------------
#
#   PostgreSQL_FOUND                    - Set to true when PostgreSQL is found.
#
#   PostgreSQL_VERSION                  - (mayor.minor) version
#   PostgreSQL_VERSION_STRING           - Full Version 
#
#   PostgreSQL_INCLUDE_DIRS             - List of Direcotries where the *.h are
#
#   PostgreSQL_LIBRARY_DIRS             - List of Directories where the *.so are
#   PostgreSQL_LIBRARIES                - List of *.so to be linked
#
#   PostgreSQL_EXTENSION_LIBRARY_DIRS   - Directory where the extension's *.so are installed
#   PostgreSQL_EXTENSION_DIRS           - Directory where the *.sql and control are installed
#
#
#
# ----------------------------------------------------------------------------
# History:
# This module is derived from the module originally found in the VTK source tree.
#
# ----------------------------------------------------------------------------
# Notes:
# - It looks for the most recent installation of postgreSQL
#
# - To look for a particular (mayor.minor) <version>
#    set(PostgreSQL_ADDITIONAL_VERSIONS <version1> <version2>)
#   before calling FIND_PACKAGE(PostgreSQL) in your CMakeLists.txt file.
#
#   This will mean that the versions you set here will be found first in the order
#   specified before the default ones are searched.
#  
#   set(PostgreSQL_DEBUG ON)            - for printing what was found (in case of success)
#
# ----------------------------------------------------------------------------
# If you have installed PostgreSQL in a non-standard location.
# ----------------------------------------------------------------------------
# You can manually set: (-Dflagname=value)
#
#   PostgreSQL_INCLUDE_DIR             - List of Direcotries where the *.h are
#
#   PostgreSQL_LIBRARY_DIR             - List of Directories where the *.so are
#   PostgreSQL_LIBRARY                - List of *.so to be linked
#
#   PostgreSQL_EXTENSION_LIBRARY_DIR   - Directory where the extension's *.so are installed
#   PostgreSQL_EXTENSION_DIR           - Directory where the *.sql and control are installed
#
# it will check if the information given is valid
#
# ----------------------------------------------------------------------------

set(PostgreSQL_INCLUDE_PATH_DESCRIPTION "top-level directory containing the PostgreSQL include directories. E.g /usr/local/include/PostgreSQL/8.4 or C:/Program Files/PostgreSQL/8.4/include")
set(PostgreSQL_INCLUDE_DIR_MESSAGE "Set the PostgreSQL_INCLUDE_DIR cmake cache entry to the ${PostgreSQL_INCLUDE_PATH_DESCRIPTION}")
set(PostgreSQL_LIBRARY_PATH_DESCRIPTION "top-level directory containing the PostgreSQL libraries.")
set(PostgreSQL_LIBRARY_DIR_MESSAGE "Set the PostgreSQL_LIBRARY_DIR cmake cache entry to the ${PostgreSQL_LIBRARY_PATH_DESCRIPTION}")
set(PostgreSQL_ROOT_DIR_MESSAGE "Set the PostgreSQL_ROOT system variable to where PostgreSQL is found on the machine E.g C:/Program Files/PostgreSQL/8.4")

# Only versions with extensions
set(PostgreSQL_KNOWN_VERSIONS ${PostgreSQL_ADDITIONAL_VERSIONS}
    "9.5" "9.4" "9.3" "9.2" "9.1")

set(PostgreSQL_ROOT_DIRECTORIES
    ENV PostgreSQL_ROOT
    ${PostgreSQL_ROOT}
    )


# Define additional search paths for root directories.
if ( WIN32 )
    foreach (suffix ${PostgreSQL_KNOWN_VERSIONS} )
        set(ADDITIONAL_SEARCH_PATHS ${ADDITIONAL_SEARCH_PATHS} "C:/Program Files/PostgreSQL/${suffix}" )
    endforeach(suffix)
endif( WIN32 )

set( PostgreSQL_ROOT_DIRECTORIES
    ${ADDITIONAL_SEARCH_PATHS}
    ${PostgreSQL_ROOT_DIRECTORIES}
    ${PostgreSQL_ROOT}
    )


#
# Look for an installation.
#
if (NOT EXISTS "${PostgreSQL_INCLUDE_DIR}")
    find_path(PostgreSQL_INCLUDE_DIR
        NAMES libpq-fe.h
        PATHS
        # Look in other places.
        ${PostgreSQL_ROOT_DIRECTORIES}
        PATH_SUFFIXES
        include
        pgsql
        postgresql
        # Help the user find it if we cannot.
        DOC "The ${PostgreSQL_INCLUDE_DIR_MESSAGE}"
        )


    if ( UNIX )
        # find where the includes fliles are installed for the particular version os postgreSQL
        foreach (suffix ${PostgreSQL_KNOWN_VERSIONS} )
            set(postgresql_additional_search_paths ${postgresql_additional_search_paths} "${PostgreSQL_INCLUDE_DIR}/${suffix}/server" )
        endforeach()
    endif()

    #
    # Look for THE installation.
    #
    find_path(PostgreSQL_TYPE_INCLUDE_DIR
        NAMES catalog/pg_type.h
        PATHS
        # Look in other places.
        ${postgresql_additional_search_paths}
        ${PostgreSQL_ROOT_DIRECTORIES}
        PATH_SUFFIXES
        include
        postgresql
        pgsql/server
        postgresql/server
        include/server
        # Help the user find it if we cannot.
        DOC "The ${PostgreSQL_INCLUDE_DIR_MESSAGE}"
        )

    if (UNIX)
        set (PostgreSQL_INCLUDE_DIR ${PostgreSQL_TYPE_INCLUDE_DIR})
    endif()

    # pgRouting does not use libpq-fe so we can ignore its directory
    unset(postgresql_additional_search_paths)
    #endif()

endif()


if (EXISTS ${PostgreSQL_INCLUDE_DIR} AND EXISTS "${PostgreSQL_INCLUDE_DIR}/pg_config.h")
    file(STRINGS "${PostgreSQL_INCLUDE_DIR}/pg_config.h" pgsql_version_str
        REGEX "^#define[\t ]+PG_VERSION[\t ]+\".*\"")

    string(REGEX REPLACE "^#define[\t ]+PG_VERSION[\t ]+\"([^\"]*)\".*" "\\1"
        PostgreSQL_VERSION_STRING "${pgsql_version_str}")
    unset(pgsql_version_str)
endif()
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+).*" "\\1.\\2" PostgreSQL_VERSION ${PostgreSQL_VERSION_STRING})




if ( APPLE )

    # TODO Fill for APPLE

endif()

if ( WIN32 )

    #need to remove the word include
    string(REGEX REPLACE "/include" ""
        striped_word_include "${PostgreSQL_INCLUDE_DIR}")

    #-- Installing: e:/jenkins/postgresql/rel/pg9.5w64gcc48/share/extension/pgrouting.control
    if (NOT EXISTS "${PostgreSQL_EXTENSION_DIR}")
        set(PostgreSQL_EXTENSION_DIR "${striped_word_include}/share/extension")
    endif()

    #- Installing: e:/jenkins/postgresql/rel/pg9.5w64gcc48/lib/libpgrouting--2.2.0.dll.a
    if (NOT EXISTS "${PostgreSQL_EXTENSION_LIBRARY_DIR}")
        set(PostgreSQL_EXTENSION_LIBRARY_DIR "${striped_word_include}/lib")
    endif()

    #--   Where all library are: e:/jenkins/postgresql/rel/pg9.5w64gcc48/bin
    if (NOT EXISTS "${PostgreSQL_LIBRARY_DIR}")
        set(PostgreSQL_LIBRARY_DIR "${striped_word_include}/bin")
    endif()

    #--   Library: e:/jenkins/postgresql/rel/pg9.5w64gcc48/bin/libpq.dll
    if (NOT EXISTS "${PostgreSQL_LIBRARY}")
        set(PostgreSQL_LIBRARY "${striped_word_include}/bin/libpq.dll")
    endif()

    set(PostgreSQL_INCLUDE_DIR "${striped_word_include}/include")
    unset(strip_word_include)
endif()


if ( UNIX )
    if (NOT EXISTS "${PostgreSQL_LIBRARY}")
        set(PostgreSQL_LIBRARY "/usr/lib/libpq.so")
    endif()

    if (NOT EXISTS PostgreSQL_LIBRARY_DIR)
        set(PostgreSQL_LIBRARY_DIR "/usr/lib/postgresql")
    endif()

    if (NOT EXISTS PostgreSQL_EXTENSION_LIBRARY_DIR)
        set(PostgreSQL_EXTENSION_LIBRARY_DIR "/usr/lib/postgresql/${PostgreSQL_VERSION}/lib")
    endif()

    if (NOT EXISTS PostgreSQL_EXTENSION_DIR)
        set(PostgreSQL_EXTENSION_DIR "/usr/share/postgresql/${PostgreSQL_VERSION}/extension")
    endif()

endif()


# Did we find the things needed for pgRouting?
set( PostgreSQL_FOUND FALSE )
if ( EXISTS "${PostgreSQL_INCLUDE_DIR}" AND
        EXISTS "${PostgreSQL_TYPE_INCLUDE_DIR}" AND
        EXISTS "${PostgreSQL_LIBRARY_DIR}" AND
        EXISTS "${PostgreSQL_EXTENSION_LIBRARY_DIR}" AND
        EXISTS "${PostgreSQL_EXTENSION_DIR}" )
    set( PostgreSQL_FOUND TRUE )
    if (WIN32)
        set (PostgreSQL_INCLUDE_DIRS
            "${PostgreSQL_TYPE_INCLUDE_DIR}" "${PostgreSQL_INCLUDE_DIR}"
            "${PostgreSQL_TYPE_INCLUDE_DIR}/port/win32" "${PostgreSQL_INCLUDE_DIR}/port/win32")
    else()
        set( PostgreSQL_INCLUDE_DIRS ${PostgreSQL_INCLUDE_DIR})
    endif()
    set( PostgreSQL_LIBRARY_DIRS ${PostgreSQL_LIBRARY_DIR})
    set( PostgreSQL_LIBRARIES ${PostgreSQL_LIBRARY})
    set( PostgreSQL_EXTENSION_LIBRARY_DIRS ${PostgreSQL_EXTENSION_LIBRARY_DIR})
    set( PostgreSQL_EXTENSION_DIRS ${PostgreSQL_EXTENSION_DIR})

    if(PostgreSQL_DEBUG)
        message("
        PostgreSQL_VERSION: ${PostgreSQL_VERSION}
        PostgreSQL_VERSION_STRING: ${PostgreSQL_VERSION_STRING}
        PostgreSQL_INCLUDE_DIR: ${PostgreSQL_INCLUDE_DIR}
        PostgreSQL_TYPE_INCLUDE_DIR: ${PostgreSQL_TYPE_INCLUDE_DIR}
        PostgreSQL_INCLUDE_DIRS: ${PostgreSQL_INCLUDE_DIRS}
        PostgreSQL_LIBRARY_DIRS: ${PostgreSQL_LIBRARY_DIRS}
        PostgreSQL_EXTENSION_LIBRARY_DIRS: ${PostgreSQL_EXTENSION_LIBRARY_DIRS}
        PostgreSQL_EXTENSION_DIRS: ${PostgreSQL_EXTENSION_DIRS}
        PostgreSQL_LIBRARIES: ${PostgreSQL_LIBRARIES}")
    endif()

else()
    message(FATAL_ERROR "PostgreSQL was not found. ${PostgreSQL_DIR_MESSAGE}
    PostgreSQL_VERSION_STRING: ${PostgreSQL_VERSION_STRING}
    PostgreSQL_INCLUDE_DIR: ${PostgreSQL_INCLUDE_DIR}
    PostgreSQL_TYPE_INCLUDE_DIR: ${PostgreSQL_TYPE_INCLUDE_DIR}
    PostgreSQL_EXTENSION_LIBRARY_DIR: ${PostgreSQL_EXTENSION_LIBRARY_DIR}
    PostgreSQL_LIBRARY_DIR: ${PostgreSQL_LIBRARY_DIR}
    PostgreSQL_EXTENSION_DIR: ${PostgreSQL_EXTENSION_DIR}
    PostgreSQL_LIBRARY: ${PostgreSQL_LIBRARY}")
endif()

