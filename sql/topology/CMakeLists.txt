
SET(LOCAL_FILES
    pgrouting_topology.sql
    pgrouting_analytics.sql
    create_vertices_table.sql
    nodeNetwork.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PgRouting_SQL_FILES ${PgRouting_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
