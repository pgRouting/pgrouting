
SET(LOCAL_FILES
    _dijkstra.sql
    dijkstra.sql
    dijkstraCost.sql
    dijkstraCostMatrix.sql
    _dijkstraVia.sql
    dijkstraVia.sql
    dijkstraNear.sql
    dijkstraNearCost.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PROJECT_SQL_FILES ${PROJECT_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
