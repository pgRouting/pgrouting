# This file is part of the pgRouting project.
# Copyright (c) 2013-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

ADD_LIBRARY(ksp OBJECT
    ksp.c
    withPoints_ksp.c

    kPaths_process.cpp
    kPathsWithPoints_process.cpp

    turnRestrictedPath.c
    turnRestrictedPath_driver.cpp
)
