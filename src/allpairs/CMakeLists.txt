ADD_LIBRARY(allpairs OBJECT
    process_allpairs.c

    floydWarshall.c
    floydWarshall_driver.cpp

    johnson.c
    johnson_driver.cpp)
