#
# Format convention
# - command (lower case)
# - VARIABLE (upper case)
# (http://www.cmake.org/cmake/help/v2.8.12/cmake.html)
#



#set (PGROUTING_DEBUG "1")

#-------------------
# add the signatrues subdirectory
#-------------------

add_subdirectory(sigs)


#-------------------
# From the parent get the subdirectories that have the SQL files
#
# configuring the PGROUTING_SQL_FILES variable
#-------------------

set(PGROUTING_MODULE_PATHNAME "$libdir/libpgrouting-${PGROUTING_LIB_VERSION}")

foreach (subdir ${PGROUTING_SQL_DIRECTORIES})
    add_subdirectory(${subdir})
endforeach()

if (PGROUTING_DEBUG)
    message(STATUS "PGROUTING_SQL_FILES=")
    message(STATUS "PGROUTING_MODULE_PATHNAME ${PGROUTING_MODULE_PATHNAME}")
    foreach (f ${PGROUTING_SQL_FILES})
        message(STATUS ${f})
    endforeach()
endif()



#-------------------
# TARGET: PGROUTING_SQL_FILE
#
# Builds:  pgrouting--${PGROUTING_VERSION}.sql
#
# add the scripts subdirectory
# the variable ${PGROUTING_SQL_FILES} should already be set at this point
#
# has the perl for building the current sql file
# has the perl for building the update files
#-------------------

set(PGROUTING_CURRENT_SQL_FILE "pgrouting--${PGROUTING_VERSION}.sql")

# subdirectory that has the perl files
add_subdirectory(scripts)



add_custom_command(
    OUTPUT ${PGROUTING_CURRENT_SQL_FILE}
    COMMAND ${PERL_EXECUTABLE} ./scripts/build-extension-file.pl '${PGROUTING_SQL_FILES}'
    DEPENDS
    scripts/build-extension-file.pl
    ${PGROUTING_SQL_FILES}
    )

add_custom_target(PGROUTING_SQL_FILE ALL
    DEPENDS
    ${PGROUTING_CURRENT_SQL_FILE}
    )

#-------------------
# TARGET: update_files
#-------------------

if (PGROUTING_DEBUG)
    message(STATUS "OLD_SIGNATURES=")
    foreach (s ${OLD_SIGNATURES})
        message(STATUS ${s})
    endforeach()
endif()

foreach (old_s ${OLD_SIGNATURES})
    SET (UPDATE_FILES ${UPDATE_FILES} pgrouting--${old_s}--${PGROUTING_VERSION}.sql)
    SET (PGROUTING_SQL_FILES_TO_INSTALL ${PGROUTING_SQL_FILES_TO_INSTALL} "${CMAKE_CURRENT_BINARY_DIR}/pgrouting--${old_s}--${PGROUTING_VERSION}.sql")
    add_custom_command(
        OUTPUT pgrouting--${old_s}--${PGROUTING_VERSION}.sql
        COMMAND ${PERL_EXECUTABLE} ./scripts/build-extension-update-files1.pl ${old_s} ${PGROUTING_DEBUG}
        DEPENDS
        ./scripts/build-extension-update-files1.pl
        ${PGROUTING_CURRENT_SQL_FILE}
        PGROUTING_SQL_FILE
        ./sigs/pgrouting--${old_s}.sig
        )
endforeach()

add_custom_target(update_files ALL
    DEPENDS
    ${UPDATE_FILES}
    )

SET(PGROUTING_SQL_FILES_TO_INSTALL  ${PGROUTING_SQL_FILES_TO_INSTALL} ${CMAKE_CURRENT_BINARY_DIR}/${PGROUTING_CURRENT_SQL_FILE} "${CMAKE_CURRENT_BINARY_DIR}/common/pgrouting.control" PARENT_SCOPE)
