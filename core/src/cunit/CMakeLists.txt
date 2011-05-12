set(SOURCES ../boost_wrapper.cpp)
add_executable(run_tests dijkstra_test.cpp ${SOURCES})
set_target_properties(run_tests PROPERTIES LINK_FLAGS -Wl,-lcunit COMPILE_FLAGS -std=gnu++0x)
ADD_TEST(CoreFunctionsTest run_tests)
