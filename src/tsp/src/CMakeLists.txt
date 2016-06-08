ADD_LIBRARY(tsp OBJECT
    eucledianTSP.c
    newTSP.c

    newTSP_driver.cpp
    eucledianTSP_driver.cpp

    Dmatrix.cpp
    eucledianDmatrix.cpp
    tour.cpp

    #Code for the old signature
    tsp2.c
    tsplib.c
    )
