# This file is part of the pgRouting project.
# Copyright (c) 2013-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(ksp OBJECT
    ksp.c
    ksp_driver.cpp

    withPoints_ksp.c
    withPoints_ksp_driver.cpp

    turnRestrictedPath.c
    turnRestrictedPath_driver.cpp
    )
