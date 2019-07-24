# Do not use extensions
SET(LOCAL_FILES
    doc-pgr_kruskalBFS
    doc-pgr_kruskalDD
    doc-pgr_kruskalDFS
    doc-pgr_kruskal
    doc-pgr_primBFS
    doc-pgr_primDD
    doc-pgr_primDFS
    doc-pgr_prim
    doc-pgr_randomSpanTree
    )

foreach (f ${LOCAL_FILES})
    configure_file("${f}.result" "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}.queries")
    list(APPEND LOCAL_DOC_FILES  "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}.queries")
endforeach()

set(PGROUTING_DOC_FILES ${PGROUTING_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
