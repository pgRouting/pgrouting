# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/kdijkstra.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/kdijkstra-V2.2.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
