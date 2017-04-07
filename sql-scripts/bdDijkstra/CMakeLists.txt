# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/_bdDijkstra.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/bdDijkstra.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/bdDijkstraCost.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
