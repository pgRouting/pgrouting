
SET(LOCAL_FILES
    pgRouting-header.sql
    pgrouting-types.sql
    pgrouting_version.sql
    pgrouting_utilities.sql
    pgr_parameter_check.sql
    utilities_pgr.sql
    createIndex.sql
    _point_toId.sql
    )

# Do not modify bellow this line

foreach (f ${LOCAL_FILES})
    LIST(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${f} ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

configure_file(pgrouting.control pgrouting.control)

add_custom_target (common_sql DEPENDS ${LOCAL_FILES} SOURCES ${LOCAL_FILES})
add_custom_target (pgrouting_control DEPENDS pgrouting.control)


SET(PACKAGE_SQL_FILES ${PACKAGE_SQL_FILES} PARENT_SCOPE)

