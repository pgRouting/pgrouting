# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/_max_flow.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/deprecated_max_flow.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/edmondsKarp.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/boykovKolmogorov.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/pushRelabel.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/maxFlow.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/maxCardinalityMatch.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/edge_disjoint_paths.sql

        #deprecated
        ${CMAKE_CURRENT_SOURCE_DIR}/maximum_cardinality_matching.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/deprecated_max_flow.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
