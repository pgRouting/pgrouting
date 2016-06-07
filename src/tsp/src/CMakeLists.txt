ADD_LIBRARY(tsp OBJECT
    eucledianTSP.c
    newTSP.c

    newTSP_driver.cpp
    eucledianTSP_driver.cpp

    Dmatrix.cpp
    eucledianDmatrix.cpp
    tour.cpp

    #Code to be deleted on V3.0
    tsp2.c
    tsplib.c
    )
