
SET(LOCAL_FILES
    _lineGraphFull.sql
    lineGraphFull.sql
    _lineGraph.sql
    lineGraph.sql
    )

# Do not modify below this line

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PROJECT_SQL_FILES ${PROJECT_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
