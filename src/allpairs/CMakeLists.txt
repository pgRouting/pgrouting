# This file is part of the pgRouting project.
# Copyright (c) 2015-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(allpairs OBJECT
    floydWarshall.c
    johnson.c

    allpairs_process.cpp
    allpairs_driver.cpp)
