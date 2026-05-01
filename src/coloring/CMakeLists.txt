# This file is part of the pgRouting project.
# Copyright (c) 2020-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(coloring OBJECT
    sequentialVertexColoring.c
    sequentialVertexColoring_driver.cpp
    bipartite.c
    bipartite_driver.cpp
    edgeColoring.c
    edgeColoring_driver.cpp
    edgeColoring.cpp
    )
