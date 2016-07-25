# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/max_flow.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/maximum_cardinality_matching.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/edge_disjoint_paths.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
