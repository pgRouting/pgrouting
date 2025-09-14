SET(LOCAL_FILES
    pgr_extractVertices.rst
    pgr_findCloseEdges.rst
    pgr_separateCrossing.rst
    pgr_separateTouching.rst
    utilities-family.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PROJECT_DOC_FILES ${PROJECT_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)

