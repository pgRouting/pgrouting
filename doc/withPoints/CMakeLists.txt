
SET(LOCAL_FILES
    pgr_withPointsDD.rst
    pgr_withPoints.rst
    withPoints-family.rst
    pgr_withPointsCost.rst
    pgr_withPointsCostMatrix.rst
    pgr_withPointsKSP.rst
    pgr_withPointsVia.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()


add_subdirectory("images")
set(PROJECT_DOC_FILES ${PROJECT_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
set(PROJECT_IMG_FILES ${PROJECT_IMG_FILES} PARENT_SCOPE)

