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

    makeConnected_driver.cpp
    componentsResult.cpp
    components.cpp
    connectedComponents_driver.cpp
	strongComponents_driver.cpp
    biconnectedComponents_driver.cpp
    articulationPoints_driver.cpp
    bridges_driver.cpp)
