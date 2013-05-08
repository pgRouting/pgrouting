set(LIBRARY_OUTPUT_PATH ../../../lib/)

include_directories(${PGROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR})
if(WIN32)
    include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32)
endif(WIN32)

add_library(routing_tsp ${LIBRARY_MODE_TARGET} tsp.c tsp.h tsplib.c)

if(APPLE)
    set_target_properties(routing_tsp
        PROPERTIES
        LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)

install(TARGETS routing_tsp DESTINATION ${LIBRARY_INSTALL_PATH})
