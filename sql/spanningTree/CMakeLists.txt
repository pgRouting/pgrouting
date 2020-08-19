
SET(LOCAL_FILES
    _prim.sql
    _kruskal.sql
    #_randomSpanTree.sql
    
    prim.sql
    primDFS.sql
    primBFS.sql
    primDD.sql

    kruskal.sql
    kruskalDFS.sql
    kruskalBFS.sql
    kruskalDD.sql
    #randomSpanTree.sql
    )

# Do not modify below this line

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PGROUTING_SQL_FILES ${PGROUTING_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
