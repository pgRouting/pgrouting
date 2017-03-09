ADD_LIBRARY(max_flow OBJECT
    max_flow_many_to_many.c
    maximum_cardinality_matching.c
    edge_disjoint_paths_many_to_many.c

    max_flow_driver.cpp
    maximum_cardinality_matching_driver.cpp
    edge_disjoint_paths_driver.cpp

    pgr_maxflow.cpp
    pgr_flowgraph.cpp
    )
