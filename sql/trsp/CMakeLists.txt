
SET(LOCAL_FILES
    _trsp.sql
    trsp.sql
    trsp_V2.2.sql
    vias_trsp_V2.2.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PgRouting_SQL_FILES ${PgRouting_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
