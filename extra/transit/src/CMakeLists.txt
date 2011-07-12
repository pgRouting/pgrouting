SET(LIBRARY_OUTPUT_PATH ../../../lib/)
ADD_LIBRARY(transit_routing SHARED transit.c utility.c)
ADD_LIBRARY(scheduled_route SHARED transit.c scheduled_route.cpp transit_data.c utility.c)
INSTALL(TARGETS transit_routing DESTINATION ${LIBRARY_INSTALL_PATH})
