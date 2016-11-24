ADD_LIBRARY(astar OBJECT
    many_to_many_astar.c
    many_to_one_astar.c
    one_to_many_astar.c
    one_to_one_astar.c

    astar_many_to_many_driver.cpp
    )
