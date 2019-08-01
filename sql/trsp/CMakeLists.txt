
SET(LOCAL_FILES
    _trsp.sql
    trsp.sql
    _pgr_trsp.sql
    _array_reverse.sql
    pgr_trsp.sql

    _pgr_trspViaVertices.sql
    pgr_trspViaVertices.sql

    pgr_trspViaEdges.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PGROUTING_SQL_FILES ${PGROUTING_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
