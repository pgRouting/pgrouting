
SET(LOCAL_FILES
    twoNodesoneEdge_a.png
    twoNodesoneEdge_b.png

    threeNodestwoEdges_a.png
    threeNodestwoEdges_b.png
    threeNodestwoEdges_c.png

    undirected_sampledata_b.png
    undirected_sampledata_a.png
    undirected_sampledata_c.png
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/images/${f}" COPYONLY)
    list(APPEND LOCAL_IMG_FILES  "${PGR_DOCUMENTATION_SOURCE_DIR}/images/${f}")
endforeach()

set(PgRouting_IMG_FILES ${PgRouting_IMG_FILES} ${LOCAL_IMG_FILES} PARENT_SCOPE)

