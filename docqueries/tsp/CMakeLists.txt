# This file is part of the pgRouting project.
# Copyright (c) 2019-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
# Do not use extensions
set(LOCAL_FILES
    TSPeuclidean
    TSP
    )

configure_file("wi29.sql" "${PGR_DOCUMENTATION_SOURCE_DIR}/wi29.queries")

foreach (f ${LOCAL_FILES})
    configure_file("${f}.result" "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}.queries")
    list(APPEND LOCAL_DOC_FILES  "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}.queries")
endforeach()

set(PROJECT_DOC_FILES ${PROJECT_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
