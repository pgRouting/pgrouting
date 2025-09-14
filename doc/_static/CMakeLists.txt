
SET(LOCAL_FILES
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
