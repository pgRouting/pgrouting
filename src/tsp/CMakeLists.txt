# This file is part of the pgRouting project.
# Copyright (c) 2007-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(tsp OBJECT
    euclideanTSP.c
    TSP.c
    tsp.cpp

    TSP_driver.cpp
    euclideanTSP_driver.cpp
    )
