
SET(LOCAL_FILES
    _maxflow.sql
    _maxFlowMinCost.sql
    edmondsKarp.sql
    boykovKolmogorov.sql
    pushRelabel.sql
    _maxCardinalityMatch.sql
    maxCardinalityMatch.sql
    _edgeDisjointPaths.sql
    edgeDisjointPaths.sql
    maxFlowMinCost.sql
    maxFlowMinCost_Cost.sql
    maxFlow.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PGROUTING_SQL_FILES ${PGROUTING_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
