
SET(LOCAL_FILES
     transformation-family.rst
     pgr_lineGraphFull.rst
     pgr_lineGraph.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

add_subdirectory("images")
set(PGROUTING_DOC_FILES ${PGROUTING_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
set(PGROUTING_IMG_FILES ${PGROUTING_IMG_FILES} PARENT_SCOPE)
