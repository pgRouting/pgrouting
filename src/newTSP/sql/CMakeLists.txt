# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    
    ${CMAKE_CURRENT_SOURCE_DIR}/eucledianDMatrix.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/eucledianTSP.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/xyd_tsp.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
