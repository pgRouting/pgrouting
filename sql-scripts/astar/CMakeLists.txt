
SET(LOCAL_FILES
    _astar.sql
    astar.sql
    astarCost.sql
    )

foreach (f ${LOCAL_FILES}) 
    configure_file(${f} ${f})
endforeach()

# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/_astar.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/astar.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/astarCost.sql
    )

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
