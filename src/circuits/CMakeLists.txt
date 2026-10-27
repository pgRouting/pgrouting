# This file is part of the pgRouting project.
# Copyright (c) 2022-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

ADD_LIBRARY(circuits OBJECT
    hawickCircuits.c
    hawickUniqueCircuits.c
    hawickCircuits_deprecated.c

    circuits_process.cpp
    hawick_circuits.cpp
)
