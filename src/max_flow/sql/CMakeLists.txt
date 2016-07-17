# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/max_flow_one_to_one.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/max_flow_many_to_many.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/max_flow_many_to_one.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/max_flow_one_to_many.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/maximum_cardinality_matching.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/edge_disjoint_paths.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/flow_helpers.sql

)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
