# Do not use extensions
SET(LOCAL_FILES
    doc-pgr_boykovKolmogorov
    doc-pgr_edgeDisjointPaths
    doc-pgr_edmondsKarp
    doc-pgr_maxCardinalityMatch
    doc-pgr_maxFlow
    doc-pgr_pushRelabel
    doc-pgr_maxFlowMinCost
    doc-pgr_maxFlowMinCost_Cost

    )

foreach (f ${LOCAL_FILES})
    configure_file("${f}.result" "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}.queries")
    list(APPEND LOCAL_DOC_FILES  "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}.queries")
endforeach()

set(PGROUTING_DOC_FILES ${PGROUTING_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
