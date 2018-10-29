
SET(LOCAL_FILES
    pgr_randomSpanTree.rst
    spanningTree-family.rst
    pgr_kruskal.rst
    pgr_kruskalDD.rst
    pgr_kruskalDFS.rst
    pgr_prim.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PGROUTING_DOC_FILES ${PGROUTING_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)

