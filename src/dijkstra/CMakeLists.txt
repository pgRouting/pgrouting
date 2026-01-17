# This file is part of the pgRouting project.
# Copyright (c) 2007-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(dijkstra OBJECT
        dijkstra.c
        dijkstraVia.c

        shortestPath_driver.cpp
        shortestPath_process.cpp
        dijkstraVia_driver.cpp)
