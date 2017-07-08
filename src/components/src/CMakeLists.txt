ADD_LIBRARY(components OBJECT
    connectedComponentsV.c
	strongComponentsV.c
	biconnectedComponents.c
    articulationPoints.c

    connectedComponentsV_driver.cpp
	strongComponentsV_driver.cpp
    biconnectedComponents_driver.cpp
    articulationPoints_driver.cpp)
