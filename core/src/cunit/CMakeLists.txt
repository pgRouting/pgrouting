set(SOURCES ../boost_wrapper.cpp)
set(CMAKE_CXX_FLAGS "-std=gnu++0x")
add_executable(run_tests dijkstra_test.cpp ${SOURCES})
target_link_libraries(run_tests cunit)

ADD_TEST(CoreFunctionsTest run_tests)
