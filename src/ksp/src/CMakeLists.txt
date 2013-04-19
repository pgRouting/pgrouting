set(LIBRARY_OUTPUT_PATH ../../../lib/)

include_directories(${PGROUTING_INCLUDE_DIRECTORIES} ${POSTGRESQL_INCLUDE_DIR})
if(WIN32)
  include_directories(${POSTGRESQL_INCLUDE_DIR}/port/win32)
endif(WIN32)

add_library(routing_ksp SHARED BaseGraph.cpp BaseGraph.h BasePath.cpp BasePath.h DijkstraShortestPathAlg.cpp DijkstraShortestPathAlg.h DynamicGraph.h Graph.cpp GraphElements.h Graph.h HeaderTest.cpp TGraph.h TPath.h YenTopKShortestPathsAlg.cpp YenTopKShortestPathsAlg.h ksp.c KSPDriver.cpp KSPGraph.h KSPGraph.cpp )

if(WIN32)
  if(MSVC)
    set_target_properties(routing_ksp PROPERTIES COMPILE_FLAGS "-DHAVE_GETHOSTNAME")
  else(MSVC)
    set_target_properties(routing_ksp PROPERTIES COMPILE_FLAGS "-DBUILDING_DLL")
    set_target_properties(routing_ksp PROPERTIES LINK_FLAGS -Wl,--export-all-symbols)
  endif(MSVC)
endif(WIN32)
if(APPLE)
  set_target_properties(routing
    PROPERTIES
    LINK_FLAGS "-bundle_loader ${POSTGRESQL_EXECUTABLE} -bundle")
endif(APPLE)

install(TARGETS routing_ksp DESTINATION ${LIBRARY_INSTALL_PATH})
