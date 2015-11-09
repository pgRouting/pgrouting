ADD_LIBRARY(dijkstra OBJECT
        one_to_one_dijkstra.c
        one_to_many_dijkstra.c
        many_to_many_dijkstra.c
        many_to_one_dijkstra.c

        one_to_one_dijkstra_driver.cpp
        one_to_many_dijkstra_driver.cpp
        many_to_many_dijkstra_driver.cpp 
        many_to_one_dijkstra_driver.cpp)
