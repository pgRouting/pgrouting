
SET(LOCAL_FILES
    withPoints.sql
    withPointsCost.sql
    withPointsVia.sql
    )

foreach (f ${LOCAL_FILES}) 
    configure_file(${f} ${f})
    LIST(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

add_custom_target(withPoints_sql DEPENDS ${LOCAL_FILES})

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
