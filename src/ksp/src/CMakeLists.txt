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

add_library(routing_ksp ${LIBRARY_MODE_TARGET} DijkstraShortestPathAlg.cpp DijkstraShortestPathAlg.h  Graph.cpp GraphElements.h Graph.h  YenTopKShortestPathsAlg.cpp YenTopKShortestPathsAlg.h ksp.c KSPDriver.cpp KSPGraph.h KSPGraph.cpp )

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
