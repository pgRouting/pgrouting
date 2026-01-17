# This file is part of the pgRouting project.
# Copyright (c) 2024-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
ADD_LIBRARY(metrics OBJECT
		betweennessCentrality.c
		betweennessCentrality_driver.cpp
        bandwidth.c
	    metrics_driver.cpp
        metrics_process.cpp
    )
