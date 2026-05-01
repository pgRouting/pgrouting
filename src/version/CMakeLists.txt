# This file is part of the pgRouting project.
# Copyright (c) 2019-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(version OBJECT
        version.c
        )
configure_file(version.h.in ${CMAKE_CURRENT_LIST_DIR}/version.h)
