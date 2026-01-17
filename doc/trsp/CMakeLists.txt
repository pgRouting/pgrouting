# This file is part of the pgRouting project.
# Copyright (c) 2017-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

set(LOCAL_FILES
    TRSP-family.rst
    pgr_trsp.rst
    pgr_trspVia.rst
    pgr_trspVia_withPoints.rst
    pgr_trsp_withPoints.rst
    pgr_turnRestrictedPath.rst
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/${f}")
    list(APPEND LOCAL_DOC_FILES  ${PGR_DOCUMENTATION_SOURCE_DIR}/${f})
endforeach()

set(PROJECT_DOC_FILES ${PROJECT_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)

