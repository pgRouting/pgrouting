# This file is part of the pgRouting project.
# Copyright (c) 2015-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(contraction OBJECT
    contractGraph.c
    contractGraph_driver.cpp
    contract.cpp
    contractionHierarchies.c
    contractionHierarchies_driver.cpp
    )
