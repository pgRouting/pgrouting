# This file is part of the pgRouting project.
# Copyright (c) 2016-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(max_flow OBJECT
    max_flow.c
    minCostMaxFlow.c
    maximum_cardinality_matching.c
    edge_disjoint_paths.c

    max_flow_driver.cpp
    maximum_cardinality_matching_driver.cpp
    edge_disjoint_paths_driver.cpp
    minCostMaxFlow_driver.cpp

    maximumcardinalitymatching.cpp
    maxflow.cpp
    flowgraph.cpp
    minCostMaxFlow.cpp
    )
