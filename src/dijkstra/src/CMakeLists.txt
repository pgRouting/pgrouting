#SET(LIBRARY_OUTPUT_PATH ../../../lib/)
#IF(APPLE)
#    SET(LIBRARY_MODE_TARGET "MODULE")
#ELSE(APPLE)
#    SET(LIBRARY_MODE_TARGET "SHARED")
#ENDIF(APPLE)
#ADD_LIBRARY(routing ${LIBRARY_MODE_TARGET} dijkstra.c boost_wrapper.cpp)
#INSTALL(TARGETS routing DESTINATION ${LIBRARY_INSTALL_PATH})
#IF(APPLE)
#    SET_TARGET_PROPERTIES(routing PROPERTIES LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
#ENDIF(APPLE)

ADD_LIBRARY(dijkstra OBJECT ../../common/src/postgres_connection.c
                            dijkstra.c 
                            1_to_many_dijkstra.c 
                            many_to_1_dijkstra.c 
                            many_to_many_dijkstra.c 
                            dijkstra_driver.cpp)
