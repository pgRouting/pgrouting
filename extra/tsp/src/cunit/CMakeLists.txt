set(SOURCES ../tsp_solver.cpp)
add_executable(run_tests tsp_test.cpp ${SOURCES})
set_target_properties(run_tests PROPERTIES LINK_FLAGS -Wl,-lcunit COMPILE_FLAGS -std=gnu++0x)
ADD_TEST(TSPFunctionsTest run_tests)
