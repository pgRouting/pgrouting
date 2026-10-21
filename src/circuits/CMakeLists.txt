# This file is part of the pgRouting project.
# Copyright (c) 2022-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

ADD_LIBRARY(circuits OBJECT
    hawickCircuits_deprecated.c

    hawick_circuits.cpp
    hawickCircuits_driver.cpp
)
