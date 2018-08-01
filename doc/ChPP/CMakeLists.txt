
SET(LOCAL_FILES
    ChPP-family.rst
    pgr_directedChPP.rst
    pgr_directedChPP_Cost.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PgRouting_DOC_FILES ${PgRouting_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)

