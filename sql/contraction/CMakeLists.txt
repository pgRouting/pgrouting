
SET(LOCAL_FILES
    _contraction.sql
    contraction.sql
    _contractionHierarchies.sql
    contractionHierarchies.sql
    deadEndContraction.sql
    linearContraction.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PROJECT_SQL_FILES ${PROJECT_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
