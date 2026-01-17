# This file is part of the pgRouting project.
# Copyright (c) 2015-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(withPoints OBJECT
    withPoints.c

    get_new_queries.cpp
    withPoints.cpp

    withPointsVia.c
    withPointsVia_driver.cpp
    )
