
SET(LOCAL_FILES
    noUnderUtilities.sql
    bd_astar_v2.sql
    bdDijkstra-v2.sql
    pgrouting_conversion_tools.sql
    pgrouting_dmatrix_tools.sql
    pgr_vidsToDMatrix.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PgRouting_SQL_FILES ${PgRouting_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
