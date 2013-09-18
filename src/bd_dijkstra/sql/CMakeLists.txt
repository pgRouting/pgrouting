# Append in local scope
list(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/routing_bd_dijkstra.sql)

# set in parent scope
set(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)

