
SET(LOCAL_FILES
    contraction-family.rst
    pgr_contraction.rst
    pgr_contractionDeadEnd.rst
    pgr_contractionLinear.rst
    pgr_contractionHierarchies.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

add_subdirectory("images")
set(PROJECT_DOC_FILES ${PROJECT_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
set(PROJECT_IMG_FILES ${PROJECT_IMG_FILES} PARENT_SCOPE)

