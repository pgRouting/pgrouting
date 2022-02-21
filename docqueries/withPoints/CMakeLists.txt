# Do not use extensions
SET(LOCAL_FILES
    doc-pgr_withPointsCostMatrix
    doc-pgr_withPointsCost
    doc-pgr_withPointsDD
    doc-pgr_withPointsKSP-cygwin
    doc-pgr_withPointsKSP
    doc-pgr_withPoints
    withPointsVia
    )

foreach (f ${LOCAL_FILES})
    configure_file("${f}.result" "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}.queries")
    list(APPEND LOCAL_DOC_FILES  "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}.queries")
endforeach()

set(PROJECT_DOC_FILES ${PROJECT_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
