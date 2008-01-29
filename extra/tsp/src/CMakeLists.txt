SET(LIBRARY_OUTPUT_PATH ../../../lib/)
ADD_LIBRARY(routing_tsp SHARED tsp.c tsp.h tsp_solver.cpp)
MESSAGE("Installation directory for libraries is set to ${LIBRARY_INSTALL_PATH}")
INSTALL(TARGETS routing_tsp DESTINATION ${LIBRARY_INSTALL_PATH})

