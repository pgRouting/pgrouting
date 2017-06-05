
SET(LOCAL_FILES
    costMatrix-category.rst
    pgr_aStarCostMatrix.rst
    pgr_bdDijkstraCostMatrix.rst
    pgr_withPointsCostMatrix.rst
    cost-category.rst
    pgr_bdAstarCostMatrix.rst
    pgr_dijkstraCostMatrix.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PgRouting_DOC_FILES ${PgRouting_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)

