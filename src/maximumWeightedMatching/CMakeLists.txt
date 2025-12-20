add_library(maximum_weighted_matching STATIC
    maximum_weighted_matching.cpp
)

target_link_libraries(maximum_weighted_matching PUBLIC pgRouting boost_graph)

