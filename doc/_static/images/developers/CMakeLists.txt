# This file is part of the pgRouting project.
# Copyright (c) 2025-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
set(LOCAL_FILES
    "disconnectEdgeDirected.png"
    "disconnectEdgeUndirected.png"
    "disconnectVertexDirected.png"
    "disconnectVertexUndirected.png"
)

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f} COPYONLY)
    list(APPEND LOCAL_IMG_FILES "${CMAKE_CURRENT_BINARY_DIR}/${f}")
endforeach()

set(PROJECT_IMG_FILES ${PROJECT_IMG_FILES} ${LOCAL_IMG_FILES} PARENT_SCOPE)
