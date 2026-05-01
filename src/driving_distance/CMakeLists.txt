# This file is part of the pgRouting project.
# Copyright (c) 2007-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(driving_distance OBJECT
    driving_distance.c
    driving_distance_withPoints.c

    driving_distance_withPoints_driver.cpp
)
