# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/routing_core_wrappers.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/routing_topology.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/matching.sql)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
#MESSAGE("core/common/sql: ${PACKAGE_SQL_FILES}")
