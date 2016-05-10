# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/dijkstra.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/dijkstraCost.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/dijkstraVia.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/dijkstraDmatrix.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
