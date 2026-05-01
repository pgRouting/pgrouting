# This file is part of the pgRouting project.
# Copyright (c) 2013-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
add_library(trsp OBJECT
    # keeping these for backwards compatibility
    edgeInfo.cpp
    trspHandler.cpp

    trsp.c
    trsp_driver.cpp

    trsp_withPoints.c
    trsp_withPoints_driver.cpp

    trspVia.c
    trspVia_driver.cpp

    trspVia_withPoints.c
    trspVia_withPoints_driver.cpp
    )
