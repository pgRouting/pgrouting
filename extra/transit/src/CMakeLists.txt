SET(LIBRARY_OUTPUT_PATH ../../../lib/)
ADD_LIBRARY(transit_routing SHARED transit.c utility.c)
ADD_LIBRARY(scheduled_routing SHARED scheduled_route.cpp transit_data.c)
INSTALL(TARGETS transit_routing DESTINATION ${LIBRARY_INSTALL_PATH})
