
SET(LOCAL_FILES
    tsp_v2.0_matrix.sql
    tsp_v2.0_coordinates.sql
    routing_tsp_wrappers.sql
    TSP.sql
    eucledianTSP.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PgRouting_SQL_FILES ${PgRouting_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
