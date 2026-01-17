# This file is part of the pgRouting project.
# Copyright (c) 2020-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(traversal OBJECT
    depthFirstSearch.c
    depthFirstSearch_driver.cpp
    )
