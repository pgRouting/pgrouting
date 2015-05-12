# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_version.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_utilities.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_topology.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_analytics.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_network_check.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_node_network.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_conversion_tools.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_dmatrix_tools.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgr_parameter_check.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
#MESSAGE("core/common/sql: ${PACKAGE_SQL_FILES}")
