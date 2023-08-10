ADD_LIBRARY(metrics OBJECT
    bandwidth.c
    bandwidth_driver.cpp 

    centralPointDominance.c
    centralPointDominance_driver.cpp

    edgeBetweennessCentrality.c
    edgeBetweennessCentrality_driver.cpp
    
    ithBandwidth.c
    ithBandwidth_driver.cpp
    
    vertexBetweennessCentrality.c
    vertexBetweennessCentrality_driver.cpp
    )
