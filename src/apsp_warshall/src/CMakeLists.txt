# FIXME: The following patch is required by apsp_johnson_boost_wrapper.cpp.
# With -fdelete-null-pointer-checks optimization, the assignments
# to Edge bundled properties doesn't occur.
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-delete-null-pointer-checks")

ADD_LIBRARY(apsp_warshall OBJECT apsp.c apsp_boost_wrapper.cpp)
