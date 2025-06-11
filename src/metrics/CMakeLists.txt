ADD_LIBRARY(metrics OBJECT
		betweennessCentrality.c
		betweennessCentrality_driver.cpp
        bandwidth.c
        bandwidth_driver.cpp
        bandwidth_process.cpp
        metrics_driver.cpp
        metrics_process.cpp
    )
