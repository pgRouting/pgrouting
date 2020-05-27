
SET(LOCAL_FILES
	_bellman_ford.sql
    bellman_ford.sql
    #_bellman_ford_neg.sql
    #bellman_ford_neg.sql
    _edwardMoore.sql
    edwardMoore.sql
    )

# Do not modify below this line

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PGROUTING_SQL_FILES ${PGROUTING_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
