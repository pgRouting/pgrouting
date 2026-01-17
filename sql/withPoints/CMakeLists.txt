# This file is part of the pgRouting project.
# Copyright (c) 2015-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

set(LOCAL_FILES
    _withPoints.sql
    withPoints.sql
    withPointsCost.sql
    withPointsCostMatrix.sql
    _withPointsVia.sql
    withPointsVia.sql
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PROJECT_SQL_FILES ${PROJECT_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
