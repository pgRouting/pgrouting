
SET(LOCAL_FILES
    routing_ksp.sql
    withPoints_ksp.sql
    )

foreach (f ${LOCAL_FILES}) 
    configure_file(${f} ${f})
endforeach()

# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/routing_ksp.sql 
    ${CMAKE_CURRENT_SOURCE_DIR}/withPoints_ksp.sql 
)
        
# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)

