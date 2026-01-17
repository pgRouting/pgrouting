# This file is part of the pgRouting project.
# Copyright (c) 2025-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

set(LOCAL_FILES
  custom.css
  page_history.js
)

foreach (f ${LOCAL_FILES})
  configure_file(${f} ${f} @ONLY)
  list(APPEND LOCAL_DOC_FILES ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

configure_file(favicon.ico favicon.ico COPYONLY)
list(APPEND LOCAL_DOC_FILES ${CMAKE_CURRENT_BINARY_DIR}/favicon.ico)

add_subdirectory(images)
set(PROJECT_DOC_FILES ${PROJECT_DOC_FILES} ${LOCAL_DOC_FILES} PARENT_SCOPE)
