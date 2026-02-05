# This file is part of the pgRouting project.
# Copyright (c) 2018-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

ADD_LIBRARY(bellman_ford OBJECT
    bellman_ford.c
    bellman_ford_driver.cpp
    #bellman_ford_neg.c
    #bellman_ford_neg_driver.cpp
    edwardMoore.c
)
