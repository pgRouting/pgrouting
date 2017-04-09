
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

add_custom_target (common_sql DEPENDS ${LOCAL_FILES})
add_custom_target (pgrouting_control DEPENDS pgrouting.control)
configure_file(pgrouting.control pgrouting.control)

foreach (f ${LOCAL_FILES}) 
    configure_file(${f} ${f})
    LIST(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()


# set in parent scope
SET(PACKAGE_SQL_FILES ${PACKAGE_SQL_FILES} PARENT_SCOPE)
