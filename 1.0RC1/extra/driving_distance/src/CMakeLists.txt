EXEC_PROGRAM(pg_config
 ARGS --libdir
 OUTPUT_VARIABLE LIB_DIR)

IF(LIB_DIR)
  MESSAGE("Output directory for libraries is set to ${LIB_DIR}")
ELSE(LIB_DIR)
  SET(LIB_DIR ${PGROUTING_BINARY_DIR}/lib)
  MESSAGE("pg_config not found, output directory for libraries is set to ${PGROUTING_BINARY_DIR}/lib")
ENDIF(LIB_DIR)

IF(UNIX)
  SET(LIBRARY_OUTPUT_PATH ${LIB_DIR})
  SET(SQL_INSTALL_PATH /usr/share/postlbs)
ELSE(UNIX)
  SET(LIBRARY_OUTPUT_PATH ${PGROUTING_BINARY_DIR}/lib)
  SET(SQL_INSTALL_PATH ${PGROUTING_BINARY_DIR}/lib)
ENDIF(UNIX)

ADD_LIBRARY(routing_dd SHARED alpha.c alpha_drivedist.cpp alpha.h boost_drivedist.cpp drivedist.c drivedist.h)




