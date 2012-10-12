SET(LIBRARY_OUTPUT_PATH ../../../lib/)
LINK_LIBRARIES(${CGAL_LIBRARIES})
ADD_LIBRARY(routing_ktargets SHARED k_targets_sp.h k_targets_sp.c k_targets_boost_wrapper.cpp)
INSTALL(TARGETS routing_ktargets DESTINATION ${LIBRARY_INSTALL_PATH})

