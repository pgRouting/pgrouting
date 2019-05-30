
SET(LOCAL_FILES
    _connectedComponents.sql
    connectedComponents.sql
    _strongComponents.sql
    strongComponents.sql
    _biconnectedComponents.sql
    biconnectedComponents.sql
    _articulationPoints.sql
    articulationPoints.sql
    _bridges.sql
    bridges.sql
    )

# Do not modify below this line

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PGROUTING_SQL_FILES ${PGROUTING_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
