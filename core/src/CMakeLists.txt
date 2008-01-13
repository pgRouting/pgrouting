
SET(LIBRARY_OUTPUT_PATH ../../lib/)
ADD_LIBRARY(routing SHARED dijkstra.c astar.c shooting_star.c boost_wrapper.cpp astar_boost_wrapper.cpp shooting_star_boost_wrapper.cpp)
INSTALL(TARGETS routing DESTINATION ${LIBRARY_INSTALL_PATH})


