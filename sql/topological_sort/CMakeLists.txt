
SET(LOCAL_FILES
    _topological_sort.sql
    topological_sort.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PGROUTING_SQL_FILES ${PGROUTING_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
