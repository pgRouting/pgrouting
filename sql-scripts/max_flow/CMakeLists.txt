
SET(LOCAL_FILES
    _max_flow.sql
    edmondsKarp.sql
    boykovKolmogorov.sql
    pushRelabel.sql
    maxFlow.sql
    maxCardinalityMatch.sql
    edge_disjoint_paths.sql
    )

foreach (f ${LOCAL_FILES}) 
    configure_file(${f} ${f})
    LIST(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

add_custom_target(max_flow_sql DEPENDS ${LOCAL_FILES})

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
