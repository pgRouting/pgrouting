SET(LIBRARY_OUTPUT_PATH ../../../lib/)
LINK_LIBRARIES(${CGAL_LIBRARIES} ${GMP_LIBRARIES} ${BOOST_THREAD_LIBRARIES})

ADD_LIBRARY(routing_dd ${LIBRARY_MODE_TARGET}
    alpha.c
    alpha_drivedist.cpp

    boost_interface_drivedist.cpp

    drivedist.c
    many_to_dist_driving_distance.c
    ./../../common/src/postgres_connection.c
 )

IF(WIN32)
  SET_TARGET_PROPERTIES(routing_dd PROPERTIES COMPILE_FLAGS "-DBOOST_THREAD_USE_LIB -DBoost_USE_STATIC_LIBS -DBOOST_USE_WINDOWS_H")
ENDIF(WIN32)

if(APPLE)
    set_target_properties(routing_dd
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)

INSTALL(TARGETS routing_dd DESTINATION ${LIBRARY_INSTALL_PATH})
