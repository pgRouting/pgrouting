
SET(LOCAL_FILES
    index.rst
    pgRouting-concepts.rst
    pgRouting-installation.rst
    pgRouting-introduction.rst
    proposed.rst
    experimental.rst
    release_notes.rst
    routingFunctions.rst
    sampledata.rst
    support.rst
    cost-category.rst
    costMatrix-category.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PGROUTING_DOC_FILES ${PGROUTING_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)

