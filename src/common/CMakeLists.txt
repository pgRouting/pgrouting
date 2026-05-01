# This file is part of the pgRouting project.
# Copyright (c) 2013-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(common OBJECT
    postgres_connection.c
    e_report.c
    path.cpp

    check_parameters.c

    time_msg.c
    assert.cpp
    identifier.cpp

    basic_vertex.cpp
    xy_vertex.cpp
    ch_vertex.cpp
    ch_edge.cpp
    basic_edge.cpp
    alloc.cpp
    )
