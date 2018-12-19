
SET(LOCAL_FILES
    pgr_analyzeGraph.rst
    pgr_createTopology.rst
    pgr_nodeNetwork.rst
    pgr_analyzeOneWay.rst
    pgr_createVerticesTable.rst
    pgr_extractVertices.rst
    topology-functions.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()


add_subdirectory("images")
set(PGROUTING_DOC_FILES ${PGROUTING_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
set(PGROUTING_IMG_FILES ${PGROUTING_IMG_FILES} PARENT_SCOPE)

