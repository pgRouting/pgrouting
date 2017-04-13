
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

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

add_custom_target(common_sql DEPENDS ${PACKAGE_SQL_FILES} SOURCES ${LOCAL_FILES})

set(PgRouting_SQL_FILES ${PgRouting_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)

configure_file(pgrouting.control pgrouting.control)
add_custom_target(pgrouting_control DEPENDS pgrouting.control)
