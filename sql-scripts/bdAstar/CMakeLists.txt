
SET(LOCAL_FILES
    _bdAstar.sql
    bdAstar.sql
    bdAstarCost.sql
    )

foreach (f ${LOCAL_FILES}) 
    configure_file(${f} ${f})
endforeach()

# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/_bdAstar.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/bdAstar.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/bdAstarCost.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
