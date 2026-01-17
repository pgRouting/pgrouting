# This file is part of the pgRouting project.
# Copyright (c) 2018-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(cpp_common OBJECT
    Dmatrix.cpp
    compPaths.cpp
    rule.cpp
    bpoint.cpp
    messages.cpp
    combinations.cpp

    to_postgres.cpp
    get_check_data.cpp
    pgdata_fetchers.cpp
    pgdata_getters.cpp
    undirectedNoCostBG.cpp
    undirectedHasCostBG.cpp
    )
