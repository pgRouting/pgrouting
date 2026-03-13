# This file is part of the pgRouting project.
# Copyright (c) 2007-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

ADD_LIBRARY(astar OBJECT
    astar.c

    astar_driver.cpp
    astar_process.cpp
)
