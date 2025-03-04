
SET(LOCAL_FILES
    BFS-category.rst
    cost-category.rst
    costMatrix-category.rst
    DFS-category.rst
    drivingDistance-category.rst
    KSP-category.rst
    spanningTree-category.rst
    via-category.rst
    VRP-category.rst
    withPoints-category.rst
)

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PROJECT_DOC_FILES ${PROJECT_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
set(PROJECT_IMG_FILES ${PROJECT_IMG_FILES} PARENT_SCOPE)
