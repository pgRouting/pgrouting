#
# Format convention
# - command (lower case)
# - VARIABLE (upper case)
# (http://www.cmake.org/cmake/help/v2.8.12/cmake.html)
#

if (false)
if(POLICY CMP0046)
    cmake_policy(SET CMP0046 OLD)
endif()


if(POLICY CMP0046)
   cmake_policy(SET CMP0046 OLD)
endif()

if(MSVC)
    cmake_policy(SET CMP0046 OLD)
endif()

#=============================================

if (PgRouting_DEBUG)
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
endif()

set (PgRouting_DEBUG "0")
#-------------------
# add the scripts subdirectory
# has the perl for building the update files
#-------------------

add_subdirectory(scripts)
add_subdirectory(sigs)


#-------------------
# From the parent get the subdirectories that have the SQL files
#-------------------

set(PgRouting_SQL_DIRECTORIES
    ${PgRouting_SOURCE_NAMES}
    ${PgRouting_SQL_DIRECTORIES})

set(PACKAGE_SQL_FILES "")

foreach (subdir ${PgRouting_SQL_DIRECTORIES})
    add_subdirectory(${subdir})
    LIST(APPEND SCRIPT_DEPENDENCIES ${subdir}/${subdir}_sql)
endforeach()

if (PgRouting_DEBUG)
    message(STATUS "SCRIPT_DEPENDENCIES=")
    foreach (d ${SCRIPT_DEPENDENCIES})
        message(STATUS ${d})
    endforeach()
endif()


#-------------------
# Target current_script
#
# 
#-------------------

set(file_out "pgrouting--${PGROUTING_VERSION}.sql")


if (PgRouting_DEBUG)
    message(STATUS "PACKAGE_SQL_FILES=")
    foreach (f ${PACKAGE_SQL_FILES})
        message(STATUS ${f})
    endforeach()
endif()

add_custom_command(
    OUTPUT ${file_out}
    COMMAND ${PERL_EXECUTABLE} ./scripts/build-extension-file.pl '${PACKAGE_SQL_FILES}'
    DEPENDS ${SCRIPT_DEPENDENCIES} build_update_script ${PACKAGE_SQL_FILES}
    )

add_custom_target(current_script ALL
    DEPENDS
    ${file_out}
    )

#-------------------
# Target update_files
#
#
#-------------------

message(STATUS "THIS CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")

if (PgRouting_DEBUG)
    message(STATUS "OLD_SIGNATURES=")
    foreach (s ${OLD_SIGNATURES})
        message(STATUS ${s})
    endforeach()
endif()

foreach (old_s ${OLD_SIGNATURES})
    SET (UPDATE_FILES ${UPDATE_FILES} pgrouting--${old_s}--${PGROUTING_VERSION}.sql)
    SET (PgRouting_SQL_FILES ${PgRouting_SQL_FILES} "${CMAKE_BINARY_DIR}/sql-scripts/pgrouting--${old_s}--${PGROUTING_VERSION}.sql")
    add_custom_command(
        OUTPUT pgrouting--${old_s}--${PGROUTING_VERSION}.sql
        COMMAND ${PERL_EXECUTABLE} ./scripts/build-extension-update-files1.pl ${old_s} ${PgRouting_DEBUG}
        DEPENDS ${SCRIPT_DEPENDENCIES}
        ${file_out}
        build_update_script
        signatures_sigs
        current_script
        ./sigs/pgrouting--${old_s}.sig
        )
endforeach()

add_custom_target(update_files ALL
    DEPENDS
    ${UPDATE_FILES}
    )

SET(PgRouting_SQL_FILES  ${PgRouting_SQL_FILES} ${CMAKE_BINARY_DIR}/sql-scripts/${file_out} PARENT_SCOPE)

