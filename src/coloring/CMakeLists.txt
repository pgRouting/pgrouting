# This file is part of the pgRouting project.
# Copyright (c) 2020-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(coloring OBJECT
    sequentialVertexColoring.c
    bipartite.c
    edgeColoring.c

    bipartite.cpp
    edgeColoring.cpp
    sequentialVertexColoring.cpp

    coloring_process.cpp
    coloring_driver.cpp
)
