ADD_LIBRARY(components OBJECT
    connectedComponents.c
	strongComponents.c
	biconnectedComponents.c
    articulationPoints.c
    bridges.c
    makeConnected.c

    makeConnected_driver.cpp
    componentsResult.cpp
    components.cpp
    connectedComponents_driver.cpp
	strongComponents_driver.cpp
    biconnectedComponents_driver.cpp
    articulationPoints_driver.cpp
    bridges_driver.cpp)
