# This file is part of the pgRouting project.
# Copyright (c) 2016-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(max_flow OBJECT
    # the C files
    max_flow.c
    minCostMaxFlow.c
    maximum_cardinality_matching.c
    maxWeightedMatching.c
    edge_disjoint_paths.c

    # driver & process files
    maxFlow_driver.cpp
    maxFlow_process.cpp
    minCostMaxFlow_driver.cpp


    # implementation
    maximumcardinalitymatching.cpp
    maxflow.cpp
    flowgraph.cpp
    minCostMaxFlow.cpp
    maxWeightedMatching.cpp
)
