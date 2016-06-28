ADD_LIBRARY(astar OBJECT
    astarOneToOne.c
    astarOneToOne_driver.cpp

    #for version 3.0
    #astarOneToMany.c
    #astarOneToMany_driver.cpp

    #astarManyToOne.c
    #astarManyToMany.c
    #astarManyToMany_driver.cpp
    )
