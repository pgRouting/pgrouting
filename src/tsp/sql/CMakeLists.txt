# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/routing_tsp.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/routing_tsp_wrappers.sql
    
    #good with BIGINT
    ${CMAKE_CURRENT_SOURCE_DIR}/eucledianDMatrix.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/_makeMatrix.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
