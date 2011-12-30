SET(LIBRARY_OUTPUT_PATH ../../../lib/)
ADD_LIBRARY(routing_trsp SHARED trsp.h trsp.c utils.h GraphDefinition.h trsp_core.cpp GraphDefinition.cpp)
INSTALL(TARGETS routing_trsp DESTINATION ${LIBRARY_INSTALL_PATH})
