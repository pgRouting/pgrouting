# This file is part of the pgRouting project.
# Copyright (c) 2013-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

set(LOCAL_FILES
    _dijkstra.sql
    dijkstra.sql
    dijkstraCost.sql
    dijkstraCostMatrix.sql
    _dijkstraVia.sql
    dijkstraVia.sql
    dijkstraNear.sql
    dijkstraNearCost.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PROJECT_SQL_FILES ${PROJECT_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
