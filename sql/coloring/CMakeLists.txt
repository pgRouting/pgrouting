
SET(LOCAL_FILES
    _sequentialVertexColoring.sql
    sequentialVertexColoring.sql
    _bipartite.sql
    bipartite.sql
    _edgeColoring.sql
    edgeColoring.sql
    )

# Do not modify below this line



foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PROJECT_SQL_FILES ${PROJECT_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
