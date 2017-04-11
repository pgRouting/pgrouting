
SET(LOCAL_FILES
    MY_FUNCTION_NAME.sql
    )

# Do not modify bellow this line

foreach (f ${LOCAL_FILES})
    LIST(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
    configure_file(${f} ${f})
endforeach()

add_custom_target(MY_FUNCTION_NAME_sql DEPENDS ${LOCAL_FILES})


SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
