
SET(LOCAL_FILES
    tsp_v2.0_matrix.sql
    tsp_v2.0_coordinates.sql
    _makeDistanceMatrix.sql
    TSP.sql
    eucledianTSP.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PGROUTING_SQL_FILES ${PGROUTING_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
