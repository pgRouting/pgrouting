# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    #This are the old signature
    ${CMAKE_CURRENT_SOURCE_DIR}/routing_tsp.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/routing_tsp_wrappers.sql

    #this one implements the new tsp
    ${CMAKE_CURRENT_SOURCE_DIR}/TSP.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/eucledianTSP.sql

    #this file wrap pgr_tsp with pgr_eucledianTSP
    ${CMAKE_CURRENT_SOURCE_DIR}/tsp_v2.0_coordinates.sql

    
    #${CMAKE_CURRENT_SOURCE_DIR}/_makeMatrix.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
