
SET(LOCAL_FILES
    index.rst
    pgRouting-concepts.rst
    pgRouting-installation.rst
    pgRouting-introduction.rst
    proposed.rst
    release_notes.rst
    routingFunctions.rst
    sampledata.rst
    support.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PgRouting_DOC_FILES ${PgRouting_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)

