SET(LOCAL_FILES
    _cuthillMckeeOrdering.sql
    cuthillMckeeOrdering.sql
    _minDegreeOrdering.sql
    minDegreeOrdering.sql
    _sloanOrdering.sql
    sloanOrdering.sql
    _kingOrdering.sql
    kingOrdering.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PROJECT_SQL_FILES ${PROJECT_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
