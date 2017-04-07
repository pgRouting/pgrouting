
SET(LOCAL_FILES
    withPointsCostMatrix.sql
    dijkstraCostMatrix.sql
    bdDijkstraCostMatrix.sql
    astarCostMatrix.sql
    bdAstarCostMatrix.sql
    )

foreach (f ${LOCAL_FILES}) 
    configure_file(${f} ${f})
    LIST(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
