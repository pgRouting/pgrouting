
SET(LOCAL_FILES
    _dijkstra.sql
    dijkstra.sql
    dijkstraCost.sql
    dijkstraVia.sql
    )

foreach (f ${LOCAL_FILES}) 
    configure_file(${f} ${f})
endforeach()


# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/_dijkstra.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/dijkstra.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/dijkstraCost.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/dijkstraVia.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
