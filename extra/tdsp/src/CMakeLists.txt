SET(LIBRARY_OUTPUT_PATH ../../../lib/)
ADD_LIBRARY(routing_tdsp SHARED tdsp_core.cpp tdsp.h tdsp.c edge_wrapper.h weight_map.h binary_heap.h)
INSTALL(TARGETS routing_tdsp DESTINATION ${LIBRARY_INSTALL_PATH})
