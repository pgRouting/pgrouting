
SET(LOCAL_FILES
    pgr_withPointsDD.rst
    pgr_withPoints.rst
    withPoints-family.rst
    pgr_withPointsCost.rst
    pgr_withPointsKSP.rst
    pgr_withPointsVia.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()


add_subdirectory("images")
set(PgRouting_DOC_FILES ${PgRouting_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
set(PgRouting_IMG_FILES ${PgRouting_IMG_FILES} PARENT_SCOPE)

