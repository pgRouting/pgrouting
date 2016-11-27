ADD_LIBRARY(max_flow OBJECT
        max_flow_one_to_one.c
        max_flow_many_to_many.c
        max_flow_one_to_many.c
        max_flow_many_to_one.c

        maximum_cardinality_matching.c

        edge_disjoint_paths_one_to_one.c
        edge_disjoint_paths_one_to_many.c
        edge_disjoint_paths_many_to_many.c
        edge_disjoint_paths_many_to_one.c

        max_flow_one_to_one_driver.cpp
        max_flow_many_to_many_driver.cpp
        max_flow_one_to_many_driver.cpp
        max_flow_many_to_one_driver.cpp

        maximum_cardinality_matching_driver.cpp

        #edge_disjoint_paths_one_to_one_driver.cpp
        #edge_disjoint_paths_one_to_many_driver.cpp
        edge_disjoint_paths_many_to_many_driver.cpp
        edge_disjoint_paths_many_to_one_driver.cpp
)
