
SET(LOCAL_FILES
    flow-family.rst
    pgr_maxFlow.rst
    pgr_boykovKolmogorov.rst
    pgr_edmondsKarp.rst
    pgr_edgeDisjointPaths.rst
    pgr_maxCardinalityMatch.rst
    pgr_pushRelabel.rst
    pgr_maxFlowMinCost.rst
    # TODO rename
    pgr_maxFlowMinCost_Cost.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PGROUTING_DOC_FILES ${PGROUTING_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)

