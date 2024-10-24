
SET(LOCAL_FILES
    pgRouting-header.sql
    _point_toId.sql
    _startPoint.sql
    _endPoint.sql
    pgrouting_utilities.sql
    parameter_check.sql
    utilities_pgr.sql
    createIndex.sql
    _checkquery.sql
    _checkcolumn.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PROJECT_SQL_FILES ${PROJECT_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)

configure_file(${PROJECT_NAME_LOWER}.control ${PROJECT_NAME_LOWER}.control)
