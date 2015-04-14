set(LIBRARY_OUTPUT_PATH ../../../lib/)

include_directories(${PGROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR})
if(WIN32)
    include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32)
endif(WIN32)

if(APPLE)
    set(LIBRARY_MODE_TARGET "MODULE")
else(APPLE)
    set(LIBRARY_MODE_TARGET "SHARED")
endif(APPLE)

set(routing_ksp_SRCS pgr_dijkstra.hpp 
                     baseGraph.hpp 
                     basePath_SSEC.hpp 
                     pgr_ksp.hpp
                     ksp.c ksp.h 
                     KSPDriver.cpp KSPDriver.h 
                     vrp_assert.cpp vrp_assert.h
                     signalhandler.cpp signalhandler.h)

add_library(routing_ksp ${LIBRARY_MODE_TARGET} ${routing_ksp_SRCS})

if(WIN32)
  if(MSVC)
    set_target_properties(routing_ksp PROPERTIES COMPILE_FLAGS "-DHAVE_GETHOSTNAME")
  endif(MSVC)
endif(WIN32)

if(APPLE)
  set_target_properties(routing_ksp
    PROPERTIES
    LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)

install(TARGETS routing_ksp DESTINATION ${LIBRARY_INSTALL_PATH})
