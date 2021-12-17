
SET(LOCAL_FILES
  _trsp.sql
  trsp.sql

  _trspVia.sql
  trspVia.sql

  _trsp_withPoints.sql
  trsp_withPoints.sql

  _array_reverse.sql
  pgr_trspViaEdges.sql

  )

foreach (f ${LOCAL_FILES})
  configure_file(${f} ${f})
  list(APPEND PACKAGE_SQL_FILES  ${CMAKE_CURRENT_BINARY_DIR}/${f})
endforeach()

set(PROJECT_SQL_FILES ${PROJECT_SQL_FILES} ${PACKAGE_SQL_FILES} PARENT_SCOPE)
