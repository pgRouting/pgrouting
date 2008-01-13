SET(LIBRARY_OUTPUT_PATH ../../../lib/)
ADD_LIBRARY(routing_dd SHARED alpha.c alpha_drivedist.cpp alpha.h boost_drivedist.cpp drivedist.c drivedist.h)
INSTALL(TARGETS routing_dd DESTINATION ${LIBRARY_INSTALL_PATH})


