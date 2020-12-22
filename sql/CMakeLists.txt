add_subdirectory(sigs)


#-------------------
# From the parent get the subdirectories that have the SQL files
#
# configuring the PROJECT_SQL_FILES variable
#-------------------

set(PROJECT_MODULE_PATHNAME "$libdir/lib${PROJECT_NAME_LOWER}-${PROJECT_LIB_VERSION}")

foreach (subdir ${PROJECT_SQL_DIRECTORIES})
    add_subdirectory(${subdir})
endforeach()

if (PROJECT_DEBUG)
    message(STATUS "PROJECT_MODULE_PATHNAME ${PROJECT_MODULE_PATHNAME}")
endif()



#-------------------
# TARGET: PROJECT_SQL_FILE
#
# Builds:  ${PROJECT_NAME_LOWER}--${PROJECT_VERSION}.sql
#
# add the scripts subdirectory
# the variable ${PROJECT_SQL_FILES} should already be set at this point
#
# has the perl for building the current sql file
# has the perl for building the update files
#-------------------

set(PROJECT_EXTENSION_FILE "${PROJECT_NAME_LOWER}--${PROJECT_VERSION}.sql")

# subdirectory that has the perl files
add_subdirectory(scripts)



add_custom_command(
    OUTPUT ${PROJECT_EXTENSION_FILE}
    COMMAND ${PERL_EXECUTABLE} ./scripts/build-extension-file.pl '${PROJECT_SQL_FILES}'
    DEPENDS
    scripts/build-extension-file.pl
    ${PROJECT_SQL_FILES}
    )

add_custom_target(extension_file ALL
    DEPENDS
    ${PROJECT_EXTENSION_FILE}
    )

#-------------------
# TARGET: update_files
#-------------------
foreach (old_s ${OLD_SIGNATURES})
    string(REGEX REPLACE "^([0-9]+\\.[0-9]+).*" "\\1" minor_s ${old_s})
    SET (UPDATE_FILES ${UPDATE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME_LOWER}--${old_s}--${PROJECT_VERSION}.sql)
    add_custom_command(
        OUTPUT ${PROJECT_NAME_LOWER}--${old_s}--${PROJECT_VERSION}.sql

        COMMAND ${PERL_EXECUTABLE} ./scripts/build-extension-update-files.pl "${PROJECT_VERSION}" "${old_s}" "${CMAKE_CURRENT_BINARY_DIR}/sigs" "${CMAKE_CURRENT_BINARY_DIR}" "${PROJECT_DEBUG}"
        DEPENDS
        ./scripts/build-extension-update-files.pl
        ${PROJECT_EXTENSION_FILE}
        extension_file
        ./sigs/${PROJECT_NAME_LOWER}--${minor_s}.sig
        )
endforeach()

add_custom_target(update_files ALL
    DEPENDS
    ${UPDATE_FILES}
    )


SET(PROJECT_FILES_TO_INSTALL  ${UPDATE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_EXTENSION_FILE} ${CMAKE_CURRENT_BINARY_DIR}/common/${PROJECT_NAME_LOWER}.control PARENT_SCOPE)
