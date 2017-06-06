
SET(LOCAL_FILES
    flow-family.rst
    pgr_maxFlow.rst
    pgr_boykovKolmogorov.rst
    pgr_edmondsKarp.rst
    pgr_edgeDisjointPaths.rst
    pgr_maxCardinalityMatch.rst
    pgr_pushRelabel.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PgRouting_DOC_FILES ${PgRouting_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)

