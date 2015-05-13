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

set(routing_ksp_SRCS ./../../dijkstra/src/pgr_dijkstra.hpp 
                     ./../../common/src/baseGraph.hpp 
                     ./../../common/src/basePath_SSEC.hpp 
                     ./../../common/src/postgres_connection.h 
                     ./../../common/src/postgres_connection.c 
                     pgr_ksp.hpp
                     ksp.c ksp.h 
                     ksp_driver.cpp ksp_driver.h 
 )
#                     ./../../common/src/pgr_assert.cpp ./../../common/src/pgr_assert.h
#                     ./../../common/src/signalhandler.cpp ./../../common/src/signalhandler.h)

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
