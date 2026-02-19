# This file is part of the pgRouting project.
# Copyright (c) 2018-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

ADD_LIBRARY(spanningTree OBJECT
    mst_common.cpp
    details.cpp

    kruskal.c
    prim.c
    prim_driver.cpp

    prim.cpp
    kruskal.cpp

    spanningTree_process.cpp
    spanningTree_driver.cpp
)
