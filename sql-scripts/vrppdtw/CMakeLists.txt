
SET(LOCAL_FILES
    gsoc_vrppdtw.sql
    )

foreach (f ${LOCAL_FILES}) 
    configure_file(${f} ${f})
endforeach()

# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/gsoc_vrppdtw.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
