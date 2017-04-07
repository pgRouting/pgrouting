# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/withPoints.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/withPointsCost.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/withPointsVia.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
