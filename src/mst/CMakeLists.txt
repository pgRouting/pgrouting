ADD_LIBRARY(mst OBJECT
    mst_common.cpp
    details.cpp

    kruskal.c
    kruskal_driver.cpp

    prim.c
    prim_driver.cpp

    #randomSpanningTree.c
    #randomSpanningTree_driver.cpp
    )
