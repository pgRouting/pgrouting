# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/withPointsDmatrix.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/dijkstraDmatrix.sql

    #for version 3.0
    #${CMAKE_CURRENT_SOURCE_DIR}/astarDmatrix.sql

    # coded but not used
    #${CMAKE_CURRENT_SOURCE_DIR}/eucledianDmatrix.sql
    )

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
