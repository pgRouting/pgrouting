# This file is part of the pgRouting project.
# Copyright (c) 2017-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(components OBJECT
    connectedComponents.c
    strongComponents.c
    biconnectedComponents.c
    articulationPoints.c
    bridges.c
    makeConnected.c

    components.cpp
    makeConnected.cpp
)
