# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_topology.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_analytics.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/create_vertices_table.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/nodeNetwork.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
