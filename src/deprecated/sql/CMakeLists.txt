# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/apsp_johnson_v2.2.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/apsp_warshall_v2.2.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/astarV2.0.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/bd_astar_v2.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/bdDijkstra-v2.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/kdijkstra-V2.2.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_conversion_tools.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_dmatrix_tools.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgr_vidsToDMatrix.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/dijkstra_v2.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/routing_dd.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/maximum_cardinality_matching.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/max_flow-deprecated.sql

    ${CMAKE_CURRENT_SOURCE_DIR}/deprecated-comments.sql
    )

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
